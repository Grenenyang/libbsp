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
#include "rtkas_api_mirror.h"
#include "rtkas_types.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_mirror.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_mirror.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_mirror.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_mirror.h"
#endif

/**
 * @defgroup RTK_MODULE_PORT_MIRROR Mirror
 * @brief Mirror APIs are used for network traffic monitoring and analysis.
 * @{
 */

/**
 * @defgroup MIRROR_ASSIGN Mirroring Assignment
 * @brief Used to set port based mirroring or flow based mirrroring.
 *        When a port is configured for ingress mirroring, any packet received on that port is copied to the mirroring port.
 *        When a port is configured for egress mirroring, any packet transmitted from that port is copied to the mirroring port.
 *        Any packet field supported by Access Control Lists (ACL), such as DMAC, SMAC, and VLAN ID, can be used as characteristics of the mirrored traffic.
 * @{
 */

/**
 * @brief This API sets the ingress and egress mirrored port masks, mirroring port ID, and morg(mirror original) for a port-based mirroring function. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] mirrorInfo           Mirror information (N/A) \n
 *                                  The parameter is used to configure port-based mirroring.
 * @retval RT_ERR_OK                The settings of port-based mirroring were configured. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_mirror_port_based_set(), dva_mirror_port_based_set(), cvb_mirror_port_based_set(), cva_mirror_port_based_set()
 */
