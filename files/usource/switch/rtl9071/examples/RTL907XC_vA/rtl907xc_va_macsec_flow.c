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
#include "rtkas_api_macsec.h"
#include "rtkas_api_mka.h"

/* Declaration */
RtkApiRet cva_macsec_create_rx_sc(void);
RtkApiRet cva_macsec_create_tx_sc(void);
RtkApiRet cva_macsec_create_rx_sa(void);
RtkApiRet cva_macsec_create_tx_sa(void);
RtkApiRet cva_macsec_enable_tx_sa(void);
RtkApiRet cva_macsec_enable_rx_sa(void);
RtkApiRet cva_macsec_disable_tx_sa(void);
RtkApiRet cva_macsec_disable_rx_sa(void);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup MACSEC_EXAMPLE MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XC_VA_MACSEC_EXAMPLE RTL907XC_VA MACsec Example
 * @{
 */

/**
 * @addtogroup RTL907XC_VA_MACSEC_CREATE_RX_SC MACsec Create RX SC Example
 *
 * @brief <b> Create MACsec ingress Secure Channel </b> \n
 *        Create an ingress MACsec secure channel with specific SCI and AN = 3. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to create an ingress MACsec secure channel.
 *
 * @section Description
 *          This example demonstrates how to create an ingress secure channel for sci 00-e0-4c-11-22-33-00-01. and AN = 3\n
 *          which will use the same hardware entry as AN=1.
 *          For this secure channel, we need to add a match rule to match packets whose SCI and AN are expected \n
 *          and specify their flow control index to be the same value. \n
 *          In the flow control settings, we enabled the replay protect feature, \n
 *          and using the strict policy when validating ingress packets.
 *          Finally, we need to assign the corresponding SA entry index (recommended to use the same value) to this flow control entry. \n
 *          Subsequent SAK configurations also need to be set on the corresponding SA entry. \n
 *          \n
 *          It is important to note that if the SCI is not explicitly shown in the SecTAG, the packet \n
 *          will not match this match rule, resulting in validation failure.
 *
 * @{
 */
RtkApiRet cva_macsec_create_rx_sc(void)
{
    uint8      sci[8]   = {0x00u, 0xE0u, 0x4Cu, 0x11u, 0x22u, 0x33u, 0x00u, 0x01u};
    uint8      an       = 3u;
    uint8      entryIdx = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u);
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step1. Create Rx SA Match entry to match specific SCI and AN value and map to flow ctrl index 0 */
    MacsecMatchRuleParams_t matchRule = {
        .port      = CVA_RTK_USER_PORT_1ST,
        .direction = EM_RTK_MACSEC_DIRECTION_RX,
        .entryIdx  = entryIdx};

    matchRule.params.modifiedMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_SCI);
    matchRule.params.enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_SCI);
    CHK_FUN_RET(retVal, cva_drv_drv_memcpy(matchRule.params.sci, sci, 8u));

    matchRule.params.modifiedMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_AN_0);
    matchRule.params.modifiedMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_AN_1);
    matchRule.params.enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_AN_0);
    matchRule.params.enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_AN_1);
    matchRule.params.an = an;

    matchRule.params.modifiedMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_FLOW_CTRL);
    matchRule.params.enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_FLOW_CTRL);
    /* it is recommed to used same idx for match rule and flow control */
    matchRule.params.flowCtrlIdx = entryIdx;

    CHK_FUN_RET(retVal, rtk_macsec_set_match_rule_params(unitChip, &matchRule));

    /* Step2. Get runtime Rx SA Match rule  */
    MacsecMatchRuleParams_t runtimeMatchRuleCfg = {0};
    CHK_FUN_RET(retVal, rtk_macsec_get_match_rule_params(unitChip, &runtimeMatchRuleCfg));
    /*  Expected Behaviour: the runtime data should match the config data */

    /* Step3. Create Rx flow control entry with strict MACsec setting and enable replay protect */
    MacsecFlowControlEntry_t flowCtrl = {
        .port      = CVA_RTK_USER_PORT_1ST,
        .direction = EM_RTK_MACSEC_DIRECTION_TX,
        .entryIdx  = entryIdx};
    flowCtrl.params.saInUse        = true;
    flowCtrl.params.replayProtect  = true;
    flowCtrl.params.validateFrames = (uint8)EM_RTK_MKA_VALID_FRAMES_STRICT;
    /* it is recommed to used same idx for flow control and SA entry */
    flowCtrl.params.saEntryIdx = entryIdx;
    CHK_FUN_RET(retVal, rtk_macsec_set_flow_control_entry(unitChip, &flowCtrl));
    /*  Expected Behaviour: This flow control is ready and the Rx SC is created successfully */

    /* Step4. Get Rx flow control entry */
    MacsecFlowControlEntry_t runtimeFlowCtrl = {0};
    CHK_FUN_RET(retVal, rtk_macsec_set_flow_control_entry(unitChip, &runtimeFlowCtrl));
    /* Expected Behaviour: the runtime data should match the config data */

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_CREATE_RX_SC */

