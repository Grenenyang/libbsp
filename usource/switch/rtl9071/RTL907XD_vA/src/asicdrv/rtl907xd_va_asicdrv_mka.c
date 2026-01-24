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

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xd_va_api.h"
#include "rtkas_system_porting.h"
#include "rtl907xd_va_asicdrv_mka.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR                      ((uint32)0x01107a78u)
#define DVA_MKA_SRAM_GLOBAL_CONFIG_ENABLED_BIT                    ((uint32)0u)
#define DVA_MKA_SRAM_GLOBAL_CONFIG_FORCE_PROMOTE_TO_LIVE_PEER_BIT ((uint32)1u)

#define DVA_MKA_SRAM_GLOBAL_CONFIG_MKPDU_VERSION_ADDR (DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR + 1u)

#define DVA_MKA_SRAM_GLOBAL_CONFIG_CAK_TYPE_ADDR  (DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR + 2u)
#define DVA_MKA_SRAM_GLOBAL_CONFIG_CAK_TYPE_OTP   ((uint32)0x0u)
#define DVA_MKA_SRAM_GLOBAL_CONFIG_CAK_TYPE_FLASH ((uint32)0x1u)

#define DVA_MKA_SRAM_GLOBAL_CONFIG_UNWRAP_KEY_IDX_ADDR (DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR + 3u)

#define DVA_MKA_SRAM_CA_CONFIG_ADDR_BASE ((uint32)0x01107a7cu)
#define DVA_MKA_SRAM_CA_CONFIG_ENTRY_LEN ((uint32)0x24u)

#define DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE ((uint32)0x01107cc0u)
#define DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN ((uint32)0x38)

#define DVA_MKA_SRAM_PORT_CONFIG_FLAGS_OFFSET   ((uint32)0u)
#define DVA_MKA_SRAM_PORT_CONFIG_CAPABLE_OFFSET ((uint32)1u)

#define DVA_MKA_SRAM_PORT_CONFIG_CAKINDEX_OFFSET               ((uint32)3u)
#define DVA_MKA_SRAM_PORT_CONFIG_PRIORITY_OFFSET               ((uint32)4U)
#define DVA_MKA_SRAM_PORT_CONFIG_CONFIDENTIALITY_OFFSET_OFFSET ((uint32)5u)
#define DVA_MKA_SRAM_PORT_CONFIG_BYPASS_VLAN_NUMBER_OFFSET     ((uint32)6u)
#define DVA_MKA_SRAM_PORT_CONFIG_PHYID_OFFSET                  ((uint32)7u)
#define DVA_MKA_SRAM_PORT_CONFIG_REPLAY_PROTECT_WINDOW_OFFSET  ((uint32)12u)
#define DVA_MKA_SRAM_PORT_CONFIG_VALIDATE_FRAMES_OFFSET        ((uint32)16u)
#define DVA_MKA_SRAM_PORT_CONFIG_EXTERNAL_PHY_TYPE_OFFSET      ((uint32)20u)
#define DVA_MKA_SRAM_PORT_CONFIG_PN_EXHAUSTION_OFFSET          ((uint32)24u)
#define DVA_MKA_SRAM_PORT_CONFIG_CIPHER_SUITE_OFFSET           ((uint32)32u)
#define DVA_MKA_SRAM_PORT_CONFIG_HELLO_TIME_OFFSET             ((uint32)40u)
#define DVA_MKA_SRAM_PORT_CONFIG_LIFE_TIME_OFFSET              ((uint32)42u)
#define DVA_MKA_SRAM_PORT_CONFIG_RETIRE_TIME_OFFSET            ((uint32)44u)
#define DVA_MKA_SRAM_PORT_CONFIG_RAMPUP_TIME_OFFSET            ((uint32)46u)
#define DVA_MKA_SRAM_PORT_CONFIG_NEW_SAK_TIME_OFFSET           ((uint32)48u)
#define DVA_MKA_SRAM_PORT_CONFIG_CA_SETUP_TIME_OFFSET          ((uint32)52u)

#define DVA_MKA_CAPABILITY_NOT_IMPLEMENTED                ((uint32)0u)
#define DVA_MKA_CAPABILITY_INTEGRITY_ONLY                 ((uint32)1u)
#define DVA_MKA_CAPABILITY_CONFIDENTIALITY_OFFSET_0_ONLY  ((uint32)2u)
#define DVA_MKA_CAPABILITY_CONFIDENTIALITY_OFFSET_0_30_50 ((uint32)3U)

#define DVA_MKA_CIPHER_SUITE_GCM_AES_128     (0x0080c20001000001u)
#define DVA_MKA_CIPHER_SUITE_GCM_AES_256     (0x0080c20001000002u)
#define DVA_MKA_CIPHER_SUITE_GCM_AES_XPN_128 (0x0080c20001000003u)
#define DVA_MKA_CIPHER_SUITE_GCM_AES_XPN_256 (0x0080c20001000004u)

