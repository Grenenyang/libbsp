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

#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_mka.h"
#include "rtl907xd_v1_asicdrv_mka.h"
#include "rtl907xd_v1_asicdrv_macsec.h"
#ifndef LINUX_KERNEL_MODE
#include <stdbool.h>
#endif

#define CHK_NULL_POINTER(r, p)          \
    if((int32)RT_ERR_OK == (int32)(r))  \
    {                                   \
        if(NULL == (p))                 \
        {                               \
            (r) = -RT_ERR_NULL_POINTER; \
            continue;                   \
        }                               \
    }

#define CHK_ENABLE_PARAM(r, p) \
    if(RTK_ENABLE_END <= (p))  \
    {                          \
        (r) = -RT_ERR_ENABLE;  \
        continue;              \
    }

/**
 * @addtogroup RTK_MODULE_MKA MKA
 * @{
 */

/**
 * @addtogroup MKA_STATUS MKA Status
 * @{
 */

/**
 * @defgroup MKA_STATUS_ASIC Low Level Driver
 * @brief MKA Global Status Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get the administrative status of MKA.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] adminStatus          Administrative status (DISABLED, ENABLED). \n
 *                                  The administrative status of MKA protocol.
 * @retval RT_ERR_OK                Get MKA administrative status successfully.
 * @retval -RT_ERR_NULL_POINTER     The "adminStatus" is NULL.
 * @retval Others                   Please refer to dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_get_status(uint32 unit, RtkEnable *adminStatus)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    bool      enable  = false;
    do
    {
        /* Check if adminStatus is NULL */
        CHK_NULL_POINTER(retCode, adminStatus);

        /* Get the MKA enable status */
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_enable_get(unit, &enable));
        *adminStatus = (enable ? ENABLED : DISABLED);

    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set the administrative status of MKA.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] adminStatus           Administrative status (DISABLED, ENABLED). \n
 *                                  The administrative status of MKA protocol.
 * @retval RT_ERR_OK                Set the MKA administrative status successfully.
 * @retval -RT_ERR_ENABLE           The input value of "adminStatus" is invalid.
 * @retval Others                   Please refer to dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_set_status(uint32 unit, RtkEnable adminStatus)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    do
    {
        /* Check if adminStatus is valid */
        CHK_ENABLE_PARAM(retCode, adminStatus);

        /* Set the MKA enable status */
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_enable_set(unit, (ENABLED == adminStatus) ? true : false));
    } while(false);

    return retCode;
}

/** @} */ /* MKA_STATUS_ASIC */

/** @} */ /* MKA_STATUS */

/**
 * @addtogroup MKA_FORCE_LIVE_PEER MKA Get Force Live Peer
 * @{
 */

/**
 * @defgroup MKA_FORCE_LIVE_PEER_ASIC Low Level Driver
 * @brief MKA Force Live Peer Status Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get the status of Realtek's force-peer-promotion enhancement.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] force                Force promotion (DISABLED, ENABLED). \n
 *                                  Enable or disable for Realtek's force-peer-promotion enhancement.
 * @retval RT_ERR_OK                Get the status of Realtek's force-peer-promotion enhancement successfully.
 * @retval -RT_ERR_NULL_POINTER     The "force" is NULL.
 * @retval Others                   Please refer to dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_get_force_live_peer(uint32 unit, RtkEnable *force)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    bool      enable  = false;
    do
    {
        /* Check if force is NULL */
        CHK_NULL_POINTER(retCode, force);

        /* Get the force-peer-promotion status */
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_force_promotion_get(unit, &enable));

        *force = (enable ? ENABLED : DISABLED);

    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set the status of Realtek's force-peer-promotion enhancement.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] force                 Force promotion (DISABLED, ENABLED). \n
 *                                  Enable or disable for Realtek's force-peer-promotion enhancement.
 * @retval RT_ERR_OK                Set the status of Realtek's force-peer-promotion enhancement successfully.
 * @retval -RT_ERR_ENABLE           The input value of "force" is invalid.
 * @retval Others                   Please refer to dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_set_force_live_peer(uint32 unit, RtkEnable force)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    do
    {
        /* Check if force is valid */
        CHK_ENABLE_PARAM(retCode, force);

        /* Set the force-peer-promotion status */
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_force_promotion_set(unit, (ENABLED == force) ? true : false));
    } while(false);

    return retCode;
}

/** @} */ /* MKA_FORCE_LIVE_PEER_ASIC */

/** @} */ /* MKA_FORCE_LIVE_PEER */

/**
 * @addtogroup MKA_VERSION MKA Version
 * @{
 */

