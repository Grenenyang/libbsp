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
#include "rtkas_api_phy_test.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ext.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ext.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ext.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ext.h"
#endif

/**
 * @defgroup RTK_MODULE_PHY_TEST_MODE PHY Test
 * @brief PHY Test APIs. This component is for RTCT, test modes, and SQI.
 * @{
 */
/**
 * @defgroup RTCT RTCT Configuration
 * @brief RTCT APIs used to detect the cable status
 * @{
 */
/**
 * @brief Start to trigger RTK cable diagnostic.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in] rtctInfo RTCT trigger information (N/A). \n
 *                     Parameters required to trigger RTCT cable diagnostic.
 * @param[out] None
 * @return RT_ERR_OK           RTCT diagnostic was triggered.
 * @return -RT_ERR_NOT_SUPPORT Not supported.
 * @return Others              Please refer to dv1_rtct_run_cable_diag(), dva_rtct_run_cable_diag(), \n
 *                             cvb_rtct_run_cable_diag(), or cva_rtct_run_cable_diag().
 */
RtkApiRet rtk_rtct_run_cable_diag(UnitChip_t unitChip, RTCTInfo_t *rtctInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == rtctInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_rtct_run_cable_diag(unitChip.unit, rtctInfo->port);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_rtct_run_cable_diag(unitChip.unit, rtctInfo->port);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_rtct_run_cable_diag(unitChip.unit, rtctInfo->port);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_rtct_run_cable_diag(unitChip.unit, rtctInfo->port);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get RTK cable diagnostic result.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] rtctInfo RTCT get information (N/A). \n
 *                         Parameters required to obtain RTCT cable diagnostic.
 * @return RT_ERR_OK           RTCT diagnostic was finished.
 * @return -RT_ERR_NOT_SUPPORT Not supported.
 * @return Others              Please refer to dv1_rtct_get_cable_status(), dva_rtct_get_cable_status(), \n
 *                             cvb_rtct_get_cable_status(), or cva_rtct_get_cable_status().
 */