#define DVA_MKA_PORT_CFG_FLAGS_ENABLED_BIT                    (0u)
#define DVA_MKA_PORT_CFG_FLAGS_DESIRED_BIT                    (1u)
#define DVA_MKA_PORT_CFG_FLAGS_PROTECT_BIT                    (2u)
#define DVA_MKA_PORT_CFG_FLAGS_REPLAY_PROTECT_BIT             (3u)
#define DVA_MKA_PORT_CFG_FLAGS_FORCE_SCI_BIT                  (4u)
#define DVA_MKA_PORT_CFG_FLAGS_USE_ES_BIT                     (5u)
#define DVA_MKA_PORT_CFG_FLAGS_USE_SCB_BIT                    (6u)
#define DVA_MKA_PORT_CFG_FLAGS_CONFIDENTIALITY_PROTECTION_BIT (7u)

#define DVA_MKA_PORT_CFG_FLAGS_2_AUTOMOTIVE_BIT       (0u)
#define DVA_MKA_PORT_CFG_FLAGS_2_TIME_BASED_REKEY_BIT (1u)
#define DVA_MKA_PORT_CFG_FLAGS_2_POINT_TO_POINT_BIT   (2u)
#define DVA_MKA_PORT_CFG_FLAGS_2_CAPABILITY_BIT       (3u)

#define DVA_MKA_CONFIDENTIAL_OFFSET_NOT_USED (0u)
#define DVA_MKA_CONFIDENTIAL_OFFSET_0        (1u)
#define DVA_MKA_CONFIDENTIAL_OFFSET_30       (2u)
#define DVA_MKA_CONFIDENTIAL_OFFSET_50       (3u)
/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
static RtkApiRet dva_drv_mka_cfg_flag_set(uint32 unit, uint32 portIdx, const uint8 *flags);
static RtkApiRet dva_drv_mka_cfg_flag_get(uint32 unit, uint32 portIdx, uint8 *flags);
static RtkApiRet dva_drv_mka_cfg_replay_window_set(uint32 unit, uint32 portIdx, uint32 replayProtectWindow);
static RtkApiRet dva_drv_mka_cfg_replay_window_get(uint32 unit, uint32 portIdx, uint32 *replayProtectWindow);
static RtkApiRet dva_drv_mka_cfg_valid_frame_set(uint32 unit, uint32 portIdx, uint32 validateFrames);
static RtkApiRet dva_drv_mka_cfg_valid_frame_get(uint32 unit, uint32 portIdx, uint32 *validateFrames);
static RtkApiRet dva_drv_mka_cfg_ca_index_set(uint32 unit, uint32 portIdx, uint8 caIdx);
static RtkApiRet dva_drv_mka_cfg_ca_index_get(uint32 unit, uint32 portIdx, uint8 *caIdx);
static RtkApiRet dva_drv_mka_cfg_priority_set(uint32 unit, uint32 portIdx, uint8 priority);
static RtkApiRet dva_drv_mka_cfg_priority_get(uint32 unit, uint32 portIdx, uint8 *priority);
static RtkApiRet dva_drv_mka_cfg_pn_exhaustion_set(uint32 unit, uint32 portIdx, uint64 pnExhaustion);
static RtkApiRet dva_drv_mka_cfg_pn_exhaustion_get(uint32 unit, uint32 portIdx, uint64 *pnExhaustion);
static RtkApiRet dva_drv_mka_cfg_cipher_suite_set(uint32 unit, uint32 portIdx, uint64 cipherSuite);
static RtkApiRet dva_drv_mka_cfg_cipher_suite_get(uint32 unit, uint32 portIdx, uint64 *cipherSuite);
static RtkApiRet dva_drv_mka_cfg_confi_offset_set(uint32 unit, uint32 portIdx, uint8 confidentialityOffset);
static RtkApiRet dva_drv_mka_cfg_confi_offset_get(uint32 unit, uint32 portIdx, uint8 *confidentialityOffset);
static RtkApiRet dva_drv_mka_cfg_bypass_vlan_set(uint32 unit, uint32 portIdx, uint8 bypassVlanNumber);
static RtkApiRet dva_drv_mka_cfg_bypass_vlan_get(uint32 unit, uint32 portIdx, uint8 *bypassVlanNumber);
static RtkApiRet dva_drv_mka_cfg_external_phy_set(uint32 unit, uint32 portIdx, uint32 phyType, uint8 phyId);
static RtkApiRet dva_drv_mka_cfg_external_phy_get(uint32 unit, uint32 portIdx, uint32 *phyType, uint8 *phyId);
static RtkApiRet dva_drv_mka_cfg_hello_time_set(uint32 unit, uint32 portIdx, uint16 helloTime);
static RtkApiRet dva_drv_mka_cfg_hello_time_get(uint32 unit, uint32 portIdx, uint16 *helloTime);
static RtkApiRet dva_drv_mka_cfg_life_time_set(uint32 unit, uint32 portIdx, uint16 lifeTime);
static RtkApiRet dva_drv_mka_cfg_life_time_get(uint32 unit, uint32 portIdx, uint16 *lifeTime);
static RtkApiRet dva_drv_mka_cfg_retire_time_set(uint32 unit, uint32 portIdx, uint16 retireTime);
static RtkApiRet dva_drv_mka_cfg_retire_time_get(uint32 unit, uint32 portIdx, uint16 *retireTime);
static RtkApiRet dva_drv_mka_cfg_new_sak_time_set(uint32 unit, uint32 portIdx, uint32 newSakTime);
static RtkApiRet dva_drv_mka_cfg_new_sak_time_get(uint32 unit, uint32 portIdx, uint32 *newSakTime);
static RtkApiRet dva_drv_mka_cfg_ca_setup_time_set(uint32 unit, uint32 portIdx, uint32 caExtabTime);
static RtkApiRet dva_drv_mka_cfg_ca_setup_time_get(uint32 unit, uint32 portIdx, uint32 *caExtabTime);
/******************************************************************************/
/* Functions Definition                                                       */
/******************************************************************************/