/**
 * @defgroup MKA_VERSIOIN_ASIC Low Level Driver
 * @brief MKA Version Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get MKA version.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] version              MKA Protocol Version (EM_RTK_MKA_VERSION_1 to EM_RTK_MKA_VERSION_UNKNOWN). \n
 *                                  The MKA protocol version.
 * @retval RT_ERR_OK                Get MKA protocol version successfully.
 * @retval -RT_ERR_NULL_POINTER     The "version" is NULL.
 * @retval Others                   Please refer to dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_get_version(uint32 unit, RtkMkaVersion_e *version)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint8     value   = 0u;

    do
    {
        /* Check if version is NULL */
        CHK_NULL_POINTER(retCode, version);

        /* Get the MKA version */
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_version_get(unit, &value));

        /* Verify if the value is 1 */
        if(1u == value)
        {
            /* If so, set version to EM_RTK_MKA_VERSION_1 */
            *version = EM_RTK_MKA_VERSION_1;
        }
        /* Verify if the value is 3 */
        else if(3u == value)
        {
            /* If so, set version to EM_RTK_MKA_VERSION_3 */
            *version = EM_RTK_MKA_VERSION_3;
        }
        else
        {
            /* If not, set version to EM_RTK_MKA_VERSION_UNKNOWN */
            *version = EM_RTK_MKA_VERSION_UNKNOWN;
        }

    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set MKA version.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] version               MKA Protocol Version (EM_RTK_MKA_VERSION_1 to EM_RTK_MKA_VERSION_3). \n
 *                                  The MKA protocol version.
 * @retval RT_ERR_OK                Set MKA protocol version successfully.
 * @retval RT_ERR_OUT_OF_RANGE      The version is out of range.
 * @retval Others                   Please refer to dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_set_version(uint32 unit, RtkMkaVersion_e version)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint8     value   = 0u;

    do
    {
        /* Verify if the version is EM_RTK_MKA_VERSION_1 */
        if(EM_RTK_MKA_VERSION_1 == version)
        {
            /* If so, set value to 1 */
            value = 1u;
        }
        /* Verify if the version is EM_RTK_MKA_VERSION_3 */
        else if((EM_RTK_MKA_VERSION_3 == version))
        {
            /* If so, set value to 3 */
            value = 3u;
        }
        else
        {
            /* If not, set value to 0xFF */
            value = 0xFFu;
        }

        /* Verify if the value is not 0xFF */
        if(value != 0xFFu)
        {
            /* If so, set the MKA version */
            CHK_FUN_CONTINUE(retCode, dv1_drv_mka_version_set(unit, value));
        }
        else
        {
            /* If not, set retCode to -RT_ERR_OUT_OF_RANGE */
            retCode = -RT_ERR_OUT_OF_RANGE;
        }

    } while(false);

    return retCode;
}

/** @} */ /* MKA_VERSION_ASIC */

/** @} */ /* MKA_VERSION */

/**
 * @addtogroup MKA_CAK_SOURCE MKA Get CAK Source
 * @{
 */

