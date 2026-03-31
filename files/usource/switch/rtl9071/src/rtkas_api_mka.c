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

#include "rtkas_api_mka.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_mka.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_mka.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_mka.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_mka.h"
#endif

/**
 * @defgroup RTK_MODULE_MKA MKA
 * @brief MKA APIs. This component is used for control MKA (MACsec Key Agreement) software protocol parameters.
 * @{
 */

/**
 * @defgroup MKA_STATUS MKA Global Status
 * @brief Retrieve or configure the MKA status
 * @{
 */

/**
 * @brief Get the administrative status of MKA. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * This API retrieves the administrative status of MKA for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] mkaStatus              Pointer to MKA status information. \n
 *                                    Stores the retrieved MKA status.
 *
 * @retval RT_ERR_OK                  Successfully retrieved MKA administrative status.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_get_status(UnitChip_t unitChip, MkaStatus_t *mkaStatus)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaStatus)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_get_status(unitChip.unit, &mkaStatus->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_get_status(unitChip.unit, &mkaStatus->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_get_status(unitChip.unit, &mkaStatus->enable);
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
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief Set the administrative status of MKA. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * This API sets the administrative status of MKA for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  mkaStatus              Pointer to MKA status information. (N/A) \n
 *                                    Contains the MKA status to be set.
 * @param[out] None
 *
 * @retval RT_ERR_OK                  Successfully set MKA administrative status.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_set_status(UnitChip_t unitChip, const MkaStatus_t *mkaStatus)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaStatus)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_set_status(unitChip.unit, mkaStatus->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_set_status(unitChip.unit, mkaStatus->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_set_status(unitChip.unit, mkaStatus->enable);
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
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/** @} */ /* MKA_STATUS */

/**
 * @defgroup MKA_FORCE_LIVE_PEER MKA Force Live Peer
 * @brief Retrieve or configure the status of Realtek's force-peer-promotion enhancement
 * @{
 */

/**
 * @brief Retrieve the status of Realtek's force-peer-promotion enhancement. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * This API retrieves the status of Realtek's force-peer-promotion enhancement for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] mkaForceLivePeer       Pointer to force live peer information. (N/A) \n
 *                                    Stores the retrieved force live peer status.
 *
 * @retval RT_ERR_OK                  Successfully retrieved the status of force-peer-promotion enhancement.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_get_force_live_peer(UnitChip_t unitChip, MkaForceLivePeer_t *mkaForceLivePeer)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaForceLivePeer)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_get_force_live_peer(unitChip.unit, &mkaForceLivePeer->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_get_force_live_peer(unitChip.unit, &mkaForceLivePeer->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_get_force_live_peer(unitChip.unit, &mkaForceLivePeer->enable);
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
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief Set the status of Realtek's force-peer-promotion enhancement. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * This API sets the status of Realtek's force-peer-promotion enhancement for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  mkaForceLivePeer       Pointer to force live peer information. (N/A) \n
 *                                    Contains the force live peer status to be set.
 * @param[out] None
 *
 * @retval RT_ERR_OK                  Successfully set the status of force-peer-promotion enhancement.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_set_force_live_peer(UnitChip_t unitChip, const MkaForceLivePeer_t *mkaForceLivePeer)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaForceLivePeer)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_set_force_live_peer(unitChip.unit, mkaForceLivePeer->enable);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_set_force_live_peer(unitChip.unit, mkaForceLivePeer->enable);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_set_force_live_peer(unitChip.unit, mkaForceLivePeer->enable);
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
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/** @} */ /* MKA_FORCE_LIVE_PEER */

/**
 * @defgroup MKA_VERSION MKA Version
 * @brief Retrieve or configure te MKA version
 * @{
 */

/**
 * @brief Retrieve the MKA version. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * This API retrieves the MKA protocol version for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] mkaVersion             Pointer to MKA version information. (N/A) \n
 *                                    Stores the retrieved MKA version.
 *
 * @retval RT_ERR_OK                  Successfully retrieved MKA protocol version.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_get_version(UnitChip_t unitChip, MkaVersion_t *mkaVersion)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaVersion)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_get_version(unitChip.unit, &mkaVersion->version);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_get_version(unitChip.unit, &mkaVersion->version);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_get_version(unitChip.unit, &mkaVersion->version);
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
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief Set the MKA version. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * This API sets the MKA protocol version for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  mkaVersion             Pointer to MKA version information. (N/A) \n
 *                                    Contains the MKA version to be set.
 * @param[out] None
 *
 * @retval RT_ERR_OK                  Successfully set MKA protocol version.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_set_version(UnitChip_t unitChip, const MkaVersion_t *mkaVersion)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaVersion)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_set_version(unitChip.unit, mkaVersion->version);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_set_version(unitChip.unit, mkaVersion->version);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_set_version(unitChip.unit, mkaVersion->version);
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
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/** @} */ /* MKA_VERSION */

/**
 * @defgroup MKA_CAK_SOURCE MKA CAK Source
 * @brief Retrieve or configure the MKA CAK source type
 * @{
 */