static RtkApiRet dva_mka_params_range_check(uint32 value, uint32 minValue, uint32 maxValue)
{
    return ((minValue <= value) && (value <= maxValue)) ? RT_ERR_OK : (-RT_ERR_OUT_OF_RANGE);
}

static RtkMkaValidateFrames_e dva_drv_mka_convert_valid_frame(uint32 validateFrames)
{
    RtkMkaValidateFrames_e enumValue = EM_RTK_MKA_VALID_FRAMES_DISABLED;

    switch(validateFrames)
    {
        case(uint32)EM_RTK_MKA_VALID_FRAMES_DISABLED:
            enumValue = EM_RTK_MKA_VALID_FRAMES_DISABLED;
            break;
        case(uint32)EM_RTK_MKA_VALID_FRAMES_CHECKED:
            enumValue = EM_RTK_MKA_VALID_FRAMES_CHECKED;
            break;
        case(uint32)EM_RTK_MKA_VALID_FRAMES_STRICT:
            enumValue = EM_RTK_MKA_VALID_FRAMES_STRICT;
            break;
        case(uint32)EM_RTK_MKA_VALID_FRAMES_NULL:
            enumValue = EM_RTK_MKA_VALID_FRAMES_NULL;
            break;
        default:
            enumValue = EM_RTK_MKA_VALID_FRAMES_DISABLED;
            break;
    }

    return enumValue;
}

static RtkMkaCipherSuite_e dva_drv_mka_convert_cipher_suite(uint64 cipherSuite)
{
    RtkMkaCipherSuite_e enumValue = EM_RTK_MKA_CIPHER_SUITE_NONE;

    switch(cipherSuite)
    {
        case DVA_MKA_CIPHER_SUITE_GCM_AES_128:
            enumValue = EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128;
            break;
        case DVA_MKA_CIPHER_SUITE_GCM_AES_256:
            enumValue = EM_RTK_MKA_CIPHER_SUITE_GCM_AES_256;
            break;
        case DVA_MKA_CIPHER_SUITE_GCM_AES_XPN_128:
            enumValue = EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_128;
            break;
        case DVA_MKA_CIPHER_SUITE_GCM_AES_XPN_256:
            enumValue = EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256;
            break;
        default:
            enumValue = EM_RTK_MKA_CIPHER_SUITE_NONE;
            break;
    }

    return enumValue;
}

static RtkMkaConfidentOffset_e dva_drv_mka_convert_conf_offset(uint64 confidentialOffset)
{
    RtkMkaConfidentOffset_e enumValue = EM_RTK_CONFIDENT_OFFSET_UNKNOWN;

    switch(confidentialOffset)
    {
        case DVA_MKA_CONFIDENTIAL_OFFSET_NOT_USED:
            enumValue = EM_RTK_CONFIDENT_OFFSET_0;
            break;
        case DVA_MKA_CONFIDENTIAL_OFFSET_0:
            enumValue = EM_RTK_CONFIDENT_OFFSET_0;
            break;
        case DVA_MKA_CONFIDENTIAL_OFFSET_30:
            enumValue = EM_RTK_CONFIDENT_OFFSET_30;
            break;
        case DVA_MKA_CONFIDENTIAL_OFFSET_50:
            enumValue = EM_RTK_CONFIDENT_OFFSET_50;
            break;
        default:
            enumValue = EM_RTK_CONFIDENT_OFFSET_UNKNOWN;
            break;
    }

    return enumValue;
}

RtkApiRet dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(RtkPort port, RtkPort *portIdx)
{
    RtkApiRet retCode = RT_ERR_OK;

    if((port >= DVA_RTK_USER_PORT_1ST) && (port <= DVA_RTK_USER_PORT_END))
    {
        *portIdx = port - DVA_RTK_USER_PORT_1ST;
    }
    else
    {
        retCode = -(RT_ERR_PORT_ID);
    }

    return retCode;
}

RtkApiRet dva_drv_mka_enable_set(uint32 unit, bool enabled)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint8     value   = 0u;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR, &value));

    value &= ~((uint8)1u << DVA_MKA_SRAM_GLOBAL_CONFIG_ENABLED_BIT);
    value |= ((enabled ? 1u : 0u) << DVA_MKA_SRAM_GLOBAL_CONFIG_ENABLED_BIT);

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR, value));

    return retCode;
}