RtkApiRet rtk_rtct_get_cable_status(UnitChip_t unitChip, RTCTInfo_t *rtctInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == rtctInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_rtct_get_cable_status(unitChip.unit, rtctInfo->port, &(rtctInfo->status), &(rtctInfo->defeat));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_rtct_get_cable_status(unitChip.unit, rtctInfo->port, &(rtctInfo->status), &(rtctInfo->defeat));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_rtct_get_cable_status(unitChip.unit, rtctInfo->port, &(rtctInfo->status), &(rtctInfo->defeat));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_rtct_get_cable_status(unitChip.unit, rtctInfo->port, &(rtctInfo->status), &(rtctInfo->defeat));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* RTCT */

/**
 * @defgroup PTM UNH IOL Test - 100/1000 BASE-T1
 * @brief PHY test mode APIs provided to test signals according to IEEE 802.3bp and IEEE 802.3bw
 * @{
 */
/**
 * @brief Set test mode status.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in] phyTMInfo PHY test mode set information (N/A). \n
 *                      Parameters required to set PHY test mode.
 * @param[out] None
 * @return RT_ERR_OK           PHY test mode was set.
 * @return -RT_ERR_NOT_SUPPORT Not supported.
 * @return Others              Please refer to dv1_phy_testmode_set(), dva_phy_testmode_set(), \n
 *                             cvb_phy_testmode_set(), or cva_phy_testmode_set().
 */
RtkApiRet rtk_phy_testmode_set(UnitChip_t unitChip, PhyTestModeInfo_t *phyTMInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == phyTMInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_testmode_set(unitChip.unit, phyTMInfo->port, phyTMInfo->testMode, phyTMInfo->speed, phyTMInfo->isMaster);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_testmode_set(unitChip.unit, phyTMInfo->port, phyTMInfo->testMode, phyTMInfo->speed, phyTMInfo->isMaster);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_testmode_set(unitChip.unit, phyTMInfo->port, phyTMInfo->testMode, phyTMInfo->speed, phyTMInfo->isMaster);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_testmode_set(unitChip.unit, phyTMInfo->port, phyTMInfo->testMode, phyTMInfo->speed, phyTMInfo->isMaster);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get test mode status.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] phyTMInfo PHY test mode get information (N/A). \n
 *                          Parameters required to obtain PHY test mode.
 * @param[out] None
 * @return RT_ERR_OK           PHY test mode was obtained.
 * @return -RT_ERR_NOT_SUPPORT Not supported.
 * @return Others              Please refer to dv1_phy_testmode_get(), dva_phy_testmode_get(), \n
 *                             cvb_phy_testmode_get(), or cva_phy_testmode_get().
 */
RtkApiRet rtk_phy_testmode_get(UnitChip_t unitChip, PhyTestModeInfo_t *phyTMInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == phyTMInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_phy_testmode_get(unitChip.unit, phyTMInfo->port, phyTMInfo->speed, &(phyTMInfo->testMode));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_phy_testmode_get(unitChip.unit, phyTMInfo->port, phyTMInfo->speed, &(phyTMInfo->testMode));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_phy_testmode_get(unitChip.unit, phyTMInfo->port, phyTMInfo->speed, &(phyTMInfo->testMode));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_phy_testmode_get(unitChip.unit, phyTMInfo->port, phyTMInfo->speed, &(phyTMInfo->testMode));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* PTM */

/**
 * @defgroup SQIs SQI APIs
 * @brief APIs used to indicate MDI link conditions, including dynamic channel quality and link quality
 * @{
 */
/**
 * @defgroup DCQ Dynamic Channel Quality
 * @brief Includes APIs to obtain the measurements of MSE, pMSE, and SQI
 * @{
 */

/**
 * @defgroup MSE Mean Square Error
 * @brief Measurement of the average of the squares of the errors
 * @{
 */
/**
 * @brief Get MSE measurement.
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] mseGetInfo MSE information (N/A). \n
 *                           Parameters required to obtain MSE.
 * @param[out] None
 * @return RT_ERR_OK            MSE was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dva_port_mse_get().
 */
RtkApiRet rtk_port_mse_get(UnitChip_t unitChip, MseGetInfo_t *mseGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == mseGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_mse_get(unitChip.unit, mseGetInfo->port, &(mseGetInfo->value));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get the worst case of MSE measurement.
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] wcMseGetInfo The information of the worst case of MSE (N/A). \n
 *                             Parameters required to obtain the worst case of MSE.
 * @param[out] None
 * @return RT_ERR_OK            The value was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dva_port_wc_mse_get().
 */
RtkApiRet rtk_port_wc_mse_get(UnitChip_t unitChip, WcMseGetInfo_t *wcMseGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == wcMseGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_wc_mse_get(unitChip.unit, wcMseGetInfo->port, &(wcMseGetInfo->value));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* MSE */

/**
 * @defgroup PMSE Peak Mean Square Error
 * @brief Measurement of the peak MSE
 * @{
 */
/**
 * @brief Get the peak MSE measurement.
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] pMseGetInfo The peak MSE information (N/A). \n
 *                            Parameters required to obtain the peak MSE.
 * @param[out] None
 * @return RT_ERR_OK            The peak MSE was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dva_port_peak_mse_get().
 */
RtkApiRet rtk_port_peak_mse_get(UnitChip_t unitChip, PeakMseGetInfo_t *pMseGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == pMseGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_peak_mse_get(unitChip.unit, pMseGetInfo->port, &(pMseGetInfo->value));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get the worst case of the peak MSE measurement.
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] wcPkMseGetInfo The worst case of the peak MSE information (N/A). \n
 *                               Parameters required to obtain the worst case of the peak MSE.
 * @param[out] None
 * @return RT_ERR_OK            The worst case of the peak MSE was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dva_port_wc_peak_mse_get().
 */
RtkApiRet rtk_port_wc_peak_mse_get(UnitChip_t unitChip, WcPeakMseGetInfo_t *wcPkMseGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == wcPkMseGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_wc_peak_mse_get(unitChip.unit, wcPkMseGetInfo->port, &(wcPkMseGetInfo->value));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* PMSE */

/**
 * @defgroup SQI Signal Quality Index
 * @brief Indicate the performance of the communication.
 * @{
 */
/**
 * @brief Get SQI index.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] sqiGetInfo PHY reset information (N/A). \n
 *                           Parameters required to obtain SQI.
 * @param[out] None
 * @return RT_ERR_OK           SQI was obtained.
 * @return -RT_ERR_NOT_SUPPORT Not supported.
 * @return Others              Please refer to dv1_port_sqi_get(), dva_port_sqi_get(), \n
 *                             cvb_port_sqi_get(), or cva_port_sqi_get().
 * @note This API sets SQI for a T1 port.
 */
RtkApiRet rtk_port_sqi_get(UnitChip_t unitChip, SqiGetInfo_t *sqiGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == sqiGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_port_sqi_get(unitChip.unit, sqiGetInfo->port, sqiGetInfo->speed, &(sqiGetInfo->sqi));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_sqi_get(unitChip.unit, sqiGetInfo->port, sqiGetInfo->speed, &(sqiGetInfo->sqi));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_port_sqi_get(unitChip.unit, sqiGetInfo->port, sqiGetInfo->speed, &(sqiGetInfo->sqi));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_port_sqi_get(unitChip.unit, sqiGetInfo->port, sqiGetInfo->speed, &(sqiGetInfo->sqi));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* SQI */
/**@}*/ /* DCQ */

/**
 * @defgroup LQ Link Quality - Start-up Time and Link Losses (LQ)
 * @brief Includes APIs to obtain the measurements of LTT, LRT, RRT, and LFL.
 * @{
 */
/**
 * @defgroup LTT Link Training Time
 * @brief The time required to establish a link.
 * @{
 */
/**
 * @brief Get LTT measurement.
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] lttGetInfo LTT information (N/A). \n
 *                           Parameters required to obtain LTT.
 * @param[out] None
 * @return RT_ERR_OK            The result was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dva_port_ltt_get().
 */
RtkApiRet rtk_port_ltt_get(UnitChip_t unitChip, LttGetInfo_t *lttGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == lttGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_ltt_get(unitChip.unit, lttGetInfo->port, &(lttGetInfo->lttTime));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* LTT */

/**
 * @defgroup LRT Local Receiver Time
 * @brief The time required until the local receiver is locked.
 * @{
 */
/**
 * @brief Get LRT measurement.
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] lrtGetInfo LRT information (N/A). \n
 *                           Parameters required to obtain LRT.
 * @param[out] None
 * @return RT_ERR_OK            The result was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dva_port_lrt_get().
 */
RtkApiRet rtk_port_lrt_get(UnitChip_t unitChip, LrtGetInfo_t *lrtGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == lrtGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_lrt_get(unitChip.unit, lrtGetInfo->port, &(lrtGetInfo->lrtTime));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* LRT */

/**
 * @defgroup RRT Remote Receiver Time
 * @brief The time required for the remote receiver to send data and become locked.
 * @{
 */
/**
 * @brief Get RRT measurement.
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] rrtGetInfo RRT information (N/A). \n
 *                           Parameters required to obtain RRT.
 * @param[out] None
 * @return RT_ERR_OK            The result was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dva_port_rrt_get().
 */
RtkApiRet rtk_port_rrt_get(UnitChip_t unitChip, RrtGetInfo_t *rrtGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == rrtGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_rrt_get(unitChip.unit, rrtGetInfo->port, &(rrtGetInfo->rrtTime));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* RRT */

/**
 * @defgroup LFL Link Failures and Losses
 * @brief The number of failures that occurred since the last power cycle.
 * @{
 */
/**
 * @brief Get the link loss counter.
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] lflGetInfo LFL information (N/A). \n
 *                           Parameters required to obtain LFL.
 * @param[out] None
 * @return RT_ERR_OK            The result was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dva_port_lfl_loss_get().
 */
RtkApiRet rtk_port_lfl_loss_get(UnitChip_t unitChip, LflGetInfo_t *lflGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == lflGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_lfl_loss_get(unitChip.unit, lflGetInfo->port, &(lflGetInfo->lossCnt));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief Get the link failure counter.
 *        Supported chips: RTL907XD_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] lflGetInfo LFL information (N/A). \n
 *                           Parameters required to obtain LFL.
 * @param[out] None
 * @return RT_ERR_OK            The result was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  Not supported.
 * @return Others               Please refer to dva_port_lfl_fail_get().
 */
RtkApiRet rtk_port_lfl_fail_get(UnitChip_t unitChip, LflGetInfo_t *lflGetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == lflGetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_lfl_fail_get(unitChip.unit, lflGetInfo->port, &(lflGetInfo->failCnt));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* LFL */
/**@}*/ /* LQ */

/**@}*/ /* SQIs */
/**@}*/ /* RTK_MODULE_PHY_TEST_MODE */
