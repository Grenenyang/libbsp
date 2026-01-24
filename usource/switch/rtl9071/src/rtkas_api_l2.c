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
#include "rtl907xd_v1_api_l2.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_l2.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_l2.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_l2.h"
#endif
#include "rtkas_api_l2.h"

/**
 * @defgroup RTK_MODULE_L2 L2
 * @brief L2 APIs. This component is used to configure the address lookup table that associates source MAC addresses with specific ports.
 * @{
 */
/**
 * @defgroup HASH_ALGORITHMS Hash Algorithms
 * @brief APIs to get hash algorithms for L2 table storage
 * @{
 */
/**
 * @brief Get the hash algorithm from the address table. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip        Stacking unit and chip ID (N/A) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 *                             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] lutHashAlgoInfo L2 hash algorithm information (N/A) \n
 *                             Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The hash algorithm was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_hash_algo_get(), dva_lut_hash_algo_get(), cvb_lut_hash_algo_get(), cva_lut_hash_algo_get().
 */
RtkApiRet rtk_lut_hash_algo_get(UnitChip_t unitChip, LutHashAlgoInfo_t *lutHashAlgoInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutHashAlgoInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_hash_algo_get(unitChip.unit, &lutHashAlgoInfo->algo);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_hash_algo_get(unitChip.unit, &lutHashAlgoInfo->algo);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_hash_algo_get(unitChip.unit, &lutHashAlgoInfo->algo);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_hash_algo_get(unitChip.unit, &lutHashAlgoInfo->algo);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*HASH_ALGORITHMS*/

/**
 * @defgroup ADDRESS_LOOKUP Destination MAC Address Lookup
 * @brief APIs to lookup destination MAC address and configure operation for mismatch
 * @{
 */
/**
 * @brief Get an L2 entry by table index. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip   Stacking unit and chip ID (N/A) \n
 *                           Stacking unit ID is used to indicate which switch is to be accessed.
 *                           Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutGetInfo L2 entry retrieval information (N/A) \n
 *                           Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The L2 entry was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_entry_get(), dva_lut_entry_get(), cvb_lut_entry_get(), cva_lut_entry_get().
 */
RtkApiRet rtk_lut_entry_get(UnitChip_t unitChip, LutGetInfo_t *lutGetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutGetInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_entry_get(unitChip.unit, lutGetInfo->index, &lutGetInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_entry_get(unitChip.unit, lutGetInfo->index, &lutGetInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_entry_get(unitChip.unit, lutGetInfo->index, &lutGetInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_entry_get(unitChip.unit, lutGetInfo->index, &lutGetInfo->lutEntry);
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
 * @brief Search an L2 entry by MAC address. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip      Stacking unit and chip ID (N/A) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 *                              Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutSearchInfo L2 entry search information (N/A) \n
 *                              Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The L2 entry was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_entry_search(), dva_lut_entry_search(), cvb_lut_entry_search(), cva_lut_entry_search().
 */
RtkApiRet rtk_lut_entry_search(UnitChip_t unitChip, LutSearchInfo_t *lutSearchInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutSearchInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_entry_search(unitChip.unit, lutSearchInfo->macAddr, &lutSearchInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_entry_search(unitChip.unit, lutSearchInfo->macAddr, &lutSearchInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_entry_search(unitChip.unit, lutSearchInfo->macAddr, &lutSearchInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_entry_search(unitChip.unit, lutSearchInfo->macAddr, &lutSearchInfo->lutEntry);
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
 * @brief Search for a multicast entry by MAC and FID. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip        Stacking unit and chip ID (N/A) \n
 *                                Stacking unit ID is used to indicate which switch is to be accessed.
 *                                Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutMultiGetInfo L2 multicast entry retrieval information (N/A) \n
 *                                Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The L2 entry was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_multi_entry_get(), dva_lut_multi_entry_get(), cvb_lut_multi_entry_get(), cva_lut_multi_entry_get().
 */
RtkApiRet rtk_lut_multi_entry_get(UnitChip_t unitChip, LutMultiGetInfo_t *lutMultiGetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutMultiGetInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_multi_entry_get(unitChip.unit, lutMultiGetInfo->macAddr, lutMultiGetInfo->fid, &lutMultiGetInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_multi_entry_get(unitChip.unit, lutMultiGetInfo->macAddr, lutMultiGetInfo->fid, &lutMultiGetInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_multi_entry_get(unitChip.unit, lutMultiGetInfo->macAddr, lutMultiGetInfo->fid, &lutMultiGetInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_multi_entry_get(unitChip.unit, lutMultiGetInfo->macAddr, lutMultiGetInfo->fid, &lutMultiGetInfo->lutEntry);
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
 * @brief Search for an IP multicast entry by SIP and DIP. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip       Stacking unit and chip ID (N/A) \n
 *                               Stacking unit ID is used to indicate which switch is to be accessed.
 *                               Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutIpMultiInfo L2 IP multicast entry information (N/A) \n
 *                               Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The L2 entry was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_ip_multi_entry_get(), dva_lut_ip_multi_entry_get(), cvb_lut_ip_multi_entry_get(), cva_lut_ip_multi_entry_get().
 */
RtkApiRet rtk_lut_ip_multi_entry_get(UnitChip_t unitChip, LutIpMultiInfo_t *lutIpMultiInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutIpMultiInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_ip_multi_entry_get(unitChip.unit, lutIpMultiInfo->sip, lutIpMultiInfo->dip, &lutIpMultiInfo->portmask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_ip_multi_entry_get(unitChip.unit, lutIpMultiInfo->sip, lutIpMultiInfo->dip, &lutIpMultiInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_ip_multi_entry_get(unitChip.unit, lutIpMultiInfo->sip, lutIpMultiInfo->dip, &lutIpMultiInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_ip_multi_entry_get(unitChip.unit, lutIpMultiInfo->sip, lutIpMultiInfo->dip, &lutIpMultiInfo->portmask);
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
 * @brief Get an L2 entry from the BCAM table by BCAM index. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip      Stacking unit and chip ID (N/A) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 *                              Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutCamGetInfo L2 BCAM entry retrieval information (N/A) \n
 *                              Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The L2 entry was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_cam_entry_get(), dva_lut_cam_entry_get(), cvb_lut_cam_entry_get(), cva_lut_cam_entry_get().
 */
RtkApiRet rtk_lut_cam_entry_get(UnitChip_t unitChip, LutCamGetInfo_t *lutCamGetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutCamGetInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_cam_entry_get(unitChip.unit, lutCamGetInfo->index, &lutCamGetInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_cam_entry_get(unitChip.unit, lutCamGetInfo->index, &lutCamGetInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_cam_entry_get(unitChip.unit, lutCamGetInfo->index, &lutCamGetInfo->lutEntry);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_cam_entry_get(unitChip.unit, lutCamGetInfo->index, &lutCamGetInfo->lutEntry);
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
 * @brief Control the action when address table lookup misses. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip              Stacking unit and chip ID (N/A) \n
 *                                   Stacking unit ID is used to indicate which switch is to be accessed.
 *                                   Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutLookupMissCtrlInfo L2 lookup miss control information (N/A) \n
 *                                   Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The lookup miss control was set successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_lookup_miss_ctrl(), dva_lut_lookup_miss_ctrl(), cvb_lut_lookup_miss_ctrl(), cva_lut_lookup_miss_ctrl().
 */
RtkApiRet rtk_lut_lookup_miss_ctrl(UnitChip_t unitChip, LutLookupMissCtrlInfo_t *lutLookupMissCtrlInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutLookupMissCtrlInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_lookup_miss_ctrl(unitChip.unit, lutLookupMissCtrlInfo->port, lutLookupMissCtrlInfo->type, lutLookupMissCtrlInfo->action);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_lookup_miss_ctrl(unitChip.unit, lutLookupMissCtrlInfo->port, lutLookupMissCtrlInfo->type, lutLookupMissCtrlInfo->action);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_lookup_miss_ctrl(unitChip.unit, lutLookupMissCtrlInfo->port, lutLookupMissCtrlInfo->type, lutLookupMissCtrlInfo->action);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_lookup_miss_ctrl(unitChip.unit, lutLookupMissCtrlInfo->port, lutLookupMissCtrlInfo->type, lutLookupMissCtrlInfo->action);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*ADDRESS_LOOKUP*/

/**
 * @defgroup ADDRESS_LEARNING Source MAC Address Learning
 * @brief APIs to configure the learning of MAC addresses to the L2 table or the BCAM table. \n
 *        If there is an entry match the received packet, the switch will check whether the source port address field of the entry is also the same as the packet incoming port. If the source port is different, the status is called "SA move".
 * @{
 */
/**
 * @brief Enable MAC learning for address table lookup. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip      Stacking unit and chip ID (N/A) \n
 *                           Stacking unit ID is used to indicate which switch is to be accessed.
 *                           Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutMacLrnInfo L2 MAC learning information (N/A) \n
 *                           Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            MAC learning was set successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_mac_lrn_en(), dva_lut_mac_lrn_en(), cvb_lut_mac_lrn_en(), cva_lut_mac_lrn_en().
 */
RtkApiRet rtk_lut_mac_lrn_en(UnitChip_t unitChip, LutMacLrnInfo_t *lutMacLrnInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutMacLrnInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_mac_lrn_en(unitChip.unit, lutMacLrnInfo->portmask, lutMacLrnInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_mac_lrn_en(unitChip.unit, lutMacLrnInfo->portmask, lutMacLrnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_mac_lrn_en(unitChip.unit, lutMacLrnInfo->portmask, lutMacLrnInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_mac_lrn_en(unitChip.unit, lutMacLrnInfo->portmask, lutMacLrnInfo->enable);
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
 * @brief Add a static unicast entry to the lookup address table. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip      Stacking unit and chip ID (N/A) \n
 *                           Stacking unit ID is used to indicate which switch is to be accessed.
 *                           Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutUniAddInfo L2 unicast add information (N/A) \n
 *                           Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The entry was set successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_uni_entry_add(), dva_lut_uni_entry_add(), cvb_lut_uni_entry_add(), cva_lut_uni_entry_add().
 */
RtkApiRet rtk_lut_uni_entry_add(UnitChip_t unitChip, LutUniAddInfo_t *lutUniAddInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutUniAddInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_uni_entry_add(unitChip.unit, lutUniAddInfo->mac, lutUniAddInfo->vid, lutUniAddInfo->port, lutUniAddInfo->fid, lutUniAddInfo->ulfid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_uni_entry_add(unitChip.unit, lutUniAddInfo->mac, lutUniAddInfo->vid, lutUniAddInfo->port, lutUniAddInfo->fid, lutUniAddInfo->ulfid, lutUniAddInfo->nh);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_uni_entry_add(unitChip.unit, lutUniAddInfo->mac, lutUniAddInfo->vid, lutUniAddInfo->port, lutUniAddInfo->fid, lutUniAddInfo->ulfid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_uni_entry_add(unitChip.unit, lutUniAddInfo->mac, lutUniAddInfo->vid, lutUniAddInfo->port, lutUniAddInfo->fid, lutUniAddInfo->ulfid);
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
 * @brief Add a multicast entry to the lookup address table. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip        Stacking unit and chip ID (N/A) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 *                             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutMultiAddInfo L2 multicast add information (N/A) \n
 *                             Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The entry was set successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_multi_entry_add(), dva_lut_multi_entry_add(), cvb_lut_multi_entry_add(), cva_lut_multi_entry_add().
 */
RtkApiRet rtk_lut_multi_entry_add(UnitChip_t unitChip, LutMultiAddInfo_t *lutMultiAddInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutMultiAddInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_multi_entry_add(unitChip.unit, lutMultiAddInfo->mac, lutMultiAddInfo->vid, lutMultiAddInfo->portmask, lutMultiAddInfo->fid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_multi_entry_add(unitChip.unit, lutMultiAddInfo->mac, lutMultiAddInfo->vid, lutMultiAddInfo->portmask, lutMultiAddInfo->fid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_multi_entry_add(unitChip.unit, lutMultiAddInfo->mac, lutMultiAddInfo->vid, lutMultiAddInfo->portmask, lutMultiAddInfo->fid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_multi_entry_add(unitChip.unit, lutMultiAddInfo->mac, lutMultiAddInfo->vid, lutMultiAddInfo->portmask, lutMultiAddInfo->fid);
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
 * @brief Add an IP multicast entry to the lookup address table. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip       Stacking unit and chip ID (N/A) \n
 *                            Stacking unit ID is used to indicate which switch is to be accessed.
 *                            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutIpMultiInfo L2 IP multicast information (N/A) \n
 *                            Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The entry was set successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_ip_multi_entry_add(), dva_lut_ip_multi_entry_add(), cvb_lut_ip_multi_entry_add(), cva_lut_ip_multi_entry_add().
 */
RtkApiRet rtk_lut_ip_multi_entry_add(UnitChip_t unitChip, LutIpMultiInfo_t *lutIpMultiInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutIpMultiInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_ip_multi_entry_add(unitChip.unit, lutIpMultiInfo->sip, lutIpMultiInfo->dip, lutIpMultiInfo->portmask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_ip_multi_entry_add(unitChip.unit, lutIpMultiInfo->sip, lutIpMultiInfo->dip, lutIpMultiInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_ip_multi_entry_add(unitChip.unit, lutIpMultiInfo->sip, lutIpMultiInfo->dip, lutIpMultiInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_ip_multi_entry_add(unitChip.unit, lutIpMultiInfo->sip, lutIpMultiInfo->dip, lutIpMultiInfo->portmask);
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
 * @brief Delete an entry from the lookup address table. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip      Stacking unit and chip ID (N/A) \n
 *                           Stacking unit ID is used to indicate which switch is to be accessed.
 *                           Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutDeleteInfo L2 entry delete information (N/A) \n
 *                           Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The entry was deleted successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_entry_delete(), dva_lut_entry_delete(), cvb_lut_entry_delete(), cva_lut_entry_delete().
 */
RtkApiRet rtk_lut_entry_delete(UnitChip_t unitChip, LutDeleteInfo_t *lutDeleteInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutDeleteInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_entry_delete(unitChip.unit, lutDeleteInfo->mac, lutDeleteInfo->vid, lutDeleteInfo->fid, lutDeleteInfo->ulfid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_entry_delete(unitChip.unit, lutDeleteInfo->mac, lutDeleteInfo->vid, lutDeleteInfo->fid, lutDeleteInfo->ulfid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_entry_delete(unitChip.unit, lutDeleteInfo->mac, lutDeleteInfo->vid, lutDeleteInfo->fid, lutDeleteInfo->ulfid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_entry_delete(unitChip.unit, lutDeleteInfo->mac, lutDeleteInfo->vid, lutDeleteInfo->fid, lutDeleteInfo->ulfid);
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
 * @brief Delete an IP multicast entry from the address table. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutIpMultiDeleteInfo L2 IP multicast entry delete information (N/A) \n
 *                                  Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The entry was deleted successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_ip_multi_entry_delete(), dva_lut_ip_multi_entry_delete(), cvb_lut_ip_multi_entry_delete(), cva_lut_ip_multi_entry_delete().
 */
RtkApiRet rtk_lut_ip_multi_entry_delete(UnitChip_t unitChip, LutIpMultiDeleteInfo_t *lutIpMultiDeleteInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutIpMultiDeleteInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_ip_multi_entry_delete(unitChip.unit, lutIpMultiDeleteInfo->sip, lutIpMultiDeleteInfo->dip);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_ip_multi_entry_delete(unitChip.unit, lutIpMultiDeleteInfo->sip, lutIpMultiDeleteInfo->dip);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_ip_multi_entry_delete(unitChip.unit, lutIpMultiDeleteInfo->sip, lutIpMultiDeleteInfo->dip);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_ip_multi_entry_delete(unitChip.unit, lutIpMultiDeleteInfo->sip, lutIpMultiDeleteInfo->dip);
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
 * @brief Get the SA move legal ports of the specified port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip               Stacking unit and chip ID (N/A) \n
 *                                       Stacking unit ID is used to indicate which switch is to be accessed.
 *                                       Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutSaMoveLegalPortInfo L2 SA move legal port information (N/A) \n
 *                                       Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The SA move legal ports were retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_sa_move_legal_port_get(), dva_lut_sa_move_legal_port_get(), cvb_lut_sa_move_legal_port_get(), cva_lut_sa_move_legal_port_get().
 */
RtkApiRet rtk_lut_sa_move_legal_port_get(UnitChip_t unitChip, LutSaMoveLegalPortInfo_t *lutSaMoveLegalPortInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutSaMoveLegalPortInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_sa_move_legal_port_get(unitChip.unit, lutSaMoveLegalPortInfo->port, &lutSaMoveLegalPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_sa_move_legal_port_get(unitChip.unit, lutSaMoveLegalPortInfo->port, &lutSaMoveLegalPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_sa_move_legal_port_get(unitChip.unit, lutSaMoveLegalPortInfo->port, &lutSaMoveLegalPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_sa_move_legal_port_get(unitChip.unit, lutSaMoveLegalPortInfo->port, &lutSaMoveLegalPortInfo->portmask);
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
 * @brief Set the SA move legal ports of the specified port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip               Stacking unit and chip ID (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutSaMoveLegalPortInfo L2 SA move legal port information (N/A) \n
 *                                    Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The SA move legal ports were set successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_sa_move_legal_port_set(), dva_lut_sa_move_legal_port_set(), cvb_lut_sa_move_legal_port_set(), cva_lut_sa_move_legal_port_set().
 */
RtkApiRet rtk_lut_sa_move_legal_port_set(UnitChip_t unitChip, LutSaMoveLegalPortInfo_t *lutSaMoveLegalPortInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutSaMoveLegalPortInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_sa_move_legal_port_set(unitChip.unit, lutSaMoveLegalPortInfo->port, lutSaMoveLegalPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_sa_move_legal_port_set(unitChip.unit, lutSaMoveLegalPortInfo->port, lutSaMoveLegalPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_sa_move_legal_port_set(unitChip.unit, lutSaMoveLegalPortInfo->port, lutSaMoveLegalPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_sa_move_legal_port_set(unitChip.unit, lutSaMoveLegalPortInfo->port, lutSaMoveLegalPortInfo->portmask);
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
 * @brief Get the SA move legal port action of the specified port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                  Stacking unit and chip ID (N/A) \n
 *                                          Stacking unit ID is used to indicate which switch is to be accessed.
 *                                          Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutSaMoveLegalPortActInfo L2 SA move legal port action information (N/A) \n
 *                                          Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The SA move legal port action was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_sa_move_legal_port_get_act(), dva_lut_sa_move_legal_port_get_act(), cvb_lut_sa_move_legal_port_get_act(), cva_lut_sa_move_legal_port_get_act().
 */
RtkApiRet rtk_lut_sa_move_legal_port_get_act(UnitChip_t unitChip, LutSaMoveLegalPortActInfo_t *lutSaMoveLegalPortActInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutSaMoveLegalPortActInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_sa_move_legal_port_get_act(unitChip.unit, lutSaMoveLegalPortActInfo->port, &lutSaMoveLegalPortActInfo->legalPortAct);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_sa_move_legal_port_get_act(unitChip.unit, lutSaMoveLegalPortActInfo->port, &lutSaMoveLegalPortActInfo->legalPortAct);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_sa_move_legal_port_get_act(unitChip.unit, lutSaMoveLegalPortActInfo->port, &lutSaMoveLegalPortActInfo->legalPortAct);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_sa_move_legal_port_get_act(unitChip.unit, lutSaMoveLegalPortActInfo->port, &lutSaMoveLegalPortActInfo->legalPortAct);
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
 * @brief Set the SA move legal port action of the specified port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                  Stacking unit and chip ID (N/A) \n
 *                                       Stacking unit ID is used to indicate which switch is to be accessed.
 *                                       Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutSaMoveLegalPortActInfo L2 SA move legal port action information (N/A) \n
 *                                       Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The SA move legal port action was set successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_sa_move_legal_port_set_act(), dva_lut_sa_move_legal_port_set_act(), cvb_lut_sa_move_legal_port_set_act(), cva_lut_sa_move_legal_port_set_act().
 */
RtkApiRet rtk_lut_sa_move_legal_port_set_act(UnitChip_t unitChip, LutSaMoveLegalPortActInfo_t *lutSaMoveLegalPortActInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutSaMoveLegalPortActInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_sa_move_legal_port_set_act(unitChip.unit, lutSaMoveLegalPortActInfo->port, lutSaMoveLegalPortActInfo->legalPortAct);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_sa_move_legal_port_set_act(unitChip.unit, lutSaMoveLegalPortActInfo->port, lutSaMoveLegalPortActInfo->legalPortAct);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_sa_move_legal_port_set_act(unitChip.unit, lutSaMoveLegalPortActInfo->port, lutSaMoveLegalPortActInfo->legalPortAct);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_sa_move_legal_port_set_act(unitChip.unit, lutSaMoveLegalPortActInfo->port, lutSaMoveLegalPortActInfo->legalPortAct);
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
 * @brief Get the SA move illegal port action of the specified port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                    Stacking unit and chip ID (N/A) \n
 *                                            Stacking unit ID is used to indicate which switch is to be accessed.
 *                                            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutSaMoveIllegalPortActInfo L2 SA move illegal port action information (N/A) \n
 *                                            Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The SA move illegal port action was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_sa_move_illegal_port_get_act(), dva_lut_sa_move_illegal_port_get_act(), cvb_lut_sa_move_illegal_port_get_act(), cva_lut_sa_move_illegal_port_get_act().
 */
RtkApiRet rtk_lut_sa_move_illegal_port_get_act(UnitChip_t unitChip, LutSaMoveIllegalPortActInfo_t *lutSaMoveIllegalPortActInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutSaMoveIllegalPortActInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_sa_move_illegal_port_get_act(unitChip.unit, lutSaMoveIllegalPortActInfo->port, &lutSaMoveIllegalPortActInfo->illegalPortAct);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_sa_move_illegal_port_get_act(unitChip.unit, lutSaMoveIllegalPortActInfo->port, &lutSaMoveIllegalPortActInfo->illegalPortAct);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_sa_move_illegal_port_get_act(unitChip.unit, lutSaMoveIllegalPortActInfo->port, &lutSaMoveIllegalPortActInfo->illegalPortAct);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_sa_move_illegal_port_get_act(unitChip.unit, lutSaMoveIllegalPortActInfo->port, &lutSaMoveIllegalPortActInfo->illegalPortAct);
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
 * @brief Set the SA move illegal port action of the specified port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                    Stacking unit and chip ID (N/A) \n
 *                                         Stacking unit ID is used to indicate which switch is to be accessed.
 *                                         Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutSaMoveIllegalPortActInfo L2 SA move illegal port action information (N/A) \n
 *                                         Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The SA move illegal port action was set successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_sa_move_illegal_port_set_act(), dva_lut_sa_move_illegal_port_set_act(), cvb_lut_sa_move_illegal_port_set_act(), cva_lut_sa_move_illegal_port_set_act().
 */
RtkApiRet rtk_lut_sa_move_illegal_port_set_act(UnitChip_t unitChip, LutSaMoveIllegalPortActInfo_t *lutSaMoveIllegalPortActInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutSaMoveIllegalPortActInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_sa_move_illegal_port_set_act(unitChip.unit, lutSaMoveIllegalPortActInfo->port, lutSaMoveIllegalPortActInfo->illegalPortAct);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_sa_move_illegal_port_set_act(unitChip.unit, lutSaMoveIllegalPortActInfo->port, lutSaMoveIllegalPortActInfo->illegalPortAct);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_sa_move_illegal_port_set_act(unitChip.unit, lutSaMoveIllegalPortActInfo->port, lutSaMoveIllegalPortActInfo->illegalPortAct);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_sa_move_illegal_port_set_act(unitChip.unit, lutSaMoveIllegalPortActInfo->port, lutSaMoveIllegalPortActInfo->illegalPortAct);
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
 * @brief Get the SA move CPU ports. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] lutSaMoveCpuPortInfo L2 SA move CPU port information (N/A) \n
 *                                  Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The SA move CPU ports were retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_sa_move_cpu_port_get(), dva_lut_sa_move_cpu_port_get(), cvb_lut_sa_move_cpu_port_get(), cva_lut_sa_move_cpu_port_get().
 */
RtkApiRet rtk_lut_sa_move_cpu_port_get(UnitChip_t unitChip, LutSaMoveCpuPortInfo_t *lutSaMoveCpuPortInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutSaMoveCpuPortInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_sa_move_cpu_port_get(unitChip.unit, &lutSaMoveCpuPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_sa_move_cpu_port_get(unitChip.unit, &lutSaMoveCpuPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_sa_move_cpu_port_get(unitChip.unit, &lutSaMoveCpuPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_sa_move_cpu_port_get(unitChip.unit, &lutSaMoveCpuPortInfo->portmask);
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
 * @brief Set the SA move CPU ports. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip             Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 *                                  Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutSaMoveCpuPortInfo L2 SA move CPU port information (N/A) \n
 *                                  Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            The SA move CPU ports were set successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_sa_move_cpu_port_set(), dva_lut_sa_move_cpu_port_set(), cvb_lut_sa_move_cpu_port_set(), cva_lut_sa_move_cpu_port_set().
 */
RtkApiRet rtk_lut_sa_move_cpu_port_set(UnitChip_t unitChip, LutSaMoveCpuPortInfo_t *lutSaMoveCpuPortInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutSaMoveCpuPortInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_sa_move_cpu_port_set(unitChip.unit, lutSaMoveCpuPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_sa_move_cpu_port_set(unitChip.unit, lutSaMoveCpuPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_sa_move_cpu_port_set(unitChip.unit, lutSaMoveCpuPortInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_sa_move_cpu_port_set(unitChip.unit, lutSaMoveCpuPortInfo->portmask);
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
 * @brief Enable/Disable the legal SA move record for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                 Stacking unit and chip ID (N/A) \n
 *                                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutLegalSaMoveRecordInfo L2 legal SA move record information (N/A) \n
 *                                      Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            Enabled/Disabled the legal SA move record of the specific port successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_legal_sa_move_record_en(), dva_lut_legal_sa_move_record_en(), cvb_lut_legal_sa_move_record_en(), cva_lut_legal_sa_move_record_en().
 */
RtkApiRet rtk_lut_legal_sa_move_record_en(UnitChip_t unitChip, LutLegalSaMoveRecordInfo_t *lutLegalSaMoveRecordInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutLegalSaMoveRecordInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_legal_sa_move_record_en(unitChip.unit, lutLegalSaMoveRecordInfo->port, lutLegalSaMoveRecordInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_legal_sa_move_record_en(unitChip.unit, lutLegalSaMoveRecordInfo->port, lutLegalSaMoveRecordInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_legal_sa_move_record_en(unitChip.unit, lutLegalSaMoveRecordInfo->port, lutLegalSaMoveRecordInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_legal_sa_move_record_en(unitChip.unit, lutLegalSaMoveRecordInfo->port, lutLegalSaMoveRecordInfo->enable);
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
 * @brief Enable/Disable the legal SA move record for a specific port. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip                 Stacking unit and chip ID (N/A) \n
 *                                      Stacking unit ID is used to indicate which switch is to be accessed.
 *                                      Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  lutLegalSaMoveRecordInfo L2 legal SA move record information (N/A) \n
 *                                      Please refer to the struct field for detailed content.
 * @param[out] None
 * @retval RT_ERR_OK            Enabled/Disabled the legal SA move record for the specific port successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_legal_sa_move_record_en(), dva_lut_legal_sa_move_record_en(), cvb_lut_legal_sa_move_record_en(), cva_lut_legal_sa_move_record_en().
 */
RtkApiRet rtk_lut_illegal_sa_move_record_en(UnitChip_t unitChip, LutIllegalSaMoveRecordInfo_t *lutIllegalSaMoveRecordInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutIllegalSaMoveRecordInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_illegal_sa_move_record_en(unitChip.unit, lutIllegalSaMoveRecordInfo->port, lutIllegalSaMoveRecordInfo->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_illegal_sa_move_record_en(unitChip.unit, lutIllegalSaMoveRecordInfo->port, lutIllegalSaMoveRecordInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_illegal_sa_move_record_en(unitChip.unit, lutIllegalSaMoveRecordInfo->port, lutIllegalSaMoveRecordInfo->enable);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_illegal_sa_move_record_en(unitChip.unit, lutIllegalSaMoveRecordInfo->port, lutIllegalSaMoveRecordInfo->enable);
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
 * @brief Get the legal SA move record. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                    Stacking unit and chip ID (N/A) \n
 *                                            Stacking unit ID is used to indicate which switch is to be accessed.
 *                                            Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutLegalSaMoveRecordGetInfo L2 legal SA move record retrieval information (N/A) \n
 *                                            Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The legal SA move record was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_legal_sa_move_get_record(), dva_lut_legal_sa_move_get_record(), cvb_lut_legal_sa_move_get_record(), cva_lut_legal_sa_move_get_record().
 */
RtkApiRet rtk_lut_legal_sa_move_get_record(UnitChip_t unitChip, LutLegalSaMoveRecordGetInfo_t *lutLegalSaMoveRecordGetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutLegalSaMoveRecordGetInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_legal_sa_move_get_record(unitChip.unit, lutLegalSaMoveRecordGetInfo->idx, &lutLegalSaMoveRecordGetInfo->moveRecord);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_legal_sa_move_get_record(unitChip.unit, lutLegalSaMoveRecordGetInfo->idx, &lutLegalSaMoveRecordGetInfo->moveRecord);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_legal_sa_move_get_record(unitChip.unit, lutLegalSaMoveRecordGetInfo->idx, &lutLegalSaMoveRecordGetInfo->moveRecord);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_legal_sa_move_get_record(unitChip.unit, lutLegalSaMoveRecordGetInfo->idx, &lutLegalSaMoveRecordGetInfo->moveRecord);
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
 * @brief Get the illegal SA move record. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]     unitChip                      Stacking unit and chip ID (N/A) \n
 *                                              Stacking unit ID is used to indicate which switch is to be accessed.
 *                                              Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] lutIllegalSaMoveRecordGetInfo L2 illegal SA move record retrieval information (N/A) \n
 *                                              Please refer to the struct field for detailed content.
 * @retval RT_ERR_OK            The illegal SA move record was retrieved successfully. \n
 * @retval -RT_ERR_NULL_POINTER NULL parameter. \n
 * @retval -RT_ERR_NOT_SUPPORT  Not supported. \n
 * @retval Others               Please refer to dv1_lut_illegal_sa_move_get_record(), dva_lut_illegal_sa_move_get_record(), cvb_lut_illegal_sa_move_get_record(), cva_lut_illegal_sa_move_get_record().
 */
RtkApiRet rtk_lut_illegal_sa_move_get_record(UnitChip_t unitChip, LutIllegalSaMoveRecordGetInfo_t *lutIllegalSaMoveRecordGetInfo)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == lutIllegalSaMoveRecordGetInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_lut_illegal_sa_move_get_record(unitChip.unit, lutIllegalSaMoveRecordGetInfo->idx, &lutIllegalSaMoveRecordGetInfo->moveRecord);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_lut_illegal_sa_move_get_record(unitChip.unit, lutIllegalSaMoveRecordGetInfo->idx, &lutIllegalSaMoveRecordGetInfo->moveRecord);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_lut_illegal_sa_move_get_record(unitChip.unit, lutIllegalSaMoveRecordGetInfo->idx, &lutIllegalSaMoveRecordGetInfo->moveRecord);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_lut_illegal_sa_move_get_record(unitChip.unit, lutIllegalSaMoveRecordGetInfo->idx, &lutIllegalSaMoveRecordGetInfo->moveRecord);
                break;
#endif
            default:
                ret = RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*ADDRESS_LEARNING*/
/**@}*/ /*L2*/