RtkApiRet rtk_mirror_port_based_set(UnitChip_t unitChip, MirrorInfo_t *mirrorInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == mirrorInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mirror_port_based_set(unitChip.unit, mirrorInfo->mirrorNo, mirrorInfo->mirroringPort, mirrorInfo->mirroredIngressPortmask, mirrorInfo->mirroredEgressPortmask, mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mirror_port_based_set(unitChip.unit, mirrorInfo->mirrorNo, mirrorInfo->mirroringPort, mirrorInfo->mirroredIngressPortmask, mirrorInfo->mirroredEgressPortmask, mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mirror_port_based_set(unitChip.unit, mirrorInfo->mirrorNo, mirrorInfo->mirroringPort, mirrorInfo->mirroredIngressPortmask, mirrorInfo->mirroredEgressPortmask, mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mirror_port_based_set(unitChip.unit, mirrorInfo->mirrorNo, mirrorInfo->mirroringPort, mirrorInfo->mirroredIngressPortmask, mirrorInfo->mirroredEgressPortmask, mirrorInfo->morg);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }

    return ret;
}

/**
 * @brief This API gets the configurations including mirrored port masks, mirroring port ID, and morg(mirror original) for a port-based mirroring function. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] mirrorInfo        Mirror information (N/A) \n
 *                                  The parameter information is used to obtain port-based mirror settings..
 * @retval RT_ERR_OK                The settings of port-based mirroring were obtained. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_mirror_port_based_get(), dva_mirror_port_based_get(), cvb_mirror_port_based_get(), cva_mirror_port_based_get()
 */
RtkApiRet rtk_mirror_port_based_get(UnitChip_t unitChip, MirrorInfo_t *mirrorInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == mirrorInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mirror_port_based_get(unitChip.unit, mirrorInfo->mirrorNo, &mirrorInfo->mirroringPort, &mirrorInfo->mirroredIngressPortmask, &mirrorInfo->mirroredEgressPortmask, &mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mirror_port_based_get(unitChip.unit, mirrorInfo->mirrorNo, &mirrorInfo->mirroringPort, &mirrorInfo->mirroredIngressPortmask, &mirrorInfo->mirroredEgressPortmask, &mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mirror_port_based_get(unitChip.unit, mirrorInfo->mirrorNo, &mirrorInfo->mirroringPort, &mirrorInfo->mirroredIngressPortmask, &mirrorInfo->mirroredEgressPortmask, &mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mirror_port_based_get(unitChip.unit, mirrorInfo->mirrorNo, &mirrorInfo->mirroringPort, &mirrorInfo->mirroredIngressPortmask, &mirrorInfo->mirroredEgressPortmask, &mirrorInfo->morg);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }

    return ret;
}

/**
 * @brief This API gets the configurations including mirrored port masks, mirroring port ID, and morg(mirror original) for a flow-based mirroring function. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] mirrorInfo           Mirror information (N/A) \n
 *                                  The parameter is used to configure flow-based mirroring.
 * @retval RT_ERR_OK                The settings of flow-based mirroring were configured.
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported.
 * @retval Others                   Please refer to dv1_mirror_flow_based_set(), dva_mirror_flow_based_set(), cvb_mirror_flow_based_set(), cva_mirror_flow_based_set()
 */
RtkApiRet rtk_mirror_flow_based_set(UnitChip_t unitChip, MirrorInfo_t *mirrorInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == mirrorInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mirror_flow_based_set(unitChip.unit, mirrorInfo->mirrorNo, mirrorInfo->mirroringPort, mirrorInfo->mirroredIngressPortmask, mirrorInfo->mirroredEgressPortmask, mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mirror_flow_based_set(unitChip.unit, mirrorInfo->mirrorNo, mirrorInfo->mirroringPort, mirrorInfo->mirroredIngressPortmask, mirrorInfo->mirroredEgressPortmask, mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mirror_flow_based_set(unitChip.unit, mirrorInfo->mirrorNo, mirrorInfo->mirroringPort, mirrorInfo->mirroredIngressPortmask, mirrorInfo->mirroredEgressPortmask, mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mirror_flow_based_set(unitChip.unit, mirrorInfo->mirrorNo, mirrorInfo->mirroringPort, mirrorInfo->mirroredIngressPortmask, mirrorInfo->mirroredEgressPortmask, mirrorInfo->morg);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }

    return ret;
}

/**
 * @brief This API gets the configurations including mirrored port masks, mirroring port ID, and morg(mirror original) for a flow-based mirroring function. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] mirrorInfo        Mirror information (N/A) \n
 *                                  The parameter information is used to obtain flow-based mirror settings.
 * @retval RT_ERR_OK                The settings of flow-based mirroring were obtained. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_mirror_flow_based_get(), dva_mirror_flow_based_get(), cvb_mirror_flow_based_get(), cva_mirror_flow_based_get()
 */
RtkApiRet rtk_mirror_flow_based_get(UnitChip_t unitChip, MirrorInfo_t *mirrorInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == mirrorInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mirror_flow_based_get(unitChip.unit, mirrorInfo->mirrorNo, &mirrorInfo->mirroringPort, &mirrorInfo->mirroredIngressPortmask, &mirrorInfo->mirroredEgressPortmask, &mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mirror_flow_based_get(unitChip.unit, mirrorInfo->mirrorNo, &mirrorInfo->mirroringPort, &mirrorInfo->mirroredIngressPortmask, &mirrorInfo->mirroredEgressPortmask, &mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mirror_flow_based_get(unitChip.unit, mirrorInfo->mirrorNo, &mirrorInfo->mirroringPort, &mirrorInfo->mirroredIngressPortmask, &mirrorInfo->mirroredEgressPortmask, &mirrorInfo->morg);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mirror_flow_based_get(unitChip.unit, mirrorInfo->mirrorNo, &mirrorInfo->mirroringPort, &mirrorInfo->mirroredIngressPortmask, &mirrorInfo->mirroredEgressPortmask, &mirrorInfo->morg);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }

    return ret;
}

/**@}*/ /* MIRROR_ASSIGN */

/**
 * @defgroup MIRROR_FILTER Mirroring Filter
 * @brief Used to set mirroring filter.
 *        When the mirroring filter is disabled, both forwarded traffic and mirrored traffic can be transmitted from the mirroring port.
 *        However, when the mirroring filter is enabled, only mirrored traffic is transmitted from the mirroring port.
 * @{
 */

/**
 * @brief This API sets the mirror filter used to prevent forwarding normal packets to the mirroring port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] filterInfo           Filter information (N/A) \n
 *                                  The parameter is used to configure the mirror filter setting.
 * @retval RT_ERR_OK                Set the mirror filter successfully. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_mirror_filter_set(), dva_mirror_filter_set(), cvb_mirror_filter_set(), cva_mirror_filter_set()
 */
RtkApiRet rtk_mirror_filter_set(UnitChip_t unitChip, MirrorFilterInfo_t *filterInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == filterInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mirror_filter_set(unitChip.unit, filterInfo->mirrorNo, filterInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mirror_filter_set(unitChip.unit, filterInfo->mirrorNo, filterInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mirror_filter_set(unitChip.unit, filterInfo->mirrorNo, filterInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mirror_filter_set(unitChip.unit, filterInfo->mirrorNo, filterInfo->enable);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API gets the mirror filter used to prevent forwarding normal packets to the mirroring port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] filterInfo        Filter information (N/A) \n
 *                                  The parameter information is used to obtain mirror filter settings.
 * @retval RT_ERR_OK                The settings of mirroring filter were obtained. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_mirror_filter_get(), dva_mirror_filter_get(), cvb_mirror_filter_get(), cva_mirror_filter_get()
 */
RtkApiRet rtk_mirror_filter_get(UnitChip_t unitChip, MirrorFilterInfo_t *filterInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == filterInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mirror_filter_get(unitChip.unit, filterInfo->mirrorNo, &filterInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mirror_filter_get(unitChip.unit, filterInfo->mirrorNo, &filterInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mirror_filter_get(unitChip.unit, filterInfo->mirrorNo, &filterInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mirror_filter_get(unitChip.unit, filterInfo->mirrorNo, &filterInfo->enable);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIRROR_FILTER */

/**
 * @defgroup MIRROR_SAMPLE Mirroring Sample Rate
 * @brief Used to set mirroring sample rate
 *        Users can control the quantity of mirrored packets based on the configured function.
 *        If the sample rate is set to n, only the nth, 2nth, 3nth, and so on, packets are copied to the mirroring port.
 * @{
 */

/**
 * @brief This API sets sample rate of mirroring function. \n
 *        There is always only one mirrored packet transmitted from the mirroring port in the sample size. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] sampleInfo           Sample information (N/A) \n
 *                                  The parameter is used to configure the mirror sample rate setting.
 * @retval RT_ERR_OK                Set the mirror sample rate successfully. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_mirror_sample_rate_set(), dva_mirror_sample_rate_set(), cvb_mirror_sample_rate_set(), cva_mirror_sample_rate_set()
 */
RtkApiRet rtk_mirror_sample_rate_set(UnitChip_t unitChip, MirrorSampleInfo_t *sampleInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == sampleInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mirror_sample_rate_set(unitChip.unit, sampleInfo->mirrorNo, sampleInfo->samples);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mirror_sample_rate_set(unitChip.unit, sampleInfo->mirrorNo, sampleInfo->samples);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mirror_sample_rate_set(unitChip.unit, sampleInfo->mirrorNo, sampleInfo->samples);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mirror_sample_rate_set(unitChip.unit, sampleInfo->mirrorNo, sampleInfo->samples);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API gets sample rate of mirroring function. \n
 *        There is always only one mirrored packet transmitted from the mirroring port in the sample size. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] sampleInfo        Sample information (N/A) \n
 *                                  The parameter information is used to obtain mirror sample rate settings.
 * @retval RT_ERR_OK                The settings of mirroring sample rate were obtained. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_mirror_sample_rate_get(), dva_mirror_sample_rate_get(), cvb_mirror_sample_rate_get(), cva_mirror_sample_rate_get()
 */
RtkApiRet rtk_mirror_sample_rate_get(UnitChip_t unitChip, MirrorSampleInfo_t *sampleInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == sampleInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mirror_sample_rate_get(unitChip.unit, sampleInfo->mirrorNo, &sampleInfo->samples);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mirror_sample_rate_get(unitChip.unit, sampleInfo->mirrorNo, &sampleInfo->samples);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mirror_sample_rate_get(unitChip.unit, sampleInfo->mirrorNo, &sampleInfo->samples);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mirror_sample_rate_get(unitChip.unit, sampleInfo->mirrorNo, &sampleInfo->samples);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIRROR_SAMPLE */

/**
 * @defgroup MIRROR_RSPAN Mirror RSPAN
 * @brief Used to set mirror RSPAN(Remote Switched Port Analyzer) rx/tx. \n
 *        The mirrored switch forwards mirror packets to the mirroring switch through the intermediate switch by adding an RSPAN tag.
 * @{
 */

/**
 * @brief This API sets RSPAN TX action and tag for mirrored packets in remote mirroring. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] rspanTxInfo          RSPAN tx information (N/A) \n
 *                                  The parameter is used to configure mirror rspan tx settings.
 * @retval RT_ERR_OK                Set the mirror rspan tx successfully. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_rspan_tx_set(), dva_rspan_tx_set(), cvb_rspan_tx_set(), cva_rspan_tx_set()
 */
RtkApiRet rtk_rspan_tx_set(UnitChip_t unitChip, MirrorRspanTxInfo_t *rspanTxInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == rspanTxInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_rspan_tx_set(unitChip.unit, rspanTxInfo->mirrorNo, rspanTxInfo->mirroringPort, rspanTxInfo->action, rspanTxInfo->tag);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_rspan_tx_set(unitChip.unit, rspanTxInfo->mirrorNo, rspanTxInfo->mirroringPort, rspanTxInfo->action, rspanTxInfo->tag);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_rspan_tx_set(unitChip.unit, rspanTxInfo->mirrorNo, rspanTxInfo->mirroringPort, rspanTxInfo->action, rspanTxInfo->tag);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_rspan_tx_set(unitChip.unit, rspanTxInfo->mirrorNo, rspanTxInfo->mirroringPort, rspanTxInfo->action, rspanTxInfo->tag);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API gets RSPAN TX action and tag for mirrored packets in remote mirroring. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] rspanTxInfo       RSPAN tx information (N/A) \n
 *                                  The parameter information is used to obtain mirror rspan tx settings.
 * @retval RT_ERR_OK                The settings of mirroring rspan tx were obtained. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_rspan_tx_get(), dva_rspan_tx_get(), cvb_rspan_tx_get(), cva_rspan_tx_get()
 */
RtkApiRet rtk_rspan_tx_get(UnitChip_t unitChip, MirrorRspanTxInfo_t *rspanTxInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == rspanTxInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_rspan_tx_get(unitChip.unit, rspanTxInfo->mirrorNo, &rspanTxInfo->mirroringPort, &rspanTxInfo->action, &rspanTxInfo->tag);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_rspan_tx_get(unitChip.unit, rspanTxInfo->mirrorNo, &rspanTxInfo->mirroringPort, &rspanTxInfo->action, &rspanTxInfo->tag);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_rspan_tx_get(unitChip.unit, rspanTxInfo->mirrorNo, &rspanTxInfo->mirroringPort, &rspanTxInfo->action, &rspanTxInfo->tag);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_rspan_tx_get(unitChip.unit, rspanTxInfo->mirrorNo, &rspanTxInfo->mirroringPort, &rspanTxInfo->action, &rspanTxInfo->tag);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API sets RSPAN RX configurations including enable and vid for remote mirroring. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] rspanRxInfo          RSPAN rx information (N/A) \n
 *                                  The parameter is used to configure mirror rspan rx settings.
 * @retval RT_ERR_OK                Set the mirror rspan rx successfully. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_rspan_rx_set(), dva_rspan_rx_set(), cvb_rspan_rx_set(), cva_rspan_rx_set()
 */
RtkApiRet rtk_rspan_rx_set(UnitChip_t unitChip, MirrorRspanRxInfo_t *rspanRxInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == rspanRxInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_rspan_rx_set(unitChip.unit, rspanRxInfo->mirrorNo, rspanRxInfo->mirroredPortmask, rspanRxInfo->enable, rspanRxInfo->vid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_rspan_rx_set(unitChip.unit, rspanRxInfo->mirrorNo, rspanRxInfo->mirroredPortmask, rspanRxInfo->enable, rspanRxInfo->vid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_rspan_rx_set(unitChip.unit, rspanRxInfo->mirrorNo, rspanRxInfo->mirroredPortmask, rspanRxInfo->enable, rspanRxInfo->vid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_rspan_rx_set(unitChip.unit, rspanRxInfo->mirrorNo, rspanRxInfo->mirroredPortmask, rspanRxInfo->enable, rspanRxInfo->vid);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief This API gets RSPAN RX configurations including enable and vid for remote mirroring. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] rspanRxInfo       RSPAN rx information (N/A) \n
 *                                  The parameters information is used to obtain mirror rspan rx settings.
 * @retval RT_ERR_OK                The settings of mirroring rspan rx were obtained. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_rspan_rx_get(), dva_rspan_rx_get(), cvb_rspan_rx_get(), cva_rspan_rx_get()
 */
RtkApiRet rtk_rspan_rx_get(UnitChip_t unitChip, MirrorRspanRxInfo_t *rspanRxInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == rspanRxInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_rspan_rx_get(unitChip.unit, rspanRxInfo->mirrorNo, &rspanRxInfo->mirroredPortmask, &rspanRxInfo->enable, &rspanRxInfo->vid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_rspan_rx_get(unitChip.unit, rspanRxInfo->mirrorNo, &rspanRxInfo->mirroredPortmask, &rspanRxInfo->enable, &rspanRxInfo->vid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_rspan_rx_get(unitChip.unit, rspanRxInfo->mirrorNo, &rspanRxInfo->mirroredPortmask, &rspanRxInfo->enable, &rspanRxInfo->vid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_rspan_rx_get(unitChip.unit, rspanRxInfo->mirrorNo, &rspanRxInfo->mirroredPortmask, &rspanRxInfo->enable, &rspanRxInfo->vid);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIRROR_RSPAN */

/**
 * \defgroup MIRROR_RESET Mirror Reset
 * \brief Used to reset mirror config
 * @{
 */

/**
 * @brief This API resets mirror function 1 or 2. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip id (N/A) \n
 *                                  Stacking unit id is used for indicating which switch to access.
 *                                  Chip id is used for chip model.
 * @param[in] resetInfo             Reset information (N/A) \n
 *                                  The parameter is used to reset mirror settings.
 * @retval RT_ERR_OK                Reset the mirror settings successfully. \n
 * @retval -RT_ERR_NULL_POINTER     Null pointer for parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval Others                   Please refer to dv1_mirror_reset(), dva_mirror_reset(), cvb_mirror_reset(), cva_mirror_reset()
 */
RtkApiRet rtk_mirror_reset(UnitChip_t unitChip, MirrorResetInfo_t *resetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == resetInfo)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mirror_reset(unitChip.unit, resetInfo->mirrorNo);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mirror_reset(unitChip.unit, resetInfo->mirrorNo);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mirror_reset(unitChip.unit, resetInfo->mirrorNo);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mirror_reset(unitChip.unit, resetInfo->mirrorNo);
                break;
#endif
            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**@}*/ /* MIRROR_RESET */

/**@}*/ /* RTK_MODULE_PORT_MIRROR */
