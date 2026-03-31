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
#include "rtl907xd_va_api.h"

#define RTK_MKA_CAK_KEY_SIZE             (32u)
#define RTK_MKA_UNWRAP_KEY_IDX_DONT_CARE (0u)

/* Declaration */
RtkApiRet dva_macsec_mka_cak_in_otp(void);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup MACSEC_EXAMPLE MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XD_VA_MACSEC_EXAMPLE RTL907XD_VA MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XD_VA_MACSEC_MKA_OTP_TYPE_CAK MACsec MKA OTP Type CAK Example
 *
 * @brief <b> OTP-Type CAK </b> \n
 *        Pre-shared CAK in OTP. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to configure MKA to use the \n
 *          pre-shared CAK which is stored in the OTP.
 *
 * @section Description
 *          This example demonstrates how to use the wrapped pre-shared CAK (Connectivity Association Key) stored in flash. \n
 *          Assume the AES wrap Key is 00010203 04050607 08090A0B 0C0D0E0F 10111213 14151617 18191A1B 1C1D1E1F \n
 *          CAK#0 is 135bd758 b0ee5c11 c55ff6ab 19fdb199 \n
 *          CAK#1 is a29efdb6 3d6fba73 c65daab2 295340a8 37a8886e 94a905b5 c9c7ef1d 9dbb297e \n
 *          (see IEEE 802.1x-2020) G.4. \n
 *          Assume that: \n
 *          The port 1 will use AES_GCM_128 cipher suite and use \n
 *          CAK#0 (135b...b199, 128bit) as its pairwise CAK, the corresponding CKN is "RTK_DEMOL_1". \n
 *          It also change the hello time to 500 ms \n
 *
 *          The port 2 will use AEX_XPN_GCM_256 cipher suite and use \n
 *          CAK#1 (a29e...297e, 256bit) as its pairwise CAK, the corresponding CKN is "RTK_DEMOL_2". \n
 *          It also enable time-based rekey with 10 minutes duration.
 *
 * @note    This example is not support runtime configuration yet
 *
 * @{
 */
RtkApiRet dva_macsec_mka_cak_in_otp(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = DVA_UNIT,
        .chip = CHIP_907XD_VA};

    uint8 port1CaIdx = RTK_MKA_MIN_CAK_IDX;
    uint8 port2CaIdx = RTK_MKA_MIN_CAK_IDX + 1u;

#if 0
    /* Step 0. program CAK to OTP */
    /* CAK#0   is 135bd758 b0ee5c11 c55ff6ab 19fdb199                                     */
    /* the 128-bit CAK is placed in OTP in little-endian and is padding to 32 bytes       */
    /* clang-format off */
    uint8 cak1InOtp[RTK_MKA_CAK_KEY_SIZE] = {
        0x99u, 0xB1u, 0xFDu, 0x19u, 0xABu, 0xF6u, 0x5Fu, 0xC5u, 0x11u, 0x5Cu, 0xEEu, 0xB0u, 0x58u, 0xD7u, 0x5Bu, 0x13u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    };

    /* CAK#1   is a29efdb6 3d6fba73 c65daab2 295340a8 37a8886e 94a905b5 c9c7ef1d 9dbb29   */
    /* the 256-bit CAK is placed in OTP in little-endian*/
    uint8 cak2InOtp[RTK_MKA_CAK_KEY_SIZE] = {
        0x7Eu, 0x29u, 0xBBu, 0x9Du, 0x1Du, 0xEFu, 0xC7u, 0xC9u, 0xB5u, 0x05u, 0xA9u, 0x94u, 0x6Eu, 0x88u, 0xA8u, 0x37u,
        0xA8u, 0x40u, 0x53u, 0x29u, 0xB2u, 0xAAu, 0x5Du, 0xC6u, 0x73u, 0xBAu, 0x6Fu, 0x3Du, 0xB6u, 0xFDu, 0x9Eu, 0xA2u,
    };
    /* clang-format on */

    CHK_FUN_RET(retVal, rtk_otp_write(unitChip, 0xD00u, cak1InOtp, RTK_MKA_CAK_KEY_SIZE));
    CHK_FUN_RET(retVal, rtk_otp_write(unitChip, 0xD00u + 0x20u, cak2InOtp, RTK_MKA_CAK_KEY_SIZE));
#endif

    /* Step 0. enable MKA and config to run 802.1x-2020(version 3) */
    MkaStatus_t status = {.enable = ENABLED};
    CHK_FUN_RET(retVal, rtk_mka_set_status(unitChip, &status));

    MkaVersion_t mkaVersion = {.version = EM_RTK_MKA_VERSION_3};
    CHK_FUN_RET(retVal, rtk_mka_set_version(unitChip, &mkaVersion));

    /* Step 1. set CAK source to OTP (CAK is stored in OTP) */
    MkaCakSource_t cakSource = {.source = EM_RTK_MKA_CAK_SOURCE_OTP, .unwrapIdx = RTK_MKA_UNWRAP_KEY_IDX_DONT_CARE};
    CHK_FUN_RET(retVal, rtk_mka_set_cak_source(unitChip, &cakSource));

    /* Step 2. config CAK idx 0 with corresponding ckn:"RTK_MKA_DEMO_1", and key len is 128 bits */
    MkaCaConfig_t caConfig1 = {
        .idx    = port1CaIdx,
        .config = {
            .enable = true,
            .ckn    = "RTK_MKA_DEMO_1",
            .keyLen = RTK_MKA_KEY_LEN_128_BITS,
        }};
    CHK_FUN_RET(retVal, rtk_mka_set_ca_config(unitChip, &caConfig1));

    /* Step 3. config CAK idx 1 with corresponding ckn:"RTK_MKA_DEMO_2", and key len is 256 bits */
    MkaCaConfig_t caConfig2 = {
        .idx    = port2CaIdx,
        .config = {
            .enable = true,
            .ckn    = "RTK_MKA_DEMO_2",
            .keyLen = RTK_MKA_KEY_LEN_256_BITS,
        }};
    CHK_FUN_RET(retVal, rtk_mka_set_ca_config(unitChip, &caConfig2));

    /* Step 4. config port 1 to use cipher suite GCM-AES-128, CAK idx 0
               and change hello time to 500 us */
    MkaPortConfig_t port1Config =
        {
            .port   = DVA_RTK_USER_PORT_1ST,
            .config = {
                .enabled                   = true,
                .desired                   = true,
                .protect                   = true,
                .forceSci                  = true,
                .confidentialityProtection = true,
                .validateFrames            = EM_RTK_MKA_VALID_FRAMES_STRICT,
                .priority                  = 0x80,
                .confidentialityOffset     = EM_RTK_CONFIDENT_OFFSET_0,
                .caIndex                   = port1CaIdx,
                .cipherSuite               = EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128,
                .helloTime                 = 500u,
            }};
    CHK_FUN_RET(retVal, rtk_mka_set_port_config(unitChip, &port1Config));

    /* Step 5. config port 2 to use cipher suite GCM-XPN-256, CAK idx 1,
               and enable time-based rekey which 10 minutes */
    MkaPortConfig_t port2Config =
        {
            .port   = DVA_RTK_USER_PORT_2ND,
            .config = {
                .enabled                   = true,
                .desired                   = true,
                .protect                   = true,
                .forceSci                  = true,
                .confidentialityProtection = true,
                .validateFrames            = EM_RTK_MKA_VALID_FRAMES_STRICT,
                .priority                  = 0x80,
                .confidentialityOffset     = EM_RTK_CONFIDENT_OFFSET_0,
                .caIndex                   = port2CaIdx,
                .cipherSuite               = EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256,
                .newSakTime                = 10u * 60u * 1000u,
            }};
    CHK_FUN_RET(retVal, rtk_mka_set_port_config(unitChip, &port2Config));

    /* Expected Behaviour:
     *    After generated configuration image and reboot switch with this configuration,
     *    The port 1 should use CAK#0 (135b...b199, 128bit) as its pairwise CAK, and the corresponding CKN is "RTK_DEMOL_1",
     *    and the MKPDU will be sent every 500us
     *
     *    The port 2 should use CAK#1 (a29e...297e, 256bit) as its pairwise CAK, and the corresponding CKN is "RTK_DEMOL_2",
     *    and it will generate new SAK every 10 minutes
     */

    return retVal;
}

/**@}*/ /* RTL907XD_VA_MACSEC_MKA_OTP_TYPE_CAK */
/**@}*/ /* RTL907XD_VA_MACSEC_EXAMPLE */
/**@}*/ /* MACSEC_EXAMPLE */
/**@}*/ /* EXAMPLE */