RtkApiRet dva_drv_mka_enable_get(uint32 unit, bool *enabled)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint8     value   = 0u;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR, &value));

    *enabled = ((((value >> DVA_MKA_SRAM_GLOBAL_CONFIG_ENABLED_BIT) & 0x1u) == 0x1u) ? true : false);

    return retCode;
}

RtkApiRet dva_drv_mka_force_promotion_set(uint32 unit, bool force)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint8     value   = 0u;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR, &value));

    value &= ~((uint8)1u << DVA_MKA_SRAM_GLOBAL_CONFIG_FORCE_PROMOTE_TO_LIVE_PEER_BIT);
    value |= ((force ? 1u : 0u) << DVA_MKA_SRAM_GLOBAL_CONFIG_FORCE_PROMOTE_TO_LIVE_PEER_BIT);

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR, value));

    return retCode;
}

RtkApiRet dva_drv_mka_force_promotion_get(uint32 unit, bool *force)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint8     value   = 0u;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_BASE_ADDR, &value));

    *force = ((((value >> DVA_MKA_SRAM_GLOBAL_CONFIG_FORCE_PROMOTE_TO_LIVE_PEER_BIT) & 0x1u) == 0x1u) ? true : false);

    return retCode;
}

RtkApiRet dva_drv_mka_version_set(uint32 unit, uint8 version)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_MKPDU_VERSION_ADDR, version));

    return retCode;
}

RtkApiRet dva_drv_mka_version_get(uint32 unit, uint8 *version)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_MKPDU_VERSION_ADDR, version));

    return retCode;
}

RtkApiRet dva_drv_mka_cak_type_set(uint32 unit, uint8 type)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_CAK_TYPE_ADDR, type));

    return retCode;
}

RtkApiRet dva_drv_mka_cak_type_get(uint32 unit, uint8 *type)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_CAK_TYPE_ADDR, type));

    return retCode;
}

RtkApiRet dva_drv_mka_cak_unwrap_idx_set(uint32 unit, uint8 unwrapKeyIdx)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_UNWRAP_KEY_IDX_ADDR, unwrapKeyIdx));

    return retCode;
}

RtkApiRet dva_drv_mka_cak_unwrap_idx_get(uint32 unit, uint8 *unwrapKeyIdx)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, DVA_MKA_SRAM_GLOBAL_CONFIG_UNWRAP_KEY_IDX_ADDR, unwrapKeyIdx));

    return retCode;
}

RtkApiRet dva_drv_mka_ca_info_set(uint32 unit, uint32 index, bool enable, uint8 keyLen, const uint8 *ckn)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_CA_CONFIG_ADDR_BASE + (index * DVA_MKA_SRAM_CA_CONFIG_ENTRY_LEN);
    uint32    offset   = 0u;
    uint32    data     = 0u;

    if(enable)
    {
        CHK_FUN_RET(retCode, dva_mka_params_range_check(keyLen, RTK_MKA_KEY_LEN_128_BITS, RTK_MKA_KEY_LEN_256_BITS));
        CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr, 1u));
        CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr + 1u, keyLen));

        for(offset = 4u; offset < DVA_MKA_SRAM_CA_CONFIG_ENTRY_LEN; offset += 4u)
        {
#if 0
            data = caInfo->ckn[offset - 2u] + (caInfo->ckn[offset - 1u] << 8u) + (caInfo->ckn[offset] << 16u) + (caInfo->ckn[offset + 1u] << 24u);
#else
            CHK_FUN_RET(retCode, dva_drv_memcpy((uint8 *)&data, &ckn[offset - 4u], sizeof(uint32)));
#endif
            CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr + offset, data));
        }
    }
    else
    {
        /* keyLen = 0 means disable this entry, reset all bits to 0 */
        CHK_FUN_RET(retCode, dva_drv_indirect_sram_2bytes_set(unit, sramAddr, 0));

        for(offset = 4u; offset < DVA_MKA_SRAM_CA_CONFIG_ENTRY_LEN; offset += 4u)
        {
            CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr + offset, 0));
        }
    }

    return retCode;
}