/**
 * @addtogroup RTL907XC_VA_MACSEC_CREATE_TX_SC MACsec Create TX SC Example
 *
 * @brief <b> Create MACsec egress Secure Channel </b> \n
 *            Create an egress MACsec secure channel with AN = 3. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to create an egress MACsec secure channel.
 *
 * @section Description
 *          This example demonstrates how to create an egress secure channel for AN=3,   \n
 *          which will use the same hardware entry as AN=1.
 *          For this secure channel, we need to add a match rule to match packets whose source port is the controlled port \n
 *          and specify their flow control index to be the same value. \n
 *          In the flow control settings, we enabled the confidentiality feature, \n
 *          so in the future, all MACsec packets will be encrypted in addition to having integrity protection. \n
 *          Finally, we need to assign the corresponding SA entry index (recommended to use the same value) to this flow control entry. \n
 *          Subsequent SAK configurations also need to be set on the corresponding SA entry.
 *
 * @{
 */
RtkApiRet cva_macsec_create_tx_sc(void)
{
    uint8      an       = 3u;
    uint8      entryIdx = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u);
    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step1. Create Tx SA Match entry to match all packets from controlled port */
    MacsecMatchRuleParams_t matchRule = {
        .port      = CVA_RTK_USER_PORT_1ST,
        .direction = EM_RTK_MACSEC_DIRECTION_TX,
        .entryIdx  = entryIdx};

    matchRule.params.modifiedMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_SOURCE_PORT);
    matchRule.params.enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_SOURCE_PORT);
    matchRule.params.sourcePort = RTK_MACSEC_SOURCE_PORT_CONTROLLED_PORT;

    matchRule.params.modifiedMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_FLOW_CTRL);
    matchRule.params.enableMask |= ((uint16)1u << RTK_MACSEC_SA_MATCH_FLOW_CTRL);
    /* it is recommed to used same idx for match rule and flow control */
    matchRule.params.flowCtrlIdx = entryIdx;

    CHK_FUN_RET(retVal, rtk_macsec_set_match_rule_params(unitChip, &matchRule));

    /* Step2. Get runtime Rx SA Match rule  */
    MacsecMatchRuleParams_t runtimeMatchRuleCfg = {0};
    CHK_FUN_RET(retVal, rtk_macsec_get_match_rule_params(unitChip, &runtimeMatchRuleCfg));
    /* Expected Behaviour: the runtime data should match the config data */

    /* Step3. Create Tx flow control entry with encryption */
    MacsecFlowControlEntry_t flowCtrl = {
        .port      = CVA_RTK_USER_PORT_1ST,
        .direction = EM_RTK_MACSEC_DIRECTION_TX,
        .entryIdx  = entryIdx};

    flowCtrl.params.saInUse      = true;
    flowCtrl.params.protectFrame = true;
    flowCtrl.params.confProtect  = true;
    /* it is recommed to used same idx for flow control and SA entry */
    flowCtrl.params.saEntryIdx = entryIdx;
    CHK_FUN_RET(retVal, rtk_macsec_set_flow_control_entry(unitChip, &flowCtrl));
    /* Expected Behaviour: This flow control is ready and the Tx SC is created successfully */

    /* Step4. Get Tx flow control entry */
    MacsecFlowControlEntry_t runtimeFlowCtrl = {0};
    CHK_FUN_RET(retVal, rtk_macsec_set_flow_control_entry(unitChip, &runtimeFlowCtrl));
    /* Expected Behaviour: the runtime data should match the config data */

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_CREATE_TX_SC */

/**
 * @addtogroup RTL907XC_VA_MACSEC_CREATE_RX_SA MACsec Create RX SA Example
 *
 * @brief <b> Create MACsec ingress MACSsec SA </b> \n
 *            Create an ingress MACsec SA with specific SAK and other parameters. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to create an egress MACsec SA.
 *
 * @section Description
 *          This example demonstrates how to create an ingress MACsec SA with \n
 *          SAK:    AD-7A-2B-D0-3E-AC-83-5A-6F-62-0F-DC-B5-06-B3-45. \n
 *          (HKEY): 73-A2-3D-80-12-1D-E2-D5-a8-50-25-3F-CF-43-12-0E \n
 *          SCI:    00-E0-4C-11-22-33-00-01 \n
 *          ciphersuite: AES_GCM_128 \n
 *          AN: 3 \n
 *
 * @{
 */