/**
 * @defgroup MKA_CAK_SOURCE_ASIC Low Level Driver
 * @brief MKA Get CAK Source Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get MKA CAK source Type.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] cakSource            CAK source Type (EM_RTK_MKA_CAK_SOURCE_OTP to EM_RTK_MKA_CAK_SOURCE_FLASH). \n
 *                                  The source type of CAK.
 * @param[out] unwrapIdx            Unwrap AES Key Index (RTK_MKA_MIN_CAK_IDX to RTK_MKA_MAX_CAK_IDX). \n
 *                                  The unwrap AES key Index for flash type CAK.
 * @retval RT_ERR_OK                Get the CAK source type successfully.
 * @retval -RT_ERR_NULL_POINTER     The "cakSource" or "unwrapIdx" is NULL.
 * @retval Others                   Please refer to dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_get_cak_source(uint32 unit, RtkMkaCakSource_e *cakSource, uint8 *unwrapIdx)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;
    uint8     value   = 0u;

    do
    {
        /* Check if cakSource or unwrapIdx is NULL */
        CHK_NULL_POINTER(retCode, cakSource);
        CHK_NULL_POINTER(retCode, unwrapIdx);

        /* Get the CAK source type */
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_cak_type_get(unit, &value));

        switch(value)
        {
            case DV1_MKA_CAK_SOURCE_OTP:
                /* If so, set cakSource to EM_RTK_MKA_CAK_SOURCE_OTP */
                *cakSource = EM_RTK_MKA_CAK_SOURCE_OTP;
                break;

            case DV1_MKA_CAK_SOURCE_FLASH:
                /* If so, set cakSource to EM_RTK_MKA_CAK_SOURCE_FLASH */
                *cakSource = EM_RTK_MKA_CAK_SOURCE_FLASH;
                /* Get the AES unwrap key index only for flash type CAK */
                CHK_FUN_CONTINUE(retCode, dv1_drv_mka_cak_unwrap_idx_get(unit, unwrapIdx));
                break;

            default:
                /* If not, set cakSource to EM_RTK_MKA_CAK_SOURCE_UNKNOWN */
                *cakSource = EM_RTK_MKA_CAK_SOURCE_UNKNOWN;
                break;
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set MKA CAK source Type.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] cakSource             CAK Source Type (EM_RTK_MKA_CAK_SOURCE_OTP to EM_RTK_MKA_CAK_SOURCE_FLASH). \n
 *                                  The source type of CAK.
 * @param[in] unwrapIdx             Unwrap AES Key Index (RTK_MKA_MIN_CAK_IDX to RTK_MKA_MAX_CAK_IDX). \n
 *                                  The unwrap AES key Index for flash type CAK.
 * @retval RT_ERR_OK                Set the CAK source type successfully.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter is out of range.
 * @retval Others                   Please refer to dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_set_cak_source(uint32 unit, RtkMkaCakSource_e cakSource, uint8 unwrapIdx)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;

    do
    {
        /* Verify if the CAK source is FLASH */
        if(EM_RTK_MKA_CAK_SOURCE_FLASH == cakSource)
        {
            /* Verify if the unwrapIdx is within range */
            if(unwrapIdx <= RTK_MKA_MAX_CAK_IDX)
            {
                /* If so, set CAK source type to flash */
                CHK_FUN_CONTINUE(retCode, dv1_drv_mka_cak_type_set(unit, DV1_MKA_CAK_SOURCE_FLASH));
                /* Set the unwrap index only for flash type CAK */
                CHK_FUN_CONTINUE(retCode, dv1_drv_mka_cak_unwrap_idx_set(unit, unwrapIdx));
            }
            else
            {
                /* If not, set retCode to -RT_ERR_OUT_OF_RANGE */
                retCode = (-RT_ERR_OUT_OF_RANGE);
            }
        }
        /* Verify if the CAK source is OTP */
        else if(EM_RTK_MKA_CAK_SOURCE_OTP == cakSource)
        {
            /* If so, set CAK source type to otp */
            CHK_FUN_CONTINUE(retCode, dv1_drv_mka_cak_type_set(unit, DV1_MKA_CAK_SOURCE_OTP));
        }
        else
        {
            /* If not, set retCode to -RT_ERR_OUT_OF_RANGE */
            retCode = (-RT_ERR_OUT_OF_RANGE);
        }
    } while(false);

    return retCode;
}

/** @} */ /* MKA_CAK_SOURCE_ASIC */

/** @} */ /* MKA_CAK_SOURCE */

/**
 * @addtogroup MKA_CA_CONFIG MKA CA Config
 * @{
 */

/**
 * @defgroup MKA_CA_CONFIG_ASIC Low Level Driver
 * @brief MKA CA Config Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get MKA CA (secure Connectivity Association) configuration information.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] caIndex               CA Index (RTK_MKA_MIN_CAK_IDX to RTK_MKA_MAX_CAK_IDX). \n
 *                                  The specific CA index.
 * @param[out] caConfig             CA Configuration entry(N/A). \n
 *                                  The configuration parameters of a specific CA.
 * @retval RT_ERR_OK                Get the CA configuration information successfully.
 * @retval -RT_ERR_ENTRY_INDEX      The caIndex is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "caConfig" is NULL.
 * @retval Others                   Please refer to dv1_drv_mka_ca_info_set(), dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_get_ca_config(uint32 unit, uint8 caIndex, RtkMkaCaConfig_t *caConfig)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;

    do
    {
        /* Check if caConfig is NULL */
        CHK_NULL_POINTER(retCode, caConfig);

        /* Verify if the caIndex is within range */
        if(caIndex <= RTK_MKA_MAX_CAK_IDX)
        {
            /* If so, get the specific CA information */
            CHK_FUN_CONTINUE(retCode, dv1_drv_mka_ca_info_get(unit, caIndex, &caConfig->enable, &caConfig->keyLen, caConfig->ckn));
        }
        else
        {
            /* If not, set retCode to -RT_ERR_ENTRY_INDEX */
            retCode = (-RT_ERR_ENTRY_INDEX);
        }
    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set MKA CA (secure Connectivity Association) configuration information.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] caIndex               CA Index (RTK_MKA_MIN_CAK_IDX to RTK_MKA_MAX_CAK_IDX). \n
 *                                  The specific CA index.
 * @param[in] caConfig              CA Configuration entry(N/A). \n
 *                                  The configuration parameters of a specific CA.
 * @retval RT_ERR_OK                Set the CA configuration information successfully.
 * @retval -RT_ERR_ENTRY_INDEX      The caIndex is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "caConfig" is NULL.
 * @retval -RT_ERR_OUT_OF_RANGE     The parameter of "caConfig" is out of range.
 * @retval Others                   Please refer to dv1_drv_mka_ca_info_set(), dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_set_ca_config(uint32 unit, uint8 caIndex, const RtkMkaCaConfig_t *caConfig)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;

    do
    {
        /* Check if caConfig is NULL */
        CHK_NULL_POINTER(retCode, caConfig);

        /* Verify if the caIndex is within range */
        if(caIndex <= RTK_MKA_MAX_CAK_IDX)
        {
            /* If so, set the specific CA information */
            CHK_FUN_CONTINUE(retCode, dv1_drv_mka_ca_info_set(unit, caIndex, caConfig->enable, caConfig->keyLen, caConfig->ckn));
        }
        else
        {
            /* If not, set retCode to -RT_ERR_ENTRY_INDEX */
            retCode = (-RT_ERR_ENTRY_INDEX);
        }

    } while(false);

    return retCode;
}