/**
 * @brief Retrieve the MKA CAK source type. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * This API retrieves the CAK source type for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] mkaCakSource           Pointer to CAK source information. (N/A) \n
 *                                    Stores the retrieved CAK source type.
 *
 * @retval RT_ERR_OK                  Successfully retrieved the CAK source type.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_get_cak_source(UnitChip_t unitChip, MkaCakSource_t *mkaCakSource)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaCakSource)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_get_cak_source(unitChip.unit, &mkaCakSource->source, &mkaCakSource->unwrapIdx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_get_cak_source(unitChip.unit, &mkaCakSource->source, &mkaCakSource->unwrapIdx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_get_cak_source(unitChip.unit, &mkaCakSource->source, &mkaCakSource->unwrapIdx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mka_get_cak_source(unitChip.unit, &mkaCakSource->source, &mkaCakSource->unwrapIdx);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief Set the MKA CAK source type. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * This API sets the CAK source type for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  mkaCakSource           Pointer to CAK source information. (N/A) \n
 *                                    Contains the CAK source type to be set.
 * @param[out] None
 *
 * @retval RT_ERR_OK                  Successfully set the CAK source type.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_set_cak_source(UnitChip_t unitChip, const MkaCakSource_t *mkaCakSource)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaCakSource)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_set_cak_source(unitChip.unit, mkaCakSource->source, mkaCakSource->unwrapIdx);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_set_cak_source(unitChip.unit, mkaCakSource->source, mkaCakSource->unwrapIdx);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_set_cak_source(unitChip.unit, mkaCakSource->source, mkaCakSource->unwrapIdx);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mka_set_cak_source(unitChip.unit, mkaCakSource->source, mkaCakSource->unwrapIdx);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/** @} */ /* MKA_CAK_SOURCE */

/**
 * @defgroup MKA_CA_CONFIG MKA CA Config
 * @brief Retrieve or configure MKA CA (secure Connectivity Association) configuration information
 * @{
 */

/**
 * @brief Retrieve MKA CA (secure Connectivity Association) configuration information. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * This API retrieves the CA configuration information for the specified chip.
 *
 * @param[in]     unitChip            Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] mkaCaConfig         Pointer to CA configuration information. (N/A) \n
 *                                    Stores the retrieved CA configuration information.
 *
 * @retval RT_ERR_OK                  Successfully retrieved the CA configuration information.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_get_ca_config(UnitChip_t unitChip, MkaCaConfig_t *mkaCaConfig)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaCaConfig)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_get_ca_config(unitChip.unit, mkaCaConfig->idx, &mkaCaConfig->config);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_get_ca_config(unitChip.unit, mkaCaConfig->idx, &mkaCaConfig->config);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_get_ca_config(unitChip.unit, mkaCaConfig->idx, &mkaCaConfig->config);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mka_get_ca_config(unitChip.unit, mkaCaConfig->idx, &mkaCaConfig->config);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief Set MKA CA (secure Connectivity Association) configuration information. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * This API sets the CA configuration information for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  mkaCaConfig            Pointer to CA configuration information. (N/A) \n
 *                                    Contains the CA configuration information to be set.
 * @param[out] None
 *
 * @retval RT_ERR_OK                  Successfully set the CA configuration information.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_set_ca_config(UnitChip_t unitChip, const MkaCaConfig_t *mkaCaConfig)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaCaConfig)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_set_ca_config(unitChip.unit, mkaCaConfig->idx, &mkaCaConfig->config);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_set_ca_config(unitChip.unit, mkaCaConfig->idx, &mkaCaConfig->config);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_set_ca_config(unitChip.unit, mkaCaConfig->idx, &mkaCaConfig->config);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mka_set_ca_config(unitChip.unit, mkaCaConfig->idx, &mkaCaConfig->config);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/** @} */ /* MKA_CA_CONFIG */

/**
 * @defgroup MKA_PORT_CONFIG MKA Port Config
 * @brief Retrieve or configure the MKA port configuration
 * @{
 */

/**
 * @brief Retrieve MKA port configuration. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * This API retrieves the port configuration for the specified chip.
 *
 * @param[in]     unitChip            Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] mkaPortConfig       Pointer to port configuration information. (N/A) \n
 *                                    Stores the retrieved port configuration.
 *
 * @retval RT_ERR_OK                  Successfully retrieved the port configuration.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_get_port_config(UnitChip_t unitChip, MkaPortConfig_t *mkaPortConfig)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaPortConfig)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_get_port_config(unitChip.unit, mkaPortConfig->port, &mkaPortConfig->config);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_get_port_config(unitChip.unit, mkaPortConfig->port, &mkaPortConfig->config);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_get_port_config(unitChip.unit, mkaPortConfig->port, &mkaPortConfig->config);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mka_get_port_config(unitChip.unit, mkaPortConfig->port, &mkaPortConfig->config);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/**
 * @brief Set MKA port configuration. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 *        Timer related configuarions (Hello Time/Life Time/Retire Time/New SAK Time/CA Set-up Time) are only supported on RTL907XD_VA \n
 *        Modifying these values on other chips will have no effect.
 *
 * This API sets the port configuration for the specified chip.
 *
 * @param[in]  unitChip               Stacking unit and chip ID. (N/A) \n
 *                                    Stacking unit ID is used to indicate which switch is to be accessed.
 *                                    Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  mkaPortConfig          Pointer to port configuration information. (N/A) \n
 *                                    Contains the port configuration to be set.
 * @param[out] None
 *
 * @retval RT_ERR_OK                  Successfully set the port configuration.
 * @retval -RT_ERR_NOT_SUPPORT        Not supported.
 * @retval Others                     Refer to the specific chip API for detailed return values.
 */
RtkApiRet rtk_mka_set_port_config(UnitChip_t unitChip, const MkaPortConfig_t *mkaPortConfig)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL != mkaPortConfig)
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_mka_set_port_config(unitChip.unit, mkaPortConfig->port, &mkaPortConfig->config);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_mka_set_port_config(unitChip.unit, mkaPortConfig->port, &mkaPortConfig->config);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_mka_set_port_config(unitChip.unit, mkaPortConfig->port, &mkaPortConfig->config);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_mka_set_port_config(unitChip.unit, mkaPortConfig->port, &mkaPortConfig->config);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    else
    {
        ret = -(RT_ERR_NULL_POINTER);
    }

    return ret;
}

/** @} */ /* MKA_PORT_CONFIG */

/** @} */ /* MODULE_MKA */
