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
#include "rtkas_api_qav.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api.h"
#include "rtl907xd_v1_api_qos.h"
#include "rtl907xd_v1_api_qav.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_api_qos.h"
#include "rtl907xd_va_api_qav.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api.h"
#include "rtl907xc_vb_api_qos.h"
#include "rtl907xc_vb_api_qav.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api.h"
#include "rtl907xc_va_api_qos.h"
#include "rtl907xc_va_api_qav.h"
#endif

/**
 * @defgroup RTK_MODULE_QAV QAV
 * @brief Qav API. This component is used to configure the Qav functionality.
 * @{
 */
/**
 * @defgroup QAV_TS Traffic Scheduling
 * @brief Traffic scheduling includes Credit-Based Shaping (CBS) and slope setting parameters for precise bandwidth management and efficient data transmission.
 * @{
 */
/**
 * @brief Set the transmission selection algorithm to Strict Priority or Credit Based Shaper and set CBS bandwidth for specific egress port and specific egress queue. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qavCbs Qav CBS (N/A) \n
 *            The struct includes the port, queue, enable and bandwidth. \n
 *            The egress port, egress queue number, enable value and limited bandwidth parameters define the specific transmission port, the queue for outgoing traffic, the enable value, and the constrained bandwidth for traffic shaping, respectively.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qav_cbs_set(), dva_qav_cbs_set(), cvb_qav_cbs_set(), cva_qav_cbs_set().
 */
