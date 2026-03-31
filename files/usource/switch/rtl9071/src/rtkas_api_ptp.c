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
#include "rtkas_api_ptp.h"
#include "rtkas_types.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#endif

/**
 * @defgroup RTK_MODULE_PTP PTP
 * @brief PTP(Presision Time Protocol) APIs. This component is for PTP.
 * @{
 */
/**
 * @defgroup PTP_TIME Time application interface configuration
 * @brief These APIs are used to get and set the enable status of Pulse Per Second (PPS) functionality.
 * @{
 */
/**
 * @brief Retrieve the PPS (Pulse Per Second) enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPpsenable ptp ppsenable get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_ppsenable(), cvb_ptp_get_ppsenable(), dv1_ptp_get_ppsenable(), dva_ptp_get_ppsenable().
 */
RtkApiRet rtk_ptp_get_ppsenable(UnitChip_t unitChip, PtpPpsenable_t *ptpPpsEnable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPpsEnable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_ppsenable(unitChip.unit, ptpPpsEnable->ppsIdx, &ptpPpsEnable->ppsenable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_ppsenable(unitChip.unit, ptpPpsEnable->ppsIdx, &ptpPpsEnable->ppsenable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_ppsenable(unitChip.unit, ptpPpsEnable->ppsIdx, &ptpPpsEnable->ppsenable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_ppsenable(unitChip.unit, ptpPpsEnable->ppsIdx, &ptpPpsEnable->ppsenable);
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
 * @brief Set the PPS (Pulse Per Second) firmware enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPpsenable ptp ppsenable set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_ppsenable(), cvb_ptp_set_ppsenable(), dv1_ptp_set_ppsenable(), dva_ptp_set_ppsenable().
 */
RtkApiRet rtk_ptp_set_ppsenable(UnitChip_t unitChip, PtpPpsenable_t *ptpPpsEnable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPpsEnable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_ppsenable(unitChip.unit, ptpPpsEnable->ppsIdx, ptpPpsEnable->ppsenable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_ppsenable(unitChip.unit, ptpPpsEnable->ppsIdx, ptpPpsEnable->ppsenable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_ppsenable(unitChip.unit, ptpPpsEnable->ppsIdx, ptpPpsEnable->ppsenable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_ppsenable(unitChip.unit, ptpPpsEnable->ppsIdx, ptpPpsEnable->ppsenable);
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
 * @brief Retrieve related GPIO output of the hardware PPS (Pulse Per Second) enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpTaiPpsEn ptp tai pps en get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_tai_pps_en(), cvb_ptp_get_tai_pps_en(), dv1_ptp_get_tai_pps_en(), dva_ptp_get_tai_pps_en().
 */
RtkApiRet rtk_ptp_get_tai_pps_en(UnitChip_t unitChip, PtpTaiPpsEn_t *ptpTaiPpsEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpTaiPpsEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_tai_pps_en(unitChip.unit, ptpTaiPpsEn->ppsidx, &ptpTaiPpsEn->ppsEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_tai_pps_en(unitChip.unit, ptpTaiPpsEn->ppsidx, &ptpTaiPpsEn->ppsEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_tai_pps_en(unitChip.unit, ptpTaiPpsEn->ppsidx, &ptpTaiPpsEn->ppsEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_tai_pps_en(unitChip.unit, ptpTaiPpsEn->ppsidx, &ptpTaiPpsEn->ppsEn);
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
 * @brief  Set the related GPIO output of hardware PPS (Pulse Per Second) enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSysPpsEn ptp sys pps en set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_sys_pps_en(), cvb_ptp_set_sys_pps_en(), dv1_ptp_set_sys_pps_en(), dva_ptp_set_sys_pps_en().
 */
RtkApiRet rtk_ptp_set_sys_pps_en(UnitChip_t unitChip, PtpSysPpsEn_t *ptpSysPpsEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSysPpsEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_sys_pps_en(unitChip.unit, ptpSysPpsEn->ppsidx, ptpSysPpsEn->ppsEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_sys_pps_en(unitChip.unit, ptpSysPpsEn->ppsidx, ptpSysPpsEn->ppsEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_sys_pps_en(unitChip.unit, ptpSysPpsEn->ppsidx, ptpSysPpsEn->ppsEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_sys_pps_en(unitChip.unit, ptpSysPpsEn->ppsidx, ptpSysPpsEn->ppsEn);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* PTP_TIME */

/**
 * @defgroup PTP_CLOCK Clock related configuration
 * @brief These APIs are used to get and set the enable status of PTP clock functionalities.
 * @{
 */
/**
 * @brief Retrieve the per-port time synchronization enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPortTimeSyncEn ptp port time sync en get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_port_time_sync_en(), cvb_ptp_get_port_time_sync_en(), dv1_ptp_get_port_time_sync_en(), dva_ptp_get_port_time_sync_en().
 */
RtkApiRet rtk_ptp_get_port_time_sync_en(UnitChip_t unitChip, PtpPortTimeSyncEn_t *ptpPortTimeSyncEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortTimeSyncEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_port_time_sync_en(unitChip.unit, ptpPortTimeSyncEn->port, &ptpPortTimeSyncEn->timeSyncEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_port_time_sync_en(unitChip.unit, ptpPortTimeSyncEn->port, &ptpPortTimeSyncEn->timeSyncEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_port_time_sync_en(unitChip.unit, ptpPortTimeSyncEn->port, &ptpPortTimeSyncEn->timeSyncEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_port_time_sync_en(unitChip.unit, ptpPortTimeSyncEn->port, &ptpPortTimeSyncEn->timeSyncEn);
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
 * @brief Set the per-port time synchronization enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPortTimeSyncEn ptp port time sync en set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_port_time_sync_en(), cvb_ptp_set_port_time_sync_en(), dv1_ptp_set_port_time_sync_en(), dva_ptp_set_port_time_sync_en().
 */
RtkApiRet rtk_ptp_set_port_time_sync_en(UnitChip_t unitChip, PtpPortTimeSyncEn_t *ptpPortTimeSyncEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortTimeSyncEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_port_time_sync_en(unitChip.unit, ptpPortTimeSyncEn->port, ptpPortTimeSyncEn->timeSyncEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_port_time_sync_en(unitChip.unit, ptpPortTimeSyncEn->port, ptpPortTimeSyncEn->timeSyncEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_port_time_sync_en(unitChip.unit, ptpPortTimeSyncEn->port, ptpPortTimeSyncEn->timeSyncEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_port_time_sync_en(unitChip.unit, ptpPortTimeSyncEn->port, ptpPortTimeSyncEn->timeSyncEn);
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
 * @brief Retrieve the PTP (Precision Time Protocol) enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpHwEnable ptp hw enable get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_hw_enable(), cvb_ptp_get_hw_enable(), dv1_ptp_get_hw_enable(), dva_ptp_get_hw_enable().
 */
RtkApiRet rtk_ptp_get_hw_enable(UnitChip_t unitChip, PtpHwEnable_t *ptpHwEnable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpHwEnable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_hw_enable(unitChip.unit, &ptpHwEnable->ptpEnable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_hw_enable(unitChip.unit, &ptpHwEnable->ptpEnable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_hw_enable(unitChip.unit, &ptpHwEnable->ptpEnable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_hw_enable(unitChip.unit, &ptpHwEnable->ptpEnable);
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
 * @brief Set the hardware PTP (Precision Time Protocol) enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpHwEnable ptp hw enable set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_hw_enable(), cvb_ptp_set_hw_enable(), dv1_ptp_set_hw_enable(), dva_ptp_set_hw_enable().
 */
RtkApiRet rtk_ptp_set_hw_enable(UnitChip_t unitChip, PtpHwEnable_t *ptpHwEnable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpHwEnable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_hw_enable(unitChip.unit, ptpHwEnable->ptpEnable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_hw_enable(unitChip.unit, ptpHwEnable->ptpEnable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_hw_enable(unitChip.unit, ptpHwEnable->ptpEnable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_hw_enable(unitChip.unit, ptpHwEnable->ptpEnable);
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
 * @brief Retrieve the system time enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSysTimeEn ptp sys time en get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_sys_time_en(), cvb_ptp_get_sys_time_en(), dv1_ptp_get_sys_time_en(), dva_ptp_get_sys_time_en().
 */
RtkApiRet rtk_ptp_get_sys_time_en(UnitChip_t unitChip, PtpSysTimeEn_t *ptpSysTimeEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSysTimeEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_sys_time_en(unitChip.unit, &ptpSysTimeEn->sysTimeEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_sys_time_en(unitChip.unit, &ptpSysTimeEn->sysTimeEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_sys_time_en(unitChip.unit, &ptpSysTimeEn->sysTimeEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_sys_time_en(unitChip.unit, &ptpSysTimeEn->sysTimeEn);
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
 * @brief Set the system time enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSysTimeEn ptp sys time en set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_sys_time_en(), cvb_ptp_set_sys_time_en(), dv1_ptp_set_sys_time_en(), dva_ptp_set_sys_time_en().
 */
RtkApiRet rtk_ptp_set_sys_time_en(UnitChip_t unitChip, PtpSysTimeEn_t *ptpSysTimeEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSysTimeEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_sys_time_en(unitChip.unit, ptpSysTimeEn->sysTimeEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_sys_time_en(unitChip.unit, ptpSysTimeEn->sysTimeEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_sys_time_en(unitChip.unit, ptpSysTimeEn->sysTimeEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_sys_time_en(unitChip.unit, ptpSysTimeEn->sysTimeEn);
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
 * @brief Retrieve the MASTER clock enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMasterEn ptp master en get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_master_en(), cvb_ptp_get_master_en(), dv1_ptp_get_master_en(), dva_ptp_get_master_en().
 */
RtkApiRet rtk_ptp_get_master_en(UnitChip_t unitChip, PtpMasterEn_t *ptpMasterEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMasterEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_master_en(unitChip.unit, &ptpMasterEn->masterEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_master_en(unitChip.unit, &ptpMasterEn->masterEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_master_en(unitChip.unit, &ptpMasterEn->masterEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_master_en(unitChip.unit, &ptpMasterEn->masterEn);
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
 * @brief Set the MASTER clock enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpMasterEn ptp master en set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_master_en(), cvb_ptp_set_master_en(), dv1_ptp_set_master_en(), dva_ptp_set_master_en().
 */
RtkApiRet rtk_ptp_set_master_en(UnitChip_t unitChip, PtpMasterEn_t *ptpMasterEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMasterEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_master_en(unitChip.unit, ptpMasterEn->masterEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_master_en(unitChip.unit, ptpMasterEn->masterEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_master_en(unitChip.unit, ptpMasterEn->masterEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_master_en(unitChip.unit, ptpMasterEn->masterEn);
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
 * @brief Retrieve the SLAVE2 clock enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSlave2En ptp slave2 en get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_slave2_en(), cvb_ptp_get_slave2_en(), dv1_ptp_get_slave2_en(), dva_ptp_get_slave2_en().
 */
RtkApiRet rtk_ptp_get_slave2_en(UnitChip_t unitChip, PtpSlave2En_t *ptpSlave2En)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlave2En)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_slave2_en(unitChip.unit, &ptpSlave2En->slave2En);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_slave2_en(unitChip.unit, &ptpSlave2En->slave2En);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_slave2_en(unitChip.unit, &ptpSlave2En->slave2En);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_slave2_en(unitChip.unit, &ptpSlave2En->slave2En);
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
 * @brief Set the SLAVE2 clock enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSlave2En ptp slave2 en set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_slave2_en(), cvb_ptp_set_slave2_en(), dv1_ptp_set_slave2_en(), dva_ptp_set_slave2_en().
 */
RtkApiRet rtk_ptp_set_slave2_en(UnitChip_t unitChip, PtpSlave2En_t *ptpSlave2En)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlave2En)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_slave2_en(unitChip.unit, ptpSlave2En->slave2En);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_slave2_en(unitChip.unit, ptpSlave2En->slave2En);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_slave2_en(unitChip.unit, ptpSlave2En->slave2En);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_slave2_en(unitChip.unit, ptpSlave2En->slave2En);
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
 * @brief Retrieve the SLAVE1 clock enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSlave1En ptp slave1 en get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_slave1_en(), cvb_ptp_get_slave1_en(), dv1_ptp_get_slave1_en(), dva_ptp_get_slave1_en().
 */
RtkApiRet rtk_ptp_get_slave1_en(UnitChip_t unitChip, PtpSlave1En_t *ptpSlave1En)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlave1En)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_slave1_en(unitChip.unit, &ptpSlave1En->slave1En);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_slave1_en(unitChip.unit, &ptpSlave1En->slave1En);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_slave1_en(unitChip.unit, &ptpSlave1En->slave1En);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_slave1_en(unitChip.unit, &ptpSlave1En->slave1En);
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
 * @brief Set the SLAVE1 clock enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSlave1En ptp slave1 en set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_slave1_en(), cvb_ptp_set_slave1_en(), dv1_ptp_set_slave1_en(), dva_ptp_set_slave1_en().
 */
RtkApiRet rtk_ptp_set_slave1_en(UnitChip_t unitChip, PtpSlave1En_t *ptpSlave1En)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlave1En)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_slave1_en(unitChip.unit, ptpSlave1En->slave1En);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_slave1_en(unitChip.unit, ptpSlave1En->slave1En);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_slave1_en(unitChip.unit, ptpSlave1En->slave1En);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_slave1_en(unitChip.unit, ptpSlave1En->slave1En);
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
 * @brief Retrieve the SLAVE0 clock enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSlave0En ptp slave0 en get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_slave0_en(), cvb_ptp_get_slave0_en(), dv1_ptp_get_slave0_en(), dva_ptp_get_slave0_en().
 */
RtkApiRet rtk_ptp_get_slave0_en(UnitChip_t unitChip, PtpSlave0En_t *ptpSlave0En)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlave0En)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_slave0_en(unitChip.unit, &ptpSlave0En->slave0En);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_slave0_en(unitChip.unit, &ptpSlave0En->slave0En);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_slave0_en(unitChip.unit, &ptpSlave0En->slave0En);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_slave0_en(unitChip.unit, &ptpSlave0En->slave0En);
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
 * @brief Set the SLAVE0 clock enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSlave0En ptp slave0 en set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_slave0_en(), cvb_ptp_set_slave0_en(), dv1_ptp_set_slave0_en(), dva_ptp_set_slave0_en().
 */
RtkApiRet rtk_ptp_set_slave0_en(UnitChip_t unitChip, PtpSlave0En_t *ptpSlave0En)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlave0En)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_slave0_en(unitChip.unit, ptpSlave0En->slave0En);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_slave0_en(unitChip.unit, ptpSlave0En->slave0En);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_slave0_en(unitChip.unit, ptpSlave0En->slave0En);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_slave0_en(unitChip.unit, ptpSlave0En->slave0En);
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
 * @brief When the RTL906x_7x is a time-aware bridge (TC+BC), it will synchronize time from its slave port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpClkstable ptp clkstable get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_clkstable_get(), cvb_ptp_clkstable_get(), dv1_ptp_clkstable_get(), dva_ptp_clkstable_get().
 */
RtkApiRet rtk_ptp_clkstable_get(UnitChip_t unitChip, PtpClkstable_t *ptpClkStable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpClkStable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_clkstable_get(unitChip.unit, ptpClkStable->domainid, &ptpClkStable->clkstable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_clkstable_get(unitChip.unit, ptpClkStable->domainid, &ptpClkStable->clkstable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_clkstable_get(unitChip.unit, ptpClkStable->domainid, &ptpClkStable->clkstable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_clkstable_get(unitChip.unit, ptpClkStable->domainid, &ptpClkStable->clkstable);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* PTP_CLOCK */

/**
 * @defgroup PTP_PARSER Packet parser related configuration
 * @brief These APIs are used to get and set the conditions for PTP packet parsing and interface latency.
 * @{
 */
/**
 * @brief Retrieve the redirect port mask value. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpRedirectCpumask ptp redirect cpumask get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_redirect_cpumask(), cvb_ptp_get_redirect_cpumask(), dv1_ptp_get_redirect_cpumask(), dva_ptp_get_redirect_cpumask().
 */
RtkApiRet rtk_ptp_get_redirect_cpumask(UnitChip_t unitChip, PtpRedirectCpumask_t *ptpRedirectCpuMask)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpRedirectCpuMask)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_redirect_cpumask(unitChip.unit, &ptpRedirectCpuMask->cpumask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_redirect_cpumask(unitChip.unit, &ptpRedirectCpuMask->cpumask);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_redirect_cpumask(unitChip.unit, &ptpRedirectCpuMask->cpumask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_redirect_cpumask(unitChip.unit, &ptpRedirectCpuMask->cpumask);
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
 * @brief Set the redirect port mask value. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpRedirectCpumask ptp redirect cpumask set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_redirect_cpumask(), cvb_ptp_set_redirect_cpumask(), dv1_ptp_set_redirect_cpumask(), dva_ptp_set_redirect_cpumask().
 */
RtkApiRet rtk_ptp_set_redirect_cpumask(UnitChip_t unitChip, PtpRedirectCpumask_t *ptpRedirectCpuMask)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpRedirectCpuMask)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_redirect_cpumask(unitChip.unit, ptpRedirectCpuMask->cpumask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_redirect_cpumask(unitChip.unit, ptpRedirectCpuMask->cpumask);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_redirect_cpumask(unitChip.unit, ptpRedirectCpuMask->cpumask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_redirect_cpumask(unitChip.unit, ptpRedirectCpuMask->cpumask);
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
 * @brief Retrieve the action of the redirect PTP packet. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpRedirectAct ptp redirect act get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_redirect_act(), cvb_ptp_get_redirect_act(), dv1_ptp_get_redirect_act(), dva_ptp_get_redirect_act().
 */
RtkApiRet rtk_ptp_get_redirect_act(UnitChip_t unitChip, PtpRedirectAct_t *ptpRedirectAct)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpRedirectAct)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_redirect_act(unitChip.unit, &ptpRedirectAct->ptpRedirAct);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_redirect_act(unitChip.unit, &ptpRedirectAct->ptpRedirAct);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_redirect_act(unitChip.unit, &ptpRedirectAct->ptpRedirAct);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_redirect_act(unitChip.unit, &ptpRedirectAct->ptpRedirAct);
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
 * @brief Set the action for the redirect PTP packet. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpRedirectAct ptp redirect act set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_redirect_act(), cvb_ptp_set_redirect_act(), dv1_ptp_set_redirect_act(), dva_ptp_set_redirect_act().
 */
RtkApiRet rtk_ptp_set_redirect_act(UnitChip_t unitChip, PtpRedirectAct_t *ptpRedirectAct)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpRedirectAct)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_redirect_act(unitChip.unit, ptpRedirectAct->ptpRedirAct);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_redirect_act(unitChip.unit, ptpRedirectAct->ptpRedirAct);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_redirect_act(unitChip.unit, ptpRedirectAct->ptpRedirAct);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_redirect_act(unitChip.unit, ptpRedirectAct->ptpRedirAct);
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
 * @brief Retrieve the Ethertype check enable status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpEtherCheckEnable0 ptp ether check enable0 get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_ether_check_enable0(), cvb_ptp_get_ether_check_enable0(), dv1_ptp_get_ether_check_enable0(), dva_ptp_get_ether_check_enable0().
 */
RtkApiRet rtk_ptp_get_ether_check_enable0(UnitChip_t unitChip, PtpEtherCheckEnable0_t *ptpEtherCheckEnable0)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpEtherCheckEnable0)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_ether_check_enable0(unitChip.unit, ptpEtherCheckEnable0->port, &ptpEtherCheckEnable0->rtkEnable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_ether_check_enable0(unitChip.unit, ptpEtherCheckEnable0->port, &ptpEtherCheckEnable0->rtkEnable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_ether_check_enable0(unitChip.unit, ptpEtherCheckEnable0->port, &ptpEtherCheckEnable0->rtkEnable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_ether_check_enable0(unitChip.unit, ptpEtherCheckEnable0->port, &ptpEtherCheckEnable0->rtkEnable);
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
 * @brief Enabling this RX parser will activate Ethertype filtering during reception. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpEtherCheckEnable0 ptp ether check enable0 set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_ether_check_enable0(), cvb_ptp_set_ether_check_enable0(), dv1_ptp_set_ether_check_enable0(), dva_ptp_set_ether_check_enable0().
 */
RtkApiRet rtk_ptp_set_ether_check_enable0(UnitChip_t unitChip, PtpEtherCheckEnable0_t *ptpEtherCheckEnable0)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpEtherCheckEnable0)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_ether_check_enable0(unitChip.unit, ptpEtherCheckEnable0->port, ptpEtherCheckEnable0->rtkEnable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_ether_check_enable0(unitChip.unit, ptpEtherCheckEnable0->port, ptpEtherCheckEnable0->rtkEnable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_ether_check_enable0(unitChip.unit, ptpEtherCheckEnable0->port, ptpEtherCheckEnable0->rtkEnable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_ether_check_enable0(unitChip.unit, ptpEtherCheckEnable0->port, ptpEtherCheckEnable0->rtkEnable);
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
 * @brief Retrieve the latch timestamp timing from RXDV/TXEN (MII). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPortPtp100Tx ptp port ptp 100tx get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_port_ptp_100tx(), cvb_ptp_get_port_ptp_100tx(), dv1_ptp_get_port_ptp_100tx(), dva_ptp_get_port_ptp_100tx().
 */
RtkApiRet rtk_ptp_get_port_ptp_100tx(UnitChip_t unitChip, PtpPortPtp100Tx_t *ptpPortPtp100Tx)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortPtp100Tx)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_port_ptp_100tx(unitChip.unit, ptpPortPtp100Tx->port, &ptpPortPtp100Tx->ptp100tx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_port_ptp_100tx(unitChip.unit, ptpPortPtp100Tx->port, &ptpPortPtp100Tx->ptp100tx);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_port_ptp_100tx(unitChip.unit, ptpPortPtp100Tx->port, &ptpPortPtp100Tx->ptp100tx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_port_ptp_100tx(unitChip.unit, ptpPortPtp100Tx->port, &ptpPortPtp100Tx->ptp100tx);
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
 * @brief Latch the timestamp timing from RXDV/TXEN (MII). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPortPtp100Tx ptp port ptp 100tx set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_port_ptp_100tx(), cvb_ptp_set_port_ptp_100tx(), dv1_ptp_set_port_ptp_100tx(), dva_ptp_set_port_ptp_100tx().
 */
RtkApiRet rtk_ptp_set_port_ptp_100tx(UnitChip_t unitChip, PtpPortPtp100Tx_t *ptpPortPtp100Tx)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortPtp100Tx)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_port_ptp_100tx(unitChip.unit, ptpPortPtp100Tx->port, ptpPortPtp100Tx->ptp100tx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_port_ptp_100tx(unitChip.unit, ptpPortPtp100Tx->port, ptpPortPtp100Tx->ptp100tx);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_port_ptp_100tx(unitChip.unit, ptpPortPtp100Tx->port, ptpPortPtp100Tx->ptp100tx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_port_ptp_100tx(unitChip.unit, ptpPortPtp100Tx->port, ptpPortPtp100Tx->ptp100tx);
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
 * @brief Retrieve the compensation time for the ingress management plane. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPortRxlatency ptp port rxlatency get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_port_rxlatency(), cvb_ptp_get_port_rxlatency(), dv1_ptp_get_port_rxlatency(), dva_ptp_get_port_rxlatency().
 */
RtkApiRet rtk_ptp_get_port_rxlatency(UnitChip_t unitChip, PtpPortRxlatency_t *ptpPortRxLatency)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortRxLatency)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_port_rxlatency(unitChip.unit, ptpPortRxLatency->port, &ptpPortRxLatency->rxLatency);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_port_rxlatency(unitChip.unit, ptpPortRxLatency->port, &ptpPortRxLatency->rxLatency);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_port_rxlatency(unitChip.unit, ptpPortRxLatency->port, &ptpPortRxLatency->rxLatency);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_port_rxlatency(unitChip.unit, ptpPortRxLatency->port, &ptpPortRxLatency->rxLatency);
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
 * @brief Set the compensation time for the ingress management plane. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPortRxlatency ptp port rxlatency set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_port_rxlatency(), cvb_ptp_set_port_rxlatency(), dv1_ptp_set_port_rxlatency(), dva_ptp_set_port_rxlatency().
 */
RtkApiRet rtk_ptp_set_port_rxlatency(UnitChip_t unitChip, PtpPortRxlatency_t *ptpPortRxLatency)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortRxLatency)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_port_rxlatency(unitChip.unit, ptpPortRxLatency->port, ptpPortRxLatency->rxLatency);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_port_rxlatency(unitChip.unit, ptpPortRxLatency->port, ptpPortRxLatency->rxLatency);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_port_rxlatency(unitChip.unit, ptpPortRxLatency->port, ptpPortRxLatency->rxLatency);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_port_rxlatency(unitChip.unit, ptpPortRxLatency->port, ptpPortRxLatency->rxLatency);
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
 * @brief Retrieve the compensation time for the egress management plane. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPortTxlatency ptp port txlatency get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_port_txlatency(), cvb_ptp_get_port_txlatency(), dv1_ptp_get_port_txlatency(), dva_ptp_get_port_txlatency().
 */
RtkApiRet rtk_ptp_get_port_txlatency(UnitChip_t unitChip, PtpPortTxlatency_t *ptpPortTxLatency)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortTxLatency)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_port_txlatency(unitChip.unit, ptpPortTxLatency->port, &ptpPortTxLatency->txLatency);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_port_txlatency(unitChip.unit, ptpPortTxLatency->port, &ptpPortTxLatency->txLatency);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_port_txlatency(unitChip.unit, ptpPortTxLatency->port, &ptpPortTxLatency->txLatency);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_port_txlatency(unitChip.unit, ptpPortTxLatency->port, &ptpPortTxLatency->txLatency);
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
 * @brief Set the compensation time for the egress management plane. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPortTxlatency ptp port txlatency set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_port_txlatency(), cvb_ptp_set_port_txlatency(), dv1_ptp_set_port_txlatency(), dva_ptp_set_port_txlatency().
 */
RtkApiRet rtk_ptp_set_port_txlatency(UnitChip_t unitChip, PtpPortTxlatency_t *ptpPortTxLatency)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortTxLatency)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_port_txlatency(unitChip.unit, ptpPortTxLatency->port, ptpPortTxLatency->txLatency);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_port_txlatency(unitChip.unit, ptpPortTxLatency->port, ptpPortTxLatency->txLatency);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_port_txlatency(unitChip.unit, ptpPortTxLatency->port, ptpPortTxLatency->txLatency);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_port_txlatency(unitChip.unit, ptpPortTxLatency->port, ptpPortTxLatency->txLatency);
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
 * @brief Retrieve the slave time domain of a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPortSlaveTimeDomain ptp get port slave time domain information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_port_slave_time_domain(), cvb_ptp_get_port_slave_time_domain(), dv1_ptp_get_port_slave_time_domain(), dva_ptp_get_port_slave_time_domain().
 */
RtkApiRet rtk_ptp_get_port_slave_time_domain(UnitChip_t unitChip, PtpPortSlaveTimeDomain_t *ptpPortSlaveTimeDomain)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortSlaveTimeDomain)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_port_slave_time_domain(unitChip.unit, ptpPortSlaveTimeDomain->port, ptpPortSlaveTimeDomain->domainid, &ptpPortSlaveTimeDomain->domainvalue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_port_slave_time_domain(unitChip.unit, ptpPortSlaveTimeDomain->port, ptpPortSlaveTimeDomain->domainid, &ptpPortSlaveTimeDomain->domainvalue);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_port_slave_time_domain(unitChip.unit, ptpPortSlaveTimeDomain->port, ptpPortSlaveTimeDomain->domainid, &ptpPortSlaveTimeDomain->domainvalue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_port_slave_time_domain(unitChip.unit, ptpPortSlaveTimeDomain->port, ptpPortSlaveTimeDomain->domainid, &ptpPortSlaveTimeDomain->domainvalue);
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
 * @brief Configure the slave time domain for a specified port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPortSlaveTimeDomain ptp set port slave time domain information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_port_slave_time_domain(), cvb_ptp_set_port_slave_time_domain(), dv1_ptp_set_port_slave_time_domain(), dva_ptp_set_port_slave_time_domain().
 */
RtkApiRet rtk_ptp_set_port_slave_time_domain(UnitChip_t unitChip, PtpPortSlaveTimeDomain_t *ptpPortSlaveTimeDomain)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortSlaveTimeDomain)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_port_slave_time_domain(unitChip.unit, ptpPortSlaveTimeDomain->port, ptpPortSlaveTimeDomain->domainid, ptpPortSlaveTimeDomain->domainvalue);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_port_slave_time_domain(unitChip.unit, ptpPortSlaveTimeDomain->port, ptpPortSlaveTimeDomain->domainid, ptpPortSlaveTimeDomain->domainvalue);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_port_slave_time_domain(unitChip.unit, ptpPortSlaveTimeDomain->port, ptpPortSlaveTimeDomain->domainid, ptpPortSlaveTimeDomain->domainvalue);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_port_slave_time_domain(unitChip.unit, ptpPortSlaveTimeDomain->port, ptpPortSlaveTimeDomain->domainid, ptpPortSlaveTimeDomain->domainvalue);
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
 * @brief Define a custom EtherType for Precision Time Protocol (PTP). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpCustomEthertype ptp custom ethertype set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_custom_ethertype(), cvb_ptp_set_custom_ethertype(), dv1_ptp_set_custom_ethertype(), dva_ptp_set_custom_ethertype().
 */
RtkApiRet rtk_ptp_set_custom_ethertype(UnitChip_t unitChip, PtpCustomEthertype_t *ptpCustomEthertype)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomEthertype)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_custom_ethertype(unitChip.unit, ptpCustomEthertype->port, ptpCustomEthertype->ethertype);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_custom_ethertype(unitChip.unit, ptpCustomEthertype->port, ptpCustomEthertype->ethertype);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_custom_ethertype(unitChip.unit, ptpCustomEthertype->port, ptpCustomEthertype->ethertype);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_custom_ethertype(unitChip.unit, ptpCustomEthertype->port, ptpCustomEthertype->ethertype);
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
 * @brief Retrieve the custom EtherType for PTP. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpCustomEthertype ptp custom ethertype get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_custom_ethertype(), cvb_ptp_set_custom_ethertype(), dv1_ptp_get_custom_ethertype(), dva_ptp_get_custom_ethertype().
 */
RtkApiRet rtk_ptp_get_custom_ethertype(UnitChip_t unitChip, PtpCustomEthertype_t *ptpCustomEthertype)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomEthertype)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_custom_ethertype(unitChip.unit, ptpCustomEthertype->port, &ptpCustomEthertype->ethertype);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_custom_ethertype(unitChip.unit, ptpCustomEthertype->port, &ptpCustomEthertype->ethertype);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_custom_ethertype(unitChip.unit, ptpCustomEthertype->port, &ptpCustomEthertype->ethertype);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_custom_ethertype(unitChip.unit, ptpCustomEthertype->port, &ptpCustomEthertype->ethertype);
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
 * @brief Enable or disable the validation of custom EtherType values
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpCustomEthertypeEnable ptp set custom ethertype enable information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_ethertype_enable_custom(), cvb_ptp_set_ethertype_enable_custom(), dv1_ptp_set_ethertype_enable_custom(), dva_ptp_set_ethertype_enable_custom().
 */
RtkApiRet rtk_ptp_set_ethertype_enable_custom(UnitChip_t unitChip, PtpCustomEthertypeEnable_t *ptpCustomEthertypeEnable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomEthertypeEnable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_ethertype_enable_custom(unitChip.unit, ptpCustomEthertypeEnable->port, ptpCustomEthertypeEnable->enable);

                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_ethertype_enable_custom(unitChip.unit, ptpCustomEthertypeEnable->port, ptpCustomEthertypeEnable->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_ethertype_enable_custom(unitChip.unit, ptpCustomEthertypeEnable->port, ptpCustomEthertypeEnable->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_ethertype_enable_custom(unitChip.unit, ptpCustomEthertypeEnable->port, ptpCustomEthertypeEnable->enable);
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
 * @brief Retrieve the enable/disable status for checking custom EtherType. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpCustomEthertypeEnable ptp get custom ethertype enable information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_ethertype_enable_custom(), cvb_ptp_get_ethertype_enable_custom(), dv1_ptp_get_ethertype_enable_custom(), dva_ptp_get_ethertype_enable_custom().
 */
RtkApiRet rtk_ptp_get_ethertype_enable_custom(UnitChip_t unitChip, PtpCustomEthertypeEnable_t *ptpCustomEthertypeEnable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomEthertypeEnable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_ethertype_enable_custom(unitChip.unit, ptpCustomEthertypeEnable->port, &ptpCustomEthertypeEnable->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_ethertype_enable_custom(unitChip.unit, ptpCustomEthertypeEnable->port, &ptpCustomEthertypeEnable->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_ethertype_enable_custom(unitChip.unit, ptpCustomEthertypeEnable->port, &ptpCustomEthertypeEnable->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_ethertype_enable_custom(unitChip.unit, ptpCustomEthertypeEnable->port, &ptpCustomEthertypeEnable->enable);
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
 * @brief When enabled, packets with a VLAN header will be checked to determine if they belong to PTP. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpCustomVlanEnable ptp custom vlan enable set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_custom_set_vlan_enable(), cvb_ptp_custom_set_vlan_enable(), dv1_ptp_custom_set_vlan_enable(), dva_ptp_custom_set_vlan_enable().
 */
RtkApiRet rtk_ptp_custom_set_vlan_enable(UnitChip_t unitChip, PtpCustomVlanEnable_t *ptpCustomVlanEnable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomVlanEnable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_custom_set_vlan_enable(unitChip.unit, ptpCustomVlanEnable->port, ptpCustomVlanEnable->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_custom_set_vlan_enable(unitChip.unit, ptpCustomVlanEnable->port, ptpCustomVlanEnable->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_custom_set_vlan_enable(unitChip.unit, ptpCustomVlanEnable->port, ptpCustomVlanEnable->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_custom_set_vlan_enable(unitChip.unit, ptpCustomVlanEnable->port, ptpCustomVlanEnable->enable);
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
 * @brief When enabled, packets with a VLAN header will be checked to determine if they belong to PTP. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpCustomVlanEnable ptp custom vlan enable get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_custom_get_vlan_enable(), cvb_ptp_custom_get_vlan_enable(), dv1_ptp_custom_get_vlan_enable(), dva_ptp_custom_get_vlan_enable().
 */
RtkApiRet rtk_ptp_custom_get_vlan_enable(UnitChip_t unitChip, PtpCustomVlanEnable_t *ptpCustomVlanEnable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomVlanEnable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_custom_get_vlan_enable(unitChip.unit, ptpCustomVlanEnable->port, &ptpCustomVlanEnable->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_custom_get_vlan_enable(unitChip.unit, ptpCustomVlanEnable->port, &ptpCustomVlanEnable->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_custom_get_vlan_enable(unitChip.unit, ptpCustomVlanEnable->port, &ptpCustomVlanEnable->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_custom_get_vlan_enable(unitChip.unit, ptpCustomVlanEnable->port, &ptpCustomVlanEnable->enable);
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
 * @brief Configure the custom Destination Address (DA) for PTP, used during the parsing of incoming packets. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpCustomDamac01 ptp custom damac01 set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_custom_damac01(), cvb_ptp_set_custom_damac01(), dv1_ptp_set_custom_damac01(), dva_ptp_set_custom_damac01().
 */
RtkApiRet rtk_ptp_set_custom_damac01(UnitChip_t unitChip, PtpCustomDamac01_t *ptpCustomDamac01)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomDamac01)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_custom_damac01(unitChip.unit, ptpCustomDamac01->port, ptpCustomDamac01->damac);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_custom_damac01(unitChip.unit, ptpCustomDamac01->port, ptpCustomDamac01->damac);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_custom_damac01(unitChip.unit, ptpCustomDamac01->port, ptpCustomDamac01->damac);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_custom_damac01(unitChip.unit, ptpCustomDamac01->domainid, ptpCustomDamac01->port, ptpCustomDamac01->damac);
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
 * @brief Retrieve the custom Destination Address (DA) for PTP, used during the parsing of incoming packets. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpCustomDamac01 ptp custom damac01 get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_custom_damac01(), cvb_ptp_get_custom_damac01(), dv1_ptp_get_custom_damac01(), dva_ptp_get_custom_damac01().
 */
RtkApiRet rtk_ptp_get_custom_damac01(UnitChip_t unitChip, PtpCustomDamac01_t *ptpCustomDamac01)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomDamac01)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_custom_damac01(unitChip.unit, ptpCustomDamac01->port, ptpCustomDamac01->damac);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_custom_damac01(unitChip.unit, ptpCustomDamac01->port, ptpCustomDamac01->damac);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_custom_damac01(unitChip.unit, ptpCustomDamac01->port, ptpCustomDamac01->damac);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_custom_damac01(unitChip.unit, ptpCustomDamac01->domainid, ptpCustomDamac01->port, ptpCustomDamac01->damac);
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
 * @brief When enabled, packets with a non-standard Destination MAC (DMAC) address are compared against custom DMAC settings. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpCustomDamac01Enable ptp custom damac01 enable set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_damac01_enable_custom(), cvb_ptp_set_damac01_enable_custom(), dv1_ptp_set_damac01_enable_custom(), dva_ptp_set_damac01_enable_custom().
 */
RtkApiRet rtk_ptp_set_damac01_enable_custom(UnitChip_t unitChip, PtpCustomDamac01Enable_t *ptpCustomDamac01Enable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomDamac01Enable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_damac01_enable_custom(unitChip.unit, ptpCustomDamac01Enable->port, ptpCustomDamac01Enable->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_damac01_enable_custom(unitChip.unit, ptpCustomDamac01Enable->port, ptpCustomDamac01Enable->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_damac01_enable_custom(unitChip.unit, ptpCustomDamac01Enable->port, ptpCustomDamac01Enable->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_damac01_enable_custom(unitChip.unit, ptpCustomDamac01Enable->domainid, ptpCustomDamac01Enable->port, ptpCustomDamac01Enable->enable);
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
 * @brief When enabled, packets with a non-standard Destination MAC (DMAC) address are validated against custom DMAC settings. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpCustomDamac01Enable ptp custom damac01 enable get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_damac01_enable_custom(), cvb_ptp_get_damac01_enable_custom(), dv1_ptp_get_damac01_enable_custom(), dva_ptp_get_damac01_enable_custom().
 */
RtkApiRet rtk_ptp_get_damac01_enable_custom(UnitChip_t unitChip, PtpCustomDamac01Enable_t *ptpCustomDamac01Enable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCustomDamac01Enable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_damac01_enable_custom(unitChip.unit, ptpCustomDamac01Enable->port, &ptpCustomDamac01Enable->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_damac01_enable_custom(unitChip.unit, ptpCustomDamac01Enable->port, &ptpCustomDamac01Enable->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_damac01_enable_custom(unitChip.unit, ptpCustomDamac01Enable->port, &ptpCustomDamac01Enable->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_damac01_enable_custom(unitChip.unit, ptpCustomDamac01Enable->domainid, ptpCustomDamac01Enable->port, &ptpCustomDamac01Enable->enable);
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
 * @brief Set the vlan tag information for the domain. The VLAN tag is required as per OPEN TC11. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpTag ptp tag set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_tag_set(), cvb_ptp_tag_set(), dv1_ptp_tag_set(), dva_ptp_tag_set().
 */
RtkApiRet rtk_ptp_tag_set(UnitChip_t unitChip, PtpTag_t *ptpTag)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpTag)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_tag_set(unitChip.unit, ptpTag->domainid, &ptpTag->taginfo);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_tag_set(unitChip.unit, ptpTag->domainid, &ptpTag->taginfo);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_tag_set(unitChip.unit, ptpTag->domainid, &ptpTag->taginfo);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_tag_set(unitChip.unit, ptpTag->domainid, &ptpTag->taginfo);
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
 * @brief Retrieve the hardware interface latency for either egress or ingress. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpInterfaceLatency ptp interface latency get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_interface_latency(), cvb_ptp_get_interface_latency(), dv1_ptp_get_interface_latency(), dva_ptp_get_interface_latency().
 */
RtkApiRet rtk_ptp_get_interface_latency(UnitChip_t unitChip, PtpInterfaceLatency_t *ptpInterfaceLatency)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInterfaceLatency)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_interface_latency(unitChip.unit, ptpInterfaceLatency->direction, ptpInterfaceLatency->interface, &ptpInterfaceLatency->latency);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_interface_latency(unitChip.unit, ptpInterfaceLatency->direction, ptpInterfaceLatency->interface, &ptpInterfaceLatency->latency);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_interface_latency(unitChip.unit, ptpInterfaceLatency->direction, ptpInterfaceLatency->interface, &ptpInterfaceLatency->latency);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_interface_latency(unitChip.unit, ptpInterfaceLatency->direction, ptpInterfaceLatency->interface, &ptpInterfaceLatency->latency);
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
 * @brief Retrieve the vlan tag information for the domain. The VLAN tag is required as per OPEN TC11. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpTag ptp tag get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_tag_get(), cvb_ptp_tag_get(), dv1_ptp_tag_get(), dva_ptp_tag_get().
 */
RtkApiRet rtk_ptp_tag_get(UnitChip_t unitChip, PtpTag_t *ptpTag)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpTag)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_tag_get(unitChip.unit, ptpTag->domainid, &ptpTag->taginfo);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_tag_get(unitChip.unit, ptpTag->domainid, &ptpTag->taginfo);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_tag_get(unitChip.unit, ptpTag->domainid, &ptpTag->taginfo);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_tag_get(unitChip.unit, ptpTag->domainid, &ptpTag->taginfo);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* PTP_PARSER */

/**
 * @defgroup PTP_PROTOCOL Protocol stack related configuration
 * @brief These APIs are used to get and set PTP protocol configurations.
 * @{
 */
/**
 * @brief Convert the log value to the corresponding message interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpLogToIntval ptp log to intval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_log_to_intval(), cvb_ptp_log_to_intval(), dv1_ptp_log_to_intval(), dva_ptp_log_to_intval().
 */
RtkApiRet rtk_ptp_log_to_intval(UnitChip_t unitChip, PtpLogToIntval_t *ptpLogToIntval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpLogToIntval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_log_to_intval(unitChip.unit, ptpLogToIntval->loginterval, &ptpLogToIntval->intval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_log_to_intval(unitChip.unit, ptpLogToIntval->loginterval, &ptpLogToIntval->intval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_log_to_intval(unitChip.unit, ptpLogToIntval->loginterval, &ptpLogToIntval->intval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_log_to_intval(unitChip.unit, ptpLogToIntval->loginterval, &ptpLogToIntval->intval);
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
 * @brief Retrieve the current Pdelay_request interval (in milliseconds) for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpCurntPdelayreqIntval ptp get curnt pdelayreq intval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_curnt_pdelayreq_intval(), cvb_ptp_get_curnt_pdelayreq_intval(), dv1_ptp_get_curnt_pdelayreq_intval(), dva_ptp_get_curnt_pdelayreq_intval().
 */
RtkApiRet rtk_ptp_get_curnt_pdelayreq_intval(UnitChip_t unitChip, PtpCurntPdelayreqIntval_t *ptpCurntPdelayreqIntval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCurntPdelayreqIntval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_curnt_pdelayreq_intval(unitChip.unit, ptpCurntPdelayreqIntval->port, &ptpCurntPdelayreqIntval->curntPdelayReqIntval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_curnt_pdelayreq_intval(unitChip.unit, ptpCurntPdelayreqIntval->port, &ptpCurntPdelayreqIntval->curntPdelayReqIntval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_curnt_pdelayreq_intval(unitChip.unit, ptpCurntPdelayreqIntval->port, &ptpCurntPdelayreqIntval->curntPdelayReqIntval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_curnt_pdelayreq_intval(unitChip.unit, ptpCurntPdelayreqIntval->port, &ptpCurntPdelayreqIntval->curntPdelayReqIntval);
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
 * @brief Set the current Pdelay_request interval (in milliseconds) for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpCurntPdelayreqIntval ptp set curnt pdelayreq intval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_curnt_pdelayreq_intval(), cvb_ptp_set_curnt_pdelayreq_intval(), dv1_ptp_set_curnt_pdelayreq_intval(), dva_ptp_set_curnt_pdelayreq_intval().
 */
RtkApiRet rtk_ptp_set_curnt_pdelayreq_intval(UnitChip_t unitChip, PtpCurntPdelayreqIntval_t *ptpCurntPdelayreqIntval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCurntPdelayreqIntval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_curnt_pdelayreq_intval(unitChip.unit, ptpCurntPdelayreqIntval->port, ptpCurntPdelayreqIntval->curntPdelayReqIntval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_curnt_pdelayreq_intval(unitChip.unit, ptpCurntPdelayreqIntval->port, ptpCurntPdelayreqIntval->curntPdelayReqIntval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_curnt_pdelayreq_intval(unitChip.unit, ptpCurntPdelayreqIntval->port, ptpCurntPdelayreqIntval->curntPdelayReqIntval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_curnt_pdelayreq_intval(unitChip.unit, ptpCurntPdelayreqIntval->port, ptpCurntPdelayreqIntval->curntPdelayReqIntval);
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
 * @brief Retrieve the hardware port mode for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPortmode ptp portmode get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_portmode(), cvb_ptp_get_portmode(), dv1_ptp_get_portmode(), dva_ptp_get_portmode().
 */
RtkApiRet rtk_ptp_get_portmode(UnitChip_t unitChip, PtpPortmode_t *ptpPortMode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortMode)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_portmode(unitChip.unit, ptpPortMode->port, &ptpPortMode->portmode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_portmode(unitChip.unit, ptpPortMode->port, &ptpPortMode->portmode);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_portmode(unitChip.unit, ptpPortMode->port, &ptpPortMode->portmode);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_portmode(unitChip.unit, ptpPortMode->port, &ptpPortMode->portmode);
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
 * @brief Set the hardware port mode for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPortmode ptp portmode set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_portmode(), cvb_ptp_set_portmode(), dv1_ptp_set_portmode(), dva_ptp_set_portmode().
 */
RtkApiRet rtk_ptp_set_portmode(UnitChip_t unitChip, PtpPortmode_t *ptpPortMode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortMode)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_portmode(unitChip.unit, ptpPortMode->port, ptpPortMode->portmode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_portmode(unitChip.unit, ptpPortMode->port, ptpPortMode->portmode);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_portmode(unitChip.unit, ptpPortMode->port, ptpPortMode->portmode);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_portmode(unitChip.unit, ptpPortMode->port, ptpPortMode->portmode);
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
 * @brief Retrieve the global enable status for PTP. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpGlenable ptp glenable get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_glenable(), cvb_ptp_get_glenable(), dv1_ptp_get_glenable(), dva_ptp_get_glenable().
 */
RtkApiRet rtk_ptp_get_glenable(UnitChip_t unitChip, PtpGlenable_t *ptpGlenable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpGlenable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_glenable(unitChip.unit, &ptpGlenable->glenable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_glenable(unitChip.unit, &ptpGlenable->glenable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_glenable(unitChip.unit, &ptpGlenable->glenable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_glenable(unitChip.unit, &ptpGlenable->glenable);
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
 * @brief Set the global enable status for PTP. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpGlenable ptp glenable set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_glenable(), cvb_ptp_set_glenable(), dv1_ptp_set_glenable(), dva_ptp_set_glenable().
 */
RtkApiRet rtk_ptp_set_glenable(UnitChip_t unitChip, PtpGlenable_t *ptpGlenable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpGlenable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_glenable(unitChip.unit, ptpGlenable->glenable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_glenable(unitChip.unit, ptpGlenable->glenable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_glenable(unitChip.unit, ptpGlenable->glenable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_glenable(unitChip.unit, ptpGlenable->glenable);
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
 * @brief Retrieve the configuration for the grandmaster. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpCfggm ptp cfggm get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_cfggm(), cvb_ptp_get_cfggm(), dv1_ptp_get_cfggm(), dva_ptp_get_cfggm().
 */
RtkApiRet rtk_ptp_get_cfggm(UnitChip_t unitChip, PtpCfggm_t *ptpCfggm)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCfggm)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_cfggm(unitChip.unit, &ptpCfggm->cfggm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_cfggm(unitChip.unit, &ptpCfggm->cfggm);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_cfggm(unitChip.unit, &ptpCfggm->cfggm);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_cfggm(unitChip.unit, &ptpCfggm->cfggm);
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
 * @brief Set the configuration for the grandmaster. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpCfggm ptp cfggm set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_cfggm(), cvb_ptp_set_cfggm(), dv1_ptp_set_cfggm(), dva_ptp_set_cfggm().
 */
RtkApiRet rtk_ptp_set_cfggm(UnitChip_t unitChip, PtpCfggm_t *ptpCfggm)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCfggm)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_cfggm(unitChip.unit, ptpCfggm->cfggm);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_cfggm(unitChip.unit, ptpCfggm->cfggm);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_cfggm(unitChip.unit, ptpCfggm->cfggm);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_cfggm(unitChip.unit, ptpCfggm->cfggm);
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
 * @brief Retrieve the enable status of the automotive profile. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpAutoasenable ptp autoasenable get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_autoasenable(), cvb_ptp_get_autoasenable(), dv1_ptp_get_autoasenable(), dva_ptp_get_autoasenable().
 */
RtkApiRet rtk_ptp_get_autoasenable(UnitChip_t unitChip, PtpAutoasenable_t *ptpAutoasenable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAutoasenable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_autoasenable(unitChip.unit, ptpAutoasenable->port, &ptpAutoasenable->autoasenable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_autoasenable(unitChip.unit, ptpAutoasenable->port, &ptpAutoasenable->autoasenable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_autoasenable(unitChip.unit, ptpAutoasenable->port, &ptpAutoasenable->autoasenable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_autoasenable(unitChip.unit, ptpAutoasenable->port, &ptpAutoasenable->autoasenable);
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
 * @brief Set the enable status of the automotive profile. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpAutoasenable ptp autoasenable set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_autoasenable(), cvb_ptp_set_autoasenable(), dv1_ptp_set_autoasenable(), dva_ptp_set_autoasenable().
 */
RtkApiRet rtk_ptp_set_autoasenable(UnitChip_t unitChip, PtpAutoasenable_t *ptpAutoasenable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAutoasenable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_autoasenable(unitChip.unit, ptpAutoasenable->port, ptpAutoasenable->autoasenable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_autoasenable(unitChip.unit, ptpAutoasenable->port, ptpAutoasenable->autoasenable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_autoasenable(unitChip.unit, ptpAutoasenable->port, ptpAutoasenable->autoasenable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_autoasenable(unitChip.unit, ptpAutoasenable->port, ptpAutoasenable->autoasenable);
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
 * @brief Retrieve the enable status for per-domain signaling. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSignalingen ptp signalingen get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_signalingen(), cvb_ptp_get_signalingen(), dv1_ptp_get_signalingen(), dva_ptp_get_signalingen().
 */
RtkApiRet rtk_ptp_get_signalingen(UnitChip_t unitChip, PtpSignalingen_t *ptpSignalingEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSignalingEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_signalingen(unitChip.unit, ptpSignalingEn->domainid, &ptpSignalingEn->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_signalingen(unitChip.unit, ptpSignalingEn->domainid, &ptpSignalingEn->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_signalingen(unitChip.unit, ptpSignalingEn->domainid, &ptpSignalingEn->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_signalingen(unitChip.unit, ptpSignalingEn->domainid, &ptpSignalingEn->enable);
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
 * @brief Set the enable status for per-domain signaling. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSignalingen ptp signalingen set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_signalingen(), cvb_ptp_set_signalingen(), dv1_ptp_set_signalingen(), dva_ptp_set_signalingen().
 */
RtkApiRet rtk_ptp_set_signalingen(UnitChip_t unitChip, PtpSignalingen_t *ptpSignalingEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSignalingEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_signalingen(unitChip.unit, ptpSignalingEn->domainid, ptpSignalingEn->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_signalingen(unitChip.unit, ptpSignalingEn->domainid, ptpSignalingEn->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_signalingen(unitChip.unit, ptpSignalingEn->domainid, ptpSignalingEn->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_signalingen(unitChip.unit, ptpSignalingEn->domainid, ptpSignalingEn->enable);
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
 * @brief Retrieve the ID of the time sync slave port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSlaveportid ptp slaveportid get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_slaveportid(), cvb_ptp_get_slaveportid(), dv1_ptp_get_slaveportid(), dva_ptp_get_slaveportid().
 */
RtkApiRet rtk_ptp_get_slaveportid(UnitChip_t unitChip, PtpSlaveportid_t *ptpSlaveportid)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlaveportid)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_slaveportid(unitChip.unit, ptpSlaveportid->domainid, &ptpSlaveportid->slaveportid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_slaveportid(unitChip.unit, ptpSlaveportid->domainid, &ptpSlaveportid->slaveportid);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_slaveportid(unitChip.unit, ptpSlaveportid->domainid, &ptpSlaveportid->slaveportid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_slaveportid(unitChip.unit, ptpSlaveportid->domainid, &ptpSlaveportid->slaveportid);
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
 * @brief Set the slave port for time synchronization. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSlaveportid ptp slaveportid set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_slaveportid(), cvb_ptp_set_slaveportid(), dv1_ptp_set_slaveportid(), dva_ptp_set_slaveportid().
 */
RtkApiRet rtk_ptp_set_slaveportid(UnitChip_t unitChip, PtpSlaveportid_t *ptpSlaveportid)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlaveportid)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_slaveportid(unitChip.unit, ptpSlaveportid->domainid, ptpSlaveportid->slaveportid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_slaveportid(unitChip.unit, ptpSlaveportid->domainid, ptpSlaveportid->slaveportid);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_slaveportid(unitChip.unit, ptpSlaveportid->domainid, ptpSlaveportid->slaveportid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_slaveportid(unitChip.unit, ptpSlaveportid->domainid, ptpSlaveportid->slaveportid);
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
 * @brief Retrieve the mask for the time sync slave port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSlaveportmask ptp slaveportmask get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_slaveportmask(), cvb_ptp_get_slaveportmask(), dv1_ptp_get_slaveportmask(), dva_ptp_get_slaveportmask().
 */
RtkApiRet rtk_ptp_get_slaveportmask(UnitChip_t unitChip, PtpSlaveportmask_t *ptpSlaveportmask)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlaveportmask)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_slaveportmask(unitChip.unit, &ptpSlaveportmask->slvMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_slaveportmask(unitChip.unit, &ptpSlaveportmask->slvMask);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_slaveportmask(unitChip.unit, &ptpSlaveportmask->slvMask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_slaveportmask(unitChip.unit, &ptpSlaveportmask->slvMask);
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
 * @brief Set the mask for the slave port to synchronize time with. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSlaveportmask ptp slaveportmask set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_slaveportmask(), cvb_ptp_set_slaveportmask(), dv1_ptp_set_slaveportmask(), dva_ptp_set_slaveportmask().
 */
RtkApiRet rtk_ptp_set_slaveportmask(UnitChip_t unitChip, PtpSlaveportmask_t *ptpSlaveportmask)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSlaveportmask)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_slaveportmask(unitChip.unit, ptpSlaveportmask->slvMask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_slaveportmask(unitChip.unit, ptpSlaveportmask->slvMask);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_slaveportmask(unitChip.unit, ptpSlaveportmask->slvMask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_slaveportmask(unitChip.unit, ptpSlaveportmask->slvMask);
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
 * @brief Retrieve the PTP priority1 value for sync/follow-up. The default is 246. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPriority1 ptp priority1 get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_priority1(), cvb_ptp_get_priority1(), dv1_ptp_get_priority1(), dva_ptp_get_priority1().
 */
RtkApiRet rtk_ptp_get_priority1(UnitChip_t unitChip, PtpPriority1_t *ptpPriority1)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPriority1)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_priority1(unitChip.unit, ptpPriority1->domainid, &ptpPriority1->priority1);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_priority1(unitChip.unit, ptpPriority1->domainid, &ptpPriority1->priority1);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_priority1(unitChip.unit, ptpPriority1->domainid, &ptpPriority1->priority1);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_priority1(unitChip.unit, ptpPriority1->domainid, &ptpPriority1->priority1);
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
 * @brief Set the PTP priority1 value for sync/follow-up. The default is 246. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPriority1 ptp priority1 set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_priority1(), cvb_ptp_set_priority1(), dv1_ptp_set_priority1(), dva_ptp_set_priority1().
 */
RtkApiRet rtk_ptp_set_priority1(UnitChip_t unitChip, PtpPriority1_t *ptpPriority1)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPriority1)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_priority1(unitChip.unit, ptpPriority1->domainid, ptpPriority1->priority1);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_priority1(unitChip.unit, ptpPriority1->domainid, ptpPriority1->priority1);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_priority1(unitChip.unit, ptpPriority1->domainid, ptpPriority1->priority1);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_priority1(unitChip.unit, ptpPriority1->domainid, ptpPriority1->priority1);
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
 * @brief Retrieve the threshold for path delay to determine if the calculated path delay is valid. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpDelaythresh ptp delaythresh get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_delaythresh(), cvb_ptp_get_delaythresh(), dv1_ptp_get_delaythresh(), dva_ptp_get_delaythresh().
 */
RtkApiRet rtk_ptp_get_delaythresh(UnitChip_t unitChip, PtpDelaythresh_t *ptpDelaythresh)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDelaythresh)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_delaythresh(unitChip.unit, &ptpDelaythresh->delaythresh);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_delaythresh(unitChip.unit, &ptpDelaythresh->delaythresh);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_delaythresh(unitChip.unit, &ptpDelaythresh->delaythresh);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_delaythresh(unitChip.unit, &ptpDelaythresh->delaythresh);
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
 * @brief Set the threshold for path delay to determine if the calculated path delay is valid. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpDelaythresh ptp delaythresh set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_delaythresh(), cvb_ptp_set_delaythresh(), dv1_ptp_set_delaythresh(), dva_ptp_set_delaythresh().
 */
RtkApiRet rtk_ptp_set_delaythresh(UnitChip_t unitChip, PtpDelaythresh_t *ptpDelaythresh)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDelaythresh)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_delaythresh(unitChip.unit, &ptpDelaythresh->delaythresh);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_delaythresh(unitChip.unit, &ptpDelaythresh->delaythresh);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_delaythresh(unitChip.unit, &ptpDelaythresh->delaythresh);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_delaythresh(unitChip.unit, &ptpDelaythresh->delaythresh);
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
 * @brief Retrieve the initial interval for the Pdelay request. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpInitlogpdelayintval ptp initlogpdelayintval get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_initlogpdelayintval(), cvb_ptp_get_initlogpdelayintval(), dv1_ptp_get_initlogpdelayintval(), dva_ptp_get_initlogpdelayintval().
 */
RtkApiRet rtk_ptp_get_initlogpdelayintval(UnitChip_t unitChip, PtpInitlogpdelayintval_t *ptpInitlogpdelayintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInitlogpdelayintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_initlogpdelayintval(unitChip.unit, ptpInitlogpdelayintval->port, &ptpInitlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_initlogpdelayintval(unitChip.unit, ptpInitlogpdelayintval->port, &ptpInitlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_initlogpdelayintval(unitChip.unit, ptpInitlogpdelayintval->port, &ptpInitlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_initlogpdelayintval(unitChip.unit, ptpInitlogpdelayintval->port, &ptpInitlogpdelayintval->pdelayintval);
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
 * @brief Set the initial interval of the Pdelay request. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpInitlogpdelayintval ptp initlogpdelayintval set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_initlogpdelayintval(), cvb_ptp_set_initlogpdelayintval(), dv1_ptp_set_initlogpdelayintval(), dva_ptp_set_initlogpdelayintval().
 */
RtkApiRet rtk_ptp_set_initlogpdelayintval(UnitChip_t unitChip, PtpInitlogpdelayintval_t *ptpInitlogpdelayintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInitlogpdelayintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_initlogpdelayintval(unitChip.unit, ptpInitlogpdelayintval->port, ptpInitlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_initlogpdelayintval(unitChip.unit, ptpInitlogpdelayintval->port, ptpInitlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_initlogpdelayintval(unitChip.unit, ptpInitlogpdelayintval->port, ptpInitlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_initlogpdelayintval(unitChip.unit, ptpInitlogpdelayintval->port, ptpInitlogpdelayintval->pdelayintval);
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
 * @brief Retrieve the operational sync interval value. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpOperlogsyncintval ptp operlogsyncintval get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_operlogsyncintval(), cvb_ptp_get_operlogsyncintval(), dv1_ptp_get_operlogsyncintval(), dva_ptp_get_operlogsyncintval().
 */
RtkApiRet rtk_ptp_get_operlogsyncintval(UnitChip_t unitChip, PtpOperlogsyncintval_t *ptpOperlogsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOperlogsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_operlogsyncintval(unitChip.unit, ptpOperlogsyncintval->domainid, &ptpOperlogsyncintval->syncintval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_operlogsyncintval(unitChip.unit, ptpOperlogsyncintval->domainid, &ptpOperlogsyncintval->syncintval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_operlogsyncintval(unitChip.unit, ptpOperlogsyncintval->domainid, &ptpOperlogsyncintval->syncintval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_operlogsyncintval(unitChip.unit, ptpOperlogsyncintval->domainid, &ptpOperlogsyncintval->syncintval);
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
 * @brief Set the operational sync interval value. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpOperlogsyncintval ptp operlogsyncintval set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_operlogsyncintval(), cvb_ptp_set_operlogsyncintval(), dv1_ptp_set_operlogsyncintval(), dva_ptp_set_operlogsyncintval().
 */
RtkApiRet rtk_ptp_set_operlogsyncintval(UnitChip_t unitChip, PtpOperlogsyncintval_t *ptpOperlogsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOperlogsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_operlogsyncintval(unitChip.unit, ptpOperlogsyncintval->domainid, ptpOperlogsyncintval->syncintval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_operlogsyncintval(unitChip.unit, ptpOperlogsyncintval->domainid, ptpOperlogsyncintval->syncintval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_operlogsyncintval(unitChip.unit, ptpOperlogsyncintval->domainid, ptpOperlogsyncintval->syncintval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_operlogsyncintval(unitChip.unit, ptpOperlogsyncintval->domainid, ptpOperlogsyncintval->syncintval);
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
 * @brief Retrieve the operated Pdelay interval of a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpOperlogpdelayintval ptp operlogpdelayintval get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_operlogpdelayintval(), cvb_ptp_get_operlogpdelayintval(), dv1_ptp_get_operlogpdelayintval(), dva_ptp_get_operlogpdelayintval().
 */
RtkApiRet rtk_ptp_get_operlogpdelayintval(UnitChip_t unitChip, PtpOperlogpdelayintval_t *ptpOperlogpdelayintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOperlogpdelayintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_operlogpdelayintval(unitChip.unit, ptpOperlogpdelayintval->port, &ptpOperlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_operlogpdelayintval(unitChip.unit, ptpOperlogpdelayintval->port, &ptpOperlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_operlogpdelayintval(unitChip.unit, ptpOperlogpdelayintval->port, &ptpOperlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_operlogpdelayintval(unitChip.unit, ptpOperlogpdelayintval->port, &ptpOperlogpdelayintval->pdelayintval);
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
 * @brief Set the operational PDelay interval for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpOperlogpdelayintval ptp operlogpdelayintval set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_operlogpdelayintval(), cvb_ptp_set_operlogpdelayintval(), dv1_ptp_set_operlogpdelayintval(), dva_ptp_set_operlogpdelayintval().
 */
RtkApiRet rtk_ptp_set_operlogpdelayintval(UnitChip_t unitChip, PtpOperlogpdelayintval_t *ptpOperlogpdelayintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOperlogpdelayintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_operlogpdelayintval(unitChip.unit, ptpOperlogpdelayintval->port, ptpOperlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_operlogpdelayintval(unitChip.unit, ptpOperlogpdelayintval->port, ptpOperlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_operlogpdelayintval(unitChip.unit, ptpOperlogpdelayintval->port, ptpOperlogpdelayintval->pdelayintval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_operlogpdelayintval(unitChip.unit, ptpOperlogpdelayintval->port, ptpOperlogpdelayintval->pdelayintval);
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
 * @brief Retrieve the enable status of the virtual channel. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpExchangeinfo ptp exchangeInfo get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_exchangeInfo(), cvb_ptp_get_exchangeInfo(), dv1_ptp_get_exchangeInfo(), dva_ptp_get_exchangeInfo().
 */
RtkApiRet rtk_ptp_get_exchangeInfo(UnitChip_t unitChip, PtpExchangeinfo_t *ptpExchangeinfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpExchangeinfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_exchangeInfo(unitChip.unit, &ptpExchangeinfo->exchangeInfo);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_exchangeInfo(unitChip.unit, &ptpExchangeinfo->exchangeInfo);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_exchangeInfo(unitChip.unit, &ptpExchangeinfo->exchangeInfo);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_exchangeInfo(unitChip.unit, &ptpExchangeinfo->exchangeInfo);
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
 * @brief Set the enable status of the virtual channel. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpExchangeinfo ptp exchangeInfo set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_exchangeInfo(), cvb_ptp_set_exchangeInfo(), dv1_ptp_set_exchangeInfo(), dva_ptp_set_exchangeInfo().
 */
RtkApiRet rtk_ptp_set_exchangeInfo(UnitChip_t unitChip, PtpExchangeinfo_t *ptpExchangeinfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpExchangeinfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_exchangeInfo(unitChip.unit, ptpExchangeinfo->exchangeInfo);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_exchangeInfo(unitChip.unit, ptpExchangeinfo->exchangeInfo);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_exchangeInfo(unitChip.unit, ptpExchangeinfo->exchangeInfo);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_exchangeInfo(unitChip.unit, ptpExchangeinfo->exchangeInfo);
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
 * @brief Retrieve the bootup time for grandmaster sync/follow. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpFirstbootuptimetarget ptp firstbootuptimetarget get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_firstbootuptimetarget(), cvb_ptp_get_firstbootuptimetarget(), dv1_ptp_get_firstbootuptimetarget(), dva_ptp_get_firstbootuptimetarget().
 */
RtkApiRet rtk_ptp_get_firstbootuptimetarget(UnitChip_t unitChip, PtpFirstbootuptimetarget_t *ptpFirstbootuptimetarget)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpFirstbootuptimetarget)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_firstbootuptimetarget(unitChip.unit, &ptpFirstbootuptimetarget->timetarget);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_firstbootuptimetarget(unitChip.unit, &ptpFirstbootuptimetarget->timetarget);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_firstbootuptimetarget(unitChip.unit, &ptpFirstbootuptimetarget->timetarget);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_firstbootuptimetarget(unitChip.unit, &ptpFirstbootuptimetarget->timetarget);
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
 * @brief Set the bootup time for grandmaster sync/follow. The default is 3 seconds. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpFirstbootuptimetarget ptp firstbootuptimetarget set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_firstbootuptimetarget(), cvb_ptp_set_firstbootuptimetarget(), dv1_ptp_set_firstbootuptimetarget(), dva_ptp_set_firstbootuptimetarget().
 */
RtkApiRet rtk_ptp_set_firstbootuptimetarget(UnitChip_t unitChip, PtpFirstbootuptimetarget_t *ptpFirstbootuptimetarget)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpFirstbootuptimetarget)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_firstbootuptimetarget(unitChip.unit, ptpFirstbootuptimetarget->timetarget);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_firstbootuptimetarget(unitChip.unit, ptpFirstbootuptimetarget->timetarget);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_firstbootuptimetarget(unitChip.unit, ptpFirstbootuptimetarget->timetarget);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_firstbootuptimetarget(unitChip.unit, ptpFirstbootuptimetarget->timetarget);
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
 * @brief Retrieve the current sync message interval value in milliseconds. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpCurntsyncintval ptp curntsyncintval get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_curntsyncintval(), cvb_ptp_get_curntsyncintval(), dv1_ptp_get_curntsyncintval(), dva_ptp_get_curntsyncintval().
 */
RtkApiRet rtk_ptp_get_curntsyncintval(UnitChip_t unitChip, PtpCurntsyncintval_t *ptpCurntsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCurntsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_curntsyncintval(unitChip.unit, ptpCurntsyncintval->port, ptpCurntsyncintval->domainid, &ptpCurntsyncintval->curntsyncintval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_curntsyncintval(unitChip.unit, ptpCurntsyncintval->port, ptpCurntsyncintval->domainid, &ptpCurntsyncintval->curntsyncintval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_curntsyncintval(unitChip.unit, ptpCurntsyncintval->port, ptpCurntsyncintval->domainid, &ptpCurntsyncintval->curntsyncintval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_curntsyncintval(unitChip.unit, ptpCurntsyncintval->port, ptpCurntsyncintval->domainid, &ptpCurntsyncintval->curntsyncintval);
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
 * @brief Set the current sync message interval value in milliseconds. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpCurntsyncintval ptp curntsyncintval set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_curntsyncintval(), cvb_ptp_set_curntsyncintval(), dv1_ptp_set_curntsyncintval(), dva_ptp_set_curntsyncintval().
 */
RtkApiRet rtk_ptp_set_curntsyncintval(UnitChip_t unitChip, PtpCurntsyncintval_t *ptpCurntsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCurntsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_curntsyncintval(unitChip.unit, ptpCurntsyncintval->port, ptpCurntsyncintval->domainid, ptpCurntsyncintval->curntsyncintval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_curntsyncintval(unitChip.unit, ptpCurntsyncintval->port, ptpCurntsyncintval->domainid, ptpCurntsyncintval->curntsyncintval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_curntsyncintval(unitChip.unit, ptpCurntsyncintval->port, ptpCurntsyncintval->domainid, ptpCurntsyncintval->curntsyncintval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_curntsyncintval(unitChip.unit, ptpCurntsyncintval->port, ptpCurntsyncintval->domainid, ptpCurntsyncintval->curntsyncintval);
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
 * @brief Retrieve the protocol port mode: Grandmaster (GM) (0x0), Peer-to-Peer (P2P) (0x3)
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSwPortmode ptp sw portmode get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_sw_portmode(), cvb_ptp_get_sw_portmode(), dv1_ptp_get_sw_portmode(), dva_ptp_get_sw_portmode().
 */
RtkApiRet rtk_ptp_get_sw_portmode(UnitChip_t unitChip, PtpSwPortmode_t *ptpSwPortmode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSwPortmode)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_sw_portmode(unitChip.unit, ptpSwPortmode->port, ptpSwPortmode->domainid, &ptpSwPortmode->portmode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_sw_portmode(unitChip.unit, ptpSwPortmode->port, ptpSwPortmode->domainid, &ptpSwPortmode->portmode);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_sw_portmode(unitChip.unit, ptpSwPortmode->port, ptpSwPortmode->domainid, &ptpSwPortmode->portmode);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_sw_portmode(unitChip.unit, ptpSwPortmode->port, ptpSwPortmode->domainid, &ptpSwPortmode->portmode);
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
 * @brief Set the protocol port mode: Grandmaster (GM) (0x0), Peer-to-Peer (P2P) (0x3)
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSwPortmode ptp sw portmode set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_sw_portmode(), cvb_ptp_set_sw_portmode(), dv1_ptp_set_sw_portmode(), dva_ptp_set_sw_portmode().
 */
RtkApiRet rtk_ptp_set_sw_portmode(UnitChip_t unitChip, PtpSwPortmode_t *ptpSwPortmode)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSwPortmode)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_sw_portmode(unitChip.unit, ptpSwPortmode->port, ptpSwPortmode->domainid, ptpSwPortmode->portmode);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_sw_portmode(unitChip.unit, ptpSwPortmode->port, ptpSwPortmode->domainid, ptpSwPortmode->portmode);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_sw_portmode(unitChip.unit, ptpSwPortmode->port, ptpSwPortmode->domainid, ptpSwPortmode->portmode);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_sw_portmode(unitChip.unit, ptpSwPortmode->port, ptpSwPortmode->domainid, ptpSwPortmode->portmode);
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
 * @brief Retrieve the initial sync interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpInitlogsyncintval ptp initlogsyncintval get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_initlogsyncintval(), cvb_ptp_get_initlogsyncintval(), dv1_ptp_get_initlogsyncintval(), dva_ptp_get_initlogsyncintval().
 */
RtkApiRet rtk_ptp_get_initlogsyncintval(UnitChip_t unitChip, PtpInitlogsyncintval_t *ptpInitlogsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInitlogsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_initlogsyncintval(unitChip.unit, ptpInitlogsyncintval->port, ptpInitlogsyncintval->domainid, &ptpInitlogsyncintval->initLogSyncIntval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_initlogsyncintval(unitChip.unit, ptpInitlogsyncintval->port, ptpInitlogsyncintval->domainid, &ptpInitlogsyncintval->initLogSyncIntval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_initlogsyncintval(unitChip.unit, ptpInitlogsyncintval->port, ptpInitlogsyncintval->domainid, &ptpInitlogsyncintval->initLogSyncIntval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_initlogsyncintval(unitChip.unit, ptpInitlogsyncintval->port, ptpInitlogsyncintval->domainid, &ptpInitlogsyncintval->initLogSyncIntval);
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
 * @brief Set the initial sync interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpInitlogsyncintval ptp initlogsyncintval set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_initlogsyncintval(), cvb_ptp_set_initlogsyncintval(), dv1_ptp_set_initlogsyncintval(), dva_ptp_set_initlogsyncintval().
 */
RtkApiRet rtk_ptp_set_initlogsyncintval(UnitChip_t unitChip, PtpInitlogsyncintval_t *ptpInitlogsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInitlogsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_initlogsyncintval(unitChip.unit, ptpInitlogsyncintval->port, ptpInitlogsyncintval->domainid, ptpInitlogsyncintval->initLogSyncIntval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_initlogsyncintval(unitChip.unit, ptpInitlogsyncintval->port, ptpInitlogsyncintval->domainid, ptpInitlogsyncintval->initLogSyncIntval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_initlogsyncintval(unitChip.unit, ptpInitlogsyncintval->port, ptpInitlogsyncintval->domainid, ptpInitlogsyncintval->initLogSyncIntval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_initlogsyncintval(unitChip.unit, ptpInitlogsyncintval->port, ptpInitlogsyncintval->domainid, ptpInitlogsyncintval->initLogSyncIntval);
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
 * @brief Retrieve the domain number. This is a per-port setting and should be the same for each port if they join the same domain. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpDomainnum ptp domainnum get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_domainnum(), cvb_ptp_get_domainnum(), dv1_ptp_get_domainnum(), dva_ptp_get_domainnum().
 */
RtkApiRet rtk_ptp_get_domainnum(UnitChip_t unitChip, PtpDomainnum_t *ptpDomainnum)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDomainnum)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_domainnum(unitChip.unit, ptpDomainnum->port, ptpDomainnum->domainid, &ptpDomainnum->domainnum);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_domainnum(unitChip.unit, ptpDomainnum->port, ptpDomainnum->domainid, &ptpDomainnum->domainnum);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_domainnum(unitChip.unit, ptpDomainnum->port, ptpDomainnum->domainid, &ptpDomainnum->domainnum);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_domainnum(unitChip.unit, ptpDomainnum->port, ptpDomainnum->domainid, &ptpDomainnum->domainnum);
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
 * @brief Set the domain number. This is a per-port setting and should be the same for each port if they join the same domain. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpDomainnum ptp domainnum set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_domainnum(), cvb_ptp_set_domainnum(), dv1_ptp_set_domainnum(), dva_ptp_set_domainnum().
 */
RtkApiRet rtk_ptp_set_domainnum(UnitChip_t unitChip, PtpDomainnum_t *ptpDomainnum)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDomainnum)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_domainnum(unitChip.unit, ptpDomainnum->port, ptpDomainnum->domainid, ptpDomainnum->domainnum);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_domainnum(unitChip.unit, ptpDomainnum->port, ptpDomainnum->domainid, ptpDomainnum->domainnum);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_domainnum(unitChip.unit, ptpDomainnum->port, ptpDomainnum->domainid, ptpDomainnum->domainnum);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_domainnum(unitChip.unit, ptpDomainnum->port, ptpDomainnum->domainid, ptpDomainnum->domainnum);
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
 * @brief Retrieve the ports that join the specified domain. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpDomainmap ptp domainmap get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_domainmap(), cvb_ptp_get_domainmap(), dv1_ptp_get_domainmap(), dva_ptp_get_domainmap().
 */
RtkApiRet rtk_ptp_get_domainmap(UnitChip_t unitChip, PtpDomainmap_t *ptpDomainmap)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDomainmap)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_domainmap(unitChip.unit, ptpDomainmap->domainid, &ptpDomainmap->domainmap);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_domainmap(unitChip.unit, ptpDomainmap->domainid, &ptpDomainmap->domainmap);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_domainmap(unitChip.unit, ptpDomainmap->domainid, &ptpDomainmap->domainmap);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_domainmap(unitChip.unit, ptpDomainmap->domainid, &ptpDomainmap->domainmap);
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
 * @brief Set the ports that join the specified domain. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpDomainmap ptp domainmap set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_domainmap(), cvb_ptp_set_domainmap(), dv1_ptp_set_domainmap(), dva_ptp_set_domainmap().
 */
RtkApiRet rtk_ptp_set_domainmap(UnitChip_t unitChip, PtpDomainmap_t *ptpDomainmap)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDomainmap)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_domainmap(unitChip.unit, ptpDomainmap->domainid, ptpDomainmap->domainmap);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_domainmap(unitChip.unit, ptpDomainmap->domainid, ptpDomainmap->domainmap);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_domainmap(unitChip.unit, ptpDomainmap->domainid, ptpDomainmap->domainmap);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_domainmap(unitChip.unit, ptpDomainmap->domainid, ptpDomainmap->domainmap);
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
 * @brief Retrieve the port remapping index. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPortremapping ptp portremapping get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_portremapping(), cvb_ptp_get_portremapping(), dv1_ptp_get_portremapping(), dva_ptp_get_portremapping().
 */
RtkApiRet rtk_ptp_get_portremapping(UnitChip_t unitChip, PtpPortremapping_t *ptpPortRemapping)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortRemapping)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_portremapping(unitChip.unit, ptpPortRemapping->port, &ptpPortRemapping->remapping);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_portremapping(unitChip.unit, ptpPortRemapping->port, &ptpPortRemapping->remapping);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_portremapping(unitChip.unit, ptpPortRemapping->port, &ptpPortRemapping->remapping);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_portremapping(unitChip.unit, ptpPortRemapping->port, &ptpPortRemapping->remapping);
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
 * @brief Configure the port remapping index. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPortremapping ptp portremapping set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_portremapping(), cvb_ptp_set_portremapping(), dv1_ptp_set_portremapping(), dva_ptp_set_portremapping().
 */
RtkApiRet rtk_ptp_set_portremapping(UnitChip_t unitChip, PtpPortremapping_t *ptpPortRemapping)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortRemapping)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_portremapping(unitChip.unit, ptpPortRemapping->port, ptpPortRemapping->remapping);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_portremapping(unitChip.unit, ptpPortRemapping->port, ptpPortRemapping->remapping);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_portremapping(unitChip.unit, ptpPortRemapping->port, ptpPortRemapping->remapping);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_portremapping(unitChip.unit, ptpPortRemapping->port, ptpPortRemapping->remapping);
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
 * @brief Retrieve the enable status of the port remapping function. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPortremappingen ptp portremappingen get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_en_portremapping(), cvb_ptp_get_en_portremapping(), dv1_ptp_get_en_portremapping(), dva_ptp_get_en_portremapping().
 */
RtkApiRet rtk_ptp_get_en_portremapping(UnitChip_t unitChip, PtpPortremappingen_t *ptpPortRemappingEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortRemappingEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_en_portremapping(unitChip.unit, &ptpPortRemappingEn->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_en_portremapping(unitChip.unit, &ptpPortRemappingEn->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_en_portremapping(unitChip.unit, &ptpPortRemappingEn->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_en_portremapping(unitChip.unit, &ptpPortRemappingEn->enable);
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
 * @brief Configure the enable status of the port remapping function. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPortremappingen ptp portremappingen set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_en_portremapping(), cvb_ptp_set_en_portremapping(), dv1_ptp_set_en_portremapping(), dva_ptp_set_en_portremapping().
 */
RtkApiRet rtk_ptp_set_en_portremapping(UnitChip_t unitChip, PtpPortremappingen_t *ptpPortRemappingEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPortRemappingEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_en_portremapping(unitChip.unit, ptpPortRemappingEn->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_en_portremapping(unitChip.unit, ptpPortRemappingEn->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_en_portremapping(unitChip.unit, ptpPortRemappingEn->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_en_portremapping(unitChip.unit, ptpPortRemappingEn->enable);
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
 * @brief Retrieve the 1-Step transmit status (Hardware configuration). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpOnesteptxsync ptp onesteptxsync get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_onesteptxsync(), cvb_ptp_get_onesteptxsync(), dv1_ptp_get_onesteptxsync(), dva_ptp_get_onesteptxsync().
 */
RtkApiRet rtk_ptp_get_onesteptxsync(UnitChip_t unitChip, PtpOnesteptxsync_t *ptpOnestepTxSync)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOnestepTxSync)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_onesteptxsync(unitChip.unit, ptpOnestepTxSync->port, &ptpOnestepTxSync->oneStepTxSync);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_onesteptxsync(unitChip.unit, ptpOnestepTxSync->port, &ptpOnestepTxSync->oneStepTxSync);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_onesteptxsync(unitChip.unit, ptpOnestepTxSync->port, &ptpOnestepTxSync->oneStepTxSync);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_onesteptxsync(unitChip.unit, ptpOnestepTxSync->port, &ptpOnestepTxSync->oneStepTxSync);
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
 * @brief Configure the 1-Step transmit status (Hardware configuration). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpOnesteptxsync ptp onesteptxsync set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_onesteptxsync(), cvb_ptp_set_onesteptxsync(), dv1_ptp_set_onesteptxsync(), dva_ptp_set_onesteptxsync().
 */
RtkApiRet rtk_ptp_set_onesteptxsync(UnitChip_t unitChip, PtpOnesteptxsync_t *ptpOnestepTxSync)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOnestepTxSync)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_onesteptxsync(unitChip.unit, ptpOnestepTxSync->port, ptpOnestepTxSync->oneStepTxSync);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_onesteptxsync(unitChip.unit, ptpOnestepTxSync->port, ptpOnestepTxSync->oneStepTxSync);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_onesteptxsync(unitChip.unit, ptpOnestepTxSync->port, ptpOnestepTxSync->oneStepTxSync);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_onesteptxsync(unitChip.unit, ptpOnestepTxSync->port, ptpOnestepTxSync->oneStepTxSync);
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
 * @brief Reinitialize PTP protocol configurations. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_reinitprotocol(), cvb_ptp_reinitprotocol(), dv1_ptp_reinitprotocol(), dva_ptp_reinitprotocol().
 */
RtkApiRet rtk_ptp_reinitprotocol(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_ptp_reinitprotocol(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_ptp_reinitprotocol(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_ptp_reinitprotocol(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_ptp_reinitprotocol(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Configure the PTP protocol port mask. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpProtocolportmask ptp protocolportmask set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_protocolportmask_set(), cvb_ptp_protocolportmask_set(), dv1_ptp_protocolportmask_set(), dva_ptp_protocolportmask_set().
 */
RtkApiRet rtk_ptp_protocolportmask_set(UnitChip_t unitChip, PtpProtocolportmask_t *ptpProtocolportmask)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpProtocolportmask)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_protocolportmask_set(unitChip.unit, ptpProtocolportmask->domainid, ptpProtocolportmask->portmask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_protocolportmask_set(unitChip.unit, ptpProtocolportmask->domainid, ptpProtocolportmask->portmask);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_protocolportmask_set(unitChip.unit, ptpProtocolportmask->domainid, ptpProtocolportmask->portmask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_protocolportmask_set(unitChip.unit, ptpProtocolportmask->domainid, ptpProtocolportmask->portmask);
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
 * @brief Do not perform recovery if in first boot status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSkipfirstbootrecovery ptp skipfirstbootrecovery set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_skipfirstbootrecovery(), cvb_ptp_set_skipfirstbootrecovery(), dv1_ptp_set_skipfirstbootrecovery(), dva_ptp_set_skipfirstbootrecovery().
 */
RtkApiRet rtk_ptp_set_skipfirstbootrecovery(UnitChip_t unitChip, PtpSkipfirstbootrecovery_t *ptpSkipfirstbootrecovery)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSkipfirstbootrecovery)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_skipfirstbootrecovery(unitChip.unit, ptpSkipfirstbootrecovery->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_skipfirstbootrecovery(unitChip.unit, ptpSkipfirstbootrecovery->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_skipfirstbootrecovery(unitChip.unit, ptpSkipfirstbootrecovery->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_skipfirstbootrecovery(unitChip.unit, ptpSkipfirstbootrecovery->enable);
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
 * @brief If the DUT never receives sync from the slave port, do not perform recovery. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSilenceifneversynced ptp silenceifneversynced set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_silenceifneversynced(), cvb_ptp_set_silenceifneversynced(), dv1_ptp_set_silenceifneversynced(), dva_ptp_set_silenceifneversynced().
 */
RtkApiRet rtk_ptp_set_silenceifneversynced(UnitChip_t unitChip, PtpSilenceifneversynced_t *ptpSilenceifneversynced)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSilenceifneversynced)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_silenceifneversynced(unitChip.unit, ptpSilenceifneversynced->domainid, ptpSilenceifneversynced->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_silenceifneversynced(unitChip.unit, ptpSilenceifneversynced->domainid, ptpSilenceifneversynced->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_silenceifneversynced(unitChip.unit, ptpSilenceifneversynced->domainid, ptpSilenceifneversynced->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_silenceifneversynced(unitChip.unit, ptpSilenceifneversynced->domainid, ptpSilenceifneversynced->enable);
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
 * @brief Set overwrite with my clock ID. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpOverwritePtpid ptp overwrite ptpid set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_overwrite_ptpid_set(), cvb_ptp_overwrite_ptpid_set(), dv1_ptp_overwrite_ptpid_set(), dva_ptp_overwrite_ptpid_set().
 */
RtkApiRet rtk_ptp_overwrite_ptpid_set(UnitChip_t unitChip, PtpOverwritePtpid_t *ptpOverwritePtpid)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOverwritePtpid)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_overwrite_ptpid_set(unitChip.unit, ptpOverwritePtpid->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_overwrite_ptpid_set(unitChip.unit, ptpOverwritePtpid->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_overwrite_ptpid_set(unitChip.unit, ptpOverwritePtpid->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_overwrite_ptpid_set(unitChip.unit, ptpOverwritePtpid->enable);
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
 * @brief Send out Pdelay_req from the master port if configured. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSendPdelayFromMasterport ptp set send pdelay from masterport information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_send_pdelay_from_masterport(), cvb_ptp_set_send_pdelay_from_masterport(), dv1_ptp_set_send_pdelay_from_masterport(), dva_ptp_set_send_pdelay_from_masterport().
 */
RtkApiRet rtk_ptp_set_send_pdelay_from_masterport(UnitChip_t unitChip, PtpSendPdelayFromMasterport_t *ptpSendPdelayFromMasterport)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSendPdelayFromMasterport)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_send_pdelay_from_masterport(unitChip.unit, ptpSendPdelayFromMasterport->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_send_pdelay_from_masterport(unitChip.unit, ptpSendPdelayFromMasterport->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_send_pdelay_from_masterport(unitChip.unit, ptpSendPdelayFromMasterport->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_send_pdelay_from_masterport(unitChip.unit, ptpSendPdelayFromMasterport->enable);
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
 * @brief Configure 1-Step transmit (Protocol configuration). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpOnesteptxenSw ptp onesteptxen sw set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_onesteptxen_sw_set(), cvb_ptp_onesteptxen_sw_set(), dv1_ptp_onesteptxen_sw_set(), dva_ptp_onesteptxen_sw_set().
 */
RtkApiRet rtk_ptp_onesteptxen_sw_set(UnitChip_t unitChip, PtpOnesteptxenSw_t *ptpOnesteptxenSw)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOnesteptxenSw)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_onesteptxen_sw_set(unitChip.unit, ptpOnesteptxenSw->port, ptpOnesteptxenSw->domainid, ptpOnesteptxenSw->oneStepTxSync);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_onesteptxen_sw_set(unitChip.unit, ptpOnesteptxenSw->port, ptpOnesteptxenSw->domainid, ptpOnesteptxenSw->oneStepTxSync);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_onesteptxen_sw_set(unitChip.unit, ptpOnesteptxenSw->port, ptpOnesteptxenSw->domainid, ptpOnesteptxenSw->oneStepTxSync);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_onesteptxen_sw_set(unitChip.unit, ptpOnesteptxenSw->port, ptpOnesteptxenSw->domainid, ptpOnesteptxenSw->oneStepTxSync);
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
 * @brief rxSyncCount: A counter that increments every time a synchronization message is received. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get rx sync count information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_rx_sync_mib_get_count(), cvb_ptp_rx_sync_mib_get_count(), dv1_ptp_rx_sync_mib_get_count(), dva_ptp_rx_sync_mib_get_count().
 */
RtkApiRet rtk_ptp_rx_sync_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_rx_sync_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_rx_sync_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_rx_sync_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_rx_sync_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief rxFollowUpCount: A counter that increments every time a Follow_Up message is received. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get rx follow up count information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_rx_follow_up_mib_get_count(), cvb_ptp_rx_follow_up_mib_get_count(), dv1_ptp_rx_follow_up_mib_get_count(), dva_ptp_rx_follow_up_mib_get_count().
 */
RtkApiRet rtk_ptp_rx_follow_up_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_rx_follow_up_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_rx_follow_up_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_rx_follow_up_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_rx_follow_up_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief rxPdelayRequestCount: A counter that increments every time a Pdelay_Req message is received. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get rx pdelay request cont information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_rx_pdelay_request_mib_get_cont(), cvb_ptp_rx_pdelay_request_mib_get_cont(), dv1_ptp_rx_pdelay_request_mib_get_cont(), dva_ptp_rx_pdelay_request_mib_get_cont().
 */
RtkApiRet rtk_ptp_rx_pdelay_request_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_rx_pdelay_request_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_rx_pdelay_request_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_rx_pdelay_request_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_rx_pdelay_request_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief rxPdelayResponseCount: A counter that increments every time a Pdelay_Resp message is received. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get rx pdelay response cont information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_rx_pdelay_response_mib_get_cont(), cvb_ptp_rx_pdelay_response_mib_get_cont(), dv1_ptp_rx_pdelay_response_mib_get_cont(), dva_ptp_rx_pdelay_response_mib_get_cont().
 */
RtkApiRet rtk_ptp_rx_pdelay_response_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_rx_pdelay_response_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_rx_pdelay_response_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_rx_pdelay_response_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_rx_pdelay_response_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief rxPdelayResponseFollowUpCount: A counter that increments every time a Pdelay_Resp_Follow_Up message is received. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get rx pdelay resp follow up information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly..
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_rx_pdelay_resp_follow_up_mib_get(), cvb_ptp_rx_pdelay_resp_follow_up_mib_get(), dv1_ptp_rx_pdelay_resp_follow_up_mib_get(), dva_ptp_rx_pdelay_resp_follow_up_mib_get().
 */
RtkApiRet rtk_ptp_rx_pdelay_resp_follow_up_mib_get(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_rx_pdelay_resp_follow_up_mib_get(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_rx_pdelay_resp_follow_up_mib_get(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_rx_pdelay_resp_follow_up_mib_get(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_rx_pdelay_resp_follow_up_mib_get(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief rxPTPPacketDiscardCount: A counter that increments every time a PTP message is discarded. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get rx ptp packet discard cont information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_rx_ptp_packet_discard_mib_get_cont(), cvb_ptp_rx_ptp_packet_discard_mib_get_cont(), dv1_ptp_rx_ptp_packet_discard_mib_get_cont(), dva_ptp_rx_ptp_packet_discard_mib_get_cont().
 */
RtkApiRet rtk_ptp_rx_ptp_packet_discard_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_rx_ptp_packet_discard_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_rx_ptp_packet_discard_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_rx_ptp_packet_discard_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_rx_ptp_packet_discard_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief syncReceiptTimeoutCount: A counter that increments every time a sync receipt timeout occurs. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get sync receipt timeout count information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_sync_receipt_timeout_mib_get_count(), cvb_ptp_sync_receipt_timeout_mib_get_count(), dv1_ptp_sync_receipt_timeout_mib_get_count(), dva_ptp_sync_receipt_timeout_mib_get_count().
 */
RtkApiRet rtk_ptp_sync_receipt_timeout_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_sync_receipt_timeout_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_sync_receipt_timeout_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_sync_receipt_timeout_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_sync_receipt_timeout_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief pdelayAllowedLostResponsesExceededCount: A counter that increments when a Pdelay lost response occurs. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get pdelay allowed lost response count information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_pdelay_allowed_lost_response_mib_get_count(), cvb_ptp_pdelay_allowed_lost_response_mib_get_count(), dv1_ptp_pdelay_allowed_lost_response_mib_get_count(), dva_ptp_pdelay_allowed_lost_response_mib_get_count().
 */
RtkApiRet rtk_ptp_pdelay_allowed_lost_response_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_pdelay_allowed_lost_response_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_pdelay_allowed_lost_response_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_pdelay_allowed_lost_response_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_pdelay_allowed_lost_response_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief txSyncCount: A counter that increments every time synchronization information is transmitted. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMibGet ptp mib get tx sync count information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_tx_sync_mib_get_count(), cvb_ptp_tx_sync_mib_get_count(), dv1_ptp_tx_sync_mib_get_count(), dva_ptp_tx_sync_mib_get_count().
 */
RtkApiRet rtk_ptp_tx_sync_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_tx_sync_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_tx_sync_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_tx_sync_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_tx_sync_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief txFollowUpCount: A counter that increments every time a Follow_Up message is transmitted. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMibGet ptp mib get tx follow up count information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_tx_follow_up_mib_get_count(), cvb_ptp_tx_follow_up_mib_get_count(), dv1_ptp_tx_follow_up_mib_get_count(), dva_ptp_tx_follow_up_mib_get_count().
 */
RtkApiRet rtk_ptp_tx_follow_up_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_tx_follow_up_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_tx_follow_up_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_tx_follow_up_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_tx_follow_up_mib_get_count(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief txPdelayRequestCount: A counter that increments every time a Pdelay_Req message is transmitted. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get tx pdelay request cont information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_tx_pdelay_request_mib_get_cont(), cvb_ptp_tx_pdelay_request_mib_get_cont(), dv1_ptp_tx_pdelay_request_mib_get_cont(), dva_ptp_tx_pdelay_request_mib_get_cont().
 */
RtkApiRet rtk_ptp_tx_pdelay_request_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_tx_pdelay_request_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_tx_pdelay_request_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_tx_pdelay_request_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_tx_pdelay_request_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief txPdelayResponseCount: A counter that increments every time a Pdelay_Resp message is transmitted. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get tx pdelay response cont information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_tx_pdelay_response_mib_get_cont(), cvb_ptp_tx_pdelay_response_mib_get_cont(), dv1_ptp_tx_pdelay_response_mib_get_cont(), dva_ptp_tx_pdelay_response_mib_get_cont().
 */
RtkApiRet rtk_ptp_tx_pdelay_response_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_tx_pdelay_response_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_tx_pdelay_response_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_tx_pdelay_response_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_tx_pdelay_response_mib_get_cont(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief txPdelayResponseFollowUpCount: A counter that increments every time a Pdelay_Resp_Follow_Up message is transmitted. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMib ptp mib get tx pdelay resp follow up information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_tx_pdelay_resp_follow_up_mib_get(), cvb_ptp_tx_pdelay_resp_follow_up_mib_get(), dv1_ptp_tx_pdelay_resp_follow_up_mib_get(), dva_ptp_tx_pdelay_resp_follow_up_mib_get().
 */
RtkApiRet rtk_ptp_tx_pdelay_resp_follow_up_mib_get(UnitChip_t unitChip, PtpMib_t *ptpMib)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMib)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_tx_pdelay_resp_follow_up_mib_get(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_tx_pdelay_resp_follow_up_mib_get(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_tx_pdelay_resp_follow_up_mib_get(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_tx_pdelay_resp_follow_up_mib_get(unitChip.unit, ptpMib->port, ptpMib->domainid, &ptpMib->mib);
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
 * @brief Configure 1-Step Receive (Protocol configuration). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpOnesteprxenSw ptp onesteprxen sw set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_onesteprxen_sw_set(), cvb_ptp_onesteprxen_sw_set(), dv1_ptp_onesteprxen_sw_set(), dva_ptp_onesteprxen_sw_set().
 */
RtkApiRet rtk_ptp_onesteprxen_sw_set(UnitChip_t unitChip, PtpOnesteprxenSw_t *ptpOnesteprxenSw)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOnesteprxenSw)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_onesteprxen_sw_set(unitChip.unit, ptpOnesteprxenSw->port, ptpOnesteprxenSw->domainid, ptpOnesteprxenSw->oneStepRxSync);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_onesteprxen_sw_set(unitChip.unit, ptpOnesteprxenSw->port, ptpOnesteprxenSw->domainid, ptpOnesteprxenSw->oneStepRxSync);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_onesteprxen_sw_set(unitChip.unit, ptpOnesteprxenSw->port, ptpOnesteprxenSw->domainid, ptpOnesteprxenSw->oneStepRxSync);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_onesteprxen_sw_set(unitChip.unit, ptpOnesteprxenSw->port, ptpOnesteprxenSw->domainid, ptpOnesteprxenSw->oneStepRxSync);
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
 * @brief Configure the initialization for One Step Transmit operability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpInitonesteptxoper ptp initonesteptxoper set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_initonesteptxoper(), cvb_ptp_set_initonesteptxoper(), dv1_ptp_set_initonesteptxoper(), dva_ptp_set_initonesteptxoper().
 */
RtkApiRet rtk_ptp_set_initonesteptxoper(UnitChip_t unitChip, PtpInitonesteptxoper_t *ptpInitonesteptxoper)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInitonesteptxoper)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_initonesteptxoper(unitChip.unit, ptpInitonesteptxoper->port, ptpInitonesteptxoper->domainid, ptpInitonesteptxoper->initOneStepTXOperEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_initonesteptxoper(unitChip.unit, ptpInitonesteptxoper->port, ptpInitonesteptxoper->domainid, ptpInitonesteptxoper->initOneStepTXOperEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_initonesteptxoper(unitChip.unit, ptpInitonesteptxoper->port, ptpInitonesteptxoper->domainid, ptpInitonesteptxoper->initOneStepTXOperEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_initonesteptxoper(unitChip.unit, ptpInitonesteptxoper->port, ptpInitonesteptxoper->domainid, ptpInitonesteptxoper->initOneStepTXOperEn);
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
 * @brief Retrieve the initialization status for One Step Transmit operation. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpInitonesteptxoper ptp initonesteptxoper get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_initonesteptxoper(), cvb_ptp_get_initonesteptxoper(), dv1_ptp_get_initonesteptxoper(), dva_ptp_get_initonesteptxoper().
 */
RtkApiRet rtk_ptp_get_initonesteptxoper(UnitChip_t unitChip, PtpInitonesteptxoper_t *ptpInitonesteptxoper)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInitonesteptxoper)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_initonesteptxoper(unitChip.unit, ptpInitonesteptxoper->port, ptpInitonesteptxoper->domainid, &ptpInitonesteptxoper->initOneStepTXOperEn);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_initonesteptxoper(unitChip.unit, ptpInitonesteptxoper->port, ptpInitonesteptxoper->domainid, &ptpInitonesteptxoper->initOneStepTXOperEn);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_initonesteptxoper(unitChip.unit, ptpInitonesteptxoper->port, ptpInitonesteptxoper->domainid, &ptpInitonesteptxoper->initOneStepTXOperEn);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_initonesteptxoper(unitChip.unit, ptpInitonesteptxoper->port, ptpInitonesteptxoper->domainid, &ptpInitonesteptxoper->initOneStepTXOperEn);
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
 * @brief Retrieve the PTP flash access flag status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpFlashRuntimeAccess ptp flash runtime access get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_flash_runtime_access(), cvb_ptp_get_flash_runtime_access(), dv1_ptp_get_flash_runtime_access(), dva_ptp_get_flash_runtime_access().
 */
RtkApiRet rtk_ptp_get_flash_runtime_access(UnitChip_t unitChip, PtpFlashRuntimeAccess_t *ptpFlashRuntimeAccess)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpFlashRuntimeAccess)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_flash_runtime_access(unitChip.unit, &ptpFlashRuntimeAccess->flashAccess);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_flash_runtime_access(unitChip.unit, &ptpFlashRuntimeAccess->flashAccess);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_flash_runtime_access(unitChip.unit, &ptpFlashRuntimeAccess->flashAccess);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_flash_runtime_access(unitChip.unit, &ptpFlashRuntimeAccess->flashAccess);
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
 * @brief Configure the PTP flash access flag. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpFlashRuntimeAccess ptp flash runtime access set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_flash_runtime_access(), cvb_ptp_set_flash_runtime_access(), dv1_ptp_set_flash_runtime_access(), dva_ptp_set_flash_runtime_access().
 */
RtkApiRet rtk_ptp_set_flash_runtime_access(UnitChip_t unitChip, PtpFlashRuntimeAccess_t *ptpFlashRuntimeAccess)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpFlashRuntimeAccess)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_flash_runtime_access(unitChip.unit, ptpFlashRuntimeAccess->flashAccess);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_flash_runtime_access(unitChip.unit, ptpFlashRuntimeAccess->flashAccess);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_flash_runtime_access(unitChip.unit, ptpFlashRuntimeAccess->flashAccess);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_flash_runtime_access(unitChip.unit, ptpFlashRuntimeAccess->flashAccess);
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
 * @brief Configure the PTP Pdelay write flash threshold. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpPdelayWrflashThres ptp pdelay wrflash thres set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_pdelay_wrflash_thres(), cvb_ptp_set_pdelay_wrflash_thres(), dv1_ptp_set_pdelay_wrflash_thres(), dva_ptp_set_pdelay_wrflash_thres().
 */
RtkApiRet rtk_ptp_set_pdelay_wrflash_thres(UnitChip_t unitChip, PtpPdelayWrflashThres_t *ptpPdelayWrflashThres)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPdelayWrflashThres)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_pdelay_wrflash_thres(unitChip.unit, ptpPdelayWrflashThres->threshold);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_pdelay_wrflash_thres(unitChip.unit, ptpPdelayWrflashThres->threshold);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_pdelay_wrflash_thres(unitChip.unit, ptpPdelayWrflashThres->threshold);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_pdelay_wrflash_thres(unitChip.unit, ptpPdelayWrflashThres->threshold);
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
 * @brief Retrieve the PTP Pdelay write flash threshold. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpPdelayWrflashThres ptp pdelay wrflash thres get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_pdelay_wrflash_thres(), cvb_ptp_get_pdelay_wrflash_thres(), dv1_ptp_get_pdelay_wrflash_thres(), dva_ptp_get_pdelay_wrflash_thres().
 */
RtkApiRet rtk_ptp_get_pdelay_wrflash_thres(UnitChip_t unitChip, PtpPdelayWrflashThres_t *ptpPdelayWrflashThres)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpPdelayWrflashThres)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_pdelay_wrflash_thres(unitChip.unit, &ptpPdelayWrflashThres->threshold);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_pdelay_wrflash_thres(unitChip.unit, &ptpPdelayWrflashThres->threshold);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_pdelay_wrflash_thres(unitChip.unit, &ptpPdelayWrflashThres->threshold);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_pdelay_wrflash_thres(unitChip.unit, &ptpPdelayWrflashThres->threshold);
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
 * @brief Retrieve the Sync Lock configuration. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSynclockConfig ptp synclock config get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_synclock_config(), cvb_ptp_get_synclock_config(), dv1_ptp_get_synclock_config(), dva_ptp_get_synclock_config().
 */
RtkApiRet rtk_ptp_get_synclock_config(UnitChip_t unitChip, PtpSynclockConfig_t *ptpSynclockConfig)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSynclockConfig)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_synclock_config(unitChip.unit, &ptpSynclockConfig->synclockStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_synclock_config(unitChip.unit, &ptpSynclockConfig->synclockStatus);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_synclock_config(unitChip.unit, &ptpSynclockConfig->synclockStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_synclock_config(unitChip.unit, &ptpSynclockConfig->synclockStatus);
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
 * @brief Configure the Sync Lock settings. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSynclockConfig ptp synclock config set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_synclock_config(), cvb_ptp_set_synclock_config(), dv1_ptp_set_synclock_config(), dva_ptp_set_synclock_config().
 */
RtkApiRet rtk_ptp_set_synclock_config(UnitChip_t unitChip, PtpSynclockConfig_t *ptpSynclockConfig)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSynclockConfig)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_synclock_config(unitChip.unit, ptpSynclockConfig->synclockStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_synclock_config(unitChip.unit, ptpSynclockConfig->synclockStatus);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_synclock_config(unitChip.unit, ptpSynclockConfig->synclockStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_synclock_config(unitChip.unit, ptpSynclockConfig->synclockStatus);
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
 * @brief Retrieve the Sync Slow Down configuration. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSyncslowdownConfig ptp syncslowdown config get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_syncslowdown_config(), cvb_ptp_get_syncslowdown_config(), dv1_ptp_get_syncslowdown_config(), dva_ptp_get_syncslowdown_config().
 */
RtkApiRet rtk_ptp_get_syncslowdown_config(UnitChip_t unitChip, PtpSyncslowdownConfig_t *ptpSyncslowdownConfig)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSyncslowdownConfig)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_syncslowdown_config(unitChip.unit, &ptpSyncslowdownConfig->syncSlowdownStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_syncslowdown_config(unitChip.unit, &ptpSyncslowdownConfig->syncSlowdownStatus);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_syncslowdown_config(unitChip.unit, &ptpSyncslowdownConfig->syncSlowdownStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_syncslowdown_config(unitChip.unit, &ptpSyncslowdownConfig->syncSlowdownStatus);
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
 * @brief Configure the Sync Slow Down settings. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpSyncslowdownConfig ptp syncslowdown config set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_syncslowdown_config(), cvb_ptp_set_syncslowdown_config(), dv1_ptp_set_syncslowdown_config(), dva_ptp_set_syncslowdown_config().
 */
RtkApiRet rtk_ptp_set_syncslowdown_config(UnitChip_t unitChip, PtpSyncslowdownConfig_t *ptpSyncslowdownConfig)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSyncslowdownConfig)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_syncslowdown_config(unitChip.unit, ptpSyncslowdownConfig->syncSlowdownStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_syncslowdown_config(unitChip.unit, ptpSyncslowdownConfig->syncSlowdownStatus);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_syncslowdown_config(unitChip.unit, ptpSyncslowdownConfig->syncSlowdownStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_syncslowdown_config(unitChip.unit, ptpSyncslowdownConfig->syncSlowdownStatus);
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
 * @brief Configure the allowed faults for Pdelay response. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpAllowedfaults ptp allowedfaults set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_allowedfaults(), cvb_ptp_set_allowedfaults(), dv1_ptp_set_allowedfaults(), dva_ptp_set_allowedfaults().
 */
RtkApiRet rtk_ptp_set_allowedfaults(UnitChip_t unitChip, PtpAllowedfaults_t *ptpAllowedfaults)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAllowedfaults)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_allowedfaults(unitChip.unit, ptpAllowedfaults->port, ptpAllowedfaults->allowedfaults);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_allowedfaults(unitChip.unit, ptpAllowedfaults->port, ptpAllowedfaults->allowedfaults);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_allowedfaults(unitChip.unit, ptpAllowedfaults->port, ptpAllowedfaults->allowedfaults);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_allowedfaults(unitChip.unit, ptpAllowedfaults->port, ptpAllowedfaults->allowedfaults);
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
 * @brief Retrieve the allowed faults for Pdelay response. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpAllowedfaults ptp allowedfaults get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_allowedfaults(), cvb_ptp_get_allowedfaults(), dv1_ptp_get_allowedfaults(), dva_ptp_get_allowedfaults().
 */
RtkApiRet rtk_ptp_get_allowedfaults(UnitChip_t unitChip, PtpAllowedfaults_t *ptpAllowedfaults)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAllowedfaults)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_allowedfaults(unitChip.unit, ptpAllowedfaults->port, &ptpAllowedfaults->allowedfaults);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_allowedfaults(unitChip.unit, ptpAllowedfaults->port, &ptpAllowedfaults->allowedfaults);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_allowedfaults(unitChip.unit, ptpAllowedfaults->port, &ptpAllowedfaults->allowedfaults);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_allowedfaults(unitChip.unit, ptpAllowedfaults->port, &ptpAllowedfaults->allowedfaults);
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
 * @brief Configure the delay measurement mechanism to PTP_P2P_MECH or PTP_COMMONP2P_MECH. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpDelaymeasureMech ptp delaymeasure mech set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_delaymeasure_mech(), cvb_ptp_set_delaymeasure_mech(), dv1_ptp_set_delaymeasure_mech(), dva_ptp_set_delaymeasure_mech().
 */
RtkApiRet rtk_ptp_set_delaymeasure_mech(UnitChip_t unitChip, PtpDelaymeasureMech_t *ptpDelaymeasureMech)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDelaymeasureMech)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_delaymeasure_mech(unitChip.unit, ptpDelaymeasureMech->mechnisam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_delaymeasure_mech(unitChip.unit, ptpDelaymeasureMech->mechnisam);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_delaymeasure_mech(unitChip.unit, ptpDelaymeasureMech->mechnisam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_delaymeasure_mech(unitChip.unit, ptpDelaymeasureMech->mechnisam);
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
 * @brief Retrieve the delay measurement mechanism. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpDelaymeasureMech ptp delaymeasure mech get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_delaymeasure_mech(), cvb_ptp_get_delaymeasure_mech(), dv1_ptp_get_delaymeasure_mech(), dva_ptp_get_delaymeasure_mech().
 */
RtkApiRet rtk_ptp_get_delaymeasure_mech(UnitChip_t unitChip, PtpDelaymeasureMech_t *ptpDelaymeasureMech)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDelaymeasureMech)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_delaymeasure_mech(unitChip.unit, &ptpDelaymeasureMech->mechnisam);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_delaymeasure_mech(unitChip.unit, &ptpDelaymeasureMech->mechnisam);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_delaymeasure_mech(unitChip.unit, &ptpDelaymeasureMech->mechnisam);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_delaymeasure_mech(unitChip.unit, &ptpDelaymeasureMech->mechnisam);
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
 * @brief Configure the delay asymmetry value. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpDelayAsymmetry ptp delay asymmetry set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_delay_asymmetry(), cvb_ptp_set_delay_asymmetry(), dv1_ptp_set_delay_asymmetry(), dva_ptp_set_delay_asymmetry().
 */
RtkApiRet rtk_ptp_set_delay_asymmetry(UnitChip_t unitChip, PtpDelayAsymmetry_t *ptpDelayAsymmetry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDelayAsymmetry)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_delay_asymmetry(unitChip.unit, ptpDelayAsymmetry->port, ptpDelayAsymmetry->delayAsymmetry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_delay_asymmetry(unitChip.unit, ptpDelayAsymmetry->port, ptpDelayAsymmetry->delayAsymmetry);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_delay_asymmetry(unitChip.unit, ptpDelayAsymmetry->port, ptpDelayAsymmetry->delayAsymmetry);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_delay_asymmetry(unitChip.unit, ptpDelayAsymmetry->port, ptpDelayAsymmetry->delayAsymmetry);
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
 * @brief Retrieve the delay asymmetry value. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpDelayAsymmetry ptp delay asymmetry get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_delay_asymmetry(), cvb_ptp_get_delay_asymmetry(), dv1_ptp_get_delay_asymmetry(), dva_ptp_get_delay_asymmetry().
 */
RtkApiRet rtk_ptp_get_delay_asymmetry(UnitChip_t unitChip, PtpDelayAsymmetry_t *ptpDelayAsymmetry)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDelayAsymmetry)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_delay_asymmetry(unitChip.unit, ptpDelayAsymmetry->port, &ptpDelayAsymmetry->delayAsymmetry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_delay_asymmetry(unitChip.unit, ptpDelayAsymmetry->port, &ptpDelayAsymmetry->delayAsymmetry);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_delay_asymmetry(unitChip.unit, ptpDelayAsymmetry->port, &ptpDelayAsymmetry->delayAsymmetry);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_delay_asymmetry(unitChip.unit, ptpDelayAsymmetry->port, &ptpDelayAsymmetry->delayAsymmetry);
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
 * @brief Configure the use of management-settable log sync interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpUseMgtsettableLogsyncintval ptp set use mgtsettable logsyncintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_use_mgtsettable_logsyncintval(), cvb_ptp_set_use_mgtsettable_logsyncintval(), dv1_ptp_set_use_mgtsettable_logsyncintval(), dva_ptp_set_use_mgtsettable_logsyncintval().
 */
RtkApiRet rtk_ptp_set_use_mgtsettable_logsyncintval(UnitChip_t unitChip, PtpUseMgtsettableLogsyncintval_t *ptpUseMgtsettableLogsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableLogsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_use_mgtsettable_logsyncintval(unitChip.unit, ptpUseMgtsettableLogsyncintval->domain, ptpUseMgtsettableLogsyncintval->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_use_mgtsettable_logsyncintval(unitChip.unit, ptpUseMgtsettableLogsyncintval->domain, ptpUseMgtsettableLogsyncintval->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_use_mgtsettable_logsyncintval(unitChip.unit, ptpUseMgtsettableLogsyncintval->domain, ptpUseMgtsettableLogsyncintval->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_use_mgtsettable_logsyncintval(unitChip.unit, ptpUseMgtsettableLogsyncintval->domain, ptpUseMgtsettableLogsyncintval->enable);
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
 * @brief Retrieve the status of management-settable log sync interval usage. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpUseMgtsettableLogsyncintval ptp get use mgtsettable logsyncintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_use_mgtsettable_logsyncintval(), cvb_ptp_get_use_mgtsettable_logsyncintval(), dv1_ptp_get_use_mgtsettable_logsyncintval(), dva_ptp_get_use_mgtsettable_logsyncintval().
 */
RtkApiRet rtk_ptp_get_use_mgtsettable_logsyncintval(UnitChip_t unitChip, PtpUseMgtsettableLogsyncintval_t *ptpUseMgtsettableLogsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableLogsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_use_mgtsettable_logsyncintval(unitChip.unit, ptpUseMgtsettableLogsyncintval->domain, &ptpUseMgtsettableLogsyncintval->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_use_mgtsettable_logsyncintval(unitChip.unit, ptpUseMgtsettableLogsyncintval->domain, &ptpUseMgtsettableLogsyncintval->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_use_mgtsettable_logsyncintval(unitChip.unit, ptpUseMgtsettableLogsyncintval->domain, &ptpUseMgtsettableLogsyncintval->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_use_mgtsettable_logsyncintval(unitChip.unit, ptpUseMgtsettableLogsyncintval->domain, &ptpUseMgtsettableLogsyncintval->enable);
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
 * @brief Configure the management-settable log sync interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpMgtsettableLogsyncintval ptp set mgtsettable logsyncintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_logsyncintval_mgtsettable(), cvb_ptp_set_logsyncintval_mgtsettable(), dv1_ptp_set_logsyncintval_mgtsettable(), dva_ptp_set_logsyncintval_mgtsettable().
 */
RtkApiRet rtk_ptp_set_logsyncintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLogsyncintval_t *ptpMgtsettableLogsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableLogsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_logsyncintval_mgtsettable(unitChip.unit, ptpMgtsettableLogsyncintval->domain, ptpMgtsettableLogsyncintval->logIntval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_logsyncintval_mgtsettable(unitChip.unit, ptpMgtsettableLogsyncintval->domain, ptpMgtsettableLogsyncintval->logIntval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_logsyncintval_mgtsettable(unitChip.unit, ptpMgtsettableLogsyncintval->domain, ptpMgtsettableLogsyncintval->logIntval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_logsyncintval_mgtsettable(unitChip.unit, ptpMgtsettableLogsyncintval->domain, ptpMgtsettableLogsyncintval->logIntval);
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
 * @brief Retrieve the management-settable log sync interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMgtsettableLogsyncintval ptp get mgtsettable logsyncintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_logsyncintval_mgtsettable(), cvb_ptp_get_logsyncintval_mgtsettable(), dv1_ptp_get_logsyncintval_mgtsettable(), dva_ptp_get_logsyncintval_mgtsettable().
 */
RtkApiRet rtk_ptp_get_logsyncintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLogsyncintval_t *ptpMgtsettableLogsyncintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableLogsyncintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_logsyncintval_mgtsettable(unitChip.unit, ptpMgtsettableLogsyncintval->domain, &ptpMgtsettableLogsyncintval->logIntval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_logsyncintval_mgtsettable(unitChip.unit, ptpMgtsettableLogsyncintval->domain, &ptpMgtsettableLogsyncintval->logIntval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_logsyncintval_mgtsettable(unitChip.unit, ptpMgtsettableLogsyncintval->domain, &ptpMgtsettableLogsyncintval->logIntval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_logsyncintval_mgtsettable(unitChip.unit, ptpMgtsettableLogsyncintval->domain, &ptpMgtsettableLogsyncintval->logIntval);
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
 * @brief Configure the use of management-settable log Pdelay_Req interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpUseMgtsettableLogpdelayreqintval ptp set usemgtsettable logpdelayreqintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_usemgtsettable_logpdelayreqintval(), cvb_ptp_set_usemgtsettable_logpdelayreqintval(), dv1_ptp_set_usemgtsettable_logpdelayreqintval(), dva_ptp_set_usemgtsettable_logpdelayreqintval().
 */
RtkApiRet rtk_ptp_set_usemgtsettable_logpdelayreqintval(UnitChip_t unitChip, PtpUsemgtsettableLogpdelayreqintval_t *ptpUseMgtsettableLogpdelayreqintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableLogpdelayreqintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_usemgtsettable_logpdelayreqintval(unitChip.unit, ptpUseMgtsettableLogpdelayreqintval->port, ptpUseMgtsettableLogpdelayreqintval->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_usemgtsettable_logpdelayreqintval(unitChip.unit, ptpUseMgtsettableLogpdelayreqintval->port, ptpUseMgtsettableLogpdelayreqintval->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_usemgtsettable_logpdelayreqintval(unitChip.unit, ptpUseMgtsettableLogpdelayreqintval->port, ptpUseMgtsettableLogpdelayreqintval->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_usemgtsettable_logpdelayreqintval(unitChip.unit, ptpUseMgtsettableLogpdelayreqintval->port, ptpUseMgtsettableLogpdelayreqintval->enable);
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
 * @brief Retrieve the the use of management-settable log Pdelay_Req interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpUseMgtsettableLogpdelayreqintval ptp get usemgtsettable logpdelayreqintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_usemgtsettable_logpdelayreqintval(), cvb_ptp_get_usemgtsettable_logpdelayreqintval(), dv1_ptp_get_usemgtsettable_logpdelayreqintval(), dva_ptp_get_usemgtsettable_logpdelayreqintval().
 */
RtkApiRet rtk_ptp_get_usemgtsettable_logpdelayreqintval(UnitChip_t unitChip, PtpUsemgtsettableLogpdelayreqintval_t *ptpUseMgtsettableLogpdelayreqintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableLogpdelayreqintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_usemgtsettable_logpdelayreqintval(unitChip.unit, ptpUseMgtsettableLogpdelayreqintval->port, &ptpUseMgtsettableLogpdelayreqintval->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_usemgtsettable_logpdelayreqintval(unitChip.unit, ptpUseMgtsettableLogpdelayreqintval->port, &ptpUseMgtsettableLogpdelayreqintval->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_usemgtsettable_logpdelayreqintval(unitChip.unit, ptpUseMgtsettableLogpdelayreqintval->port, &ptpUseMgtsettableLogpdelayreqintval->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_usemgtsettable_logpdelayreqintval(unitChip.unit, ptpUseMgtsettableLogpdelayreqintval->port, &ptpUseMgtsettableLogpdelayreqintval->enable);
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
 * @brief Configure the management-settable log Pdelay_Req interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpMgtsettableLogpdelayreqintval ptp set mgtsettable logpdelayreqintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_logpdelayreqintval_mgtsettable(), cvb_ptp_set_logpdelayreqintval_mgtsettable(), dv1_ptp_set_logpdelayreqintval_mgtsettable(), dva_ptp_set_logpdelayreqintval_mgtsettable().
 */
RtkApiRet rtk_ptp_set_logpdelayreqintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLogpdelayreqintval_t *ptpMgtsettableLogpdelayreqintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableLogpdelayreqintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_logpdelayreqintval_mgtsettable(unitChip.unit, ptpMgtsettableLogpdelayreqintval->port, ptpMgtsettableLogpdelayreqintval->intval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_logpdelayreqintval_mgtsettable(unitChip.unit, ptpMgtsettableLogpdelayreqintval->port, ptpMgtsettableLogpdelayreqintval->intval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_logpdelayreqintval_mgtsettable(unitChip.unit, ptpMgtsettableLogpdelayreqintval->port, ptpMgtsettableLogpdelayreqintval->intval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_logpdelayreqintval_mgtsettable(unitChip.unit, ptpMgtsettableLogpdelayreqintval->port, ptpMgtsettableLogpdelayreqintval->intval);
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
 * @brief Retrieve the management-settable log Pdelay_Req interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMgtsettableLogpdelayreqintval ptp get mgtsettable logpdelayreqintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_logpdelayreqintval_mgtsettable(), cvb_ptp_get_logpdelayreqintval_mgtsettable(), dv1_ptp_get_logpdelayreqintval_mgtsettable(), dva_ptp_get_logpdelayreqintval_mgtsettable().
 */
RtkApiRet rtk_ptp_get_logpdelayreqintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLogpdelayreqintval_t *ptpMgtsettableLogpdelayreqintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableLogpdelayreqintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_logpdelayreqintval_mgtsettable(unitChip.unit, ptpMgtsettableLogpdelayreqintval->port, &ptpMgtsettableLogpdelayreqintval->intval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_logpdelayreqintval_mgtsettable(unitChip.unit, ptpMgtsettableLogpdelayreqintval->port, &ptpMgtsettableLogpdelayreqintval->intval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_logpdelayreqintval_mgtsettable(unitChip.unit, ptpMgtsettableLogpdelayreqintval->port, &ptpMgtsettableLogpdelayreqintval->intval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_logpdelayreqintval_mgtsettable(unitChip.unit, ptpMgtsettableLogpdelayreqintval->port, &ptpMgtsettableLogpdelayreqintval->intval);
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
 * @brief Configure the use of management-settable compute neighbor rate ratio. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpUsemgtsettableRateratio ptp set usemgtsettable rateratio information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_rateratio_usemgtsettable(), cvb_ptp_set_rateratio_usemgtsettable(), dv1_ptp_set_rateratio_usemgtsettable(), dva_ptp_set_rateratio_usemgtsettable().
 */
RtkApiRet rtk_ptp_set_rateratio_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableRateratio_t *ptpUseMgtsettableRateratio)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableRateratio)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_rateratio_usemgtsettable(unitChip.unit, ptpUseMgtsettableRateratio->port, ptpUseMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_rateratio_usemgtsettable(unitChip.unit, ptpUseMgtsettableRateratio->port, ptpUseMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_rateratio_usemgtsettable(unitChip.unit, ptpUseMgtsettableRateratio->port, ptpUseMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_rateratio_usemgtsettable(unitChip.unit, ptpUseMgtsettableRateratio->port, ptpUseMgtsettableRateratio->enable);
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
 * @brief Configure the management-settable compute neighbor rate ratio. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpUseMgtsettableRateratio ptp get usemgtsettable rateratio information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_rateratio_usemgtsettable(), cvb_ptp_get_rateratio_usemgtsettable(), dv1_ptp_get_rateratio_usemgtsettable(), dva_ptp_get_rateratio_usemgtsettable().
 */
RtkApiRet rtk_ptp_get_rateratio_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableRateratio_t *ptpUseMgtsettableRateratio)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableRateratio)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_rateratio_usemgtsettable(unitChip.unit, ptpUseMgtsettableRateratio->port, &ptpUseMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_rateratio_usemgtsettable(unitChip.unit, ptpUseMgtsettableRateratio->port, &ptpUseMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_rateratio_usemgtsettable(unitChip.unit, ptpUseMgtsettableRateratio->port, &ptpUseMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_rateratio_usemgtsettable(unitChip.unit, ptpUseMgtsettableRateratio->port, &ptpUseMgtsettableRateratio->enable);
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
 * @brief Configure the management-settable compute neighbor rate ratio. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpMgtsettableRateratio ptp set mgtsettable rateratio information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_rateratio_mgtsettable(), cvb_ptp_set_rateratio_mgtsettable(), dv1_ptp_set_rateratio_mgtsettable(), dva_ptp_set_rateratio_mgtsettable().
 */
RtkApiRet rtk_ptp_set_rateratio_mgtsettable(UnitChip_t unitChip, PtpMgtsettableRateratio_t *ptpMgtsettableRateratio)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableRateratio)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_rateratio_mgtsettable(unitChip.unit, ptpMgtsettableRateratio->port, ptpMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_rateratio_mgtsettable(unitChip.unit, ptpMgtsettableRateratio->port, ptpMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_rateratio_mgtsettable(unitChip.unit, ptpMgtsettableRateratio->port, ptpMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_rateratio_mgtsettable(unitChip.unit, ptpMgtsettableRateratio->port, ptpMgtsettableRateratio->enable);
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
 * @brief Retrieve the management-settable compute neighbor rate ratio. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMgtsettableRateratio ptp get mgtsettable rateratio information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_rateratio_mgtsettable(), cvb_ptp_get_rateratio_mgtsettable(), dv1_ptp_get_rateratio_mgtsettable(), dva_ptp_get_rateratio_mgtsettable().
 */
RtkApiRet rtk_ptp_get_rateratio_mgtsettable(UnitChip_t unitChip, PtpMgtsettableRateratio_t *ptpMgtsettableRateratio)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableRateratio)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_rateratio_mgtsettable(unitChip.unit, ptpMgtsettableRateratio->port, &ptpMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_rateratio_mgtsettable(unitChip.unit, ptpMgtsettableRateratio->port, &ptpMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_rateratio_mgtsettable(unitChip.unit, ptpMgtsettableRateratio->port, &ptpMgtsettableRateratio->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_rateratio_mgtsettable(unitChip.unit, ptpMgtsettableRateratio->port, &ptpMgtsettableRateratio->enable);
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
 * @brief Configure the use of management-settable compute mean link delay. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpUseMgtsettableLinkdelay ptp set usemgtsettable linkdelay information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_linkdelay_usemgtsettable(), cvb_ptp_set_linkdelay_usemgtsettable(), dv1_ptp_set_linkdelay_usemgtsettable(), dva_ptp_set_linkdelay_usemgtsettable().
 */
RtkApiRet rtk_ptp_set_linkdelay_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableLinkdelay_t *ptpUseMgtsettableLinkdelay)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableLinkdelay)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_linkdelay_usemgtsettable(unitChip.unit, ptpUseMgtsettableLinkdelay->port, ptpUseMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_linkdelay_usemgtsettable(unitChip.unit, ptpUseMgtsettableLinkdelay->port, ptpUseMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_linkdelay_usemgtsettable(unitChip.unit, ptpUseMgtsettableLinkdelay->port, ptpUseMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_linkdelay_usemgtsettable(unitChip.unit, ptpUseMgtsettableLinkdelay->port, ptpUseMgtsettableLinkdelay->enable);
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
 * @brief Retrieve the status of management-settable compute mean link delay usage. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpUseMgtsettableLinkdelay ptp get usemgtsettable linkdelay information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_linkdelay_usemgtsettable(), cvb_ptp_get_linkdelay_usemgtsettable(), dv1_ptp_get_linkdelay_usemgtsettable(), dva_ptp_get_linkdelay_usemgtsettable().
 */
RtkApiRet rtk_ptp_get_linkdelay_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableLinkdelay_t *ptpUseMgtsettableLinkdelay)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableLinkdelay)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_linkdelay_usemgtsettable(unitChip.unit, ptpUseMgtsettableLinkdelay->port, &ptpUseMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_linkdelay_usemgtsettable(unitChip.unit, ptpUseMgtsettableLinkdelay->port, &ptpUseMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_linkdelay_usemgtsettable(unitChip.unit, ptpUseMgtsettableLinkdelay->port, &ptpUseMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_linkdelay_usemgtsettable(unitChip.unit, ptpUseMgtsettableLinkdelay->port, &ptpUseMgtsettableLinkdelay->enable);
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
 * @brief Configure the management-settable compute mean link delay. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpMgtsettableLinkdelay ptp set mgtsettable linkdelay information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_linkdelay_mgtsettable(), cvb_ptp_set_linkdelay_mgtsettable(), dv1_ptp_set_linkdelay_mgtsettable(), dva_ptp_set_linkdelay_mgtsettable().
 */
RtkApiRet rtk_ptp_set_linkdelay_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLinkdelay_t *ptpMgtsettableLinkdelay)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableLinkdelay)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_linkdelay_mgtsettable(unitChip.unit, ptpMgtsettableLinkdelay->port, ptpMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_linkdelay_mgtsettable(unitChip.unit, ptpMgtsettableLinkdelay->port, ptpMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_linkdelay_mgtsettable(unitChip.unit, ptpMgtsettableLinkdelay->port, ptpMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_linkdelay_mgtsettable(unitChip.unit, ptpMgtsettableLinkdelay->port, ptpMgtsettableLinkdelay->enable);
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
 * @brief Retrieve the management-settable compute mean link delay. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMgtsettableLinkdelay ptp get mgtsettable linkdelay information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_linkdelay_mgtsettable(), cvb_ptp_get_linkdelay_mgtsettable(), dv1_ptp_get_linkdelay_mgtsettable(), dva_ptp_get_linkdelay_mgtsettable().
 */
RtkApiRet rtk_ptp_get_linkdelay_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLinkdelay_t *ptpMgtsettableLinkdelay)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableLinkdelay)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_linkdelay_mgtsettable(unitChip.unit, ptpMgtsettableLinkdelay->port, &ptpMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_linkdelay_mgtsettable(unitChip.unit, ptpMgtsettableLinkdelay->port, &ptpMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_linkdelay_mgtsettable(unitChip.unit, ptpMgtsettableLinkdelay->port, &ptpMgtsettableLinkdelay->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_linkdelay_mgtsettable(unitChip.unit, ptpMgtsettableLinkdelay->port, &ptpMgtsettableLinkdelay->enable);
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
 * @brief Configure the use of management-settable One Step Tx operation. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpUseMgtsettable1Steptxoper ptp set usemgtsettable 1steptxoper information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_1steptxoper_usemgtsettable(), cvb_ptp_set_1steptxoper_usemgtsettable(), dv1_ptp_set_1steptxoper_usemgtsettable(), dva_ptp_set_1steptxoper_usemgtsettable().
 */
RtkApiRet rtk_ptp_set_1steptxoper_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettable1Steptxoper_t *ptpUseMgtsettable1Steptxoper)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettable1Steptxoper)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_1steptxoper_usemgtsettable(unitChip.unit, ptpUseMgtsettable1Steptxoper->port, ptpUseMgtsettable1Steptxoper->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_1steptxoper_usemgtsettable(unitChip.unit, ptpUseMgtsettable1Steptxoper->port, ptpUseMgtsettable1Steptxoper->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_1steptxoper_usemgtsettable(unitChip.unit, ptpUseMgtsettable1Steptxoper->port, ptpUseMgtsettable1Steptxoper->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_1steptxoper_usemgtsettable(unitChip.unit, ptpUseMgtsettable1Steptxoper->port, ptpUseMgtsettable1Steptxoper->enable);
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
 * @brief Retrieve the status of management-settable One Step Tx operation usage. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpUseMgtsettable1Steptxoper ptp get usemgtsettable 1steptxoper information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_1steptxoper_usemgtsettable(), cvb_ptp_get_1steptxoper_usemgtsettable(), dv1_ptp_get_1steptxoper_usemgtsettable(), dva_ptp_get_1steptxoper_usemgtsettable().
 */
RtkApiRet rtk_ptp_get_1steptxoper_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettable1Steptxoper_t *ptpUseMgtsettable1Steptxoper)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettable1Steptxoper)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_1steptxoper_usemgtsettable(unitChip.unit, ptpUseMgtsettable1Steptxoper->port, &ptpUseMgtsettable1Steptxoper->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_1steptxoper_usemgtsettable(unitChip.unit, ptpUseMgtsettable1Steptxoper->port, &ptpUseMgtsettable1Steptxoper->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_1steptxoper_usemgtsettable(unitChip.unit, ptpUseMgtsettable1Steptxoper->port, &ptpUseMgtsettable1Steptxoper->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_1steptxoper_usemgtsettable(unitChip.unit, ptpUseMgtsettable1Steptxoper->port, &ptpUseMgtsettable1Steptxoper->enable);
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
 * @brief Configure the management-settable One Step Tx operation. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpMgtsettable1Steptxoper ptp set mgtsettable 1steptxoper information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_1steptxoper_mgtsettable(), cvb_ptp_set_1steptxoper_mgtsettable(), dv1_ptp_set_1steptxoper_mgtsettable(), dva_ptp_set_1steptxoper_mgtsettable().
 */
RtkApiRet rtk_ptp_set_1steptxoper_mgtsettable(UnitChip_t unitChip, PtpMgtsettable1Steptxoper_t *ptpMgtsettable1Steptxoper)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettable1Steptxoper)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_1steptxoper_mgtsettable(unitChip.unit, ptpMgtsettable1Steptxoper->port, ptpMgtsettable1Steptxoper->domain, ptpMgtsettable1Steptxoper->txOper);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_1steptxoper_mgtsettable(unitChip.unit, ptpMgtsettable1Steptxoper->port, ptpMgtsettable1Steptxoper->domain, ptpMgtsettable1Steptxoper->txOper);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_1steptxoper_mgtsettable(unitChip.unit, ptpMgtsettable1Steptxoper->port, ptpMgtsettable1Steptxoper->domain, ptpMgtsettable1Steptxoper->txOper);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_1steptxoper_mgtsettable(unitChip.unit, ptpMgtsettable1Steptxoper->port, ptpMgtsettable1Steptxoper->domain, ptpMgtsettable1Steptxoper->txOper);
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
 * @brief Retrieve the management-settable One Step Tx operation. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMgtsettable1Steptxoper ptp get mgtsettable 1steptxoper information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_1steptxoper_mgtsettable(), cvb_ptp_get_1steptxoper_mgtsettable(), dv1_ptp_get_1steptxoper_mgtsettable(), dva_ptp_get_1steptxoper_mgtsettable().
 */
RtkApiRet rtk_ptp_get_1steptxoper_mgtsettable(UnitChip_t unitChip, PtpMgtsettable1Steptxoper_t *ptpMgtsettable1Steptxoper)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettable1Steptxoper)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_1steptxoper_mgtsettable(unitChip.unit, ptpMgtsettable1Steptxoper->port, ptpMgtsettable1Steptxoper->domain, &ptpMgtsettable1Steptxoper->txOper);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_1steptxoper_mgtsettable(unitChip.unit, ptpMgtsettable1Steptxoper->port, ptpMgtsettable1Steptxoper->domain, &ptpMgtsettable1Steptxoper->txOper);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_1steptxoper_mgtsettable(unitChip.unit, ptpMgtsettable1Steptxoper->port, ptpMgtsettable1Steptxoper->domain, &ptpMgtsettable1Steptxoper->txOper);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_1steptxoper_mgtsettable(unitChip.unit, ptpMgtsettable1Steptxoper->port, ptpMgtsettable1Steptxoper->domain, &ptpMgtsettable1Steptxoper->txOper);
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
 * @brief Configure the use of management-settable gPTP capable message interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpUseMgtsettableGptpmsgintval ptp set usemgtsettable gptpmsgintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_gptpmsgintval_usemgtsettable(), cvb_ptp_set_gptpmsgintval_usemgtsettable(), dv1_ptp_set_gptpmsgintval_usemgtsettable(), dva_ptp_set_gptpmsgintval_usemgtsettable().
 */
RtkApiRet rtk_ptp_set_gptpmsgintval_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableGptpmsgintval_t *ptpUseMgtsettableGptpmsgintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableGptpmsgintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_gptpmsgintval_usemgtsettable(unitChip.unit, ptpUseMgtsettableGptpmsgintval->port, ptpUseMgtsettableGptpmsgintval->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_gptpmsgintval_usemgtsettable(unitChip.unit, ptpUseMgtsettableGptpmsgintval->port, ptpUseMgtsettableGptpmsgintval->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_gptpmsgintval_usemgtsettable(unitChip.unit, ptpUseMgtsettableGptpmsgintval->port, ptpUseMgtsettableGptpmsgintval->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_gptpmsgintval_usemgtsettable(unitChip.unit, ptpUseMgtsettableGptpmsgintval->port, ptpUseMgtsettableGptpmsgintval->enable);
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
 * @brief Retrieve the status of management-settable gPTP capable message interval usage. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpUseMgtsettableGptpmsgintval ptp get usemgtsettable gptpmsgintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_gptpmsgintval_usemgtsettable(), cvb_ptp_get_gptpmsgintval_usemgtsettable(), dv1_ptp_get_gptpmsgintval_usemgtsettable(), dva_ptp_get_gptpmsgintval_usemgtsettable().
 */
RtkApiRet rtk_ptp_get_gptpmsgintval_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableGptpmsgintval_t *ptpUseMgtsettableGptpmsgintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpUseMgtsettableGptpmsgintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_gptpmsgintval_usemgtsettable(unitChip.unit, ptpUseMgtsettableGptpmsgintval->port, &ptpUseMgtsettableGptpmsgintval->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_gptpmsgintval_usemgtsettable(unitChip.unit, ptpUseMgtsettableGptpmsgintval->port, &ptpUseMgtsettableGptpmsgintval->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_gptpmsgintval_usemgtsettable(unitChip.unit, ptpUseMgtsettableGptpmsgintval->port, &ptpUseMgtsettableGptpmsgintval->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_gptpmsgintval_usemgtsettable(unitChip.unit, ptpUseMgtsettableGptpmsgintval->port, &ptpUseMgtsettableGptpmsgintval->enable);
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
 * @brief Configure the management-settable log gPTP capable message interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpMgtsettableGptpmsglogintval ptp set mgtsettable gptpmsglogintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_gptpmsglogintval_mgtsettable(), cvb_ptp_set_gptpmsglogintval_mgtsettable(), dv1_ptp_set_gptpmsglogintval_mgtsettable(), dva_ptp_set_gptpmsglogintval_mgtsettable().
 */
RtkApiRet rtk_ptp_set_gptpmsglogintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableGptpmsglogintval_t *ptpMgtsettableGptpmsglogintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableGptpmsglogintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_gptpmsglogintval_mgtsettable(unitChip.unit, ptpMgtsettableGptpmsglogintval->port, ptpMgtsettableGptpmsglogintval->domain, ptpMgtsettableGptpmsglogintval->intval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_gptpmsglogintval_mgtsettable(unitChip.unit, ptpMgtsettableGptpmsglogintval->port, ptpMgtsettableGptpmsglogintval->domain, ptpMgtsettableGptpmsglogintval->intval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_gptpmsglogintval_mgtsettable(unitChip.unit, ptpMgtsettableGptpmsglogintval->port, ptpMgtsettableGptpmsglogintval->domain, ptpMgtsettableGptpmsglogintval->intval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_gptpmsglogintval_mgtsettable(unitChip.unit, ptpMgtsettableGptpmsglogintval->port, ptpMgtsettableGptpmsglogintval->domain, ptpMgtsettableGptpmsglogintval->intval);
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
 * @brief Retrieve the management-settable log gPTP capable message interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpMgtsettableGptpmsglogintval ptp get mgtsettable gptpmsglogintval information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_gptpmsglogintval_mgtsettable(), cvb_ptp_get_gptpmsglogintval_mgtsettable(), dv1_ptp_get_gptpmsglogintval_mgtsettable(), dva_ptp_get_gptpmsglogintval_mgtsettable().
 */
RtkApiRet rtk_ptp_get_gptpmsglogintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableGptpmsglogintval_t *ptpMgtsettableGptpmsglogintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpMgtsettableGptpmsglogintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_gptpmsglogintval_mgtsettable(unitChip.unit, ptpMgtsettableGptpmsglogintval->port, ptpMgtsettableGptpmsglogintval->domain, &ptpMgtsettableGptpmsglogintval->intval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_gptpmsglogintval_mgtsettable(unitChip.unit, ptpMgtsettableGptpmsglogintval->port, ptpMgtsettableGptpmsglogintval->domain, &ptpMgtsettableGptpmsglogintval->intval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_gptpmsglogintval_mgtsettable(unitChip.unit, ptpMgtsettableGptpmsglogintval->port, ptpMgtsettableGptpmsglogintval->domain, &ptpMgtsettableGptpmsglogintval->intval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_gptpmsglogintval_mgtsettable(unitChip.unit, ptpMgtsettableGptpmsglogintval->port, ptpMgtsettableGptpmsglogintval->domain, &ptpMgtsettableGptpmsglogintval->intval);
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
 * @brief Configure the initial log gPTP capable message interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpInitGptpmsgintval ptp init gptpmsgintval set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_init_gptpmsgintval(), cvb_ptp_set_init_gptpmsgintval(), dv1_ptp_set_init_gptpmsgintval(), dva_ptp_set_init_gptpmsgintval().
 */
RtkApiRet rtk_ptp_set_init_gptpmsgintval(UnitChip_t unitChip, PtpInitGptpmsgintval_t *ptpInitGptpmsgintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInitGptpmsgintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_init_gptpmsgintval(unitChip.unit, ptpInitGptpmsgintval->port, ptpInitGptpmsgintval->domain, ptpInitGptpmsgintval->intval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_init_gptpmsgintval(unitChip.unit, ptpInitGptpmsgintval->port, ptpInitGptpmsgintval->domain, ptpInitGptpmsgintval->intval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_init_gptpmsgintval(unitChip.unit, ptpInitGptpmsgintval->port, ptpInitGptpmsgintval->domain, ptpInitGptpmsgintval->intval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_init_gptpmsgintval(unitChip.unit, ptpInitGptpmsgintval->port, ptpInitGptpmsgintval->domain, ptpInitGptpmsgintval->intval);
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
 * @brief Retrieve the initial log gPTP capable message interval. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpInitGptpmsgintval ptp init gptpmsgintval get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_init_gptpmsgintval(), cvb_ptp_get_init_gptpmsgintval(), dv1_ptp_get_init_gptpmsgintval(), dva_ptp_get_init_gptpmsgintval().
 */
RtkApiRet rtk_ptp_get_init_gptpmsgintval(UnitChip_t unitChip, PtpInitGptpmsgintval_t *ptpInitGptpmsgintval)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInitGptpmsgintval)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_init_gptpmsgintval(unitChip.unit, ptpInitGptpmsgintval->port, ptpInitGptpmsgintval->domain, &ptpInitGptpmsgintval->intval);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_init_gptpmsgintval(unitChip.unit, ptpInitGptpmsgintval->port, ptpInitGptpmsgintval->domain, &ptpInitGptpmsgintval->intval);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_init_gptpmsgintval(unitChip.unit, ptpInitGptpmsgintval->port, ptpInitGptpmsgintval->domain, &ptpInitGptpmsgintval->intval);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_init_gptpmsgintval(unitChip.unit, ptpInitGptpmsgintval->port, ptpInitGptpmsgintval->domain, &ptpInitGptpmsgintval->intval);
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
 * @brief Configure the neighbor gPTP RX timeout count. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpNeigptprxtimerout ptp neigptprxtimerout set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_neigptprxtimerout(), cvb_ptp_set_neigptprxtimerout(), dv1_ptp_set_neigptprxtimerout(), dva_ptp_set_neigptprxtimerout().
 */
RtkApiRet rtk_ptp_set_neigptprxtimerout(UnitChip_t unitChip, PtpNeigptprxtimerout_t *ptpNeigptprxtimerout)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpNeigptprxtimerout)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_neigptprxtimerout(unitChip.unit, ptpNeigptprxtimerout->port, ptpNeigptprxtimerout->timeoutCnt);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_neigptprxtimerout(unitChip.unit, ptpNeigptprxtimerout->port, ptpNeigptprxtimerout->timeoutCnt);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_neigptprxtimerout(unitChip.unit, ptpNeigptprxtimerout->port, ptpNeigptprxtimerout->timeoutCnt);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_neigptprxtimerout(unitChip.unit, ptpNeigptprxtimerout->port, ptpNeigptprxtimerout->timeoutCnt);
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
 * @brief Retrieve the neighbor gPTP RX timeout count. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpNeigptprxtimerout ptp neigptprxtimerout get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_neigptprxtimerout(), cvb_ptp_get_neigptprxtimerout(), dv1_ptp_get_neigptprxtimerout(), dva_ptp_get_neigptprxtimerout().
 */
RtkApiRet rtk_ptp_get_neigptprxtimerout(UnitChip_t unitChip, PtpNeigptprxtimerout_t *ptpNeigptprxtimerout)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpNeigptprxtimerout)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_neigptprxtimerout(unitChip.unit, ptpNeigptprxtimerout->port, &ptpNeigptprxtimerout->timeoutCnt);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_neigptprxtimerout(unitChip.unit, ptpNeigptprxtimerout->port, &ptpNeigptprxtimerout->timeoutCnt);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_neigptprxtimerout(unitChip.unit, ptpNeigptprxtimerout->port, &ptpNeigptprxtimerout->timeoutCnt);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_neigptprxtimerout(unitChip.unit, ptpNeigptprxtimerout->port, &ptpNeigptprxtimerout->timeoutCnt);
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
 * @brief Retrieve the AS2020 flag to enable or disable AS2020 new features. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpAs2020Status PTP AS2020 status get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_as2020_status(), cvb_ptp_get_as2020_status(), dv1_ptp_get_as2020_status(), dva_ptp_get_as2020_status().
 */
RtkApiRet rtk_ptp_get_as2020_status(UnitChip_t unitChip, PtpAs2020Status_t *ptpAs2020Status)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAs2020Status)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_as2020_status(unitChip.unit, &ptpAs2020Status->as2020En);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_as2020_status(unitChip.unit, &ptpAs2020Status->as2020En);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_as2020_status(unitChip.unit, &ptpAs2020Status->as2020En);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_as2020_status(unitChip.unit, &ptpAs2020Status->as2020En);
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
 * @brief Configure the AS2020 flag to enable or disable AS2020 new features. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpAs2020Status ptp as2020 status set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_as2020_status(), cvb_ptp_set_as2020_status(), dv1_ptp_set_as2020_status(), dva_ptp_set_as2020_status().
 */
RtkApiRet rtk_ptp_set_as2020_status(UnitChip_t unitChip, PtpAs2020Status_t *ptpAs2020Status)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAs2020Status)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_as2020_status(unitChip.unit, ptpAs2020Status->as2020En);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_as2020_status(unitChip.unit, ptpAs2020Status->as2020En);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_as2020_status(unitChip.unit, ptpAs2020Status->as2020En);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_as2020_status(unitChip.unit, ptpAs2020Status->as2020En);
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
 * @brief Retrieve the overwrite status of clock ID when the mode is TC. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpOverwriteClockid ptp overwrite clockid get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_overwrite_clockid_get(), cvb_ptp_overwrite_clockid_get(), dv1_ptp_overwrite_clockid_get(), dva_ptp_overwrite_clockid_get().
 */
RtkApiRet rtk_ptp_overwrite_clockid_get(UnitChip_t unitChip, PtpOverwriteClockid_t *ptpOverwriteClockid)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpOverwriteClockid)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_overwrite_clockid_get(unitChip.unit, &ptpOverwriteClockid->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_overwrite_clockid_get(unitChip.unit, &ptpOverwriteClockid->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_overwrite_clockid_get(unitChip.unit, &ptpOverwriteClockid->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_overwrite_clockid_get(unitChip.unit, &ptpOverwriteClockid->enable);
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
 * @brief Send out Pdelay_req from the master port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSendPdelayFromMasterport ptp get send pdelay from masterport information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_send_pdelay_from_masterport(), cvb_ptp_get_send_pdelay_from_masterport(), dv1_ptp_get_send_pdelay_from_masterport(), dva_ptp_get_send_pdelay_from_masterport().
 */
RtkApiRet rtk_ptp_get_send_pdelay_from_masterport(UnitChip_t unitChip, PtpSendPdelayFromMasterport_t *ptpSendPdelayFromMasterport)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSendPdelayFromMasterport)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_send_pdelay_from_masterport(unitChip.unit, &ptpSendPdelayFromMasterport->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_send_pdelay_from_masterport(unitChip.unit, &ptpSendPdelayFromMasterport->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_send_pdelay_from_masterport(unitChip.unit, &ptpSendPdelayFromMasterport->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_send_pdelay_from_masterport(unitChip.unit, &ptpSendPdelayFromMasterport->enable);
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
 * @brief Retrieve the status of recovery sync message after first bootup time target. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSkipfirstbootrecovery ptp skipfirstbootrecovery get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_skipfirstbootrecovery(), cvb_ptp_get_skipfirstbootrecovery(), dv1_ptp_get_skipfirstbootrecovery(), dva_ptp_get_skipfirstbootrecovery().
 */
RtkApiRet rtk_ptp_get_skipfirstbootrecovery(UnitChip_t unitChip, PtpSkipfirstbootrecovery_t *ptpSkipfirstbootrecovery)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSkipfirstbootrecovery)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_skipfirstbootrecovery(unitChip.unit, &ptpSkipfirstbootrecovery->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_skipfirstbootrecovery(unitChip.unit, &ptpSkipfirstbootrecovery->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_skipfirstbootrecovery(unitChip.unit, &ptpSkipfirstbootrecovery->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_skipfirstbootrecovery(unitChip.unit, &ptpSkipfirstbootrecovery->enable);
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
 * @brief If the switch never receives sync from the slave port, do not perform recovery. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpSilenceifneversynced ptp silenceifneversynced get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_silenceifneversynced(), cvb_ptp_get_silenceifneversynced(), dv1_ptp_get_silenceifneversynced(), dva_ptp_get_silenceifneversynced().
 */
RtkApiRet rtk_ptp_get_silenceifneversynced(UnitChip_t unitChip, PtpSilenceifneversynced_t *ptpSilenceifneversynced)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSilenceifneversynced)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_silenceifneversynced(unitChip.unit, ptpSilenceifneversynced->domainid, &ptpSilenceifneversynced->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_silenceifneversynced(unitChip.unit, ptpSilenceifneversynced->domainid, &ptpSilenceifneversynced->enable);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_silenceifneversynced(unitChip.unit, ptpSilenceifneversynced->domainid, &ptpSilenceifneversynced->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_silenceifneversynced(unitChip.unit, ptpSilenceifneversynced->domainid, &ptpSilenceifneversynced->enable);
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
 * @brief Retrieve the PTP Internal Sublayer Service priority enable status in switch fabric (Protocol configuration). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpInterServPriEn ptp get inter serv pri en information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_en_inter_serv_pri(), cvb_ptp_get_en_inter_serv_pri(), dv1_ptp_get_en_inter_serv_pri(), dva_ptp_get_en_inter_serv_pri().
 */
RtkApiRet rtk_ptp_get_en_inter_serv_pri(UnitChip_t unitChip, PtpInterServPriEn_t *ptpInterServPriEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInterServPriEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_en_inter_serv_pri(unitChip.unit, &ptpInterServPriEn->priority_en);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_en_inter_serv_pri(unitChip.unit, &ptpInterServPriEn->priority_en);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_en_inter_serv_pri(unitChip.unit, &ptpInterServPriEn->priority_en);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_en_inter_serv_pri(unitChip.unit, &ptpInterServPriEn->priority_en);
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
 * @brief Configure the PTP Internal Sublayer Service priority enable status in switch fabric (Protocol configuration). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpInterServPriEn ptp set inter serv pri en information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_en_inter_serv_pri(), cvb_ptp_set_en_inter_serv_pri(), dv1_ptp_set_en_inter_serv_pri(), dva_ptp_set_en_inter_serv_pri().
 */
RtkApiRet rtk_ptp_set_en_inter_serv_pri(UnitChip_t unitChip, PtpInterServPriEn_t *ptpInterServPriEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInterServPriEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_en_inter_serv_pri(unitChip.unit, ptpInterServPriEn->priority_en);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_en_inter_serv_pri(unitChip.unit, ptpInterServPriEn->priority_en);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_en_inter_serv_pri(unitChip.unit, ptpInterServPriEn->priority_en);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_en_inter_serv_pri(unitChip.unit, ptpInterServPriEn->priority_en);
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
 * @brief Retrieve the PTP Internal Sublayer Service priority enable status in switch fabric (Protocol configuration). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ptpInterServPri ptp get inter serv pri information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_inter_serv_pri(), cvb_ptp_get_inter_serv_pri(), dv1_ptp_get_inter_serv_pri(), dva_ptp_get_inter_serv_pri().
 */
RtkApiRet rtk_ptp_get_inter_serv_pri(UnitChip_t unitChip, PtpInterServPri_t *ptpInterServPri)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInterServPri)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_inter_serv_pri(unitChip.unit, &ptpInterServPri->priority);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_inter_serv_pri(unitChip.unit, &ptpInterServPri->priority);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_get_inter_serv_pri(unitChip.unit, &ptpInterServPri->priority);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_inter_serv_pri(unitChip.unit, &ptpInterServPri->priority);
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
 * @brief Configure the PTP Internal Sublayer Service priority enable status in switch fabric (Protocol configuration). \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ptpInterServPri ptp set inter serv pri information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_set_inter_serv_pri(), cvb_ptp_set_inter_serv_pri(), dv1_ptp_set_inter_serv_pri(), dva_ptp_set_inter_serv_pri().
 */
RtkApiRet rtk_ptp_set_inter_serv_pri(UnitChip_t unitChip, PtpInterServPri_t *ptpInterServPri)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpInterServPri)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_inter_serv_pri(unitChip.unit, ptpInterServPri->priority);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_set_inter_serv_pri(unitChip.unit, ptpInterServPri->priority);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_set_inter_serv_pri(unitChip.unit, ptpInterServPri->priority);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_inter_serv_pri(unitChip.unit, ptpInterServPri->priority);
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
 * @brief Retrieve the PTP ready status. \n
 *        Supported chips: RTL907XC_VB/RTL907XD_VA/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] ptpSetInterServPri PTP ready status (N/A) \n
 *                 Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_get_isready(), cvb_ptp_get_isready(), cva_ptp_get_isready().
 */

RtkApiRet rtk_ptp_get_isready(UnitChip_t unitChip, PtpIsReady_t *ptpIsReady)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpIsReady)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_isready(unitChip.unit, &ptpIsReady->isPtpRdy);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_get_isready(unitChip.unit, &ptpIsReady->isPtpRdy);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_isready(unitChip.unit, &ptpIsReady->isPtpRdy);
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
 * @brief Directly retrieve the PTP time status. \n
 *         Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] ptpTimeDirectGet ptp time direct get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_time_direct_get(), cvb_ptp_time_direct_get(), dv1_ptp_time_direct_get(), dva_ptp_time_direct_get().
 */
RtkApiRet rtk_ptp_time_direct_get(UnitChip_t unitChip, PtpTimeDirectGet_t *ptpTimeDirectGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpTimeDirectGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_time_direct_get(unitChip.unit, ptpTimeDirectGet->timesel, &ptpTimeDirectGet->ts);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_ptp_time_direct_get(unitChip.unit, ptpTimeDirectGet->timesel, &ptpTimeDirectGet->ts);
                break;
#endif
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_ptp_time_direct_get(unitChip.unit, ptpTimeDirectGet->timesel, &ptpTimeDirectGet->ts);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_time_direct_get(unitChip.unit, ptpTimeDirectGet->timesel, &ptpTimeDirectGet->ts);
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
 * @brief  Retrieve the stored neidelay and neighbor rate ratio from flash. \n
 *          Supported chips: RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] ptpGetNeidelayAndNeirateratio ptp get neighbor delay and neighbor ratio information \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_get_neidelay_and_neirateratio().
 */
RtkApiRet rtk_ptp_get_neidelay_and_neirateratio(UnitChip_t unitChip, PtpGetNeidelayAndNeirateratio_t *ptpGetNeidelayAndNeirateratio)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpGetNeidelayAndNeirateratio)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_get_neidelay_and_neirateratio(unitChip.unit, ptpGetNeidelayAndNeirateratio->port, &ptpGetNeidelayAndNeirateratio->neiDelay, &ptpGetNeidelayAndNeirateratio->dNeiDelayRatio, ptpGetNeidelayAndNeirateratio->dRateRatio);
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
 * @brief  Erase all PTP data from flash. \n
 *          Supported chips: RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to cva_ptp_erase_flash_data().
 */
RtkApiRet rtk_ptp_erase_flash_data(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_ptp_erase_flash_data(unitChip.unit);
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief  Set the destination MAC address selection for 802.1AS, 1588, or customization. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpDomainProfiles ptp profiles setting information on specific domain \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_set_domain_profiles().
 */
RtkApiRet rtk_ptp_set_domain_profiles(UnitChip_t unitChip, PtpDomainProfiles_t *ptpDomainProfiles)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpDomainProfiles)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_domain_profiles(unitChip.unit, ptpDomainProfiles->domainid, ptpDomainProfiles->profiles);
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
 * @brief  Retrieve the neighbor delay value. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[inout] ptpNeidelay neighbor delay information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_neidelay_get().
 */
RtkApiRet rtk_ptp_neidelay_get(UnitChip_t unitChip, PtpNeidelay_t *ptpNeidelay)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == ptpNeidelay)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_neidelay_get(unitChip.unit, ptpNeidelay->port, &ptpNeidelay->neiDelay);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_neidelay_get(unitChip.unit, ptpNeidelay->port, &ptpNeidelay->neiDelay);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    } while(0u == 1u);
    return ret;
}
/**
 * @brief  Set the neighbor delay value. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[inout] ptpNeidelay neighbor delay information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_neidelay_set().
 */
RtkApiRet rtk_ptp_neidelay_set(UnitChip_t unitChip, PtpNeidelay_t *ptpNeidelay)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpNeidelay)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_neidelay_set(unitChip.unit, ptpNeidelay->port, ptpNeidelay->neiDelay);
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
 * @brief  Set the enable configuration for pdelay measurement. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpCaldelayEn Calculation Enable information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_caldelayen_set().
 */
RtkApiRet rtk_ptp_caldelayen_set(UnitChip_t unitChip, PtpCaldelayEn_t *ptpCaldelayEn)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpCaldelayEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_caldelayen_set(unitChip.unit, ptpCaldelayEn->port, ptpCaldelayEn->enable);
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
 * @brief  Set the autosar time sync protocol cyclic redundancy check(CRC) support configuration. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarCrcSupport CRC support information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_crc_support().
 */
RtkApiRet rtk_ptp_atsar_set_crc_support(UnitChip_t unitChip, PtpAtsarCrcSupport_t *ptpAtsarCrcSupport)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarCrcSupport)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_crc_support(unitChip.unit, ptpAtsarCrcSupport->enable);
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
 * @brief  Set the autosar time sync protocol frame priority configuration. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarFramePri Frame priority information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_frame_pri().
 */
RtkApiRet rtk_ptp_atsar_set_frame_pri(UnitChip_t unitChip, PtpAtsarFramePri_t *ptpAtsarFramePri)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarFramePri)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_frame_pri(unitChip.unit, ptpAtsarFramePri->enable);
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
 * @brief  Set the period configuration of pdelay request transmitting in autosar time sync protocol . \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarTxPdelayReqPeriod Transmitting pdelay request period information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_tx_pdelayreq_period().
 */
RtkApiRet rtk_ptp_atsar_set_tx_pdelayreq_period(UnitChip_t unitChip, PtpAtsarTxPdelayReqPeriod_t *ptpAtsarTxPdelayReqPeriod)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarTxPdelayReqPeriod)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_tx_pdelayreq_period(unitChip.unit, ptpAtsarTxPdelayReqPeriod->pdelayIntval);
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
 * @brief  Set the threshold of valid pdelay calculation result in autosar time sync protocol . \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarPdelayLatencyThresh Transmitting pdelay request period information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_pdelay_latency_thresh().
 */
RtkApiRet rtk_ptp_atsar_set_pdelay_latency_thresh(UnitChip_t unitChip, PtpAtsarPdelayLatencyThresh_t *ptpAtsarPdelayLatencyThresh)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarPdelayLatencyThresh)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_pdelay_latency_thresh(unitChip.unit, ptpAtsarPdelayLatencyThresh->delayThresh);
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
 * @brief  Set the propagation delay(neighbor delay) in autosar time sync protocol . \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarPropDelay Propagation delay information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_prop_delay().
 */
RtkApiRet rtk_ptp_atsar_set_prop_delay(UnitChip_t unitChip, PtpAtsarPropDelay_t *ptpAtsarPropDelay)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarPropDelay)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_prop_delay(unitChip.unit, ptpAtsarPropDelay->pDelay);
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
 * @brief  Set period of transmitting time sync packet in autosar time sync protocol . \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarTxPeriod Time sync transmitting period information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_tx_period().
 */
RtkApiRet rtk_ptp_atsar_set_tx_period(UnitChip_t unitChip, PtpAtsarTxPeriod_t *ptpAtsarTxPeriod)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarTxPeriod)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_tx_period(unitChip.unit, ptpAtsarTxPeriod->intval);
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
 * @brief  Set compliance with IEEE 802.1AS message format in autosar time sync protocol . \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarMsgCompliance Message compliance information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_msg_compliance().
 */
RtkApiRet rtk_ptp_atsar_set_msg_compliance(UnitChip_t unitChip, PtpAtsarMsgCompliance_t *ptpAtsarMsgCompliance)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarMsgCompliance)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_msg_compliance(unitChip.unit, ptpAtsarMsgCompliance->enable);
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
 * @brief  Set CRC validation mode in autosar time sync protocol . \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarRxCrcValidated CRC validation information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_msg_compliance().
 */
RtkApiRet rtk_ptp_atsar_set_rx_crc_validated(UnitChip_t unitChip, PtpAtsarRxCrcValidated_t *ptpAtsarRxCrcValidated)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarRxCrcValidated)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_rx_crc_validated(unitChip.unit, ptpAtsarRxCrcValidated->validated);
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
 * @brief  Set CrcFlagsRxValidated enable or disable. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarCrcFlags CRC flags information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to rtk_ptp_atsar_set_crc_flags().
 */
RtkApiRet rtk_ptp_atsar_set_crc_flags(UnitChip_t unitChip, PtpAtsarCrcFlags_t *ptpAtsarCrcFlags)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarCrcFlags)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_crc_flags(unitChip.unit, ptpAtsarCrcFlags->enable);
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
 * @brief  Set AUTOSAR Follow_Up TLV Time Sub-TLV is used or not. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarTimeSubtlv Time Sync Sub-TLV information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_time_subtlv().
 */
RtkApiRet rtk_ptp_atsar_set_time_subtlv(UnitChip_t unitChip, PtpAtsarTimeSubtlv_t *ptpAtsarTimeSubtlv)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarTimeSubtlv)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_time_subtlv(unitChip.unit, ptpAtsarTimeSubtlv->enable);
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
 * @brief  Set AUTOSAR Follow_Up TLV Status Sub-TLV is used or not. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarStatusSubtlv Status Sub-TLV information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_status_subtlv().
 */
RtkApiRet rtk_ptp_atsar_set_status_subtlv(UnitChip_t unitChip, PtpAtsarStatusSubtlv_t *ptpAtsarStatusSubtlv)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarStatusSubtlv)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_status_subtlv(unitChip.unit, ptpAtsarStatusSubtlv->enable);
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
 * @brief  Set AUTOSAR Follow_Up TLV UserData Sub-TLV is used or not. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarStatusSubtlv UserData Sub-TLV information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_udata_subtlv().
 */
RtkApiRet rtk_ptp_atsar_set_udata_subtlv(UnitChip_t unitChip, PtpAtsarUdataSubtlv_t *ptpAtsarUdataSubtlv)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarUdataSubtlv)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_udata_subtlv(unitChip.unit, ptpAtsarUdataSubtlv->enable);
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
 * @brief  Set AUTOSAR Follow_Up TLV Offset synchronization(OFS) Sub-TLV is used or not. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarStatusSubtlv UserData Sub-TLV information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_udata_subtlv().
 */
RtkApiRet rtk_ptp_atsar_set_ofs_subtlv(UnitChip_t unitChip, PtpAtsarOfsSubtlv_t *ptpAtsarOfsSubtlv)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarOfsSubtlv)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_ofs_subtlv(unitChip.unit, ptpAtsarOfsSubtlv->enable);
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
 * @brief  Set which parts of the Follow_Up messages elements shall be used for CRC calculation. \n
 *         Flag definition please refer to Autosar Time Sync Protocol. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarTxCrcTimeFlags CRC time flags information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_tx_crc_time_flags().
 */
RtkApiRet rtk_ptp_atsar_set_tx_crc_time_flags(UnitChip_t unitChip, PtpAtsarTxCrcTimeFlags_t *ptpAtsarTxCrcTimeFlags)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarTxCrcTimeFlags)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_tx_crc_time_flags(unitChip.unit, ptpAtsarTxCrcTimeFlags->flags);
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
 * @brief  Set configuration of whether or not CRC is supported. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarTxTimeCrcSecured Time CRC secured information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_tx_time_crc_secured().
 */
RtkApiRet rtk_ptp_atsar_set_tx_time_crc_secured(UnitChip_t unitChip, PtpAtsarTxTimeCrcSecured_t *ptpAtsarTxTimeCrcSecured)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarTxTimeCrcSecured)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_tx_time_crc_secured(unitChip.unit, ptpAtsarTxTimeCrcSecured->enable);
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
 * @brief  Set configuration disabling Pdelay_Resp, and Pdelay_Resp_Follow_Up transmission. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarPdelayRespEnable Pdelay resp enable information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_pdelay_resp_enable().
 */
RtkApiRet rtk_ptp_atsar_set_pdelay_resp_enable(UnitChip_t unitChip, PtpAtsarPdelayRespEnable_t *ptpAtsarPdelayRespEnable)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarPdelayRespEnable)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_pdelay_resp_enable(unitChip.unit, ptpAtsarPdelayRespEnable->enable);
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
 * @brief  Set PTP frame VLAN ID and priority field. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarTagVidPri VLAN tag and priority information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_tag_vid_pri_set().
 */
RtkApiRet rtk_ptp_tag_vid_pri_set(UnitChip_t unitChip, PtpAtsarTagVidPri_t *ptpAtsarTagVidPri)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarTagVidPri)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_tag_vid_pri_set(unitChip.unit, ptpAtsarTagVidPri->domainid, ptpAtsarTagVidPri->taginfo);
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
 * @brief  Set data id list array for Follow_Up message CRC calculation. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarFoloupDataidList Follow_Up data id list information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_foloup_dataid_list().
 */
RtkApiRet rtk_ptp_atsar_set_foloup_dataid_list(UnitChip_t unitChip, PtpAtsarFoloupDataidList_t *ptpAtsarFoloupDataidList)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarFoloupDataidList)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_foloup_dataid_list(unitChip.unit, ptpAtsarFoloupDataidList->dataid);
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
 * @brief  Set multiplicity of Offset synchronization(OFS) sub-TLV. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAtsarOfsMulticity OFS multiplicity information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_atsar_set_ofs_multicity().
 */
RtkApiRet rtk_ptp_atsar_set_ofs_multicity(UnitChip_t unitChip, PtpAtsarOfsMulticity_t *ptpAtsarOfsMulticity)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAtsarOfsMulticity)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_atsar_set_ofs_multicity(unitChip.unit, ptpAtsarOfsMulticity->ofsMulti);
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
 * @brief  Set syncReceiptTimeout configuration. \n
 *         Support chip: RTL907XD_VA,RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpSyncReceiptTimeout syncReceiptTimeout information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_set_sync_receipt_timeout().
 */
RtkApiRet rtk_ptp_set_sync_receipt_timeout(UnitChip_t unitChip, PtpSyncReceiptTimeout_t *ptpSyncReceiptTimeout)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSyncReceiptTimeout)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_ptp_set_sync_receipt_timeout(unitChip.unit, ptpSyncReceiptTimeout->domainid, ptpSyncReceiptTimeout->syncReceiptTimeout);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_sync_receipt_timeout(unitChip.unit, ptpSyncReceiptTimeout->domainid, ptpSyncReceiptTimeout->syncReceiptTimeout);
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
 * @brief  Get syncReceiptTimeout configuration. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpSyncReceiptTimeout syncReceiptTimeout information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_get_sync_receipt_timeout().
 */
RtkApiRet rtk_ptp_get_sync_receipt_timeout(UnitChip_t unitChip, PtpSyncReceiptTimeout_t *ptpSyncReceiptTimeout)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpSyncReceiptTimeout)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_sync_receipt_timeout(unitChip.unit, ptpSyncReceiptTimeout->domainid, &ptpSyncReceiptTimeout->syncReceiptTimeout);
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
 * @brief  Set allowed lost pdelay responses configuration. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAllowedLostPrespNum ptpAllowedLostPrespNum information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_set_allowed_lost_presp_num().
 */
RtkApiRet rtk_ptp_set_allowed_lost_presp_num(UnitChip_t unitChip, PtpAllowedLostPrespNum_t *ptpAllowedLostPrespNum)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAllowedLostPrespNum)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_set_allowed_lost_presp_num(unitChip.unit, ptpAllowedLostPrespNum->allowedLostResponses);
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
 * @brief  Get allowed lost pdelay responses configuration. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpAllowedLostPrespNum ptpAllowedLostPrespNum information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_drv_ptp_get_allowed_lost_presp_num().
 */
RtkApiRet rtk_ptp_get_allowed_lost_presp_num(UnitChip_t unitChip, PtpAllowedLostPrespNum_t *ptpAllowedLostPrespNum)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpAllowedLostPrespNum)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_get_allowed_lost_presp_num(unitChip.unit, &ptpAllowedLostPrespNum->allowedLostResponses);
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
 * @brief  Translate the time interval to log interval. \n
 *         Support chip: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed. \n
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] ptpIntvalToLog Interval to Log information structure. \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_ptp_intval_to_log().
 */
RtkApiRet rtk_ptp_intval_to_log(UnitChip_t unitChip, PtpIntvalToLog_t *ptpIntvalToLog)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ptpIntvalToLog)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_ptp_intval_to_log(unitChip.unit, ptpIntvalToLog->curntIntval, &ptpIntvalToLog->logIntval);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* PTP_PROTOCOL */

/**@}*/  // PTP
