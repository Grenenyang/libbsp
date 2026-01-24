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
#include "rtkas_api_flow_ctrl.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_qos.h"
#include "rtl907xd_v1_api_flow_ctrl.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_qos.h"
#include "rtl907xd_va_api_flow_ctrl.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_qos.h"
#include "rtl907xc_vb_api_flow_ctrl.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_qos.h"
#include "rtl907xc_va_api_flow_ctrl.h"
#endif

/**
 * @defgroup RTK_MODULE_FLOWCTRL Flow Control
 * @brief Flow Control API. This component is used to configure the flow control functionality.
 * @{
 */

/**
 * @defgroup FLOWCTRL_EN Flow Control Status
 * @brief Configure or retrieve the flow control status, which can be set to either enabled or disabled.
 * @{
 */

/**
 * @brief Set Flow Control status. \n
 *        Configure the flow control for an RX port to either enable or disable it, based on specific requirements. \n
 *        If the flow control of a port is enabled, the port will recognize incoming RX pause frames and will be able to transmit TX pause frames when the port's buffer reaches a certain threshold. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] flowctrlEn Flowctrl enable (en) (N/A) \n
 *            The struct includes the port and enable (en) value. \n
 *            The port refers to the RX port number, and the enable value indicates whether to enable or disable the flow control function.
 * @param[out] None
 * @retval RT_ERR_OK              The flow control status has been successfully set.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others Please refer to dv1_flow_ctrl_en_set(), dva_flow_ctrl_en_set(), cvb_flow_ctrl_en_set(), cva_flow_ctrl_en_set().
 */
RtkApiRet rtk_flow_ctrl_en_set(UnitChip_t unitChip, const FlowctrlEn_t *flowctrlEn)
{
    RtkApiRet ret;

    if(NULL == flowctrlEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_flow_ctrl_en_set(unitChip.unit, flowctrlEn->port, flowctrlEn->en);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_flow_ctrl_en_set(unitChip.unit, flowctrlEn->port, flowctrlEn->en);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_flow_ctrl_en_set(unitChip.unit, flowctrlEn->port, flowctrlEn->en);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_flow_ctrl_en_set(unitChip.unit, flowctrlEn->port, flowctrlEn->en);
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
 * @brief Get Flow Control status. \n
 *        Get the flow control status to determine if the port can handle RX pause frames and issue TX pause frames under Ethernet flow control. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] flowctrlEn Flowctrl enable (en) (N/A) \n
 *                The struct includes the port and enable (en) value. \n
 *                The port refers to the RX port number, and the enable value indicates whether to enable or disable the flow control function.
 * @retval RT_ERR_OK              The flow control status was successfully retrieved.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others Please refer to dv1_flow_ctrl_en_get(), dva_flow_ctrl_en_get(), cvb_flow_ctrl_en_get(), cva_flow_ctrl_en_get().
 */
RtkApiRet rtk_flow_ctrl_en_get(UnitChip_t unitChip, FlowctrlEn_t *flowctrlEn)
{
    RtkApiRet ret;

    if(NULL == flowctrlEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_flow_ctrl_en_get(unitChip.unit, flowctrlEn->port, &(flowctrlEn->en));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_flow_ctrl_en_get(unitChip.unit, flowctrlEn->port, &(flowctrlEn->en));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_flow_ctrl_en_get(unitChip.unit, flowctrlEn->port, &(flowctrlEn->en));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_flow_ctrl_en_get(unitChip.unit, flowctrlEn->port, &(flowctrlEn->en));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* FLOWCTRL_EN */

/**
 * @defgroup FLOWCTRL_PAGE_CNT Flow Control page count
 * @brief Configure or retrieve the flow control page count for each TX port and TX queue according to specific requirements.
 * @{
 */

/**
 * @brief Set the page count for a specific queue on a specific TX port. \n
 *        Configures the flow control page count to set threshold levels for TX port queues, managing traffic flow and preventing congestion. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] flowctrlPageCnt Flowctrl page count (N/A) \n
 *            The struct includes the port, queue and page count. \n
 *            The port refers to the TX port number, the queue refers to the TX queue number and  the page count refers to the threshold level.
 * @param[out] None
 * @retval RT_ERR_OK              The flow control page count has been successfully set.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others Please refer to dv1_flow_ctrl_page_cnt_set(), dva_flow_ctrl_page_cnt_set(), cvb_flow_ctrl_page_cnt_set(), cva_flow_ctrl_page_cnt_set().
 */
RtkApiRet rtk_flow_ctrl_page_cnt_set(UnitChip_t unitChip, const FlowctrlPageCnt_t *flowctrlPageCnt)
{
    RtkApiRet ret;

    if(NULL == flowctrlPageCnt)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_flow_ctrl_page_cnt_set(unitChip.unit, flowctrlPageCnt->port, flowctrlPageCnt->que, flowctrlPageCnt->pageCnt);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_flow_ctrl_page_cnt_set(unitChip.unit, flowctrlPageCnt->port, flowctrlPageCnt->que, flowctrlPageCnt->pageCnt);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_flow_ctrl_page_cnt_set(unitChip.unit, flowctrlPageCnt->port, flowctrlPageCnt->que, flowctrlPageCnt->pageCnt);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_flow_ctrl_page_cnt_set(unitChip.unit, flowctrlPageCnt->port, flowctrlPageCnt->que, flowctrlPageCnt->pageCnt);
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
 * @brief Retrieves the flow control page count, providing the threshold levels set for TX port queues to manage traffic flow. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] flowctrlPageCnt Flowctrl page count (N/A) \n
 *                The struct includes the port, queue and page count. \n
 *                The port refers to the TX port number, the queue refers to the TX queue number and  the page count refers to the threshold level.
 * @retval RT_ERR_OK              The flow control page count was successfully retrieved.
 * @retval -RT_ERR_NOT_SUPPORT    Not supported.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others Please refer to dv1_flow_ctrl_page_cnt_get(), dva_flow_ctrl_page_cnt_get(), cvb_flow_ctrl_page_cnt_get(), cva_flow_ctrl_page_cnt_get().
 */
RtkApiRet rtk_flow_ctrl_page_cnt_get(UnitChip_t unitChip, FlowctrlPageCnt_t *flowctrlPageCnt)
{
    RtkApiRet ret;

    if(NULL == flowctrlPageCnt)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_flow_ctrl_page_cnt_get(unitChip.unit, flowctrlPageCnt->port, flowctrlPageCnt->que, &(flowctrlPageCnt->pageCnt));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_flow_ctrl_page_cnt_get(unitChip.unit, flowctrlPageCnt->port, flowctrlPageCnt->que, &(flowctrlPageCnt->pageCnt));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_flow_ctrl_page_cnt_get(unitChip.unit, flowctrlPageCnt->port, flowctrlPageCnt->que, &(flowctrlPageCnt->pageCnt));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_flow_ctrl_page_cnt_get(unitChip.unit, flowctrlPageCnt->port, flowctrlPageCnt->que, &(flowctrlPageCnt->pageCnt));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* FLOWCTRL_PAGE_CNT */

/**@}*/ /* Flow Control */