/** @} */ /* MKA_CA_CONFIG_ASIC */

/** @} */ /* MKA_CA_CONFIG */

/**
 * @addtogroup MKA_PORT_CONFIG MKA Port Config
 * @{
 */

/**
 * @defgroup MKA_PORT_CONFIG_ASIC Low Level Driver
 * @brief MKA Port Config Low Level Driver
 * @{
 */

/**
 * @brief This API is used to get MKA port configuration.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port Identity (RTK_USER_PORT_1ST to RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[out] portConfig           Port Configuration entry(N/A). \n
 *                                  The configuration parameters of a specific port.
 * @retval RT_ERR_OK                Get the port configuration successfully.
 * @retval -RT_ERR_PORT_ID          The Port ID is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "portConfig" is NULL.
 * @retval Others                   Please refer to dv1_drv_mka_ca_info_set(), dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_get_port_config(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;

    do
    {
        /* Check if portConfig is NULL */
        CHK_NULL_POINTER(retCode, portConfig);

        /* Get all port configuration parameters due to CCM requirements */
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_get_port_config_flags(unit, port, portConfig));
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_get_port_config(unit, port, portConfig));
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_get_port_config_extra(unit, port, portConfig));

        /* RTL907XD_V1 doesn't support configure MKA timer, return default value directly */
        portConfig->helloTime   = 2000u;
        portConfig->lifeTime    = 6000u;
        portConfig->retireTime  = 3000u;
        portConfig->newSakTime  = 0u;
        portConfig->caSetupTime = 0u;

    } while(false);

    return retCode;
}

/**
 * @brief This API is used to set MKA port configuration.
 *
 * @param[in] unit                  Stacking unit (1 to 7). \n
 *                                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                  Port Identity (RTK_USER_PORT_1ST to RTK_USER_PORT_END). \n
 *                                  The specific user port identity.
 * @param[in] portConfig            Port Configuration entry(N/A). \n
 *                                  The configuration parameters of a specific port.
 * @retval RT_ERR_OK                Set the port configuration successfully.
 * @retval -RT_ERR_PORT_ID          The Port ID is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "portConfig" is NULL.
 * @retval -RT_ERR_OUT_OF_RANGE     A parameter of "portConfig" is out of range.
 * @retval Others                   Please refer to dv1_drv_mka_ca_info_set(), dv1_indirect_access_read(), dv1_indirect_access_write().
 */
RtkApiRet dv1_mka_set_port_config(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig)
{
    /* Initialize local variables */
    RtkApiRet retCode = RT_ERR_OK;

    do
    {
        /* Check if portConfig is NULL */
        CHK_NULL_POINTER(retCode, portConfig);

        /* Verify each parameter of portConfig */
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_verify_port_config(portConfig));

        /* Set all port configuration parameters due to CCM requirements */
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_set_port_config_flags(unit, port, portConfig));
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_set_port_config(unit, port, portConfig));
        CHK_FUN_CONTINUE(retCode, dv1_drv_mka_set_port_config_extra(unit, port, portConfig));

    } while(false);

    return retCode;
}

/** @} */ /* MKA_PORT_CONFIG_ASIC */

/** @} */ /* MKA_PORT_CONFIG */

/** @} */ /* RTK_MODULE_MKA */