RtkApiRet dva_drv_mka_ca_info_get(uint32 unit, uint32 index, bool *enable, uint8 *keyLen, uint8 *ckn)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_CA_CONFIG_ADDR_BASE + (index * DVA_MKA_SRAM_CA_CONFIG_ENTRY_LEN);
    uint32    offset   = 0u;
    uint32    data     = 0u;
    uint8     data8    = 0u;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr, &data8));

    *enable = (data8 == 1u);

    if(*enable)
    {
        CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr + 1u, keyLen));

        for(offset = 4u; offset < DVA_MKA_SRAM_CA_CONFIG_ENTRY_LEN; offset += 4u)
        {
            CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr + offset, &data));
            CHK_FUN_RET(retCode, dva_drv_memcpy(&ckn[offset - 4u], (uint8 *)&data, sizeof(uint32)));
        }
    }

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_flag_set(uint32 unit, uint32 portIdx, const uint8 *flags)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_FLAGS_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr, flags[0]));
    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr + 1u, flags[1]));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_flag_get(uint32 unit, uint32 portIdx, uint8 *flags)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_FLAGS_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr, &flags[0]));
    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr + 1u, &flags[1]));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_replay_window_set(uint32 unit, uint32 portIdx, uint32 replayProtectWindow)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_REPLAY_PROTECT_WINDOW_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr, replayProtectWindow));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_replay_window_get(uint32 unit, uint32 portIdx, uint32 *replayProtectWindow)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_REPLAY_PROTECT_WINDOW_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr, replayProtectWindow));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_valid_frame_set(uint32 unit, uint32 portIdx, uint32 validateFrames)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_VALIDATE_FRAMES_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr, validateFrames));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_valid_frame_get(uint32 unit, uint32 portIdx, uint32 *validateFrames)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_VALIDATE_FRAMES_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr, validateFrames));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_ca_index_set(uint32 unit, uint32 portIdx, uint8 caIdx)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_CAKINDEX_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr, caIdx));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_ca_index_get(uint32 unit, uint32 portIdx, uint8 *caIdx)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_CAKINDEX_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr, caIdx));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_priority_set(uint32 unit, uint32 portIdx, uint8 priority)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_PRIORITY_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr, priority));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_priority_get(uint32 unit, uint32 portIdx, uint8 *priority)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_PRIORITY_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr, priority));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_pn_exhaustion_set(uint32 unit, uint32 portIdx, uint64 pnExhaustion)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_PN_EXHAUSTION_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr, (uint32)(pnExhaustion & 0xFFFFFFFFu)));
    CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr + 4u, (uint32)(pnExhaustion >> 32u)));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_pn_exhaustion_get(uint32 unit, uint32 portIdx, uint64 *pnExhaustion)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_PN_EXHAUSTION_OFFSET;
    uint32    dataLo   = 0u;
    uint32    dataHi   = 0u;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr, &dataLo));
    CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr + 4u, &dataHi));
    *pnExhaustion = (((uint64)dataHi << 32u) | (uint64)dataLo);

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_cipher_suite_set(uint32 unit, uint32 portIdx, uint64 cipherSuite)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_CIPHER_SUITE_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr, (uint32)(cipherSuite & 0xFFFFFFFFu)));
    CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr + 4u, (uint32)(cipherSuite >> 32u)));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_cipher_suite_get(uint32 unit, uint32 portIdx, uint64 *cipherSuite)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_CIPHER_SUITE_OFFSET;
    uint32    dataLo   = 0u;
    uint32    dataHi   = 0u;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr, &dataLo));
    CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr + 4u, &dataHi));
    *cipherSuite = (((uint64)dataHi << 32u) | (uint64)dataLo);

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_confi_offset_set(uint32 unit, uint32 portIdx, uint8 confidentialityOffset)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_CONFIDENTIALITY_OFFSET_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr, confidentialityOffset));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_confi_offset_get(uint32 unit, uint32 portIdx, uint8 *confidentialityOffset)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_CONFIDENTIALITY_OFFSET_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr, confidentialityOffset));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_bypass_vlan_set(uint32 unit, uint32 portIdx, uint8 bypassVlanNumber)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_BYPASS_VLAN_NUMBER_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr, bypassVlanNumber));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_bypass_vlan_get(uint32 unit, uint32 portIdx, uint8 *bypassVlanNumber)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_BYPASS_VLAN_NUMBER_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr, bypassVlanNumber));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_external_phy_set(uint32 unit, uint32 portIdx, uint32 phyType, uint8 phyId)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_EXTERNAL_PHY_TYPE_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr, phyType));

    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_PHYID_OFFSET;
    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr, phyId));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_external_phy_get(uint32 unit, uint32 portIdx, uint32 *phyType, uint8 *phyId)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_EXTERNAL_PHY_TYPE_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr, phyType));

    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_PHYID_OFFSET;
    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr, phyId));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_hello_time_set(uint32 unit, uint32 portIdx, uint16 helloTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_HELLO_TIME_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_2bytes_set(unit, sramAddr, helloTime));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_hello_time_get(uint32 unit, uint32 portIdx, uint16 *helloTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_HELLO_TIME_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_2bytes_get(unit, sramAddr, helloTime));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_life_time_set(uint32 unit, uint32 portIdx, uint16 lifeTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_LIFE_TIME_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_2bytes_set(unit, sramAddr, lifeTime));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_life_time_get(uint32 unit, uint32 portIdx, uint16 *lifeTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_LIFE_TIME_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_2bytes_get(unit, sramAddr, lifeTime));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_retire_time_set(uint32 unit, uint32 portIdx, uint16 retireTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_RETIRE_TIME_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_2bytes_set(unit, sramAddr, retireTime));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_retire_time_get(uint32 unit, uint32 portIdx, uint16 *retireTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_RETIRE_TIME_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_2bytes_get(unit, sramAddr, retireTime));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_new_sak_time_set(uint32 unit, uint32 portIdx, uint32 newSakTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_NEW_SAK_TIME_OFFSET;
    uint8     value    = 0u;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr, newSakTime));

    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_CAPABLE_OFFSET;
    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_get(unit, sramAddr, &value));

    if(0u == newSakTime)
    {
        /* clear timebasedRekey bit [1] */
        value &= ~((uint8)1u << 1u);
    }
    else
    {
        /* set timebasedRekey bit [1] */
        value |= ((uint8)1u << 1u);
    }

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_1byte_set(unit, sramAddr, value));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_new_sak_time_get(uint32 unit, uint32 portIdx, uint32 *newSakTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_NEW_SAK_TIME_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr, newSakTime));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_ca_setup_time_set(uint32 unit, uint32 portIdx, uint32 caExtabTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_CA_SETUP_TIME_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_set(unit, sramAddr, caExtabTime));

    return retCode;
}

