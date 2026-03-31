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
#include "rtkas_api_qos.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api.h"
#include "rtl907xd_v1_api_qos.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api.h"
#include "rtl907xd_va_api_qos.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api.h"
#include "rtl907xc_vb_api_qos.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api.h"
#include "rtl907xc_va_api_qos.h"
#endif

/**
 * @defgroup RTK_MODULE_QOS QOS
 * @brief QoS API. This component is used to configure the QoS functionality.
 * @{
 */

/**
 * @defgroup QOS_IGR_BW Ingress Bandwidth Control
 * @brief Ingress Bandwidth Control includes the functionality to set and get ingress bandwidth.
 * @{
 */

/**
 * @brief Set port ingress bandwidth and flow control ability. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosIgrBwCtrlInfo QoS ingresss bandwidth control (N/A) \n
 *            The struct includes the port, bwCtrlEn0, bwCtrlEn1, igrBw0, igrBw1, flowCtrlEn0 and flowCtrlEn1. \n
 *            The port is the ingress port number. \n
 *            The values bwCtrlEn0 and bwCtrlEn1 indicate whether to enable or disable the bandwidth control function.\n
 *            The igrBw0 and igrBw1 values indicate the limited inbound bandwidth.\n
 *            Finally, the values flowCtrlEn0 and flowCtrlEn1 determine whether to enable or disable the flow control function, which helps in managing congestion by pausing the transmission of packets when necessary.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_igr_bw_ctrl_set(), dva_qos_igr_bw_ctrl_set(), cvb_qos_igr_bw_ctrl_set(), cva_qos_igr_bw_ctrl_set().
 */