RtkApiRet cva_macsec_create_rx_sa(void)
{
    uint8 sci[8]   = {0x00u, 0xE0u, 0x4Cu, 0x11u, 0x22u, 0x33u, 0x00u, 0x01u};
    uint8 sak[16]  = {0xADu, 0x7Au, 0x2Bu, 0xD0u, 0x3Eu, 0xACu, 0x83u, 0x5Au, 0x6Fu, 0x62u, 0x0Fu, 0xDCu, 0xB5u, 0x06u, 0xB3u, 0x45u};
    uint8 hkey[16] = {0x73u, 0xA2u, 0x3Du, 0x80u, 0x12u, 0x1Du, 0xE2u, 0xD5u, 0xA8u, 0x50u, 0x25u, 0x3Fu, 0xCFu, 0x43u, 0x12u, 0x0Eu};
    uint8 an       = 3u;
    uint8 entryIdx = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u);

    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step1. Create Rx SA entry with necessary parameters */
    MacsecSaEntry_t saEntry =
        {
            .port        = CVA_RTK_USER_PORT_1ST,
            .direction   = EM_RTK_MACSEC_DIRECTION_RX,
            .entryIdx    = entryIdx,
            .cipherSuite = RTK_MACSEC_CIPHERSUITE_GCM_128};

    saEntry.params.an           = 3u;
    saEntry.params.pn           = 0u;
    saEntry.params.replayWindow = 100u;
    CHK_FUN_RET(retVal, cva_drv_drv_memcpy(saEntry.params.sci, sci, sizeof(saEntry.params.sci)));
    CHK_FUN_RET(retVal, cva_drv_drv_memcpy(saEntry.params.key, sak, sizeof(saEntry.params.key)));
    CHK_FUN_RET(retVal, cva_drv_drv_memcpy(saEntry.params.hkey, hkey, sizeof(saEntry.params.hkey)));

    CHK_FUN_RET(retVal, rtk_macsec_set_sa_entry(unitChip, &saEntry));

    /* Step2. Get runtime Rx SA entry */
    MacsecSaEntry_t runtimeCfg =
        {
            .port        = CVA_RTK_USER_PORT_1ST,
            .direction   = EM_RTK_MACSEC_DIRECTION_TX,
            .entryIdx    = entryIdx,
            .cipherSuite = RTK_MACSEC_CIPHERSUITE_GCM_128,
        };
    CHK_FUN_RET(retVal, rtk_macsec_get_sa_entry(unitChip, &runtimeCfg));

    /* Expected Behaviour: this Rx SA is configuration and ready for enable this Rx flow */

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_CREATE_RX_SA */

/**
 * @addtogroup RTL907XC_VA_MACSEC_CREATE_TX_SA MACsec Create TX SA Example
 *
 * @brief <b> Create MACsec egress MACSsec SA </b> \n
 *            Create an egress MACsec SA with specific SAK and other parameters. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to create an egress MACsec SA.
 *
 * @section Description
 *          This example demonstrates how to create an egress MACsec SA with \n
 *          SAK:    AD-7A-2B-D0-3E-AC-83-5A-6F-62-0F-DC-B5-06-B3-45. \n
 *          (HKEY): 73-A2-3D-80-12-1D-E2-D5-a8-50-25-3F-CF-43-12-0E \n
 *          SCI:    00-E0-4C-11-22-33-00-01 \n
 *          ciphersuite: AES_GCM_128 \n
 *          AN: 3 \n
 *
 * @{
 */