static RtkApiRet dva_drv_mka_cfg_ca_setup_time_get(uint32 unit, uint32 portIdx, uint32 *caExtabTime)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    sramAddr = DVA_MKA_SRAM_PORT_CONFIG_ADDR_BASE + (portIdx * DVA_MKA_SRAM_PORT_CONFIG_ENTRY_LEN) + DVA_MKA_SRAM_PORT_CONFIG_CA_SETUP_TIME_OFFSET;

    CHK_FUN_RET(retCode, dva_drv_indirect_sram_get(unit, sramAddr, caExtabTime));

    return retCode;
}

RtkApiRet dva_drv_mka_get_port_config_flags(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    portIdx  = 0u;
    uint8     flags[2] = {0u};

    CHK_FUN_RET(retCode, dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(port, &portIdx));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_flag_get(unit, portIdx, flags));

    portConfig->enabled                   = (0u != (flags[0] & (1u << DVA_MKA_PORT_CFG_FLAGS_ENABLED_BIT)));
    portConfig->desired                   = (0u != (flags[0] & (1u << DVA_MKA_PORT_CFG_FLAGS_DESIRED_BIT)));
    portConfig->protect                   = (0u != (flags[0] & (1u << DVA_MKA_PORT_CFG_FLAGS_PROTECT_BIT)));
    portConfig->replayProtect             = (0u != (flags[0] & (1u << DVA_MKA_PORT_CFG_FLAGS_REPLAY_PROTECT_BIT)));
    portConfig->forceSci                  = (0u != (flags[0] & (1u << DVA_MKA_PORT_CFG_FLAGS_FORCE_SCI_BIT)));
    portConfig->useES                     = (0u != (flags[0] & (1u << DVA_MKA_PORT_CFG_FLAGS_USE_ES_BIT)));
    portConfig->useSCB                    = (0u != (flags[0] & (1u << DVA_MKA_PORT_CFG_FLAGS_USE_SCB_BIT)));
    portConfig->confidentialityProtection = (0u != (flags[0] & (1u << DVA_MKA_PORT_CFG_FLAGS_CONFIDENTIALITY_PROTECTION_BIT)));
    portConfig->automotiveMka             = (0u != (flags[1] & (1u << DVA_MKA_PORT_CFG_FLAGS_2_AUTOMOTIVE_BIT)));
    portConfig->isPointToPoint            = (0u != (flags[1] & (1u << DVA_MKA_PORT_CFG_FLAGS_2_POINT_TO_POINT_BIT)));

    return retCode;
}

RtkApiRet dva_drv_mka_get_port_config(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig)
{
    RtkApiRet retCode         = RT_ERR_OK;
    uint32    portIdx         = 0u;
    uint32    externalPhyType = 0u;

    CHK_FUN_RET(retCode, dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(port, &portIdx));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_replay_window_get(unit, portIdx, &portConfig->replayProtectWindow));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_ca_index_get(unit, portIdx, &portConfig->caIndex));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_priority_get(unit, portIdx, &portConfig->priority));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_pn_exhaustion_get(unit, portIdx, &portConfig->pnExhaustion));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_bypass_vlan_get(unit, portIdx, &portConfig->bypassVlanNumber));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_external_phy_get(unit, portIdx, &externalPhyType, &portConfig->externalPhyId));
    /* clang-format off */
    switch(externalPhyType)
    {
        case (uint32)EM_RTK_MKA_EXTERNAL_PHY_RTL9010:
            portConfig->externalPhyType = EM_RTK_MKA_EXTERNAL_PHY_RTL9010;
            break;
        default:
            portConfig->externalPhyType = EM_RTK_MKA_EXTERNAL_PHY_NONE;
            break;
    }
    /* clang-format on */
    return retCode;
}

RtkApiRet dva_drv_mka_get_port_config_extra(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig)
{
    RtkApiRet retCode            = RT_ERR_OK;
    uint32    portIdx            = 0u;
    uint64    cipherSuite        = 0u;
    uint8     confidentialOffset = 0u;
    uint32    validateFrames     = 0u;

    CHK_FUN_RET(retCode, dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(port, &portIdx));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_valid_frame_get(unit, portIdx, &validateFrames));
    portConfig->validateFrames = dva_drv_mka_convert_valid_frame(validateFrames);

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_cipher_suite_get(unit, portIdx, &cipherSuite));
    portConfig->cipherSuite = dva_drv_mka_convert_cipher_suite(cipherSuite);

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_confi_offset_get(unit, portIdx, &confidentialOffset));
    portConfig->confidentialityOffset = dva_drv_mka_convert_conf_offset(confidentialOffset);

    return retCode;
}

