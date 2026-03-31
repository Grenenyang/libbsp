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
#include "rtkas_api_vlan.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_vlan.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_vlan.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_vlan.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_vlan.h"
#endif

/**
 * @defgroup RTK_MODULE_VLAN VLAN
 * @brief VLAN APIs. This component is used to configure settings during the VLAN process flow.
 * @{
 */
/**
 * @defgroup VLAN_TAG_STATUS_FILTER VLAN Tag Status Filter
 * @brief APIs to configure the ingress packet filter based on VLAN tag status
 * @{
 */
/**
 * @brief Configure acceptable inner tag frame types for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanPortAcptTypeInfo VLAN port accept type information (N/A) \n
 *                                  Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The acceptable frame type of the port was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_acpt_type_set(), dva_vlan_port_acpt_type_set(), cvb_vlan_port_acpt_type_set(), cva_vlan_port_acpt_type_set().
 */
RtkApiRet rtk_vlan_port_acpt_type_set(UnitChip_t unitChip, VlanPortAcptTypeInfo_t *vlanPortAcptTypeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortAcptTypeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_acpt_type_set(unitChip.unit, vlanPortAcptTypeInfo->port, vlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_acpt_type_set(unitChip.unit, vlanPortAcptTypeInfo->port, vlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_acpt_type_set(unitChip.unit, vlanPortAcptTypeInfo->port, vlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_acpt_type_set(unitChip.unit, vlanPortAcptTypeInfo->port, vlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get acceptable inner tag frame types for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip             Stacking unit and chip ID (N/A) \n
 *                                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanPortAcptTypeInfo VLAN port accept type information (N/A) \n
 *                                     Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The acceptable frame type of the port was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_acpt_type_get(), dva_vlan_port_acpt_type_get(), cvb_vlan_port_acpt_type_get(), cva_vlan_port_acpt_type_get().
 */
RtkApiRet rtk_vlan_port_acpt_type_get(UnitChip_t unitChip, VlanPortAcptTypeInfo_t *vlanPortAcptTypeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortAcptTypeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_acpt_type_get(unitChip.unit, vlanPortAcptTypeInfo->port, &vlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_acpt_type_get(unitChip.unit, vlanPortAcptTypeInfo->port, &vlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_acpt_type_get(unitChip.unit, vlanPortAcptTypeInfo->port, &vlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_acpt_type_get(unitChip.unit, vlanPortAcptTypeInfo->port, &vlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Configure acceptable outer tag frame types for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip              Stacking unit and chip ID (N/A) \n
 *                                   Stacking unit ID is used to indicate which switch is to be accessed.
 *                                   Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  svlanPortAcptTypeInfo SVLAN port accept type information (N/A) \n
 *                                   Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The acceptable frame type of the port was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_port_acpt_type_set(), dva_svlan_port_acpt_type_set(), cvb_svlan_port_acpt_type_set(), cva_svlan_port_acpt_type_set().
 */
RtkApiRet rtk_svlan_port_acpt_type_set(UnitChip_t unitChip, SvlanPortAcptTypeInfo_t *svlanPortAcptTypeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanPortAcptTypeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_port_acpt_type_set(unitChip.unit, svlanPortAcptTypeInfo->port, svlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_port_acpt_type_set(unitChip.unit, svlanPortAcptTypeInfo->port, svlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_port_acpt_type_set(unitChip.unit, svlanPortAcptTypeInfo->port, svlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_port_acpt_type_set(unitChip.unit, svlanPortAcptTypeInfo->port, svlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get acceptable outer tag frame types for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip              Stacking unit and chip ID (N/A) \n
 *                                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] svlanPortAcptTypeInfo SVLAN port accept type information (N/A) \n
 *                                      Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The acceptable frame type of the port was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_port_acpt_type_get(), dva_svlan_port_acpt_type_get(), cvb_svlan_port_acpt_type_get(), cva_svlan_port_acpt_type_get().
 */
RtkApiRet rtk_svlan_port_acpt_type_get(UnitChip_t unitChip, SvlanPortAcptTypeInfo_t *svlanPortAcptTypeInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanPortAcptTypeInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_port_acpt_type_get(unitChip.unit, svlanPortAcptTypeInfo->port, &svlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_port_acpt_type_get(unitChip.unit, svlanPortAcptTypeInfo->port, &svlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_port_acpt_type_get(unitChip.unit, svlanPortAcptTypeInfo->port, &svlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_port_acpt_type_get(unitChip.unit, svlanPortAcptTypeInfo->port, &svlanPortAcptTypeInfo->acceptFrameType);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Configure whether to accept double tag for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                  Stacking unit and chip ID (N/A) \n
 *                                       Stacking unit ID is used to indicate which switch is to be accessed.
 *                                       Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanPortAcptDoubleTagInfo VLAN port accept double tag information (N/A) \n
 *                                       Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The acceptable double tag type of the port was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_acpt_double_tag_set(), dva_vlan_port_acpt_double_tag_set(), cvb_vlan_port_acpt_double_tag_set(), cva_vlan_port_acpt_double_tag_set().
 */
RtkApiRet rtk_vlan_port_acpt_double_tag_set(UnitChip_t unitChip, VlanPortAcptDoubleTagInfo_t *vlanPortAcptDoubleTagInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortAcptDoubleTagInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_acpt_double_tag_set(unitChip.unit, vlanPortAcptDoubleTagInfo->port, vlanPortAcptDoubleTagInfo->acceptDoubleTag);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_acpt_double_tag_set(unitChip.unit, vlanPortAcptDoubleTagInfo->port, vlanPortAcptDoubleTagInfo->acceptDoubleTag);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_acpt_double_tag_set(unitChip.unit, vlanPortAcptDoubleTagInfo->port, vlanPortAcptDoubleTagInfo->acceptDoubleTag);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_acpt_double_tag_set(unitChip.unit, vlanPortAcptDoubleTagInfo->port, vlanPortAcptDoubleTagInfo->acceptDoubleTag);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get acceptable double tag for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                  Stacking unit and chip ID (N/A) \n
 *                                          Stacking unit ID is used to indicate which switch is to be accessed.
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanPortAcptDoubleTagInfo VLAN port accept double tag information (N/A) \n
 *                                          Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The acceptable double tag type of the port was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_acpt_double_tag_get(), dva_vlan_port_acpt_double_tag_get(), cvb_vlan_port_acpt_double_tag_get(), cva_vlan_port_acpt_double_tag_get().
 */
RtkApiRet rtk_vlan_port_acpt_double_tag_get(UnitChip_t unitChip, VlanPortAcptDoubleTagInfo_t *vlanPortAcptDoubleTagInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortAcptDoubleTagInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_acpt_double_tag_get(unitChip.unit, vlanPortAcptDoubleTagInfo->port, &vlanPortAcptDoubleTagInfo->acceptDoubleTag);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_acpt_double_tag_get(unitChip.unit, vlanPortAcptDoubleTagInfo->port, &vlanPortAcptDoubleTagInfo->acceptDoubleTag);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_acpt_double_tag_get(unitChip.unit, vlanPortAcptDoubleTagInfo->port, &vlanPortAcptDoubleTagInfo->acceptDoubleTag);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_acpt_double_tag_get(unitChip.unit, vlanPortAcptDoubleTagInfo->port, &vlanPortAcptDoubleTagInfo->acceptDoubleTag);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*VLAN_TAG_STATUS_FILTER*/

/**
 * @defgroup VLAN_ASSIGNMENT VLAN Assignment
 * @brief APIs to configure port-based and tag-based VLAN
 * @{
 */
/**
 * @brief Configure the switch not to ignore VLAN tags of incoming packets for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip         Stacking unit and chip ID (N/A) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 *                              Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanTagAwareInfo VLAN tag aware information (N/A) \n
 *                              Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The tag aware function of the port was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_tag_aware_set(), dva_vlan_tag_aware_set(), cvb_vlan_tag_aware_set(), cva_vlan_tag_aware_set().
 */
RtkApiRet rtk_vlan_tag_aware_set(UnitChip_t unitChip, VlanTagAwareInfo_t *vlanTagAwareInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanTagAwareInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_tag_aware_set(unitChip.unit, vlanTagAwareInfo->port, vlanTagAwareInfo->enabled);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_tag_aware_set(unitChip.unit, vlanTagAwareInfo->port, vlanTagAwareInfo->enabled);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_tag_aware_set(unitChip.unit, vlanTagAwareInfo->port, vlanTagAwareInfo->enabled);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_tag_aware_set(unitChip.unit, vlanTagAwareInfo->port, vlanTagAwareInfo->enabled);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get the configuration of the ignoring VLAN tag function for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip         Stacking unit and chip ID (N/A) \n
 *                                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanTagAwareInfo VLAN tag aware information (N/A) \n
 *                                 Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The tag aware function of the port was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_tag_aware_get(), dva_vlan_tag_aware_get(), cvb_vlan_tag_aware_get(), cva_vlan_tag_aware_get().
 */
RtkApiRet rtk_vlan_tag_aware_get(UnitChip_t unitChip, VlanTagAwareInfo_t *vlanTagAwareInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanTagAwareInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_tag_aware_get(unitChip.unit, vlanTagAwareInfo->port, &vlanTagAwareInfo->enabled);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_tag_aware_get(unitChip.unit, vlanTagAwareInfo->port, &vlanTagAwareInfo->enabled);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_tag_aware_get(unitChip.unit, vlanTagAwareInfo->port, &vlanTagAwareInfo->enabled);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_tag_aware_get(unitChip.unit, vlanTagAwareInfo->port, &vlanTagAwareInfo->enabled);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Configure a default VLAN ID and priority of the inner VLAN tag for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip         Stacking unit and chip ID (N/A) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 *                              Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanPortPvidInfo VLAN port PVID information (N/A) \n
 *                              Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The inner port-based VLAN was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_pvid_set(), dva_vlan_port_pvid_set(), cvb_vlan_port_pvid_set(), cva_vlan_port_pvid_set().
 */
RtkApiRet rtk_vlan_port_pvid_set(UnitChip_t unitChip, VlanPortPvidInfo_t *vlanPortPvidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortPvidInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_pvid_set(unitChip.unit, vlanPortPvidInfo->port, vlanPortPvidInfo->pvid, vlanPortPvidInfo->priority);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_pvid_set(unitChip.unit, vlanPortPvidInfo->port, vlanPortPvidInfo->pvid, vlanPortPvidInfo->priority);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_pvid_set(unitChip.unit, vlanPortPvidInfo->port, vlanPortPvidInfo->pvid, vlanPortPvidInfo->priority);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_pvid_set(unitChip.unit, vlanPortPvidInfo->port, vlanPortPvidInfo->pvid, vlanPortPvidInfo->priority);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get the default VLAN ID and priority of the inner VLAN tag for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip         Stacking unit and chip ID (N/A) \n
 *                                 Stacking unit ID is used to indicate which switch is to be accessed.
 *                                 Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanPortPvidInfo VLAN port PVID information (N/A) \n
 *                                 Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The inner port-based VLAN was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_pvid_get(), dva_vlan_port_pvid_get(), cvb_vlan_port_pvid_get(), cva_vlan_port_pvid_get().
 */
RtkApiRet rtk_vlan_port_pvid_get(UnitChip_t unitChip, VlanPortPvidInfo_t *vlanPortPvidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortPvidInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_pvid_get(unitChip.unit, vlanPortPvidInfo->port, &vlanPortPvidInfo->pvid, &vlanPortPvidInfo->priority);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_pvid_get(unitChip.unit, vlanPortPvidInfo->port, &vlanPortPvidInfo->pvid, &vlanPortPvidInfo->priority);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_pvid_get(unitChip.unit, vlanPortPvidInfo->port, &vlanPortPvidInfo->pvid, &vlanPortPvidInfo->priority);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_pvid_get(unitChip.unit, vlanPortPvidInfo->port, &vlanPortPvidInfo->pvid, &vlanPortPvidInfo->priority);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Configure a default VLAN ID and priority of the outer VLAN tag for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip          Stacking unit and chip ID (N/A) \n
 *                               Stacking unit ID is used to indicate which switch is to be accessed.
 *                               Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  svlanPortPvidInfo SVLAN port PVID information (N/A) \n
 *                               Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The outer port-based VLAN was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_port_pvid_set(), dva_svlan_port_pvid_set(), cvb_svlan_port_pvid_set(), cva_svlan_port_pvid_set().
 */
RtkApiRet rtk_svlan_port_pvid_set(UnitChip_t unitChip, SvlanPortPvidInfo_t *svlanPortPvidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanPortPvidInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_port_pvid_set(unitChip.unit, svlanPortPvidInfo->port, svlanPortPvidInfo->pvid, svlanPortPvidInfo->priority, svlanPortPvidInfo->dei);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_port_pvid_set(unitChip.unit, svlanPortPvidInfo->port, svlanPortPvidInfo->pvid, svlanPortPvidInfo->priority, svlanPortPvidInfo->dei);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_port_pvid_set(unitChip.unit, svlanPortPvidInfo->port, svlanPortPvidInfo->pvid, svlanPortPvidInfo->priority, svlanPortPvidInfo->dei);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_port_pvid_set(unitChip.unit, svlanPortPvidInfo->port, svlanPortPvidInfo->pvid, svlanPortPvidInfo->priority, svlanPortPvidInfo->dei);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get the default VLAN ID, priority, and DEI of the outer VLAN tag for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip          Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] svlanPortPvidInfo SVLAN port PVID information (N/A) \n
 *                                  Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The outer port-based VLAN was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_port_pvid_get(), dva_svlan_port_pvid_get(), cvb_svlan_port_pvid_get(), cva_svlan_port_pvid_get().
 */
RtkApiRet rtk_svlan_port_pvid_get(UnitChip_t unitChip, SvlanPortPvidInfo_t *svlanPortPvidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanPortPvidInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_port_pvid_get(unitChip.unit, svlanPortPvidInfo->port, &svlanPortPvidInfo->pvid, &svlanPortPvidInfo->priority, &svlanPortPvidInfo->dei);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_port_pvid_get(unitChip.unit, svlanPortPvidInfo->port, &svlanPortPvidInfo->pvid, &svlanPortPvidInfo->priority, &svlanPortPvidInfo->dei);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_port_pvid_get(unitChip.unit, svlanPortPvidInfo->port, &svlanPortPvidInfo->pvid, &svlanPortPvidInfo->priority, &svlanPortPvidInfo->dei);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_port_pvid_get(unitChip.unit, svlanPortPvidInfo->port, &svlanPortPvidInfo->pvid, &svlanPortPvidInfo->priority, &svlanPortPvidInfo->dei);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*VLAN_ASSIGNMENT*/

/**
 * @defgroup INGRESS_VLAN_RETAGGING Ingress VLAN Retagging
 * @brief APIs to configure the VLAN ingress retagging
 * @{
 */
/**
 * @brief Initialize ingress VLAN retagging. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK           The ingress VLAN retagging function of the ports was initialized. \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported. \n
 * @retval Others              Please refer to dv1_vlan_igr_retagging_init(), dva_vlan_igr_retagging_init(), cvb_vlan_igr_retagging_init(), cva_vlan_igr_retagging_init().
 */
RtkApiRet rtk_vlan_igr_retagging_init(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_vlan_igr_retagging_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_vlan_igr_retagging_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_vlan_igr_retagging_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_vlan_igr_retagging_init(unitChip.unit);
            break;
#endif
        default:
            ret = RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/**
 * @brief Set an ingress VLAN retagging entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanIgrRetaggingInfo VLAN ingress retagging information (N/A) \n
 *                                  Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The ingress VLAN retagging entry was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_igr_retagging_set(), dva_vlan_igr_retagging_set(), cvb_vlan_igr_retagging_set(), cva_vlan_igr_retagging_set().
 */
RtkApiRet rtk_vlan_igr_retagging_set(UnitChip_t unitChip, VlanIgrRetaggingInfo_t *vlanIgrRetaggingInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanIgrRetaggingInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_igr_retagging_set(unitChip.unit, vlanIgrRetaggingInfo->idx, vlanIgrRetaggingInfo->originalVid, vlanIgrRetaggingInfo->newVid, vlanIgrRetaggingInfo->pbm, vlanIgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_igr_retagging_set(unitChip.unit, vlanIgrRetaggingInfo->idx, vlanIgrRetaggingInfo->originalVid, vlanIgrRetaggingInfo->newVid, vlanIgrRetaggingInfo->pbm, vlanIgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_igr_retagging_set(unitChip.unit, vlanIgrRetaggingInfo->idx, vlanIgrRetaggingInfo->originalVid, vlanIgrRetaggingInfo->newVid, vlanIgrRetaggingInfo->pbm, vlanIgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_igr_retagging_set(unitChip.unit, vlanIgrRetaggingInfo->idx, vlanIgrRetaggingInfo->originalVid, vlanIgrRetaggingInfo->newVid, vlanIgrRetaggingInfo->pbm, vlanIgrRetaggingInfo->valid);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get an ingress VLAN retagging entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip             Stacking unit and chip ID (N/A) \n
 *                                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanIgrRetaggingInfo VLAN ingress retagging information (N/A) \n
 *                                     Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The ingress VLAN retagging entry was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_igr_retagging_get(), dva_vlan_igr_retagging_get(), cvb_vlan_igr_retagging_get(), cva_vlan_igr_retagging_get().
 */
RtkApiRet rtk_vlan_igr_retagging_get(UnitChip_t unitChip, VlanIgrRetaggingInfo_t *vlanIgrRetaggingInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanIgrRetaggingInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_igr_retagging_get(unitChip.unit, vlanIgrRetaggingInfo->idx, &vlanIgrRetaggingInfo->originalVid, &vlanIgrRetaggingInfo->newVid, &vlanIgrRetaggingInfo->pbm, &vlanIgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_igr_retagging_get(unitChip.unit, vlanIgrRetaggingInfo->idx, &vlanIgrRetaggingInfo->originalVid, &vlanIgrRetaggingInfo->newVid, &vlanIgrRetaggingInfo->pbm, &vlanIgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_igr_retagging_get(unitChip.unit, vlanIgrRetaggingInfo->idx, &vlanIgrRetaggingInfo->originalVid, &vlanIgrRetaggingInfo->newVid, &vlanIgrRetaggingInfo->pbm, &vlanIgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_igr_retagging_get(unitChip.unit, vlanIgrRetaggingInfo->idx, &vlanIgrRetaggingInfo->originalVid, &vlanIgrRetaggingInfo->newVid, &vlanIgrRetaggingInfo->pbm, &vlanIgrRetaggingInfo->valid);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Set ingress VLAN retagging status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip               Stacking unit and chip ID (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanIgrRetaggingEnInfo VLAN ingress retagging status information (N/A) \n
 *                                    Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The ingress VLAN retagging status was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_igr_retagging_en_set(), dva_vlan_igr_retagging_en_set(), cvb_vlan_igr_retagging_en_set(), cva_vlan_igr_retagging_en_set().
 */
RtkApiRet rtk_vlan_igr_retagging_en_set(UnitChip_t unitChip, VlanIgrRetaggingEnInfo_t *vlanIgrRetaggingEnInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanIgrRetaggingEnInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_igr_retagging_en_set(unitChip.unit, vlanIgrRetaggingEnInfo->port, vlanIgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_igr_retagging_en_set(unitChip.unit, vlanIgrRetaggingEnInfo->port, vlanIgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_igr_retagging_en_set(unitChip.unit, vlanIgrRetaggingEnInfo->port, vlanIgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_igr_retagging_en_set(unitChip.unit, vlanIgrRetaggingEnInfo->port, vlanIgrRetaggingEnInfo->enable);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get ingress VLAN retagging status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip               Stacking unit and chip ID (N/A) \n
 *                                       Stacking unit ID is used to indicate which switch is to be accessed.
 *                                       Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanIgrRetaggingEnInfo VLAN ingress retagging status information (N/A) \n
 *                                       Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The ingress VLAN retagging status was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_igr_retagging_en_get(), dva_vlan_igr_retagging_en_get(), cvb_vlan_igr_retagging_en_get(), cva_vlan_igr_retagging_en_get().
 */
RtkApiRet rtk_vlan_igr_retagging_en_get(UnitChip_t unitChip, VlanIgrRetaggingEnInfo_t *vlanIgrRetaggingEnInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanIgrRetaggingEnInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_igr_retagging_en_get(unitChip.unit, vlanIgrRetaggingEnInfo->port, &vlanIgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_igr_retagging_en_get(unitChip.unit, vlanIgrRetaggingEnInfo->port, &vlanIgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_igr_retagging_en_get(unitChip.unit, vlanIgrRetaggingEnInfo->port, &vlanIgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_igr_retagging_en_get(unitChip.unit, vlanIgrRetaggingEnInfo->port, &vlanIgrRetaggingEnInfo->enable);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*INGRESS_VLAN_RETAGGING*/

/**
 * @defgroup VLAN_TABLE_LOOKUP VLAN Table Lookup
 * @brief APIs to configure the VLAN lookup table
 * @{
 */
/**
 * @brief Initialize the VLAN table. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK           The VLAN table was initialized. \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported. \n
 * @retval Others              Please refer to dv1_vlan_init(), dva_vlan_init(), cvb_vlan_init(), cva_vlan_init().
 */
RtkApiRet rtk_vlan_init(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_vlan_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_vlan_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_vlan_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_vlan_init(unitChip.unit);
            break;
#endif
        default:
            ret = RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/**
 * @brief Add or modify a VLAN entry in the VLAN table. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanInfo VLAN entry information (N/A) \n
 *                      Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The VLAN entry was added/modified. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_set(), dva_vlan_set(), cvb_vlan_set(), cva_vlan_set().
 */
RtkApiRet rtk_vlan_set(UnitChip_t unitChip, VlanInfo_t *vlanInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_set(unitChip.unit, vlanInfo->vid, &vlanInfo->vlanEntry);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_set(unitChip.unit, vlanInfo->vid, &vlanInfo->vlanEntry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_set(unitChip.unit, vlanInfo->vid, &vlanInfo->vlanEntry);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_set(unitChip.unit, vlanInfo->vid, &vlanInfo->vlanEntry);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get a VLAN entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip Stacking unit and chip ID (N/A) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 *                         Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanInfo VLAN entry information (N/A) \n
 *                         Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The VLAN entry was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_get(), dva_vlan_get(), cvb_vlan_get(), cva_vlan_get().
 */
RtkApiRet rtk_vlan_get(UnitChip_t unitChip, VlanInfo_t *vlanInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_get(unitChip.unit, vlanInfo->vid, &vlanInfo->vlanEntry);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_get(unitChip.unit, vlanInfo->vid, &vlanInfo->vlanEntry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_get(unitChip.unit, vlanInfo->vid, &vlanInfo->vlanEntry);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_get(unitChip.unit, vlanInfo->vid, &vlanInfo->vlanEntry);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Destroy a VLAN entry in the VLAN table. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip        Stacking unit and chip ID (N/A) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 *                             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanDestroyInfo VLAN entry destroy information (N/A) \n
 *                             Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The VLAN entry was destroyed. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_destroy(), dva_vlan_destroy(), cvb_vlan_destroy(), cva_vlan_destroy().
 */
RtkApiRet rtk_vlan_destroy(UnitChip_t unitChip, VlanDestroyInfo_t *vlanDestroyInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanDestroyInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_destroy(unitChip.unit, vlanDestroyInfo->vid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_destroy(unitChip.unit, vlanDestroyInfo->vid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_destroy(unitChip.unit, vlanDestroyInfo->vid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_destroy(unitChip.unit, vlanDestroyInfo->vid);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Configure whether to enable the outer tag parser ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip               Stacking unit and chip ID (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  svlanPortPktParserInfo SVLAN port packet parser information (N/A) \n
 *                                    Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The outer tag parser ability was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_port_pkt_parser_en_set(), dva_svlan_port_pkt_parser_en_set(), cvb_svlan_port_pkt_parser_en_set(), cva_svlan_port_pkt_parser_en_set().
 */
RtkApiRet rtk_svlan_port_pkt_parser_en_set(UnitChip_t unitChip, SvlanPortPktParserInfo_t *svlanPortPktParserInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanPortPktParserInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_port_pkt_parser_en_set(unitChip.unit, svlanPortPktParserInfo->port, svlanPortPktParserInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_port_pkt_parser_en_set(unitChip.unit, svlanPortPktParserInfo->port, svlanPortPktParserInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_port_pkt_parser_en_set(unitChip.unit, svlanPortPktParserInfo->port, svlanPortPktParserInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_port_pkt_parser_en_set(unitChip.unit, svlanPortPktParserInfo->port, svlanPortPktParserInfo->enable);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get whether to enable the outer tag parser ability. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip               Stacking unit and chip ID (N/A) \n
 *                                       Stacking unit ID is used to indicate which switch is to be accessed.
 *                                       Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] svlanPortPktParserInfo SVLAN port packet parser information (N/A) \n
 *                                       Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The outer tag parser ability was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_port_pkt_parser_en_get(), dva_svlan_port_pkt_parser_en_get(), cvb_svlan_port_pkt_parser_en_get(), cva_svlan_port_pkt_parser_en_get().
 */
RtkApiRet rtk_svlan_port_pkt_parser_en_get(UnitChip_t unitChip, SvlanPortPktParserInfo_t *svlanPortPktParserInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanPortPktParserInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_port_pkt_parser_en_get(unitChip.unit, svlanPortPktParserInfo->port, &svlanPortPktParserInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_port_pkt_parser_en_get(unitChip.unit, svlanPortPktParserInfo->port, &svlanPortPktParserInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_port_pkt_parser_en_get(unitChip.unit, svlanPortPktParserInfo->port, &svlanPortPktParserInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_port_pkt_parser_en_get(unitChip.unit, svlanPortPktParserInfo->port, &svlanPortPktParserInfo->enable);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Configure whether forwarding is based on outer VID or inner VID. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  svlanFwdBaseOvidInfo SVLAN forwarding based on outer VID information (N/A) \n
 *                                  Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The forwarding method was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_fwd_base_ovid_set(), dva_svlan_fwd_base_ovid_set(), cvb_svlan_fwd_base_ovid_set(), cva_svlan_fwd_base_ovid_set().
 */
RtkApiRet rtk_svlan_fwd_base_ovid_set(UnitChip_t unitChip, SvlanFwdBaseOvidInfo_t *svlanFwdBaseOvidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanFwdBaseOvidInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_fwd_base_ovid_set(unitChip.unit, svlanFwdBaseOvidInfo->port, svlanFwdBaseOvidInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_fwd_base_ovid_set(unitChip.unit, svlanFwdBaseOvidInfo->port, svlanFwdBaseOvidInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_fwd_base_ovid_set(unitChip.unit, svlanFwdBaseOvidInfo->port, svlanFwdBaseOvidInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_fwd_base_ovid_set(unitChip.unit, svlanFwdBaseOvidInfo->port, svlanFwdBaseOvidInfo->enable);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get whether forwarding is based on outer VID or inner VID. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip             Stacking unit and chip ID (N/A) \n
 *                                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] svlanFwdBaseOvidInfo SVLAN forwarding based on outer VID information (N/A) \n
 *                                     Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The forwarding method was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_fwd_base_ovid_get(), dva_svlan_fwd_base_ovid_get(), cvb_svlan_fwd_base_ovid_get(), cva_svlan_fwd_base_ovid_get().
 */
RtkApiRet rtk_svlan_fwd_base_ovid_get(UnitChip_t unitChip, SvlanFwdBaseOvidInfo_t *svlanFwdBaseOvidInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanFwdBaseOvidInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_fwd_base_ovid_get(unitChip.unit, svlanFwdBaseOvidInfo->port, &svlanFwdBaseOvidInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_fwd_base_ovid_get(unitChip.unit, svlanFwdBaseOvidInfo->port, &svlanFwdBaseOvidInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_fwd_base_ovid_get(unitChip.unit, svlanFwdBaseOvidInfo->port, &svlanFwdBaseOvidInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_fwd_base_ovid_get(unitChip.unit, svlanFwdBaseOvidInfo->port, &svlanFwdBaseOvidInfo->enable);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*VLAN_TABLE_LOOKUP*/

/**
 * @defgroup INGRESS_VLAN_FILTER Ingress VLAN Filter
 * @brief APIs to configure IEEE 802.1Q ingress filter
 * @{
 */
/**
 * @brief Configure ingress VLAN filter for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip              Stacking unit and chip ID (N/A) \n
 *                                   Stacking unit ID is used to indicate which switch is to be accessed.
 *                                   Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanPortIgrFilterInfo VLAN port ingress filter information (N/A) \n
 *                                   Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The ingress VLAN filter ability of the port was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_igr_filter_en_set(), dva_vlan_port_igr_filter_en_set(), cvb_vlan_port_igr_filter_en_set(), cva_vlan_port_igr_filter_en_set().
 */
RtkApiRet rtk_vlan_port_igr_filter_en_set(UnitChip_t unitChip, VlanPortIgrFilterInfo_t *vlanPortIgrFilterInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortIgrFilterInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_igr_filter_en_set(unitChip.unit, vlanPortIgrFilterInfo->port, vlanPortIgrFilterInfo->igrFilter);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_igr_filter_en_set(unitChip.unit, vlanPortIgrFilterInfo->port, vlanPortIgrFilterInfo->igrFilter);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_igr_filter_en_set(unitChip.unit, vlanPortIgrFilterInfo->port, vlanPortIgrFilterInfo->igrFilter);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_igr_filter_en_set(unitChip.unit, vlanPortIgrFilterInfo->port, vlanPortIgrFilterInfo->igrFilter);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get the status of the ingress VLAN filter for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip              Stacking unit and chip ID (N/A) \n
 *                                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanPortIgrFilterInfo VLAN port ingress filter information (N/A) \n
 *                                      Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The ingress VLAN filter ability of the port was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_igr_filter_en_get(), dva_vlan_port_igr_filter_en_get(), cvb_vlan_port_igr_filter_en_get(), cva_vlan_port_igr_filter_en_get().
 */
RtkApiRet rtk_vlan_port_igr_filter_en_get(UnitChip_t unitChip, VlanPortIgrFilterInfo_t *vlanPortIgrFilterInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortIgrFilterInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_igr_filter_en_get(unitChip.unit, vlanPortIgrFilterInfo->port, &vlanPortIgrFilterInfo->igrFilter);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_igr_filter_en_get(unitChip.unit, vlanPortIgrFilterInfo->port, &vlanPortIgrFilterInfo->igrFilter);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_igr_filter_en_get(unitChip.unit, vlanPortIgrFilterInfo->port, &vlanPortIgrFilterInfo->igrFilter);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_igr_filter_en_get(unitChip.unit, vlanPortIgrFilterInfo->port, &vlanPortIgrFilterInfo->igrFilter);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*INGRESS_VLAN_FILTER*/

/**
 * @defgroup EGRESS_TAG_STATUS_DECISION Egress Tag Status Decision
 * @brief APIs to configure the VLAN egress tag status
 * @{
 */
/**
 * @brief Configure inner tag status for a specific egress port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                 Stacking unit and chip ID (N/A) \n
 *                                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanPortEgrTagStatusInfo VLAN port egress tag status information (N/A) \n
 *                                      Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The egress tag status was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_egr_tag_status_set(), dva_vlan_port_egr_tag_status_set(), cvb_vlan_port_egr_tag_status_set(), cva_vlan_port_egr_tag_status_set().
 */
RtkApiRet rtk_vlan_port_egr_tag_status_set(UnitChip_t unitChip, VlanPortEgrTagStatusInfo_t *vlanPortEgrTagStatusInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortEgrTagStatusInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_egr_tag_status_set(unitChip.unit, vlanPortEgrTagStatusInfo->port, vlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_egr_tag_status_set(unitChip.unit, vlanPortEgrTagStatusInfo->port, vlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_egr_tag_status_set(unitChip.unit, vlanPortEgrTagStatusInfo->port, vlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_egr_tag_status_set(unitChip.unit, vlanPortEgrTagStatusInfo->port, vlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get inner tag status for a specific egress port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                 Stacking unit and chip ID (N/A) \n
 *                                         Stacking unit ID is used to indicate which switch is to be accessed.
 *                                         Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanPortEgrTagStatusInfo VLAN port egress tag status information (N/A) \n
 *                                         Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The egress tag status was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_egr_tag_status_get(), dva_vlan_port_egr_tag_status_get(), cvb_vlan_port_egr_tag_status_get(), cva_vlan_port_egr_tag_status_get().
 */
RtkApiRet rtk_vlan_port_egr_tag_status_get(UnitChip_t unitChip, VlanPortEgrTagStatusInfo_t *vlanPortEgrTagStatusInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortEgrTagStatusInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_egr_tag_status_get(unitChip.unit, vlanPortEgrTagStatusInfo->port, &vlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_egr_tag_status_get(unitChip.unit, vlanPortEgrTagStatusInfo->port, &vlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_egr_tag_status_get(unitChip.unit, vlanPortEgrTagStatusInfo->port, &vlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_egr_tag_status_get(unitChip.unit, vlanPortEgrTagStatusInfo->port, &vlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Configure outer tag status for a specific egress port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                  Stacking unit and chip ID (N/A) \n
 *                                       Stacking unit ID is used to indicate which switch is to be accessed.
 *                                       Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  svlanPortEgrTagStatusInfo SVLAN port egress tag status information (N/A) \n
 *                                       Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The egress tag status was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_port_egr_tag_status_set(), dva_svlan_port_egr_tag_status_set(), cvb_svlan_port_egr_tag_status_set(), cva_svlan_port_egr_tag_status_set().
 */
RtkApiRet rtk_svlan_port_egr_tag_status_set(UnitChip_t unitChip, SvlanPortEgrTagStatusInfo_t *svlanPortEgrTagStatusInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanPortEgrTagStatusInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_port_egr_tag_status_set(unitChip.unit, svlanPortEgrTagStatusInfo->port, svlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_port_egr_tag_status_set(unitChip.unit, svlanPortEgrTagStatusInfo->port, svlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_port_egr_tag_status_set(unitChip.unit, svlanPortEgrTagStatusInfo->port, svlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_port_egr_tag_status_set(unitChip.unit, svlanPortEgrTagStatusInfo->port, svlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get outer tag status for a specific egress port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                  Stacking unit and chip ID (N/A) \n
 *                                          Stacking unit ID is used to indicate which switch is to be accessed.
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] svlanPortEgrTagStatusInfo SVLAN port egress tag status information (N/A) \n
 *                                          Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The egress tag status was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_svlan_port_egr_tag_status_get(), dva_svlan_port_egr_tag_status_get(), cvb_svlan_port_egr_tag_status_get(), cva_svlan_port_egr_tag_status_get().
 */
RtkApiRet rtk_svlan_port_egr_tag_status_get(UnitChip_t unitChip, SvlanPortEgrTagStatusInfo_t *svlanPortEgrTagStatusInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == svlanPortEgrTagStatusInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_svlan_port_egr_tag_status_get(unitChip.unit, svlanPortEgrTagStatusInfo->port, &svlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_svlan_port_egr_tag_status_get(unitChip.unit, svlanPortEgrTagStatusInfo->port, &svlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_svlan_port_egr_tag_status_get(unitChip.unit, svlanPortEgrTagStatusInfo->port, &svlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_svlan_port_egr_tag_status_get(unitChip.unit, svlanPortEgrTagStatusInfo->port, &svlanPortEgrTagStatusInfo->tagStatus);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*EGRESS_TAG_STATUS_DECISION*/

/**
 * @defgroup EGRESS_VLAN_FILTER Egress VLAN Filter
 * @brief APIs to configure IEEE 802.1Q egress filter
 * @{
 */
/**
 * @brief Configure egress VLAN filter for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip              Stacking unit and chip ID (N/A) \n
 *                                   Stacking unit ID is used to indicate which switch is to be accessed.
 *                                   Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanPortEgrFilterInfo VLAN port egress filter information (N/A) \n
 *                                   Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The egress VLAN filter ability of the port was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_egr_filter_en_set(), dva_vlan_port_egr_filter_en_set(), cvb_vlan_port_egr_filter_en_set(), cva_vlan_port_egr_filter_en_set().
 */
RtkApiRet rtk_vlan_port_egr_filter_en_set(UnitChip_t unitChip, VlanPortEgrFilterInfo_t *vlanPortEgrFilterInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortEgrFilterInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_egr_filter_en_set(unitChip.unit, vlanPortEgrFilterInfo->port, vlanPortEgrFilterInfo->egrFilter);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_egr_filter_en_set(unitChip.unit, vlanPortEgrFilterInfo->port, vlanPortEgrFilterInfo->egrFilter);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_egr_filter_en_set(unitChip.unit, vlanPortEgrFilterInfo->port, vlanPortEgrFilterInfo->egrFilter);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_egr_filter_en_set(unitChip.unit, vlanPortEgrFilterInfo->port, vlanPortEgrFilterInfo->egrFilter);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get egress VLAN filter status for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip              Stacking unit and chip ID (N/A) \n
 *                                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanPortEgrFilterInfo VLAN port egress filter information (N/A) \n
 *                                      Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The egress VLAN filter ability of the port was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_port_egr_filter_en_get(), dva_vlan_port_egr_filter_en_get(), cvb_vlan_port_egr_filter_en_get(), cva_vlan_port_egr_filter_en_get().
 */
RtkApiRet rtk_vlan_port_egr_filter_en_get(UnitChip_t unitChip, VlanPortEgrFilterInfo_t *vlanPortEgrFilterInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanPortEgrFilterInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_port_egr_filter_en_get(unitChip.unit, vlanPortEgrFilterInfo->port, &vlanPortEgrFilterInfo->egrFilter);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_port_egr_filter_en_get(unitChip.unit, vlanPortEgrFilterInfo->port, &vlanPortEgrFilterInfo->egrFilter);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_port_egr_filter_en_get(unitChip.unit, vlanPortEgrFilterInfo->port, &vlanPortEgrFilterInfo->egrFilter);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_port_egr_filter_en_get(unitChip.unit, vlanPortEgrFilterInfo->port, &vlanPortEgrFilterInfo->egrFilter);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*EGRESS_VLAN_FILTER*/

/**
 * @defgroup EGRESS_VLAN_RETAGGING Egress VLAN Retagging
 * @brief APIs to configure the VLAN egress retagging
 * @{
 */
/**
 * @brief Initialize egress VLAN retagging. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK           The egress VLAN retagging function of the ports was initialized. \n
 * @retval -RT_ERR_NOT_SUPPORT Not supported. \n
 * @retval Others              Please refer to dv1_vlan_egr_retagging_init(), dva_vlan_egr_retagging_init(), cvb_vlan_egr_retagging_init(), cva_vlan_egr_retagging_init().
 */
RtkApiRet rtk_vlan_egr_retagging_init(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_vlan_egr_retagging_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_vlan_egr_retagging_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_vlan_egr_retagging_init(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_vlan_egr_retagging_init(unitChip.unit);
            break;
#endif
        default:
            ret = RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

/**
 * @brief Set an egress VLAN retagging entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanEgrRetaggingInfo VLAN egress retagging information (N/A) \n
 *                                  Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The egress VLAN retagging entry was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_egr_retagging_set(), dva_vlan_egr_retagging_set(), cvb_vlan_egr_retagging_set(), cva_vlan_egr_retagging_set().
 */
RtkApiRet rtk_vlan_egr_retagging_set(UnitChip_t unitChip, VlanEgrRetaggingInfo_t *vlanEgrRetaggingInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanEgrRetaggingInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_egr_retagging_set(unitChip.unit, vlanEgrRetaggingInfo->idx, vlanEgrRetaggingInfo->originalVid, vlanEgrRetaggingInfo->newVid, vlanEgrRetaggingInfo->pbm, vlanEgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_egr_retagging_set(unitChip.unit, vlanEgrRetaggingInfo->idx, vlanEgrRetaggingInfo->originalVid, vlanEgrRetaggingInfo->newVid, vlanEgrRetaggingInfo->pbm, vlanEgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_egr_retagging_set(unitChip.unit, vlanEgrRetaggingInfo->idx, vlanEgrRetaggingInfo->originalVid, vlanEgrRetaggingInfo->newVid, vlanEgrRetaggingInfo->pbm, vlanEgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_egr_retagging_set(unitChip.unit, vlanEgrRetaggingInfo->idx, vlanEgrRetaggingInfo->originalVid, vlanEgrRetaggingInfo->newVid, vlanEgrRetaggingInfo->pbm, vlanEgrRetaggingInfo->valid);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get an egress VLAN retagging entry. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip             Stacking unit and chip ID (N/A) \n
 *                                     Stacking unit ID is used to indicate which switch is to be accessed.
 *                                     Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanEgrRetaggingInfo VLAN egress retagging information (N/A) \n
 *                                     Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The egress VLAN retagging entry was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_egr_retagging_get(), dva_vlan_egr_retagging_get(), cvb_vlan_egr_retagging_get(), cva_vlan_egr_retagging_get().
 */
RtkApiRet rtk_vlan_egr_retagging_get(UnitChip_t unitChip, VlanEgrRetaggingInfo_t *vlanEgrRetaggingInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanEgrRetaggingInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_egr_retagging_get(unitChip.unit, vlanEgrRetaggingInfo->idx, &vlanEgrRetaggingInfo->originalVid, &vlanEgrRetaggingInfo->newVid, &vlanEgrRetaggingInfo->pbm, &vlanEgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_egr_retagging_get(unitChip.unit, vlanEgrRetaggingInfo->idx, &vlanEgrRetaggingInfo->originalVid, &vlanEgrRetaggingInfo->newVid, &vlanEgrRetaggingInfo->pbm, &vlanEgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_egr_retagging_get(unitChip.unit, vlanEgrRetaggingInfo->idx, &vlanEgrRetaggingInfo->originalVid, &vlanEgrRetaggingInfo->newVid, &vlanEgrRetaggingInfo->pbm, &vlanEgrRetaggingInfo->valid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_egr_retagging_get(unitChip.unit, vlanEgrRetaggingInfo->idx, &vlanEgrRetaggingInfo->originalVid, &vlanEgrRetaggingInfo->newVid, &vlanEgrRetaggingInfo->pbm, &vlanEgrRetaggingInfo->valid);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Set egress VLAN retagging status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip               Stacking unit and chip ID (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  vlanEgrRetaggingEnInfo VLAN egress retagging status information (N/A) \n
 *                                    Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The egress VLAN retagging status was set. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_egr_retagging_en_set(), dva_vlan_egr_retagging_en_set(), cvb_vlan_egr_retagging_en_set(), cva_vlan_egr_retagging_en_set().
 */
RtkApiRet rtk_vlan_egr_retagging_en_set(UnitChip_t unitChip, VlanEgrRetaggingEnInfo_t *vlanEgrRetaggingEnInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanEgrRetaggingEnInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_egr_retagging_en_set(unitChip.unit, vlanEgrRetaggingEnInfo->port, vlanEgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_egr_retagging_en_set(unitChip.unit, vlanEgrRetaggingEnInfo->port, vlanEgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_egr_retagging_en_set(unitChip.unit, vlanEgrRetaggingEnInfo->port, vlanEgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_egr_retagging_en_set(unitChip.unit, vlanEgrRetaggingEnInfo->port, vlanEgrRetaggingEnInfo->enable);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Get egress VLAN retagging status. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip               Stacking unit and chip ID (N/A) \n
 *                                       Stacking unit ID is used to indicate which switch is to be accessed.
 *                                       Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] vlanEgrRetaggingEnInfo VLAN egress retagging status information (N/A) \n
 *                                       Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The egress VLAN retagging status was retrieved. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_vlan_egr_retagging_en_get(), dva_vlan_egr_retagging_en_get(), cvb_vlan_egr_retagging_en_get(), cva_vlan_egr_retagging_en_get().
 */
RtkApiRet rtk_vlan_egr_retagging_en_get(UnitChip_t unitChip, VlanEgrRetaggingEnInfo_t *vlanEgrRetaggingEnInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == vlanEgrRetaggingEnInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_vlan_egr_retagging_en_get(unitChip.unit, vlanEgrRetaggingEnInfo->port, &vlanEgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_vlan_egr_retagging_en_get(unitChip.unit, vlanEgrRetaggingEnInfo->port, &vlanEgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_vlan_egr_retagging_en_get(unitChip.unit, vlanEgrRetaggingEnInfo->port, &vlanEgrRetaggingEnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_vlan_egr_retagging_en_get(unitChip.unit, vlanEgrRetaggingEnInfo->port, &vlanEgrRetaggingEnInfo->enable);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*EGRESS_VLAN_RETAGGING*/
/**@}*/ /*VLAN*/