RtkApiRet cva_macsec_create_tx_sa(void)
{
    uint8 sci[8]   = {0x00u, 0xE0u, 0x4Cu, 0x11u, 0x22u, 0x33u, 0x00u, 0x01u};
    uint8 sak[16]  = {0xADu, 0x7Au, 0x2Bu, 0xD0u, 0x3Eu, 0xACu, 0x83u, 0x5Au, 0x6Fu, 0x62u, 0x0Fu, 0xDCu, 0xB5u, 0x06u, 0xB3u, 0x45u};
    uint8 hkey[16] = {0x73u, 0xA2u, 0x3Du, 0x80u, 0x12u, 0x1Du, 0xE2u, 0xD5u, 0xA8u, 0x50u, 0x25u, 0x3Fu, 0xCFu, 0x43u, 0x12u, 0x0Eu};
    uint8 an       = 3u;
    uint8 entryIdx = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u);

    RtkApiRet  retVal   = RT_ERR_OK;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step1. Create Tx SA Match entry to match all packets from controlled port */
    MacsecSaEntry_t saEntry =
        {
            .port        = CVA_RTK_USER_PORT_1ST,
            .direction   = EM_RTK_MACSEC_DIRECTION_TX,
            .entryIdx    = entryIdx,
            .cipherSuite = RTK_MACSEC_CIPHERSUITE_GCM_128,
        };

    saEntry.params.an = an;
    saEntry.params.pn = 0u;
    CHK_FUN_RET(retVal, cva_drv_drv_memcpy(saEntry.params.sci, sci, sizeof(saEntry.params.sci)));
    CHK_FUN_RET(retVal, cva_drv_drv_memcpy(saEntry.params.key, sak, sizeof(saEntry.params.key)));
    CHK_FUN_RET(retVal, cva_drv_drv_memcpy(saEntry.params.hkey, hkey, sizeof(saEntry.params.hkey)));

    CHK_FUN_RET(retVal, rtk_macsec_set_sa_entry(unitChip, &saEntry));

    /* Step2. Get runtime Tx SA entry */
    MacsecSaEntry_t runtimeCfg =
        {
            .port        = CVA_RTK_USER_PORT_1ST,
            .direction   = EM_RTK_MACSEC_DIRECTION_TX,
            .entryIdx    = entryIdx,
            .cipherSuite = RTK_MACSEC_CIPHERSUITE_GCM_128,
        };
    CHK_FUN_RET(retVal, rtk_macsec_get_sa_entry(unitChip, &runtimeCfg));

    /* Expected Behaviour: this tx sa is configuration and ready for enable this tx flow */

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_CREATE_TX_SA */

/**
 * @addtogroup RTL907XC_VA_MACSEC_ENABLE_RX_SA MACsec Enable RX SA Example
 *
 * @brief <b> Enable MACsec ingress MACSsec SA </b> \n
 *            Enable an ingress MACsec SA. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to enable an ingress MACsec SA.
 *
 * @section Description
 *          This example demonstrates how to enable an ingress MACsec SA. \n
 *          After enabling, all data packets using this SA will start using the specified SAK \n
 *          for confidentiality and/or integrity protection.
 *
 * @{
 */
RtkApiRet cva_macsec_enable_rx_sa(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    uint8      an       = 3u;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step1. Enable this match rule, it means all packets match this match rule now will be processed by Rx MACsec engine */
    MacsecMatchRuleStatus_t matchRuleStatus =
        {
            .port      = CVA_RTK_USER_PORT_1ST,
            .direction = EM_RTK_MACSEC_DIRECTION_RX,
            .entryIdx  = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u),
            .enable    = true};
    CHK_FUN_RET(retVal, rtk_macsec_set_match_rule_status(unitChip, &matchRuleStatus));

    /* Step2. Get the status of this match rule, it should be true */
    MacsecMatchRuleStatus_t runTimeStatus =
        {
            .port      = CVA_RTK_USER_PORT_1ST,
            .direction = EM_RTK_MACSEC_DIRECTION_RX,
            .entryIdx  = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u)};
    CHK_FUN_RET(retVal, rtk_macsec_get_match_rule_status(unitChip, &runTimeStatus));

    /* Expected Behaviour: this rx flow is active */

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_ENABLE_RX_SA */

/**
 * @addtogroup RTL907XC_VA_MACSEC_ENABLE_TX_SA MACsec Enable TX SA Example
 *
 * @brief <b> Enable MACsec egress MACSsec SA </b> \n
 *            Enable an egress MACsec SA. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to enable an egress MACsec SA.
 *
 * @section Description
 *          This example demonstrates how to enable an egress MACsec SA. \n
 *          After enabling, all data packets using this SA will start using the specified SAK \n
 *          for confidentiality and/or integrity protection.
 *
 * @{
 */