RtkApiRet dva_drv_mka_get_port_timers(uint32 unit, RtkPort port, RtkMkaPortConfig_t *portConfig)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint32    portIdx = 0u;

    CHK_FUN_RET(retCode, dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(port, &portIdx));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_hello_time_get(unit, portIdx, &portConfig->helloTime));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_life_time_get(unit, portIdx, &portConfig->lifeTime));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_retire_time_get(unit, portIdx, &portConfig->retireTime));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_new_sak_time_get(unit, portIdx, &portConfig->newSakTime));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_ca_setup_time_get(unit, portIdx, &portConfig->caSetupTime));

    return retCode;
}

RtkApiRet dva_drv_mka_verify_port_config(const RtkMkaPortConfig_t *portConfig)
{
    RtkApiRet retCode = RT_ERR_OK;

    const uint64 pnExhaustionRangeLow[5] =
        {
            [EM_RTK_MKA_CIPHER_SUITE_NONE]            = RTK_MKA_MIN_PN_EXHAUSTION,
            [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128]     = RTK_MKA_MIN_PN_EXHAUSTION,
            [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_256]     = RTK_MKA_MIN_PN_EXHAUSTION,
            [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_128] = RTK_MKA_MIN_XPN_PN_EXHAUSTION,
            [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256] = RTK_MKA_MIN_XPN_PN_EXHAUSTION,
        };

    const uint64 pnExhaustionRangeHigh[5] =
        {
            [EM_RTK_MKA_CIPHER_SUITE_NONE]            = RTK_MKA_MAX_PN_EXHAUSTION,
            [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128]     = RTK_MKA_MAX_PN_EXHAUSTION,
            [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_256]     = RTK_MKA_MAX_PN_EXHAUSTION,
            [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_128] = RTK_MKA_MAX_XPN_PN_EXHAUSTION,
            [EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256] = RTK_MKA_MAX_XPN_PN_EXHAUSTION,
        };

    CHK_FUN_RET(retCode, dva_mka_params_range_check(portConfig->caIndex, RTK_MKA_MIN_CAK_IDX, RTK_MKA_MAX_CAK_IDX));
    CHK_FUN_RET(retCode, dva_mka_params_range_check(portConfig->bypassVlanNumber, RTK_MKA_MIN_BYPASS_VLAN_NUMBER, RTK_MKA_MAX_BYPASS_VLAN_NUMBER));

    CHK_FUN_RET(retCode, dva_mka_params_range_check((uint32)portConfig->validateFrames, (uint32)EM_RTK_MKA_VALID_FRAMES_DISABLED, (uint32)EM_RTK_MKA_VALID_FRAMES_NULL));
    CHK_FUN_RET(retCode, dva_mka_params_range_check((uint32)portConfig->confidentialityOffset, (uint32)EM_RTK_CONFIDENT_OFFSET_0, (uint32)EM_RTK_CONFIDENT_OFFSET_50));
    CHK_FUN_RET(retCode, dva_mka_params_range_check((uint32)portConfig->externalPhyType, (uint32)EM_RTK_MKA_EXTERNAL_PHY_NONE, (uint32)EM_RTK_MKA_EXTERNAL_PHY_RTL9010));
    CHK_FUN_RET(retCode, dva_mka_params_range_check((uint32)portConfig->cipherSuite, (uint32)EM_RTK_MKA_CIPHER_SUITE_NONE, (uint32)EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256));

    /* pnExhaustion is 64 bits, is not supported by dva_mka_params_range_check */
    if((pnExhaustionRangeLow[portConfig->cipherSuite] > portConfig->pnExhaustion) || (portConfig->pnExhaustion > pnExhaustionRangeHigh[portConfig->cipherSuite]))
    {
        retCode = -(RT_ERR_OUT_OF_RANGE);
    }

    return retCode;
}