RtkApiRet rtk_qav_cbs_set(UnitChip_t unitChip, const QavCbs_t *qavCbs)
{
    RtkApiRet ret;

    if(NULL == qavCbs)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qav_cbs_set(unitChip.unit, qavCbs->port, qavCbs->que, qavCbs->en, qavCbs->bw);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qav_cbs_set(unitChip.unit, qavCbs->port, qavCbs->que, qavCbs->en, qavCbs->bw);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qav_cbs_set(unitChip.unit, qavCbs->port, qavCbs->que, qavCbs->en, qavCbs->bw);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qav_cbs_set(unitChip.unit, qavCbs->port, qavCbs->que, qavCbs->en, qavCbs->bw);
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
 * @brief Get the transmission selection algorithm and read the CBS bandwidth from specific egress port and specific egress queue. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qavCbs Qav CBS (N/A) \n
 *                The struct includes the port, queue, enable and bandwidth. \n
 *                The egress port, egress queue number, enable value and limited bandwidth parameters define the specific transmission port, the queue for outgoing traffic, the enable value, and the constrained bandwidth for traffic shaping, respectively.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qav_cbs_get(), dva_qav_cbs_get(), cvb_qav_cbs_get(), cva_qav_cbs_get().
 */
RtkApiRet rtk_qav_cbs_get(UnitChip_t unitChip, QavCbs_t *qavCbs)
{
    RtkApiRet ret;

    if(NULL == qavCbs)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qav_cbs_get(unitChip.unit, qavCbs->port, qavCbs->que, &qavCbs->en, &qavCbs->bw);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qav_cbs_get(unitChip.unit, qavCbs->port, qavCbs->que, &qavCbs->en, &qavCbs->bw);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qav_cbs_get(unitChip.unit, qavCbs->port, qavCbs->que, &qavCbs->en, &qavCbs->bw);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qav_cbs_get(unitChip.unit, qavCbs->port, qavCbs->que, &qavCbs->en, &qavCbs->bw);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* QAV_TS */

/**
 * @defgroup QAV_TC Traffic Classification
 * @brief Traffic classification includes priority regeneration, with the option to select either the inner or outer tag for regeneration.
 * @{
 */
/**
 * @brief Set the priority regeneration table for the specific priority in the specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qavPriRegen Qav priority regeneration (N/A) \n
 *            The struct includes the port, oriPri, en and regenPri. \n
 *            The port is the transmission port, where oriPri represents the original priority, en indicates the enable value, and regenPri denotes the regenerated priority.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qav_pri_regen_set(), dva_qav_pri_regen_set(), cvb_qav_pri_regen_set(), cva_qav_pri_regen_set().
 */
RtkApiRet rtk_qav_pri_regen_set(UnitChip_t unitChip, const QavPriRegen_t *qavPriRegen)
{
    RtkApiRet ret;

    if(NULL == qavPriRegen)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qav_pri_regen_set(unitChip.unit, qavPriRegen->port, qavPriRegen->oriPri, qavPriRegen->en, qavPriRegen->regenPri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qav_pri_regen_set(unitChip.unit, qavPriRegen->port, qavPriRegen->oriPri, qavPriRegen->en, qavPriRegen->regenPri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qav_pri_regen_set(unitChip.unit, qavPriRegen->port, qavPriRegen->oriPri, qavPriRegen->en, qavPriRegen->regenPri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qav_pri_regen_set(unitChip.unit, qavPriRegen->port, qavPriRegen->oriPri, qavPriRegen->en, qavPriRegen->regenPri);
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
 * @brief Get the priority regeneration table for the specific priority in the specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qavPriRegen Qav priority regeneration (N/A) \n
 *                The struct includes the port, oriPri, en and regenPri. \n
 *                The port is the transmission port, where oriPri represents the original priority, en indicates the enable value, and regenPri denotes the regenerated priority.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qav_pri_regen_get(), dva_qav_pri_regen_get(), cvb_qav_pri_regen_get(), cva_qav_pri_regen_get().
 */
RtkApiRet rtk_qav_pri_regen_get(UnitChip_t unitChip, QavPriRegen_t *qavPriRegen)
{
    RtkApiRet ret;

    if(NULL == qavPriRegen)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qav_pri_regen_get(unitChip.unit, qavPriRegen->port, qavPriRegen->oriPri, &qavPriRegen->en, &qavPriRegen->regenPri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qav_pri_regen_get(unitChip.unit, qavPriRegen->port, qavPriRegen->oriPri, &qavPriRegen->en, &qavPriRegen->regenPri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qav_pri_regen_get(unitChip.unit, qavPriRegen->port, qavPriRegen->oriPri, &qavPriRegen->en, &qavPriRegen->regenPri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qav_pri_regen_get(unitChip.unit, qavPriRegen->port, qavPriRegen->oriPri, &qavPriRegen->en, &qavPriRegen->regenPri);
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
 * @brief Set the priority selection for regeneration. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qavRegenTagSel Qav regeneration tag selection (N/A) \n
 *            The struct includes the port, enable of inner priority and enable of outer priority. \n
 *            The port is the transmission port, where enIpri indicates the enable value for the inner tag and enOpri indicates the enable value for the outer tag.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qav_regen_tag_sel_set(), dva_qav_regen_tag_sel_set(), cvb_qav_regen_tag_sel_set(), cva_qav_regen_tag_sel_set().
 */
RtkApiRet rtk_qav_regen_tag_sel_set(UnitChip_t unitChip, const QavRegenTagSel_t *qavRegenTagSel)
{
    RtkApiRet ret;

    if(NULL == qavRegenTagSel)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qav_regen_tag_sel_set(unitChip.unit, qavRegenTagSel->port, qavRegenTagSel->enIpri, qavRegenTagSel->enOpri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qav_regen_tag_sel_set(unitChip.unit, qavRegenTagSel->port, qavRegenTagSel->enIpri, qavRegenTagSel->enOpri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qav_regen_tag_sel_set(unitChip.unit, qavRegenTagSel->port, qavRegenTagSel->enIpri, qavRegenTagSel->enOpri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qav_regen_tag_sel_set(unitChip.unit, qavRegenTagSel->port, qavRegenTagSel->enIpri, qavRegenTagSel->enOpri);
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
 * @brief Get the priority selection for regeneration. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qavRegenTagSel Qav regeneration tag selection (N/A) \n
 *                The struct includes the port, enable of inner priority and enable of outer priority. \n
 *                The port is the transmission port, where enIpri indicates the enable value for the inner tag and enOpri indicates the enable value for the outer tag.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qav_regen_tag_sel_get(), dva_qav_regen_tag_sel_get(), cvb_qav_regen_tag_sel_get(), cva_qav_regen_tag_sel_get().
 */
RtkApiRet rtk_qav_regen_tag_sel_get(UnitChip_t unitChip, QavRegenTagSel_t *qavRegenTagSel)
{
    RtkApiRet ret;

    if(NULL == qavRegenTagSel)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qav_regen_tag_sel_get(unitChip.unit, qavRegenTagSel->port, &qavRegenTagSel->enIpri, &qavRegenTagSel->enOpri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qav_regen_tag_sel_get(unitChip.unit, qavRegenTagSel->port, &qavRegenTagSel->enIpri, &qavRegenTagSel->enOpri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qav_regen_tag_sel_get(unitChip.unit, qavRegenTagSel->port, &qavRegenTagSel->enIpri, &qavRegenTagSel->enOpri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qav_regen_tag_sel_get(unitChip.unit, qavRegenTagSel->port, &qavRegenTagSel->enIpri, &qavRegenTagSel->enOpri);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* QAV_TC */

/**@}*/ /* QAV */