RtkApiRet cva_macsec_enable_tx_sa(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    uint8      an       = 3u;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step1. Enable this match rule, it means all packets match this match rule now will be processed by Tx MACsec engine */
    MacsecMatchRuleStatus_t matchRuleStatus =
        {
            .port      = CVA_RTK_USER_PORT_1ST,
            .direction = EM_RTK_MACSEC_DIRECTION_TX,
            .entryIdx  = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u),
            .enable    = true};
    CHK_FUN_RET(retVal, rtk_macsec_set_match_rule_status(unitChip, &matchRuleStatus));

    /* Step2. Get the status of this match rule, it should be true */
    MacsecMatchRuleStatus_t runTimeStatus =
        {
            .port      = CVA_RTK_USER_PORT_1ST,
            .direction = EM_RTK_MACSEC_DIRECTION_TX,
            .entryIdx  = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u)};
    CHK_FUN_RET(retVal, rtk_macsec_get_match_rule_status(unitChip, &runTimeStatus));

    /* Expected Behaviour: this tx flow is active */

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_ENABLE_TX_SA */

/**
 * @addtogroup RTL907XC_VA_MACSEC_DISABLE_RX_SA MACsec Disable RX SA Example
 *
 * @brief <b> Disable MACsec ingress MACSsec SA </b> \n
 *            Disable an ingress MACsec SA. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to disable an ingress MACsec SA
 *
 * @section Description
 *          This example demonstrates how to disable an ingress MACsec SA. \n
 *          After disabling, all data packets using this SA will be discarded \n
 *          due to the inability to pass the confidentiality and/or integrity checks.
 *
 * @{
 */
RtkApiRet cva_macsec_disable_rx_sa(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    uint8      an       = 3u;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step1. Disable this match rule, it means all packets match this match rule now will not be processed by Rx MACsec engine */
    MacsecMatchRuleStatus_t matchRuleStatus =
        {
            .port      = CVA_RTK_USER_PORT_1ST,
            .direction = EM_RTK_MACSEC_DIRECTION_RX,
            .entryIdx  = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u),
            .enable    = false};
    CHK_FUN_RET(retVal, rtk_macsec_set_match_rule_status(unitChip, &matchRuleStatus));

    /* Step2. Get the status of this match rule, it should be false */
    MacsecMatchRuleStatus_t runTimeStatus =
        {
            .port      = CVA_RTK_USER_PORT_1ST,
            .direction = EM_RTK_MACSEC_DIRECTION_RX,
            .entryIdx  = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u)};
    CHK_FUN_RET(retVal, rtk_macsec_get_match_rule_status(unitChip, &runTimeStatus));

    /* Expected Behaviour: this rx flow is inactive */

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_DISABLE_RX_SA */

/**
 * @addtogroup RTL907XC_VA_MACSEC_DISABLE_TX_SA MACsec Disable TX SA Example
 *
 * @brief <b> Disable MACsec egress MACSsec SA </b> \n
 *            Disable an egress MACsec SA. \n
 *
 * @section Purpose
 *          The purpose of this function is to demonstrate how to disable an egress MACsec SA
 *
 * @section Description
 *          This example demonstrates how to disable an egress MACsec SA. \n
 *          After disabling, all data packets using this SA will be discarded \n
 *          due to the inability to pass the confidentiality and/or integrity checks.
 *
 * @{
 */
RtkApiRet cva_macsec_disable_tx_sa(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    uint8      an       = 3u;
    UnitChip_t unitChip = {
        .unit = CVA_UNIT,
        .chip = CHIP_907XC_VA};

    /* Step1. Disable this match rule, it means all packets match this match rule now will not be processed by Rx MACsec engine */
    MacsecMatchRuleStatus_t matchRuleStatus =
        {
            .port      = CVA_RTK_USER_PORT_1ST,
            .direction = EM_RTK_MACSEC_DIRECTION_TX,
            .entryIdx  = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u),
            .enable    = false};
    CHK_FUN_RET(retVal, rtk_macsec_set_match_rule_status(unitChip, &matchRuleStatus));

    /* Step2. Get the status of this match rule, it should be false */
    MacsecMatchRuleStatus_t runTimeStatus =
        {
            .port      = CVA_RTK_USER_PORT_1ST,
            .direction = EM_RTK_MACSEC_DIRECTION_TX,
            .entryIdx  = RTK_MACSEC_MIN_ENTRY_IDX + (an % 2u)};
    CHK_FUN_RET(retVal, rtk_macsec_get_match_rule_status(unitChip, &runTimeStatus));

    /* Expected Behaviour: this tx flow is inactive */

    return retVal;
}

/**@}*/ /* RTL907XC_VA_MACSEC_DISABLE_TX_SA */
/**@}*/ /* RTL907XC_VA_MACSEC_EXAMPLE */
/**@}*/ /* MACSEC_EXAMPLE */
/**@}*/ /* EXAMPLE */