RtkApiRet dva_drv_mka_set_port_config_flags(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig)
{
    RtkApiRet retCode  = RT_ERR_OK;
    uint32    portIdx  = 0u;
    uint8     flags[2] = {0u};

    CHK_FUN_RET(retCode, dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(port, &portIdx));
    if(portConfig->enabled)
    {
        flags[0] |= (1u << DVA_MKA_PORT_CFG_FLAGS_ENABLED_BIT);
    }

    if(portConfig->desired)
    {
        flags[0] |= (1u << DVA_MKA_PORT_CFG_FLAGS_DESIRED_BIT);
    }

    if(portConfig->protect)
    {
        flags[0] |= (1u << DVA_MKA_PORT_CFG_FLAGS_PROTECT_BIT);
    }

    if(portConfig->replayProtect)
    {
        flags[0] |= (1u << DVA_MKA_PORT_CFG_FLAGS_REPLAY_PROTECT_BIT);
    }

    if(portConfig->forceSci)
    {
        flags[0] |= (1u << DVA_MKA_PORT_CFG_FLAGS_FORCE_SCI_BIT);
    }

    if(portConfig->useES)
    {
        flags[0] |= (1u << DVA_MKA_PORT_CFG_FLAGS_USE_ES_BIT);
    }

    if(portConfig->useSCB)
    {
        flags[0] |= (1u << DVA_MKA_PORT_CFG_FLAGS_USE_SCB_BIT);
    }

    if(portConfig->confidentialityProtection)
    {
        flags[0] |= (1u << DVA_MKA_PORT_CFG_FLAGS_CONFIDENTIALITY_PROTECTION_BIT);
    }

    if(portConfig->automotiveMka)
    {
        flags[1] |= (1u << DVA_MKA_PORT_CFG_FLAGS_2_AUTOMOTIVE_BIT);
    }

    if(portConfig->newSakTime != 0u)
    {
        flags[1] |= (1u << DVA_MKA_PORT_CFG_FLAGS_2_TIME_BASED_REKEY_BIT);
    }

    if(portConfig->isPointToPoint)
    {
        flags[1] |= (1u << DVA_MKA_PORT_CFG_FLAGS_2_POINT_TO_POINT_BIT);
    }

    flags[1] |= (3u << DVA_MKA_PORT_CFG_FLAGS_2_CAPABILITY_BIT);

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_flag_set(unit, portIdx, flags));

    return retCode;
}

RtkApiRet dva_drv_mka_set_port_config(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint32    portIdx = 0u;

    CHK_FUN_RET(retCode, dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(port, &portIdx));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_priority_set(unit, portIdx, portConfig->priority));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_replay_window_set(unit, portIdx, portConfig->replayProtectWindow));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_ca_index_set(unit, portIdx, portConfig->caIndex));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_pn_exhaustion_set(unit, portIdx, portConfig->pnExhaustion));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_valid_frame_set(unit, portIdx, (uint32)portConfig->validateFrames));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_bypass_vlan_set(unit, portIdx, portConfig->bypassVlanNumber));

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_external_phy_set(unit, portIdx, (uint32)portConfig->externalPhyType, portConfig->externalPhyId));

    return retCode;
}

RtkApiRet dva_drv_mka_set_port_config_extra(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig)
{
    RtkApiRet retCode            = RT_ERR_OK;
    uint32    portIdx            = 0u;
    uint64    cipherSuite        = 0u;
    uint8     confidentialOffset = 0u;

    CHK_FUN_RET(retCode, dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(port, &portIdx));

    switch(portConfig->cipherSuite)
    {
        case EM_RTK_MKA_CIPHER_SUITE_GCM_AES_128:
            cipherSuite = DVA_MKA_CIPHER_SUITE_GCM_AES_128;
            break;
        case EM_RTK_MKA_CIPHER_SUITE_GCM_AES_256:
            cipherSuite = DVA_MKA_CIPHER_SUITE_GCM_AES_256;
            break;
        case EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_128:
            cipherSuite = DVA_MKA_CIPHER_SUITE_GCM_AES_XPN_128;
            break;
        case EM_RTK_MKA_CIPHER_SUITE_GCM_AES_XPN_256:
            cipherSuite = DVA_MKA_CIPHER_SUITE_GCM_AES_XPN_256;
            break;
        default:
            retCode = (-RT_ERR_OUT_OF_RANGE);
            break;
    }
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_cipher_suite_set(unit, portIdx, cipherSuite));

    if(true == portConfig->confidentialityProtection)
    {
        switch(portConfig->confidentialityOffset)
        {
            case EM_RTK_CONFIDENT_OFFSET_0:
                confidentialOffset = DVA_MKA_CONFIDENTIAL_OFFSET_0;
                break;
            case EM_RTK_CONFIDENT_OFFSET_30:
                confidentialOffset = DVA_MKA_CONFIDENTIAL_OFFSET_30;
                break;
            case EM_RTK_CONFIDENT_OFFSET_50:
                confidentialOffset = DVA_MKA_CONFIDENTIAL_OFFSET_50;
                break;
            default:
                retCode = (-RT_ERR_OUT_OF_RANGE);
                break;
        }
    }
    else
    {
        confidentialOffset = DVA_MKA_CONFIDENTIAL_OFFSET_NOT_USED;
    }

    CHK_FUN_RET(retCode, dva_drv_mka_cfg_confi_offset_set(unit, portIdx, confidentialOffset));

    return retCode;
}

RtkApiRet dva_drv_mka_set_port_timers(uint32 unit, RtkPort port, const RtkMkaPortConfig_t *portConfig)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint32    portIdx = 0u;

    CHK_FUN_RET(retCode, dva_drv_mka_helper_verify_user_port_and_covert_to_port_idx(port, &portIdx));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_hello_time_set(unit, portIdx, portConfig->helloTime));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_life_time_set(unit, portIdx, portConfig->lifeTime));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_retire_time_set(unit, portIdx, portConfig->retireTime));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_new_sak_time_set(unit, portIdx, portConfig->newSakTime));
    CHK_FUN_RET(retCode, dva_drv_mka_cfg_ca_setup_time_set(unit, portIdx, portConfig->caSetupTime));

    return retCode;
}
