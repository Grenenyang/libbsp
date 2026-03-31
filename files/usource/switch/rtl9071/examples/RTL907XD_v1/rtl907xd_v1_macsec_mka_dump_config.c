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

#include "rtkas_api_ext.h"
#include "rtkas_api_mka.h"

#define RTK_MKA_CAK_WARP_KEY_SIZE        (32u)
#define RTK_MKA_CAK_KEY_SIZE             (32u)
#define RTK_MKA_DEFAULT_UNWRAP_KEY_IDX   (0u)
#define RTK_MKA_UNWRAP_KEY_IDX_DONT_CARE (0u)

/* Declaration */
RtkApiRet dv1_macsec_mka_dump_cak_config(void);
RtkApiRet dv1_macsec_mka_dump_port_config(void);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup MACSEC_EXAMPLE MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XD_V1_MACSEC_EXAMPLE RTL907XD_V1 MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XD_V1_MACSEC_MKA_DUMP_CAK_CONFIG MACsec MKA Dump CAK Config Example
 *
 * @brief <b> MKA Global/CA Configuration </b> \n
 *        Display MKA global / CA configuration. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to retrieve and display the global configurations and per-CA configuration of MKA.
 *
 * @section Description
 *          This example demonstrates how to retrieve MKA global configurations and per-CA configuration \n
 *          and display them in a key-value format.
 *
 * @{
 */
RtkApiRet dv1_macsec_mka_dump_cak_config(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    uint8      idx      = RTK_MKA_MIN_CAK_IDX;
    uint8      i        = 0u;
    UnitChip_t unitChip = {
        .unit = DV1_UNIT,
        .chip = CHIP_907XD_V1};

    const char *cakSourceTypeStr[3] = {
        [EM_RTK_MKA_CAK_SOURCE_OTP]     = "OTP",
        [EM_RTK_MKA_CAK_SOURCE_FLASH]   = "Flash",
        [EM_RTK_MKA_CAK_SOURCE_UNKNOWN] = "Unknown CAK source",
    };

    const char *mkaVersionStr[4] = {
        [EM_RTK_MKA_VERSION_1]       = "MKA Version 1(802.1X-2010)",
        [EM_RTK_MKA_VERSION_2]       = "MKA Version 2(802.1Xbx-2014)",
        [EM_RTK_MKA_VERSION_3]       = "MKA Version 3(802.1X-2020)",
        [EM_RTK_MKA_VERSION_UNKNOWN] = "Unknown MKA version",
    };

    /* Step 1. get MKA status */
    MkaStatus_t status = {0};
    CHK_FUN_RET(retVal, rtk_mka_get_status(unitChip, &status));
    CHK_FUN_RET(retVal, rtlglue_printf("MKA status:%s\n", ((ENABLED == status.enable) ? "enable" : "disable")));

    /* Step 2. get the status of prorietary force-live-peer enhancement */
    MkaForceLivePeer_t forceLivePeer = {0};
    CHK_FUN_RET(retVal, rtk_mka_get_force_live_peer(unitChip, &forceLivePeer));
    CHK_FUN_RET(retVal, rtlglue_printf("Force Peer Promotion:%s\n", ((ENABLED == forceLivePeer.enable) ? "enable" : "disable")));

    /* Step 3. get the MKA version */
    MkaVersion_t mkaVersion = {0};
    CHK_FUN_RET(retVal, rtk_mka_get_version(unitChip, &mkaVersion));
    CHK_FUN_RET(retVal, rtlglue_printf("MKA version:%s\n", mkaVersionStr[mkaVersion.version]));

    /* Step 4. get the CAK soure type */
    MkaCakSource_t cakSource = {0};
    CHK_FUN_RET(retVal, rtk_mka_get_cak_source(unitChip, &cakSource));

    CHK_FUN_RET(retVal, rtlglue_printf("CAK Source Type:%s\n", cakSourceTypeStr[cakSource.source]));
    if(EM_RTK_MKA_CAK_SOURCE_FLASH == cakSource.source)
    {
        CHK_FUN_RET(retVal, rtlglue_printf("  -- unwrap AES Key Index:%X\n", cakSource.unwrapIdx));
    }

    /* Step 5. get all CA configurations */
    for(idx = RTK_MKA_MIN_CAK_IDX; idx < RTK_MKA_MAX_CAK_IDX; ++idx)
    {
        MkaCaConfig_t caConfig = {.idx = idx};
        CHK_FUN_RET(retVal, rtk_mka_get_ca_config(unitChip, &caConfig));
        if(caConfig.config.enable)
        {
            CHK_FUN_RET(retVal, rtlglue_printf("CAK Config#%X\n", idx));
            CHK_FUN_RET(retVal, rtlglue_printf("enabled   : %s\n", caConfig.config.enable ? "yes" : "no"));
            CHK_FUN_RET(retVal, rtlglue_printf("key length: %s\n", (caConfig.config.keyLen == RTK_MKA_KEY_LEN_128_BITS) ? "128 bits" : "256 bits"));
            CHK_FUN_RET(retVal, rtlglue_printf("CKN       : "));
            for(i = 0u; i < (RTK_MKA_MAX_CKN_LEN - 1u); ++i)
            {
                CHK_FUN_RET(retVal, rtlglue_printf("%02x-", caConfig.config.ckn[i]));
            }
            CHK_FUN_RET(retVal, rtlglue_printf("%02x\n", caConfig.config.ckn[i]));
        }
    }

    return retVal;
}

/**@}*/ /* RTL907XD_V1_MACSEC_MKA_DUMP_CAK_CONFIG */

/**
 * @addtogroup RTL907XD_V1_MACSEC_MKA_DUMP_PORT_CONFIG MACsec MKA Dump Port Config Example
 *
 * @brief <b> MKA Port Configuration </b> \n
 *        Display MKA per-port configuration. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to retrieve and display the per-port configuration of MKA.
 *
 * @section Description
 *          This example demonstrates how to retrieve per-port configurations of MKA and display them in a key-value format.
 *
 * @{
 */
RtkApiRet dv1_macsec_mka_dump_port_config(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = DV1_UNIT,
        .chip = CHIP_907XD_V1};

    RtkPort     port           = 0u;
    const char *validateStr[4] = {
        [EM_RTK_MKA_VALID_FRAMES_DISABLED] = "Disabled",
        [EM_RTK_MKA_VALID_FRAMES_CHECKED]  = "Checked",
        [EM_RTK_MKA_VALID_FRAMES_STRICT]   = "Strict",
        [EM_RTK_MKA_VALID_FRAMES_NULL]     = "Null",
    };

    const char *cipherSuiteStr[5] = {
        [EM_RTK_MKA_CIPHER_SUITE_NONE]            = "None",
        [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128]     = "GCM-AES-128",
        [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_256]     = "GCM-AES-256",
        [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_128] = "GCM-AES-XPN-128",
        [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256] = "GCM-AES-XPN-256",
    };

    const char *confidentialOffsetStr[3] = {
        [EM_RTK_CONFIDENT_OFFSET_0]  = "0",
        [EM_RTK_CONFIDENT_OFFSET_30] = "30",
        [EM_RTK_CONFIDENT_OFFSET_50] = "50",
    };

    const char *externalPhyTypeStr[2] = {
        [EM_RTK_MKA_EXTERNAL_PHY_NONE]    = "None",
        [EM_RTK_MKA_EXTERNAL_PHY_RTL9010] = "RTL9010B",
    };

    /* Step 1. iterate through all ports to get the MKA configuration */
    for(port = DV1_RTK_USER_PORT_1ST; port < DV1_RTK_USER_PORT_END; ++port)
    {
        MkaPortConfig_t portConfig = {.port = port};

        CHK_FUN_RET(retVal, rtk_mka_get_port_config(unitChip, &portConfig));

        CHK_FUN_RET(retVal, rtlglue_printf("port#%u MKA/SECY parameters\n", port));
        if(portConfig.config.enabled)
        {
            CHK_FUN_RET(retVal, rtlglue_printf("enabled                   : %s\n", "yes"));
            CHK_FUN_RET(retVal, rtlglue_printf("MKA CAK index             : %u\n", portConfig.config.caIndex));
            CHK_FUN_RET(retVal, rtlglue_printf("MKA server priority       : %u\n", portConfig.config.priority));
            CHK_FUN_RET(retVal, rtlglue_printf("MKA PN Exhaustion         : %llu\n", portConfig.config.pnExhaustion));
            CHK_FUN_RET(retVal, rtlglue_printf("MKA cipher suite          : %s\n", cipherSuiteStr[portConfig.config.cipherSuite]));
            if(EM_RTK_MKA_EXTERNAL_PHY_NONE != portConfig.config.externalPhyType)
            {
                CHK_FUN_RET(retVal, rtlglue_printf("external phy type         : %s\n", externalPhyTypeStr[portConfig.config.externalPhyType]));
                CHK_FUN_RET(retVal, rtlglue_printf("external phy id           : %u\n", portConfig.config.externalPhyId));
            }
            CHK_FUN_RET(retVal, rtlglue_printf("Hello Time                : %u(us)\n", portConfig.config.helloTime));
            CHK_FUN_RET(retVal, rtlglue_printf("Life Time                 : %u(us)\n", portConfig.config.lifeTime));
            CHK_FUN_RET(retVal, rtlglue_printf("Retire Time               : %u(us)\n", portConfig.config.retireTime));
            CHK_FUN_RET(retVal, rtlglue_printf("=== SECY Parameters ===\n"));
            CHK_FUN_RET(retVal, rtlglue_printf("desired                   : %s\n", portConfig.config.desired ? "yes" : "no"));
            CHK_FUN_RET(retVal, rtlglue_printf("protect                   : %s\n", portConfig.config.protect ? "yes" : "no"));
            CHK_FUN_RET(retVal, rtlglue_printf("replayProtect             : %s\n", portConfig.config.replayProtect ? "yes" : "no"));
            CHK_FUN_RET(retVal, rtlglue_printf("forceSci                  : %s\n", portConfig.config.forceSci ? "yes" : "no"));
            CHK_FUN_RET(retVal, rtlglue_printf("useES                     : %s\n", portConfig.config.useES ? "yes" : "no"));
            CHK_FUN_RET(retVal, rtlglue_printf("useSCB                    : %s\n", portConfig.config.useSCB ? "yes" : "no"));
            CHK_FUN_RET(retVal, rtlglue_printf("confidentiality protection: %s\n", portConfig.config.confidentialityProtection ? "yes" : "no"));
            CHK_FUN_RET(retVal, rtlglue_printf("clear vlan number         : %u\n", portConfig.config.bypassVlanNumber));
            CHK_FUN_RET(retVal, rtlglue_printf("replay protect window size: %u\n", portConfig.config.replayProtectWindow));
            CHK_FUN_RET(retVal, rtlglue_printf("confidential offset       : %s\n", (portConfig.config.confidentialityOffset <= EM_RTK_CONFIDENT_OFFSET_50) ? confidentialOffsetStr[portConfig.config.confidentialityOffset] : "Unknown"));
            CHK_FUN_RET(retVal, rtlglue_printf("validate Frame            : %s\n", validateStr[portConfig.config.validateFrames]));
        }
        else
        {
            CHK_FUN_RET(retVal, rtlglue_printf("enabled                   : %s\n", "no"));
        }
    }

    return retVal;
}

/**@}*/ /* RTL907XD_V1_MACSEC_MKA_DUMP_PORT_CONFIG */
/**@}*/ /* RTL907XD_V1_MACSEC_EXAMPLE */
/**@}*/ /* MACSEC_EXAMPLE */
/**@}*/ /* EXAMPLE */