RtkApiRet rtk_qos_igr_bw_ctrl_set(UnitChip_t unitChip, const QosIgrBwCtrlInfo_t *qosIgrBwCtrlInfo)
{
    RtkApiRet ret;

    if(NULL == qosIgrBwCtrlInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_igr_bw_ctrl_set(unitChip.unit, qosIgrBwCtrlInfo->port, &(qosIgrBwCtrlInfo->qosIgrBwCtrl));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_igr_bw_ctrl_set(unitChip.unit, qosIgrBwCtrlInfo->port, &(qosIgrBwCtrlInfo->qosIgrBwCtrl));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_igr_bw_ctrl_set(unitChip.unit, qosIgrBwCtrlInfo->port, &(qosIgrBwCtrlInfo->qosIgrBwCtrl));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_igr_bw_ctrl_set(unitChip.unit, qosIgrBwCtrlInfo->port, &(qosIgrBwCtrlInfo->qosIgrBwCtrl));
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
 * @brief Get port ingress bandwidth and flow control ability. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosIgrBwCtrlInfo QoS ingresss bandwidth control (N/A) \n
 *                The struct includes the port, bwCtrlEn0, bwCtrlEn1, igrBw0, igrBw1, flowCtrlEn0 and flowCtrlEn1. \n
 *                The port is the ingress port number. \n
 *                The values bwCtrlEn0 and bwCtrlEn1 indicate whether to enable or disable the bandwidth control function.\n
 *                The igrBw0 and igrBw1 values indicate the limited inbound bandwidth.\n
 *                Finally, the values flowCtrlEn0 and flowCtrlEn1 determine whether to enable or disable the flow control function, which helps in managing congestion by pausing the transmission of packets when necessary.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_igr_bw_ctrl_get(), dva_qos_igr_bw_ctrl_get(), cvb_qos_igr_bw_ctrl_get(), cva_qos_igr_bw_ctrl_get().
 */
RtkApiRet rtk_qos_igr_bw_ctrl_get(UnitChip_t unitChip, QosIgrBwCtrlInfo_t *qosIgrBwCtrlInfo)
{
    RtkApiRet ret;

    if(NULL == qosIgrBwCtrlInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_igr_bw_ctrl_get(unitChip.unit, qosIgrBwCtrlInfo->port, &(qosIgrBwCtrlInfo->qosIgrBwCtrl));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_igr_bw_ctrl_get(unitChip.unit, qosIgrBwCtrlInfo->port, &(qosIgrBwCtrlInfo->qosIgrBwCtrl));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_igr_bw_ctrl_get(unitChip.unit, qosIgrBwCtrlInfo->port, &(qosIgrBwCtrlInfo->qosIgrBwCtrl));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_igr_bw_ctrl_get(unitChip.unit, qosIgrBwCtrlInfo->port, &(qosIgrBwCtrlInfo->qosIgrBwCtrl));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* QOS_IGR_BW */

/**
 * @defgroup QOS_TC Traffic Classification
 * @brief Traffic Classification includes the functionality to set and get port priority, priority selection, weight table, CVLAN priority remapping, SVLAN priority remapping, DSCP priority remapping, CVLAN priority remapping table, SVLAN priority remapping table, DSCP priority remapping table and priority-to-queue mapping.
 * @{
 */

/**
 * @brief Set the internal priority for port-based priority source. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosPortPri QoS port priority (N/A) \n
 *            The struct includes the port and priority. \n
 *            The port is the ingress port number. \n
 *            The priority is the Quality of Service (QoS) port-based priority.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_port_pri_set(), dva_qos_port_pri_set(), cvb_qos_port_pri_set(), cva_qos_port_pri_set().
 */
RtkApiRet rtk_qos_port_pri_set(UnitChip_t unitChip, const QosPortPri_t *qosPortPri)
{
    RtkApiRet ret;

    if(NULL == qosPortPri)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_port_pri_set(unitChip.unit, qosPortPri->port, qosPortPri->pri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_port_pri_set(unitChip.unit, qosPortPri->port, qosPortPri->pri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_port_pri_set(unitChip.unit, qosPortPri->port, qosPortPri->pri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_port_pri_set(unitChip.unit, qosPortPri->port, qosPortPri->pri);
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
 * @brief Get the internal priority for port-based priority source. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosPortPri QoS port priority (N/A) \n
 *                The struct includes the port and priority. \n
 *                The port is the ingress port number. \n
 *                The priority is the Quality of Service (QoS) port-based priority.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_port_pri_get(), dva_qos_port_pri_get(), cvb_qos_port_pri_get(), cva_qos_port_pri_get().
 */
RtkApiRet rtk_qos_port_pri_get(UnitChip_t unitChip, QosPortPri_t *qosPortPri)
{
    RtkApiRet ret;

    if(NULL == qosPortPri)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_port_pri_get(unitChip.unit, qosPortPri->port, &qosPortPri->pri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_port_pri_get(unitChip.unit, qosPortPri->port, &qosPortPri->pri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_port_pri_get(unitChip.unit, qosPortPri->port, &qosPortPri->pri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_port_pri_get(unitChip.unit, qosPortPri->port, &qosPortPri->pri);
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
 * @brief Set a weight for each priority source. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosPriSelInfo QoS priority selection information (N/A) \n
 *            The struct includes the group, portPri, hybridPri, dscpPri, cvlanPri and svlanPri. \n
 *            The group is the arbitration table index. \n
 *            The portPri is the weight of port-based priority. \n
 *            The hybridPri is the weight of hybrid-based priority. \n
 *            The dscpPri is the weight of DSCP priority. \n
 *            The cvlanPri is the weight of CVLAN priority. \n
 *            The svlanPri is the weight of SVLAN priority.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_pri_sel_set(), dva_qos_pri_sel_set(), cvb_qos_pri_sel_set(), cva_qos_pri_sel_set().
 */
RtkApiRet rtk_qos_pri_sel_set(UnitChip_t unitChip, const QosPriSelInfo_t *qosPriSelInfo)
{
    RtkApiRet ret;

    if(NULL == qosPriSelInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_pri_sel_set(unitChip.unit, qosPriSelInfo->group, &(qosPriSelInfo->qosPriSel));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_pri_sel_set(unitChip.unit, qosPriSelInfo->group, &(qosPriSelInfo->qosPriSel));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_pri_sel_set(unitChip.unit, qosPriSelInfo->group, &(qosPriSelInfo->qosPriSel));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_pri_sel_set(unitChip.unit, qosPriSelInfo->group, &(qosPriSelInfo->qosPriSel));
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
 * @brief Get weights of priority sources. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosPriSelInfo QoS priority selection information (N/A) \n
 *                The struct includes the group, portPri, hybridPri, dscpPri, cvlanPri and svlanPri. \n
 *                The group is the arbitration table index. \n
 *                The portPri is the weight of port-based priority. \n
 *                The hybridPri is the weight of hybrid-based priority. \n
 *                The dscpPri is the weight of DSCP priority. \n
 *                The cvlanPri is the weight of CVLAN priority. \n
 *                The svlanPri is the weight of SVLAN priority.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_pri_sel_get(), dva_qos_pri_sel_get(), cvb_qos_pri_sel_get(), cva_qos_pri_sel_get().
 */
RtkApiRet rtk_qos_pri_sel_get(UnitChip_t unitChip, QosPriSelInfo_t *qosPriSelInfo)
{
    RtkApiRet ret;

    if(NULL == qosPriSelInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_pri_sel_get(unitChip.unit, qosPriSelInfo->group, &(qosPriSelInfo->qosPriSel));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_pri_sel_get(unitChip.unit, qosPriSelInfo->group, &(qosPriSelInfo->qosPriSel));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_pri_sel_get(unitChip.unit, qosPriSelInfo->group, &(qosPriSelInfo->qosPriSel));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_pri_sel_get(unitChip.unit, qosPriSelInfo->group, &(qosPriSelInfo->qosPriSel));
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
 * @brief Set a specific ingress port to use priority arbitration table index. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosPriWtTbl QoS priority weight table (N/A) \n
 *            The struct includes the port and index (idx). \n
 *            The port is the ingress port number. \n
 *            The index is the weight table index.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_pri_wt_tbl_set(), dva_qos_pri_wt_tbl_set(), cvb_qos_pri_wt_tbl_set(), cva_qos_pri_wt_tbl_set().
 */
RtkApiRet rtk_qos_pri_wt_tbl_set(UnitChip_t unitChip, const QosPriWtTbl_t *qosPriWtTbl)
{
    RtkApiRet ret;

    if(NULL == qosPriWtTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_pri_wt_tbl_set(unitChip.unit, qosPriWtTbl->port, qosPriWtTbl->idx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_pri_wt_tbl_set(unitChip.unit, qosPriWtTbl->port, qosPriWtTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_pri_wt_tbl_set(unitChip.unit, qosPriWtTbl->port, qosPriWtTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_pri_wt_tbl_set(unitChip.unit, qosPriWtTbl->port, qosPriWtTbl->idx);
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
 * @brief Get which priority arbitration table index is used for a specific ingress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosPriWtTbl QoS priority weight table (N/A) \n
 *                The struct includes the port and index (idx). \n
 *                The port is the ingress port number. \n
 *                The index is the weight table index.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_pri_wt_tbl_get(), dva_qos_pri_wt_tbl_get(), cvb_qos_pri_wt_tbl_get(), cva_qos_pri_wt_tbl_get().
 */
RtkApiRet rtk_qos_pri_wt_tbl_get(UnitChip_t unitChip, QosPriWtTbl_t *qosPriWtTbl)
{
    RtkApiRet ret;

    if(NULL == qosPriWtTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_pri_wt_tbl_get(unitChip.unit, qosPriWtTbl->port, &qosPriWtTbl->idx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_pri_wt_tbl_get(unitChip.unit, qosPriWtTbl->port, &qosPriWtTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_pri_wt_tbl_get(unitChip.unit, qosPriWtTbl->port, &qosPriWtTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_pri_wt_tbl_get(unitChip.unit, qosPriWtTbl->port, &qosPriWtTbl->idx);
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
 * @brief Set the inner tag remapping table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosCvlanPriRemap QoS CVALN priority remapping (N/A) \n
 *            The struct includes the idx, iPri, iDei and mappingPri. \n
 *            The idx is the cvlan priority remapping table index. \n
 *            The iPri is the inner priority. \n
 *            The iDei is the inner dei. \n
 *            The mappingPri is the mapped priority.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_pri_remap_set(), dva_qos_cvlan_pri_remap_set(), cvb_qos_cvlan_pri_remap_set(), cva_qos_cvlan_pri_remap_set().
 */
RtkApiRet rtk_qos_cvlan_pri_remap_set(UnitChip_t unitChip, const QosCvlanPriRemap_t *qosCvlanPriRemap)
{
    RtkApiRet ret;

    if(NULL == qosCvlanPriRemap)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_pri_remap_set(unitChip.unit, qosCvlanPriRemap->idx, qosCvlanPriRemap->iPri, qosCvlanPriRemap->iDei, qosCvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_pri_remap_set(unitChip.unit, qosCvlanPriRemap->idx, qosCvlanPriRemap->iPri, qosCvlanPriRemap->iDei, qosCvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_pri_remap_set(unitChip.unit, qosCvlanPriRemap->idx, qosCvlanPriRemap->iPri, qosCvlanPriRemap->iDei, qosCvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_pri_remap_set(unitChip.unit, qosCvlanPriRemap->idx, qosCvlanPriRemap->iPri, qosCvlanPriRemap->iDei, qosCvlanPriRemap->mappingPri);
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
 * @brief Get the inner tag remapping table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosCvlanPriRemap QoS CVALN priority remapping (N/A) \n
 *                The struct includes the idx, iPri, iDei and mappingPri. \n
 *                The idx is the CVALN priority remapping table index. \n
 *                The iPri is the inner priority. \n
 *                The iDei is the inner dei. \n
 *                The mappingPri is the mapped priority.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_pri_remap_get(), dva_qos_cvlan_pri_remap_get(), cvb_qos_cvlan_pri_remap_get(), cva_qos_cvlan_pri_remap_get().
 */
RtkApiRet rtk_qos_cvlan_pri_remap_get(UnitChip_t unitChip, QosCvlanPriRemap_t *qosCvlanPriRemap)
{
    RtkApiRet ret;

    if(NULL == qosCvlanPriRemap)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_pri_remap_get(unitChip.unit, qosCvlanPriRemap->idx, qosCvlanPriRemap->iPri, qosCvlanPriRemap->iDei, &qosCvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_pri_remap_get(unitChip.unit, qosCvlanPriRemap->idx, qosCvlanPriRemap->iPri, qosCvlanPriRemap->iDei, &qosCvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_pri_remap_get(unitChip.unit, qosCvlanPriRemap->idx, qosCvlanPriRemap->iPri, qosCvlanPriRemap->iDei, &qosCvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_pri_remap_get(unitChip.unit, qosCvlanPriRemap->idx, qosCvlanPriRemap->iPri, qosCvlanPriRemap->iDei, &qosCvlanPriRemap->mappingPri);
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
 * @brief Set the outer tag remapping table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosSvlanPriRemap QoS svlan priority remapping (N/A) \n
 *            The struct includes the idx, oPri, oDei and mappingPri. \n
 *            The idx is the svlan priority remapping table index. \n
 *            The oPri is the outer priority. \n
 *            The oDei is the outer dei. \n
 *            The mappingPri is the mapped priority.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_pri_remap_set(), dva_qos_svlan_pri_remap_set(), cvb_qos_svlan_pri_remap_set(), cva_qos_svlan_pri_remap_set().
 */
RtkApiRet rtk_qos_svlan_pri_remap_set(UnitChip_t unitChip, const QosSvlanPriRemap_t *qosSvlanPriRemap)
{
    RtkApiRet ret;

    if(NULL == qosSvlanPriRemap)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_pri_remap_set(unitChip.unit, qosSvlanPriRemap->idx, qosSvlanPriRemap->oPri, qosSvlanPriRemap->oDei, qosSvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_pri_remap_set(unitChip.unit, qosSvlanPriRemap->idx, qosSvlanPriRemap->oPri, qosSvlanPriRemap->oDei, qosSvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_pri_remap_set(unitChip.unit, qosSvlanPriRemap->idx, qosSvlanPriRemap->oPri, qosSvlanPriRemap->oDei, qosSvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_pri_remap_set(unitChip.unit, qosSvlanPriRemap->idx, qosSvlanPriRemap->oPri, qosSvlanPriRemap->oDei, qosSvlanPriRemap->mappingPri);
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
 * @brief Get the outer tag remapping table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosSvlanPriRemap QoS svlan priority remapping (N/A) \n
 *                The struct includes the idx, oPri, oDei and mappingPri. \n
 *                The idx is the svlan priority remapping table index. \n
 *                The oPri is the outer priority. \n
 *                The oDei is the outer dei. \n
 *                The mappingPri is the mapped priority.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_pri_remap_get(), dva_qos_svlan_pri_remap_get(), cvb_qos_svlan_pri_remap_get(), cva_qos_svlan_pri_remap_get().
 */
RtkApiRet rtk_qos_svlan_pri_remap_get(UnitChip_t unitChip, QosSvlanPriRemap_t *qosSvlanPriRemap)
{
    RtkApiRet ret;

    if(NULL == qosSvlanPriRemap)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_pri_remap_get(unitChip.unit, qosSvlanPriRemap->idx, qosSvlanPriRemap->oPri, qosSvlanPriRemap->oDei, &qosSvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_pri_remap_get(unitChip.unit, qosSvlanPriRemap->idx, qosSvlanPriRemap->oPri, qosSvlanPriRemap->oDei, &qosSvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_pri_remap_get(unitChip.unit, qosSvlanPriRemap->idx, qosSvlanPriRemap->oPri, qosSvlanPriRemap->oDei, &qosSvlanPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_pri_remap_get(unitChip.unit, qosSvlanPriRemap->idx, qosSvlanPriRemap->oPri, qosSvlanPriRemap->oDei, &qosSvlanPriRemap->mappingPri);
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
 * @brief Set the DSCP remapping table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosDscpPriRemap QoS DSCP priority remapping (N/A) \n
 *            The struct includes the idx, dscp and mappingPri. \n
 *            The idx is the svlan priority remapping table index. \n
 *            The dscp is the DSCP value. \n
 *            The mappingPri is the mapped priority.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_pri_remap_set(), dva_qos_dscp_pri_remap_set(), cvb_qos_dscp_pri_remap_set(), cva_qos_dscp_pri_remap_set().
 */
RtkApiRet rtk_qos_dscp_pri_remap_set(UnitChip_t unitChip, const QosDscpPriRemap_t *qosDscpPriRemap)
{
    RtkApiRet ret;

    if(NULL == qosDscpPriRemap)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_pri_remap_set(unitChip.unit, qosDscpPriRemap->idx, qosDscpPriRemap->dscp, qosDscpPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_pri_remap_set(unitChip.unit, qosDscpPriRemap->idx, qosDscpPriRemap->dscp, qosDscpPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_pri_remap_set(unitChip.unit, qosDscpPriRemap->idx, qosDscpPriRemap->dscp, qosDscpPriRemap->mappingPri);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_pri_remap_set(unitChip.unit, qosDscpPriRemap->idx, qosDscpPriRemap->dscp, qosDscpPriRemap->mappingPri);
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
 * @brief Get the DSCP remapping table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosDscpPriRemap QoS DSCP priority remapping (N/A) \n
 *                The struct includes the idx, dscp and mappingPri. \n
 *                The idx is the svlan priority remapping table index. \n
 *                The dscp is the DSCP value. \n
 *                The mappingPri is the mapped priority.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_pri_remap_get(), dva_qos_dscp_pri_remap_get(), cvb_qos_dscp_pri_remap_get(), cva_qos_dscp_pri_remap_get().
 */
RtkApiRet rtk_qos_dscp_pri_remap_get(UnitChip_t unitChip, QosDscpPriRemap_t *qosDscpPriRemap)
{
    RtkApiRet ret;

    if(NULL == qosDscpPriRemap)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_pri_remap_get(unitChip.unit, qosDscpPriRemap->idx, qosDscpPriRemap->dscp, &(qosDscpPriRemap->mappingPri));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_pri_remap_get(unitChip.unit, qosDscpPriRemap->idx, qosDscpPriRemap->dscp, &(qosDscpPriRemap->mappingPri));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_pri_remap_get(unitChip.unit, qosDscpPriRemap->idx, qosDscpPriRemap->dscp, &(qosDscpPriRemap->mappingPri));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_pri_remap_get(unitChip.unit, qosDscpPriRemap->idx, qosDscpPriRemap->dscp, &(qosDscpPriRemap->mappingPri));
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
 * @brief Set a specific ingress port to use the inner tag remapping table index. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosCvlanRemapTbl QoS cvlan remapping table (N/A) \n
 *            The struct includes the port and index (idx). \n
 *            The port is the ingress port number. \n
 *            The index is the cvlan remapping table index.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_remap_tbl_set(), dva_qos_cvlan_remap_tbl_set(), cvb_qos_cvlan_remap_tbl_set(), cva_qos_cvlan_remap_tbl_set().
 */
RtkApiRet rtk_qos_cvlan_remap_tbl_set(UnitChip_t unitChip, const QosCvlanRemapTbl_t *qosCvlanRemapTbl)
{
    RtkApiRet ret;

    if(NULL == qosCvlanRemapTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_remap_tbl_set(unitChip.unit, qosCvlanRemapTbl->port, qosCvlanRemapTbl->idx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_remap_tbl_set(unitChip.unit, qosCvlanRemapTbl->port, qosCvlanRemapTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_remap_tbl_set(unitChip.unit, qosCvlanRemapTbl->port, qosCvlanRemapTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_remap_tbl_set(unitChip.unit, qosCvlanRemapTbl->port, qosCvlanRemapTbl->idx);
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
 * @brief Get which inner tag remapping table index is used for a specific ingress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosCvlanRemapTbl QoS cvlan remapping table (N/A) \n
 *                The struct includes the port and index (idx). \n
 *                The port is the ingress port number. \n
 *                The index is the cvlan remapping table index.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_remap_tbl_get(), dva_qos_cvlan_remap_tbl_get(), cvb_qos_cvlan_remap_tbl_get(), cva_qos_cvlan_remap_tbl_get().
 */
RtkApiRet rtk_qos_cvlan_remap_tbl_get(UnitChip_t unitChip, QosCvlanRemapTbl_t *qosCvlanRemapTbl)
{
    RtkApiRet ret;

    if(NULL == qosCvlanRemapTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_remap_tbl_get(unitChip.unit, qosCvlanRemapTbl->port, &(qosCvlanRemapTbl->idx));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_remap_tbl_get(unitChip.unit, qosCvlanRemapTbl->port, &(qosCvlanRemapTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_remap_tbl_get(unitChip.unit, qosCvlanRemapTbl->port, &(qosCvlanRemapTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_remap_tbl_get(unitChip.unit, qosCvlanRemapTbl->port, &(qosCvlanRemapTbl->idx));
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
 * @brief Set a specific ingress port to use the outer tag remapping table index. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosSvlanRemapTbl QoS SVLAN remapping table (N/A) \n
 *            The struct includes the port and index (idx). \n
 *            The port is the ingress port number. \n
 *            The index is the SVLAN remapping table index.
 * @param[out] None
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_remap_tbl_set(), dva_qos_svlan_remap_tbl_set(), cvb_qos_svlan_remap_tbl_set(), cva_qos_svlan_remap_tbl_set().
 */
RtkApiRet rtk_qos_svlan_remap_tbl_set(UnitChip_t unitChip, const QosSvlanRemapTbl_t *qosSvlanRemapTbl)
{
    RtkApiRet ret;

    if(NULL == qosSvlanRemapTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_remap_tbl_set(unitChip.unit, qosSvlanRemapTbl->port, qosSvlanRemapTbl->idx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_remap_tbl_set(unitChip.unit, qosSvlanRemapTbl->port, qosSvlanRemapTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_remap_tbl_set(unitChip.unit, qosSvlanRemapTbl->port, qosSvlanRemapTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_remap_tbl_set(unitChip.unit, qosSvlanRemapTbl->port, qosSvlanRemapTbl->idx);
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
 * @brief Get which outer tag remapping table index is used for a specific ingress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosSvlanRemapTbl QoS SVLAN remapping table (N/A) \n
 *                The struct includes the port and index (idx). \n
 *                The port is the ingress port number. \n
 *                The index is the SVLAN remapping table index.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_remap_tbl_get(), dva_qos_svlan_remap_tbl_get(), cvb_qos_svlan_remap_tbl_get(), cva_qos_svlan_remap_tbl_get().
 */
RtkApiRet rtk_qos_svlan_remap_tbl_get(UnitChip_t unitChip, QosSvlanRemapTbl_t *qosSvlanRemapTbl)
{
    RtkApiRet ret;

    if(NULL == qosSvlanRemapTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_remap_tbl_get(unitChip.unit, qosSvlanRemapTbl->port, &(qosSvlanRemapTbl->idx));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_remap_tbl_get(unitChip.unit, qosSvlanRemapTbl->port, &(qosSvlanRemapTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_remap_tbl_get(unitChip.unit, qosSvlanRemapTbl->port, &(qosSvlanRemapTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_remap_tbl_get(unitChip.unit, qosSvlanRemapTbl->port, &(qosSvlanRemapTbl->idx));
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
 * @brief Set a specific ingress port to use DSCP remapping table index. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosDscpRemapTbl QoS DSCP remapping table (N/A) \n
 *            The struct includes the port and index (idx). \n
 *            The port is the ingress port number. \n
 *            The index is the DSCP remapping table index.
 * @param[out] None
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_remap_tbl_set(), dva_qos_dscp_remap_tbl_set(), cvb_qos_dscp_remap_tbl_set(), cva_qos_dscp_remap_tbl_set().
 */
RtkApiRet rtk_qos_dscp_remap_tbl_set(UnitChip_t unitChip, const QosDscpRemapTbl_t *qosDscpRemapTbl)
{
    RtkApiRet ret;

    if(NULL == qosDscpRemapTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_remap_tbl_set(unitChip.unit, qosDscpRemapTbl->port, qosDscpRemapTbl->idx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_remap_tbl_set(unitChip.unit, qosDscpRemapTbl->port, qosDscpRemapTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_remap_tbl_set(unitChip.unit, qosDscpRemapTbl->port, qosDscpRemapTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_remap_tbl_set(unitChip.unit, qosDscpRemapTbl->port, qosDscpRemapTbl->idx);
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
 * @brief Get which DSCP remapping table index is used for a specific ingress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosDscpRemapTbl QoS DSCP remapping table (N/A) \n
 *                The struct includes the port and index (idx). \n
 *                The port is the ingress port number. \n
 *                The index is the DSCP remapping table index.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_remap_tbl_get(), dva_qos_dscp_remap_tbl_get(), cvb_qos_dscp_remap_tbl_get(), cva_qos_dscp_remap_tbl_get().
 */
RtkApiRet rtk_qos_dscp_remap_tbl_get(UnitChip_t unitChip, QosDscpRemapTbl_t *qosDscpRemapTbl)
{
    RtkApiRet ret;

    if(NULL == qosDscpRemapTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_remap_tbl_get(unitChip.unit, qosDscpRemapTbl->port, &(qosDscpRemapTbl->idx));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_remap_tbl_get(unitChip.unit, qosDscpRemapTbl->port, &(qosDscpRemapTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_remap_tbl_get(unitChip.unit, qosDscpRemapTbl->port, &(qosDscpRemapTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_remap_tbl_get(unitChip.unit, qosDscpRemapTbl->port, &(qosDscpRemapTbl->idx));
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
 * @brief Set the internal-priority-to-traffic-class mapping table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosPriQueMapping QoS piroirty queue mapping (N/A) \n
 *            The struct includes the port, priority (pri) and queue (que). \n
 *            The port is the ingress port number. \n
 *            The priority is the internal priority value. \n
 *            The queue is the egress queue number.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_pri_que_mapping_set(), dva_qos_pri_que_mapping_set(), cvb_qos_pri_que_mapping_set(), cva_qos_pri_que_mapping_set().
 */
RtkApiRet rtk_qos_pri_que_mapping_set(UnitChip_t unitChip, const QosPriQueMapping_t *qosPriQueMapping)
{
    RtkApiRet ret;

    if(NULL == qosPriQueMapping)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_pri_que_mapping_set(unitChip.unit, qosPriQueMapping->port, qosPriQueMapping->pri, qosPriQueMapping->que);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_pri_que_mapping_set(unitChip.unit, qosPriQueMapping->port, qosPriQueMapping->pri, qosPriQueMapping->que);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_pri_que_mapping_set(unitChip.unit, qosPriQueMapping->port, qosPriQueMapping->pri, qosPriQueMapping->que);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_pri_que_mapping_set(unitChip.unit, qosPriQueMapping->port, qosPriQueMapping->pri, qosPriQueMapping->que);
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
 * @brief Get the internal-priority-to-traffic-class mapping table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosPriQueMapping QoS piroirty queue mapping (N/A) \n
 *                The struct includes the port, priority (pri) and queue (que). \n
 *                The port is the ingress port number. \n
 *                The priority is the internal priority value. \n
 *                The queue is the egress queue number.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_pri_que_mapping_get(), dva_qos_pri_que_mapping_get(), cvb_qos_pri_que_mapping_get(), cva_qos_pri_que_mapping_get().
 */
RtkApiRet rtk_qos_pri_que_mapping_get(UnitChip_t unitChip, QosPriQueMapping_t *qosPriQueMapping)
{
    RtkApiRet ret;

    if(NULL == qosPriQueMapping)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_pri_que_mapping_get(unitChip.unit, qosPriQueMapping->port, qosPriQueMapping->pri, &(qosPriQueMapping->que));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_pri_que_mapping_get(unitChip.unit, qosPriQueMapping->port, qosPriQueMapping->pri, &(qosPriQueMapping->que));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_pri_que_mapping_get(unitChip.unit, qosPriQueMapping->port, qosPriQueMapping->pri, &(qosPriQueMapping->que));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_pri_que_mapping_get(unitChip.unit, qosPriQueMapping->port, qosPriQueMapping->pri, &(qosPriQueMapping->que));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* QOS_TC */

/**
 * @defgroup QOS_TS Traffic Scheduling
 * @brief Traffic Scheduling includes the functionality to set and get queue scheduler and queue wfq/wrr weights.
 * @{
 */

/**
 * @brief Set a traffic scheduler for a specific queue of a specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosQueSchInfo QoS queue scheduler information (N/A) \n
 *            The struct includes the port, queue (que) and scheduler (queSch). \n
 *            The port is the egress port number. \n
 *            The queue is the QoS queue number. \n
 *            The scheduler is the QoS queue scheduler.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_que_sch_set(), dva_qos_que_sch_set(), cvb_qos_que_sch_set(), cva_qos_que_sch_set().
 */
RtkApiRet rtk_qos_que_sch_set(UnitChip_t unitChip, const QosQueSchInfo_t *qosQueSchInfo)
{
    RtkApiRet ret;

    if(NULL == qosQueSchInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_que_sch_set(unitChip.unit, qosQueSchInfo->port, qosQueSchInfo->que, qosQueSchInfo->queSch);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_que_sch_set(unitChip.unit, qosQueSchInfo->port, qosQueSchInfo->que, qosQueSchInfo->queSch);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_que_sch_set(unitChip.unit, qosQueSchInfo->port, qosQueSchInfo->que, qosQueSchInfo->queSch);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_que_sch_set(unitChip.unit, qosQueSchInfo->port, qosQueSchInfo->que, qosQueSchInfo->queSch);
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
 * @brief Get the traffic scheduler for a specific queue of a specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosQueSchInfo QoS queue scheduler information (N/A) \n
 *                The struct includes the port, queue (que) and scheduler (queSch). \n
 *                The port is the egress port number. \n
 *                The queue is the QoS queue number. \n
 *                The scheduler is the QoS queue scheduler.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_que_sch_get(), dva_qos_que_sch_get(), cvb_qos_que_sch_get(), cva_qos_que_sch_get().
 */
RtkApiRet rtk_qos_que_sch_get(UnitChip_t unitChip, QosQueSchInfo_t *qosQueSchInfo)
{
    RtkApiRet ret;

    if(NULL == qosQueSchInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_que_sch_get(unitChip.unit, qosQueSchInfo->port, qosQueSchInfo->que, &(qosQueSchInfo->queSch));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_que_sch_get(unitChip.unit, qosQueSchInfo->port, qosQueSchInfo->que, &(qosQueSchInfo->queSch));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_que_sch_get(unitChip.unit, qosQueSchInfo->port, qosQueSchInfo->que, &(qosQueSchInfo->queSch));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_que_sch_get(unitChip.unit, qosQueSchInfo->port, qosQueSchInfo->que, &(qosQueSchInfo->queSch));
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
<<<<<<< HEAD
 * @brief Set a weight value of WRR/WFQ scheduler for a specific queue of a specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosQueWfqWrrWt QoS queue wfq wrr weight (N/A) \n
 *            The struct includes the port, queue (que) and weight (queWt). \n
 *            The port is the egress port number. \n
 *            The queue is the QoS queue number. \n
 *            The weight (queWt) determines the scheduling priority for the queue \n
 *            Higher weights give the queue more bandwidth relative to others. \n
 *            This value is used in both Weighted Fair Queueing (WFQ) and Weighted Round Robin (WRR) scheduling algorithms to allocate resources efficiently.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_que_wfq_wrr_wt_set(), dva_qos_que_wfq_wrr_wt_set(), cvb_qos_que_wfq_wrr_wt_set(), cva_qos_que_wfq_wrr_wt_set().
 */
RtkApiRet rtk_qos_que_wfq_wrr_wt_set(UnitChip_t unitChip, const QosQueWfqWrrWt_t *qosQueWfqWrrWt)
{
    RtkApiRet ret;

    if(NULL == qosQueWfqWrrWt)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_que_wfq_wrr_wt_set(unitChip.unit, qosQueWfqWrrWt->port, qosQueWfqWrrWt->que, qosQueWfqWrrWt->queWt);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_que_wfq_wrr_wt_set(unitChip.unit, qosQueWfqWrrWt->port, qosQueWfqWrrWt->que, qosQueWfqWrrWt->queWt);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_que_wfq_wrr_wt_set(unitChip.unit, qosQueWfqWrrWt->port, qosQueWfqWrrWt->que, qosQueWfqWrrWt->queWt);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_que_wfq_wrr_wt_set(unitChip.unit, qosQueWfqWrrWt->port, qosQueWfqWrrWt->que, qosQueWfqWrrWt->queWt);
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
 * @brief Get the weight value of WRR/WFQ scheduler for a specific queue of a specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosQueWfqWrrWt QoS queue wfq wrr weight (N/A) \n
 *                The struct includes the port, queue (que) and weight (queWt). \n
 *                The port is the egress port number. \n
 *                The queue is the QoS queue number. \n
 *                The weight (queWt) determines the scheduling priority for the queue \n
 *                Higher weights give the queue more bandwidth relative to others. \n
 *                This value is used in both Weighted Fair Queueing (WFQ) and Weighted Round Robin (WRR) scheduling algorithms to allocate resources efficiently.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_que_wfq_wrr_wt_get(), dva_qos_que_wfq_wrr_wt_get(), cvb_qos_que_wfq_wrr_wt_get(), cva_qos_que_wfq_wrr_wt_get().
 */
RtkApiRet rtk_qos_que_wfq_wrr_wt_get(UnitChip_t unitChip, QosQueWfqWrrWt_t *qosQueWfqWrrWt)
{
    RtkApiRet ret;

    if(NULL == qosQueWfqWrrWt)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_que_wfq_wrr_wt_get(unitChip.unit, qosQueWfqWrrWt->port, qosQueWfqWrrWt->que, &(qosQueWfqWrrWt->queWt));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_que_wfq_wrr_wt_get(unitChip.unit, qosQueWfqWrrWt->port, qosQueWfqWrrWt->que, &(qosQueWfqWrrWt->queWt));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_que_wfq_wrr_wt_get(unitChip.unit, qosQueWfqWrrWt->port, qosQueWfqWrrWt->que, &(qosQueWfqWrrWt->queWt));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_que_wfq_wrr_wt_get(unitChip.unit, qosQueWfqWrrWt->port, qosQueWfqWrrWt->que, &(qosQueWfqWrrWt->queWt));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* QOS_TS */

/**
 * @defgroup QOS_EGR_BW Egress Bandwidth Control
 * @brief Egress Bandwidth Control includes the functionality to set and get egress bandwidth.
 * @{
 */

/**
 * @brief Set a port bandwidth for a specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosEgrBwCtrl QoS egress bandwdith control (N/A) \n
 *            The struct includes the port, en and bw. \n
 *            The port is the egress port number. \n
 *            The en indicates if egress port bandwidth control is enabled. \n
 *            The bw is the maximum allowed bandwidth. \n
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_egr_bw_ctrl_set(), dva_qos_egr_bw_ctrl_set().
 */
RtkApiRet rtk_qos_egr_bw_ctrl_set(UnitChip_t unitChip, const QosEgrBwCtrl_t *qosEgrBwCtrl)
{
    RtkApiRet ret;

    if(NULL == qosEgrBwCtrl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_egr_bw_ctrl_set(unitChip.unit, qosEgrBwCtrl->port, qosEgrBwCtrl->en, qosEgrBwCtrl->bw);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_egr_bw_ctrl_set(unitChip.unit, qosEgrBwCtrl->port, qosEgrBwCtrl->en, qosEgrBwCtrl->bw);
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
 * @brief Get the port bandwidth for a specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosEgrBwCtrl QoS egress bandwdith control (N/A) \n
 *                The struct includes the port, en and bw. \n
 *                The port is the egress port number. \n
 *                The en indicates if egress port bandwidth control is enabled. \n
 *                The bw is the maximum allowed bandwidth. \n
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_egr_bw_ctrl_get(), dva_qos_egr_bw_ctrl_get().
 */
RtkApiRet rtk_qos_egr_bw_ctrl_get(UnitChip_t unitChip, QosEgrBwCtrl_t *qosEgrBwCtrl)
{
    RtkApiRet ret;

    if(NULL == qosEgrBwCtrl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_egr_bw_ctrl_get(unitChip.unit, qosEgrBwCtrl->port, &(qosEgrBwCtrl->en), &(qosEgrBwCtrl->bw));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_egr_bw_ctrl_get(unitChip.unit, qosEgrBwCtrl->port, &(qosEgrBwCtrl->en), &(qosEgrBwCtrl->bw));
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
 * @brief Set a queue bandwidth for a specific queue of a specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosEgrQueBwCtrl QoS egress queue bandwdith control (N/A) \n
 *            The struct includes the port, que, en and bw. \n
 *            The port is the egress port number. \n
 *            The que is QoS queue number. \n
 *            The en indicates if egress queue bandwidth control is enabled. \n
 *            The bw is the maximum allowed bandwidth.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_egr_que_bw_ctrl_set(), dva_qos_egr_que_bw_ctrl_set(), cvb_qos_egr_que_bw_ctrl_set(), cva_qos_egr_que_bw_ctrl_set().
 */
RtkApiRet rtk_qos_egr_que_bw_ctrl_set(UnitChip_t unitChip, const QosEgrQueBwCtrl_t *qosEgrQueBwCtrl)
{
    RtkApiRet ret;

    if(NULL == qosEgrQueBwCtrl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_egr_que_bw_ctrl_set(unitChip.unit, qosEgrQueBwCtrl->port, qosEgrQueBwCtrl->que, qosEgrQueBwCtrl->en, qosEgrQueBwCtrl->bw);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_egr_que_bw_ctrl_set(unitChip.unit, qosEgrQueBwCtrl->port, qosEgrQueBwCtrl->que, qosEgrQueBwCtrl->en, qosEgrQueBwCtrl->bw);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_egr_que_bw_ctrl_set(unitChip.unit, qosEgrQueBwCtrl->port, qosEgrQueBwCtrl->que, qosEgrQueBwCtrl->en, qosEgrQueBwCtrl->bw);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_egr_que_bw_ctrl_set(unitChip.unit, qosEgrQueBwCtrl->port, qosEgrQueBwCtrl->que, qosEgrQueBwCtrl->en, qosEgrQueBwCtrl->bw);
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
 * @brief Get the queue bandwidth for a specific queue of a specific port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosEgrQueBwCtrl QoS egress queue bandwdith control (N/A) \n
 *                The struct includes the port, que, en and bw. \n
 *                The port is the egress port number. \n
 *                The que is QoS queue number. \n
 *                The en indicates if egress queue bandwidth control is enabled. \n
 *                The bw is the maximum allowed bandwidth.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_egr_que_bw_ctrl_get(), dva_qos_egr_que_bw_ctrl_get(), cvb_qos_egr_que_bw_ctrl_get(), cva_qos_egr_que_bw_ctrl_get().
 */
RtkApiRet rtk_qos_egr_que_bw_ctrl_get(UnitChip_t unitChip, QosEgrQueBwCtrl_t *qosEgrQueBwCtrl)
{
    RtkApiRet ret;

    if(NULL == qosEgrQueBwCtrl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_egr_que_bw_ctrl_get(unitChip.unit, qosEgrQueBwCtrl->port, qosEgrQueBwCtrl->que, &(qosEgrQueBwCtrl->en), &(qosEgrQueBwCtrl->bw));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_egr_que_bw_ctrl_get(unitChip.unit, qosEgrQueBwCtrl->port, qosEgrQueBwCtrl->que, &(qosEgrQueBwCtrl->en), &(qosEgrQueBwCtrl->bw));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_egr_que_bw_ctrl_get(unitChip.unit, qosEgrQueBwCtrl->port, qosEgrQueBwCtrl->que, &(qosEgrQueBwCtrl->en), &(qosEgrQueBwCtrl->bw));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_egr_que_bw_ctrl_get(unitChip.unit, qosEgrQueBwCtrl->port, qosEgrQueBwCtrl->que, &(qosEgrQueBwCtrl->en), &(qosEgrQueBwCtrl->bw));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* QOS_EGR_BW */

/**
 * @defgroup QOS_TPR Traffic Priority Remarking
 * @brief Traffic Priority Remarking includes the functionality to set and get CVLAN priority remarking enable,
 *        SVLAN priority remarking enable, DSCP priority remarking enable, CVLAN priority remarking, SVLAN priority remarking, DSCP priority remarking,
 *        CVLAN priority remarking table, SVLAN priority remarking table and DSCP priority remarking table.
 * @{
 */

/**
 * @brief Set the inner tag priority remarking status. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosCvlanRmkEn QoS CVLAN remarking enable (N/A) \n
 *            The struct includes the port and en. \n
 *            The port is the egress port number. \n
 *            The en indicates if CVLAN remarking is enabled.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_rmk_en_set(), dva_qos_cvlan_rmk_en_set(), cvb_qos_cvlan_rmk_en_set(), cva_qos_cvlan_rmk_en_set().
 */
RtkApiRet rtk_qos_cvlan_rmk_en_set(UnitChip_t unitChip, const QosCvlanRmkEn_t *qosCvlanRmkEn)
{
    RtkApiRet ret;

    if(NULL == qosCvlanRmkEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_rmk_en_set(unitChip.unit, qosCvlanRmkEn->port, qosCvlanRmkEn->en);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_rmk_en_set(unitChip.unit, qosCvlanRmkEn->port, qosCvlanRmkEn->en);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_rmk_en_set(unitChip.unit, qosCvlanRmkEn->port, qosCvlanRmkEn->en);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_rmk_en_set(unitChip.unit, qosCvlanRmkEn->port, qosCvlanRmkEn->en);
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
 * @brief Get the inner tag priority remarking status. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosCvlanRmkEn QoS CVALN remarking enable (N/A) \n
 *                The struct includes the port and en. \n
 *                The port is the egress port number. \n
 *                The en indicates if CVLAN remarking is enabled.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_rmk_en_get(), dva_qos_cvlan_rmk_en_get(), cvb_qos_cvlan_rmk_en_get()), cva_qos_cvlan_rmk_en_get().
 */
RtkApiRet rtk_qos_cvlan_rmk_en_get(UnitChip_t unitChip, QosCvlanRmkEn_t *qosCvlanRmkEn)
{
    RtkApiRet ret;

    if(NULL == qosCvlanRmkEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_rmk_en_get(unitChip.unit, qosCvlanRmkEn->port, &(qosCvlanRmkEn->en));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_rmk_en_get(unitChip.unit, qosCvlanRmkEn->port, &(qosCvlanRmkEn->en));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_rmk_en_get(unitChip.unit, qosCvlanRmkEn->port, &(qosCvlanRmkEn->en));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_rmk_en_get(unitChip.unit, qosCvlanRmkEn->port, &(qosCvlanRmkEn->en));
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
 * @brief Set the outer tag priority remarking status. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosSvlanRmkEn QoS SVLAN remarking enable (N/A) \n
 *            The struct includes the port and en. \n
 *            The port is the egress port number. \n
 *            The en indicates if SVLAN remarking is enabled.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_rmk_en_set(), dva_qos_svlan_rmk_en_set(), cvb_qos_svlan_rmk_en_set(), cva_qos_svlan_rmk_en_set().
 */
RtkApiRet rtk_qos_svlan_rmk_en_set(UnitChip_t unitChip, const QosSvlanRmkEn_t *qosSvlanRmkEn)
{
    RtkApiRet ret;

    if(NULL == qosSvlanRmkEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_rmk_en_set(unitChip.unit, qosSvlanRmkEn->port, qosSvlanRmkEn->en);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_rmk_en_set(unitChip.unit, qosSvlanRmkEn->port, qosSvlanRmkEn->en);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_rmk_en_set(unitChip.unit, qosSvlanRmkEn->port, qosSvlanRmkEn->en);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_rmk_en_set(unitChip.unit, qosSvlanRmkEn->port, qosSvlanRmkEn->en);
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
 * @brief Get the outer tag priority remarking status. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosSvlanRmkEn QoS SVLAN remarking enable (N/A) \n
 *                The struct includes the port and en. \n
 *                The port is the egress port number. \n
 *                The en indicates if SVLAN remarking is enabled.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_rmk_en_get(), dva_qos_svlan_rmk_en_get(), cvb_qos_svlan_rmk_en_get(), cva_qos_svlan_rmk_en_get().
 */
RtkApiRet rtk_qos_svlan_rmk_en_get(UnitChip_t unitChip, QosSvlanRmkEn_t *qosSvlanRmkEn)
{
    RtkApiRet ret;

    if(NULL == qosSvlanRmkEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_rmk_en_get(unitChip.unit, qosSvlanRmkEn->port, &(qosSvlanRmkEn->en));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_rmk_en_get(unitChip.unit, qosSvlanRmkEn->port, &(qosSvlanRmkEn->en));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_rmk_en_get(unitChip.unit, qosSvlanRmkEn->port, &(qosSvlanRmkEn->en));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_rmk_en_get(unitChip.unit, qosSvlanRmkEn->port, &(qosSvlanRmkEn->en));
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
 * @brief Set the DSCP priority remarking status. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosDscpRmkEn QoS DSCP remarking enable (N/A) \n
 *            The struct includes the port and en. \n
 *            The port is the egress port number. \n
 *            The en indicates if DSCP remarking is enabled.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_rmk_en_set(), dva_qos_dscp_rmk_en_set(), cvb_qos_dscp_rmk_en_set(), cva_qos_dscp_rmk_en_set().
 */
RtkApiRet rtk_qos_dscp_rmk_en_set(UnitChip_t unitChip, const QosDscpRmkEn_t *qosDscpRmkEn)
{
    RtkApiRet ret;

    if(NULL == qosDscpRmkEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_rmk_en_set(unitChip.unit, qosDscpRmkEn->port, qosDscpRmkEn->en);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_rmk_en_set(unitChip.unit, qosDscpRmkEn->port, qosDscpRmkEn->en);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_rmk_en_set(unitChip.unit, qosDscpRmkEn->port, qosDscpRmkEn->en);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_rmk_en_set(unitChip.unit, qosDscpRmkEn->port, qosDscpRmkEn->en);
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
 * @brief Get the DSCP priority remarking status. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosDscpRmkEn QoS DSCP remarking enable (N/A) \n
 *                The struct includes the port and en. \n
 *                The port is the egress port number. \n
 *                The en indicates if DSCP remarking is enabled.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_rmk_en_get(), dva_qos_dscp_rmk_en_get(), cvb_qos_dscp_rmk_en_get(), cva_qos_dscp_rmk_en_get().
 */
RtkApiRet rtk_qos_dscp_rmk_en_get(UnitChip_t unitChip, QosDscpRmkEn_t *qosDscpRmkEn)
{
    RtkApiRet ret;

    if(NULL == qosDscpRmkEn)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_rmk_en_get(unitChip.unit, qosDscpRmkEn->port, &(qosDscpRmkEn->en));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_rmk_en_get(unitChip.unit, qosDscpRmkEn->port, &(qosDscpRmkEn->en));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_rmk_en_get(unitChip.unit, qosDscpRmkEn->port, &(qosDscpRmkEn->en));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_rmk_en_get(unitChip.unit, qosDscpRmkEn->port, &(qosDscpRmkEn->en));
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
 * @brief Set egress port ctag priority and dei remark table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosCvlanRmk QoS CVLAN remarking (N/A) \n
 *            This struct includes idx, oriPri, rmkPri, and rmkDei. \n
 *            The idx is the remarking table index. \n
 *            The oriPri is the origin priority. \n
 *            The rmkPri is the remarked priority. \n
 *            The rmkDei is the remarked DEI value.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_rmk_set(), dva_qos_cvlan_rmk_set(), cvb_qos_cvlan_rmk_set(), cva_qos_cvlan_rmk_set().
 */
RtkApiRet rtk_qos_cvlan_rmk_set(UnitChip_t unitChip, const QosCvlanRmk_t *qosCvlanRmk)
{
    RtkApiRet ret;

    if(NULL == qosCvlanRmk)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_rmk_set(unitChip.unit, qosCvlanRmk->idx, qosCvlanRmk->oriPri, qosCvlanRmk->rmkPri, qosCvlanRmk->rmkDei);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_rmk_set(unitChip.unit, qosCvlanRmk->idx, qosCvlanRmk->oriPri, qosCvlanRmk->rmkPri, qosCvlanRmk->rmkDei);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_rmk_set(unitChip.unit, qosCvlanRmk->idx, qosCvlanRmk->oriPri, qosCvlanRmk->rmkPri, qosCvlanRmk->rmkDei);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_rmk_set(unitChip.unit, qosCvlanRmk->idx, qosCvlanRmk->oriPri, qosCvlanRmk->rmkPri, qosCvlanRmk->rmkDei);
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
 * @brief Get egress port ctag priority & dei remark table configuration. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosCvlanRmk QoS CVLAN remarking (N/A) \n
 *                This struct includes idx, oriPri, rmkPri, and rmkDei. \n
 *                The idx is the remarking table index. \n
 *                The oriPri is the origin priority. \n
 *                The rmkPri is the remarked priority. \n
 *                The rmkDei is the remarked DEI value.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_rmk_get(), dva_qos_cvlan_rmk_get(), cvb_qos_cvlan_rmk_get(), cva_qos_cvlan_rmk_get().
 */
RtkApiRet rtk_qos_cvlan_rmk_get(UnitChip_t unitChip, QosCvlanRmk_t *qosCvlanRmk)
{
    RtkApiRet ret;

    if(NULL == qosCvlanRmk)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_rmk_get(unitChip.unit, qosCvlanRmk->idx, qosCvlanRmk->oriPri, &(qosCvlanRmk->rmkPri), &(qosCvlanRmk->rmkDei));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_rmk_get(unitChip.unit, qosCvlanRmk->idx, qosCvlanRmk->oriPri, &(qosCvlanRmk->rmkPri), &(qosCvlanRmk->rmkDei));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_rmk_get(unitChip.unit, qosCvlanRmk->idx, qosCvlanRmk->oriPri, &(qosCvlanRmk->rmkPri), &(qosCvlanRmk->rmkDei));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_rmk_get(unitChip.unit, qosCvlanRmk->idx, qosCvlanRmk->oriPri, &(qosCvlanRmk->rmkPri), &(qosCvlanRmk->rmkDei));
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
 * @brief Set egress port stag priority and dei remark table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosSvlanRmk QoS SVLAN remarking (N/A) \n
 *            This struct includes idx, oriPri, rmkPri, and rmkDei. \n
 *            The idx is the remarking table index. \n
 *            The oriPri is the origin priority. \n
 *            The rmkPri is the remarked priority. \n
 *            The rmkDei is the remarked DEI value.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_rmk_set(), dva_qos_svlan_rmk_set(), cvb_qos_svlan_rmk_set(), cva_qos_svlan_rmk_set().
 */
RtkApiRet rtk_qos_svlan_rmk_set(UnitChip_t unitChip, const QosSvlanRmk_t *qosSvlanRmk)
{
    RtkApiRet ret;

    if(NULL == qosSvlanRmk)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_rmk_set(unitChip.unit, qosSvlanRmk->idx, qosSvlanRmk->oriPri, qosSvlanRmk->rmkPri, qosSvlanRmk->rmkDei);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_rmk_set(unitChip.unit, qosSvlanRmk->idx, qosSvlanRmk->oriPri, qosSvlanRmk->rmkPri, qosSvlanRmk->rmkDei);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_rmk_set(unitChip.unit, qosSvlanRmk->idx, qosSvlanRmk->oriPri, qosSvlanRmk->rmkPri, qosSvlanRmk->rmkDei);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_rmk_set(unitChip.unit, qosSvlanRmk->idx, qosSvlanRmk->oriPri, qosSvlanRmk->rmkPri, qosSvlanRmk->rmkDei);
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
 * @brief Get egress port stag priority & dei remark table configuration. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosSvlanRmk QoS SVLAN remarking (N/A) \n
 *                This struct includes idx, oriPri, rmkPri, and rmkDei. \n
 *                The idx is the remarking table index. \n
 *                The oriPri is the origin priority. \n
 *                The rmkPri is the remarked priority. \n
 *                The rmkDei is the remarked DEI value.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_rmk_get(), dva_qos_svlan_rmk_get(), cvb_qos_svlan_rmk_get(), cva_qos_svlan_rmk_get().
 */
RtkApiRet rtk_qos_svlan_rmk_get(UnitChip_t unitChip, QosSvlanRmk_t *qosSvlanRmk)
{
    RtkApiRet ret;

    if(NULL == qosSvlanRmk)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_rmk_get(unitChip.unit, qosSvlanRmk->idx, qosSvlanRmk->oriPri, &(qosSvlanRmk->rmkPri), &(qosSvlanRmk->rmkDei));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_rmk_get(unitChip.unit, qosSvlanRmk->idx, qosSvlanRmk->oriPri, &(qosSvlanRmk->rmkPri), &(qosSvlanRmk->rmkDei));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_rmk_get(unitChip.unit, qosSvlanRmk->idx, qosSvlanRmk->oriPri, &(qosSvlanRmk->rmkPri), &(qosSvlanRmk->rmkDei));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_rmk_get(unitChip.unit, qosSvlanRmk->idx, qosSvlanRmk->oriPri, &(qosSvlanRmk->rmkPri), &(qosSvlanRmk->rmkDei));
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
 * @brief Set egress port DSCP remark table. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosDscpRmk QoS DSCP remarking (N/A) \n
 *            This struct includes idx, oriPri and rmkDscp. \n
 *            The idx is the remarking table index. \n
 *            The oriPri is the origin priority. \n
 *            The rmkDscp is the remarked DSCP value.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_rmk_set(), dva_qos_dscp_rmk_set(), cvb_qos_dscp_rmk_set(), cva_qos_dscp_rmk_set().
 */
RtkApiRet rtk_qos_dscp_rmk_set(UnitChip_t unitChip, const QosDscpRmk_t *qosDscpRmk)
{
    RtkApiRet ret;

    if(NULL == qosDscpRmk)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_rmk_set(unitChip.unit, qosDscpRmk->idx, qosDscpRmk->oriPri, qosDscpRmk->rmkDscp);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_rmk_set(unitChip.unit, qosDscpRmk->idx, qosDscpRmk->oriPri, qosDscpRmk->rmkDscp);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_rmk_set(unitChip.unit, qosDscpRmk->idx, qosDscpRmk->oriPri, qosDscpRmk->rmkDscp);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_rmk_set(unitChip.unit, qosDscpRmk->idx, qosDscpRmk->oriPri, qosDscpRmk->rmkDscp);
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
 * @brief Get egress port DSCP remark table configuration. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosDscpRmk QoS DSCP remarking (N/A) \n
 *                This struct includes idx, oriPri and rmkDscp. \n
 *                The idx is the remarking table index. \n
 *                The oriPri is the origin priority. \n
 *                The rmkDscp is the remarked DSCP value.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_rmk_get(), dva_qos_dscp_rmk_get(), cvb_qos_dscp_rmk_get(), cva_qos_dscp_rmk_get().
 */
RtkApiRet rtk_qos_dscp_rmk_get(UnitChip_t unitChip, QosDscpRmk_t *qosDscpRmk)
{
    RtkApiRet ret;

    if(NULL == qosDscpRmk)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_rmk_get(unitChip.unit, qosDscpRmk->idx, qosDscpRmk->oriPri, &(qosDscpRmk->rmkDscp));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_rmk_get(unitChip.unit, qosDscpRmk->idx, qosDscpRmk->oriPri, &(qosDscpRmk->rmkDscp));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_rmk_get(unitChip.unit, qosDscpRmk->idx, qosDscpRmk->oriPri, &(qosDscpRmk->rmkDscp));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_rmk_get(unitChip.unit, qosDscpRmk->idx, qosDscpRmk->oriPri, &(qosDscpRmk->rmkDscp));
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
 * @brief Set a specific egress port to use inner tag remarking table index. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosCvlanRmkTbl QoS CVLAN remarking table (N/A) \n
 *            The struct includes the port and index (idx). \n
 *            The port is the egress port number. \n
 *            The index is the CVLAN remarking table index.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_rmk_tbl_set(), dva_qos_cvlan_rmk_tbl_set(), cvb_qos_cvlan_rmk_tbl_set(), cva_qos_cvlan_rmk_tbl_set().
 */
RtkApiRet rtk_qos_cvlan_rmk_tbl_set(UnitChip_t unitChip, const QosCvlanRmkTbl_t *qosCvlanRmkTbl)
{
    RtkApiRet ret;

    if(NULL == qosCvlanRmkTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_rmk_tbl_set(unitChip.unit, qosCvlanRmkTbl->port, qosCvlanRmkTbl->idx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_rmk_tbl_set(unitChip.unit, qosCvlanRmkTbl->port, qosCvlanRmkTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_rmk_tbl_set(unitChip.unit, qosCvlanRmkTbl->port, qosCvlanRmkTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_rmk_tbl_set(unitChip.unit, qosCvlanRmkTbl->port, qosCvlanRmkTbl->idx);
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
 * @brief Get egress port CVLAN remark table index configuration. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosCvlanRmkTbl QoS CVLAN remarking table (N/A) \n
 *                The struct includes the port and index (idx). \n
 *                The port is the egress port number. \n
 *                The index is the CVLAN remapping table index.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_cvlan_rmk_tbl_get(), dva_qos_cvlan_rmk_tbl_get(), cvb_qos_cvlan_rmk_tbl_get(), cva_qos_cvlan_rmk_tbl_get().
 */
RtkApiRet rtk_qos_cvlan_rmk_tbl_get(UnitChip_t unitChip, QosCvlanRmkTbl_t *qosCvlanRmkTbl)
{
    RtkApiRet ret;

    if(NULL == qosCvlanRmkTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_cvlan_rmk_tbl_get(unitChip.unit, qosCvlanRmkTbl->port, &(qosCvlanRmkTbl->idx));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_cvlan_rmk_tbl_get(unitChip.unit, qosCvlanRmkTbl->port, &(qosCvlanRmkTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_cvlan_rmk_tbl_get(unitChip.unit, qosCvlanRmkTbl->port, &(qosCvlanRmkTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_cvlan_rmk_tbl_get(unitChip.unit, qosCvlanRmkTbl->port, &(qosCvlanRmkTbl->idx));
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
 * @brief Set a specific egress port to use outer tag remarking table index. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosSvlanRmkTbl QoS SVLAN remarking table (N/A) \n
 *            The struct includes the port and index (idx). \n
 *            The port is the egress port number. \n
 *            The index is the SVLAN remarking table index.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_rmk_tbl_set(), dva_qos_svlan_rmk_tbl_set(), cvb_qos_svlan_rmk_tbl_set(), cva_qos_svlan_rmk_tbl_set().
 */
RtkApiRet rtk_qos_svlan_rmk_tbl_set(UnitChip_t unitChip, const QosSvlanRmkTbl_t *qosSvlanRmkTbl)
{
    RtkApiRet ret;

    if(NULL == qosSvlanRmkTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_rmk_tbl_set(unitChip.unit, qosSvlanRmkTbl->port, qosSvlanRmkTbl->idx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_rmk_tbl_set(unitChip.unit, qosSvlanRmkTbl->port, qosSvlanRmkTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_rmk_tbl_set(unitChip.unit, qosSvlanRmkTbl->port, qosSvlanRmkTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_rmk_tbl_set(unitChip.unit, qosSvlanRmkTbl->port, qosSvlanRmkTbl->idx);
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
 * @brief Get which outer tag remarking table index is used for a specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.\n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosSvlanRmkTbl QoS SVLAN remarking table (N/A) \n
 *                The struct includes the port and index (idx). \n
 *                The port is the egress port number. \n
 *                The index is the SVLAN remapping table index.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_svlan_rmk_tbl_get(), dva_qos_svlan_rmk_tbl_get(), cvb_qos_svlan_rmk_tbl_get(), cva_qos_svlan_rmk_tbl_get().
 */
RtkApiRet rtk_qos_svlan_rmk_tbl_get(UnitChip_t unitChip, QosSvlanRmkTbl_t *qosSvlanRmkTbl)
{
    RtkApiRet ret;

    if(NULL == qosSvlanRmkTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_svlan_rmk_tbl_get(unitChip.unit, qosSvlanRmkTbl->port, &(qosSvlanRmkTbl->idx));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_svlan_rmk_tbl_get(unitChip.unit, qosSvlanRmkTbl->port, &(qosSvlanRmkTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_svlan_rmk_tbl_get(unitChip.unit, qosSvlanRmkTbl->port, &(qosSvlanRmkTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_svlan_rmk_tbl_get(unitChip.unit, qosSvlanRmkTbl->port, &(qosSvlanRmkTbl->idx));
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
 * @brief Set a specific egress port to use DSCP remarking table index. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in] qosDscpRmkTbl QoS DSCP rearking table (N/A) \n
 *            The struct includes the port and index (idx). \n
 *            The port is the egress port number. \n
 *            The index is the DSCP remarking table index.
 * @param[out] None
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_rmk_tbl_set(), dva_qos_dscp_rmk_tbl_set(), cvb_qos_dscp_rmk_tbl_set(), cva_qos_dscp_rmk_tbl_set().
 */
RtkApiRet rtk_qos_dscp_rmk_tbl_set(UnitChip_t unitChip, const QosDscpRmkTbl_t *qosDscpRmkTbl)
{
    RtkApiRet ret;

    if(NULL == qosDscpRmkTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_rmk_tbl_set(unitChip.unit, qosDscpRmkTbl->port, qosDscpRmkTbl->idx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_rmk_tbl_set(unitChip.unit, qosDscpRmkTbl->port, qosDscpRmkTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_rmk_tbl_set(unitChip.unit, qosDscpRmkTbl->port, qosDscpRmkTbl->idx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_rmk_tbl_set(unitChip.unit, qosDscpRmkTbl->port, qosDscpRmkTbl->idx);
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
 * @brief Get which DSCP remarking table index is used for a specific egress port. \n
 *        Support chip: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed. \n
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] qosSvlanRmkTbl QoS DSCP remarking table (N/A) \n
 *                The struct includes the port and index (idx). \n
 *                The port is the egress port number. \n
 *                The index is the DSCP remapping table index.
 * @retval RT_ERR_OK The API was executed correctly.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_qos_dscp_rmk_tbl_get(), dva_qos_dscp_rmk_tbl_get(), cvb_qos_dscp_rmk_tbl_get(), cva_qos_dscp_rmk_tbl_get().
 */
RtkApiRet rtk_qos_dscp_rmk_tbl_get(UnitChip_t unitChip, QosDscpRmkTbl_t *qosDscpRmkTbl)
{
    RtkApiRet ret;

    if(NULL == qosDscpRmkTbl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_qos_dscp_rmk_tbl_get(unitChip.unit, qosDscpRmkTbl->port, &(qosDscpRmkTbl->idx));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_qos_dscp_rmk_tbl_get(unitChip.unit, qosDscpRmkTbl->port, &(qosDscpRmkTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_qos_dscp_rmk_tbl_get(unitChip.unit, qosDscpRmkTbl->port, &(qosDscpRmkTbl->idx));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_qos_dscp_rmk_tbl_get(unitChip.unit, qosDscpRmkTbl->port, &(qosDscpRmkTbl->idx));
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}
/**@}*/ /* QOS_TPR */

/**@}*/ /* QOS */
