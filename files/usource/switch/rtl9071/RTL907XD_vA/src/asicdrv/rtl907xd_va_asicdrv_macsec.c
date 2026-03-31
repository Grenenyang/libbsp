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
#include "rtkas_types.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv_macsec.h"
#include "rtl907xd_va_api_macsec.h"
#include "rtl907xd_va_api_esm.h"
#include "rtkas_system_porting.h"
/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DVA_MACSEC_TX_DATA_REG_BASE (DVA_PORT0_MACSEC_REG_RWD_AE)
#define DVA_MACSEC_TX_ADDR_REG_BASE (DVA_PORT0_MACSEC_REG_ADDR_AE)
#define DVA_MACSEC_TX_COMD_REG_BASE (DVA_PORT0_MACSEC_REG_CMD_AE)
#define DVA_MACSEC_RX_DATA_REG_BASE (DVA_PORT0_MACSEC_REG_RWD_AI)
#define DVA_MACSEC_RX_ADDR_REG_BASE (DVA_PORT0_MACSEC_REG_ADDR_AI)
#define DVA_MACSEC_RX_COMD_REG_BASE (DVA_PORT0_MACSEC_REG_CMD_AI)

#define DVA_MACSEC_TX_CMD_READ  (DVA_PORT0_MACSEC_REG_CMD_AE_REG_RD_REQ_AE_LSP)
#define DVA_MACSEC_TX_CMD_WRITE (DVA_PORT0_MACSEC_REG_CMD_AE_REG_WR_REQ_AE_LSP)
#define DVA_MACSEC_RX_CMD_READ  (DVA_PORT0_MACSEC_REG_CMD_AI_REG_RD_REQ_AI_LSP)
#define DVA_MACSEC_RX_CMD_WRITE (DVA_PORT0_MACSEC_REG_CMD_AI_REG_WR_REQ_AI_LSP)

#define DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY  (32u)
#define DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY (32u)
#define DVA_MACSEC_SECY_SA_MAX_ENTRY        (32u)
#define DVA_MACSEC_SECY_CTRL_PACKET_ENTRY   (8u)

#define DVA_MACSEC_SECY_SAM_ENTRY_OFFSET          ((uint32)0x40u)
#define DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET    ((uint32)0x4u)
#define DVA_MACSEC_SECY_COUNTER_ITEM_OFFSET       ((uint32)0x8u)
#define DVA_MACSEC_SECY_SA_COUNTER_ENTRY_OFFSET   ((uint32)0x80u)
#define DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET ((uint32)0x60u)
#define DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET ((uint32)0x50u)
#define DVA_MACSEC_SECY_CTRL_PACKET_DA_OFFSET     ((uint32)0x8u)
#define DVA_MACSEC_SECY_CTRL_PACKET_ETHER_OFFSET  ((uint32)0x4u)

#define DVA_MACSEC_SECY_SAM_MAC_SA_MATCH_LO_BASE ((uint32)0x4000u)
#define DVA_MACSEC_SECY_SAM_MAC_SA_MATCH_HI_BASE ((uint32)0x4004u)
#define DVA_MACSEC_SECY_SAM_MAC_DA_MATCH_LO_BASE ((uint32)0x4008u)
#define DVA_MACSEC_SECY_SAM_MAC_DA_MATCH_HI_BASE ((uint32)0x400cu)
#define DVA_MACSEC_SECY_SAM_MISC_MATCH_BASE      ((uint32)0x4010u)
#define DVA_MACSEC_SECY_SAM_SCI_MATCH_LO_BASE    ((uint32)0x4014u)
#define DVA_MACSEC_SECY_SAM_SCI_MATCH_HI_BASE    ((uint32)0x4018u)
#define DVA_MACSEC_SECY_SAM_MASK_BASE            ((uint32)0x401cu)
#define DVA_MACSEC_SECY_SAM_EXT_MATCH_BASE       ((uint32)0x4020u)
#define DVA_MACSEC_SECY_SAM_ENTRY_ENABLE_BASE    ((uint32)0x6000u)
#define DVA_MACSEC_SECY_SAM_ENTRY_SET_BASE       ((uint32)0x6080u)
#define DVA_MACSEC_SECY_SAM_ENTRY_CLEAR_BASE     ((uint32)0x60c0u)
#define DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE       ((uint32)0x7000u)
#define DVA_MACSEC_SECY_SA_COUNTER_BASE          ((uint32)0x8000u)
#define DVA_MACSEC_SECY_GLOBAL_COUNTER_BASE      ((uint32)0xC400u)
#define DVA_MACSEC_SECY_COUNT_CONTROL            ((uint32)0xC810u)

/* 128 bits */
/* common */
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_CTRL   ((uint32)0x0000u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_ID     ((uint32)0x0004u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_KEY_0  ((uint32)0x0008u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_KEY_1  ((uint32)0x000Cu)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_KEY_2  ((uint32)0x0010u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_KEY_3  ((uint32)0x0014u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_HKEY_0 ((uint32)0x0018u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_HKEY_1 ((uint32)0x001Cu)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_HKEY_2 ((uint32)0x0020u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_HKEY_3 ((uint32)0x0024u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_SEQ    ((uint32)0x0028u)
/* Egress */
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_EGRESS_IV0         ((uint32)0x002Cu)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_EGRESS_IV1         ((uint32)0x0030u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_EGRESS_SA_UPD_CTRL ((uint32)0x003Cu)
/* Ingress Common */
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_INGRESS_MASK ((uint32)0x002Cu)
/* Ingress impl SCI */
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_INGRESS_IV0 ((uint32)0x0030u)
#define DVA_MACSEC_SECY_TRANSFORM_128BIT_INGRESS_IV1 ((uint32)0x0034u)
/* 192/256 bits */
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_CTRL   ((uint32)0x0000u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_ID     ((uint32)0x0004u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_KEY_0  ((uint32)0x0008u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_KEY_1  ((uint32)0x000Cu)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_KEY_2  ((uint32)0x0010u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_KEY_3  ((uint32)0x0014u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_KEY_4  ((uint32)0x0018u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_KEY_5  ((uint32)0x001Cu)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_KEY_6  ((uint32)0x0020u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_KEY_7  ((uint32)0x0024u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_HKEY_0 ((uint32)0x0028u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_HKEY_1 ((uint32)0x002cu)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_HKEY_2 ((uint32)0x0030u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_HKEY_3 ((uint32)0x0034u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_SEQ    ((uint32)0x0038u)
/* Egress */
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_EGRESS_IV0         ((uint32)0x003Cu)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_EGRESS_IV1         ((uint32)0x0040u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_EGRESS_SA_UPD_CTRL ((uint32)0x004Cu)
/* Ingress Common */
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_INGRESS_MASK ((uint32)0x003Cu)
/* Ingress impl SCI */
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_INGRESS_IV0 ((uint32)0x0040u)
#define DVA_MACSEC_SECY_TRANSFORM_256BIT_INGRESS_IV1 ((uint32)0x0044u)
/* XPN */
/* 128 bits */
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_SEQ_0 ((uint32)0x0028u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_SEQ_1 ((uint32)0x002Cu)
/* Ingress */
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_INGRESS_MASK ((uint32)0x0030u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_INGRESS_IV0  ((uint32)0x0034u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_INGRESS_IV1  ((uint32)0x0038u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_INGRESS_IV2  ((uint32)0x003Cu)
/* Egress */
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_EGRESS_IS0  ((uint32)0x0034u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_EGRESS_IS1  ((uint32)0x0038u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_EGRESS_IS2  ((uint32)0x003Cu)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_EGRESS_IV0  ((uint32)0x0040u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_EGRESS_IV1  ((uint32)0x0044u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_SA_UPD_CTRL ((uint32)0x004Cu)
/* 256 bits */
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_SEQ_0 ((uint32)0x0038u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_SEQ_1 ((uint32)0x003Cu)
/* Ingress */
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_INGRESS_MASK ((uint32)0x0040u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_INGRESS_IV0  ((uint32)0x0044u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_INGRESS_IV1  ((uint32)0x0048u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_INGRESS_IV2  ((uint32)0x004Cu)
/* Egress */
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_EGRESS_IS0  ((uint32)0x0044u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_EGRESS_IS1  ((uint32)0x0048u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_EGRESS_IS2  ((uint32)0x004Cu)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_EGRESS_IV0  ((uint32)0x0050u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_EGRESS_IV1  ((uint32)0x0054u)
#define DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_SA_UPD_CTRL ((uint32)0x005Cu)

/* Control Packet Classification */

#define DVA_MACSEC_SECY_CP_MAC_DA_MATCH_0              ((uint32)0x7800u)
#define DVA_MACSEC_SECY_CP_MAC_DA_ET_MATCH_0           ((uint32)0x7804u)
#define DVA_MACSEC_SECY_CP_ET_MATCH_10                 ((uint32)0x7850u)
#define DVA_MACSEC_SECY_CP_MATCH_MODE                  ((uint32)0x78F8u)
#define DVA_MACSEC_SECY_CP_MATCH_ENABLE                ((uint32)0x78FCu)
#define DVA_MACSEC_SECY_CP_MATCH_ENABLE_MAC_DA_BIT     (0u)
#define DVA_MACSEC_SECY_CP_MATCH_ENABLE_ETHER_TYPE_BIT (21u)

/* vlan tag parser */
#define DVA_MACSEC_SECY_VLAN_PARSER_CONTROL                ((uint32)0x7900u)
#define DVA_MACSEC_SECY_VLAN_TAGS_TPID                     ((uint32)0x7904u)
#define DVA_MACSEC_SECY_VLAN_TAGS_TPID2                    ((uint32)0x7908u)
#define DVA_MACSEC_SECY_VLAN_PARSER_CONTROL_Q_IN_Q_BIT     (29u)
#define DVA_MACSEC_SECY_VLAN_PARSER_CONTROL_PARSE_STAG_BIT (30u)

/* debug */
#define DVA_MACSEC_SECY_SECTAG_IDENTIFIED_DEBUG ((uint32)0x7c24u)
#define DVA_MACSEC_SECY_VLAN_PARSER2_DEBUG      ((uint32)0x7c84u)

#define DVA_MACSEC_INDIRECT_ACCESS_TIMEOUT (50u)

#define DVA_MACSEC_DEFAULT_VLAN_PARSER_REG_VALUE (0x98FAC688u)
/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
typedef RtkApiRet (*dva_MacsecRegGetFunc)(uint32 unit, RtkPort sysPort, uint32 addr, uint32 *data);
typedef RtkApiRet (*dva_MacsecRegSetFunc)(uint32 unit, RtkPort sysPort, uint32 addr, uint32 data);

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
static RtkApiRet dva_macsec_register_tx_set(uint32 unit, RtkPort sysPort, uint32 addr, uint32 data);
static RtkApiRet dva_macsec_register_tx_get(uint32 unit, RtkPort sysPort, uint32 addr, uint32 *data);
static RtkApiRet dva_macsec_register_rx_set(uint32 unit, RtkPort sysPort, uint32 addr, uint32 data);
static RtkApiRet dva_macsec_register_rx_get(uint32 unit, RtkPort sysPort, uint32 addr, uint32 *data);

/******************************************************************************/
/* Global Variables                                                           */
/******************************************************************************/
/* clang-format off */
static const dva_MacsecRegGetFunc dva_g_regGet[2] =
{
        [EM_RTK_MACSEC_DIRECTION_TX] = &dva_macsec_register_tx_get,
        [EM_RTK_MACSEC_DIRECTION_RX] = &dva_macsec_register_rx_get,
};

static const dva_MacsecRegSetFunc dva_g_regSet[2] =
{
        [EM_RTK_MACSEC_DIRECTION_TX] = &dva_macsec_register_tx_set,
        [EM_RTK_MACSEC_DIRECTION_RX] = &dva_macsec_register_rx_set,
};

static const uint32 dva_g_xfromRecordIv0Addr[2][4] =
{
    [EM_RTK_MACSEC_DIRECTION_TX] = 
    {
        [RTK_MACSEC_CIPHERSUITE_GCM_128]     = DVA_MACSEC_SECY_TRANSFORM_128BIT_EGRESS_IV0,
        [RTK_MACSEC_CIPHERSUITE_GCM_256]     = DVA_MACSEC_SECY_TRANSFORM_256BIT_EGRESS_IV0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_128] = DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_EGRESS_IV0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_256] = DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_EGRESS_IV0,
    },
    [EM_RTK_MACSEC_DIRECTION_RX] = 
    {
        [RTK_MACSEC_CIPHERSUITE_GCM_128]     = DVA_MACSEC_SECY_TRANSFORM_128BIT_INGRESS_IV0,
        [RTK_MACSEC_CIPHERSUITE_GCM_256]     = DVA_MACSEC_SECY_TRANSFORM_256BIT_INGRESS_IV0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_128] = DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_INGRESS_IV0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_256] = DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_INGRESS_IV0,
    }
};

static const uint32 dva_g_xfromRecordSeqAddr[2][4] =
{
    [EM_RTK_MACSEC_DIRECTION_TX] = 
    {
        [RTK_MACSEC_CIPHERSUITE_GCM_128]     = DVA_MACSEC_SECY_TRANSFORM_128BIT_SEQ,
        [RTK_MACSEC_CIPHERSUITE_GCM_256]     = DVA_MACSEC_SECY_TRANSFORM_256BIT_SEQ,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_128] = DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_SEQ_0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_256] = DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_SEQ_0,
    },
    [EM_RTK_MACSEC_DIRECTION_RX] = 
    {
        [RTK_MACSEC_CIPHERSUITE_GCM_128]     = DVA_MACSEC_SECY_TRANSFORM_128BIT_SEQ,
        [RTK_MACSEC_CIPHERSUITE_GCM_256]     = DVA_MACSEC_SECY_TRANSFORM_256BIT_SEQ,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_128] = DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_SEQ_0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_256] = DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_SEQ_0,
    }
};

static const uint32 dva_g_xfromRecordMaskAddr[4] =
{
    [RTK_MACSEC_CIPHERSUITE_GCM_128]     = DVA_MACSEC_SECY_TRANSFORM_128BIT_INGRESS_MASK,
    [RTK_MACSEC_CIPHERSUITE_GCM_256]     = DVA_MACSEC_SECY_TRANSFORM_256BIT_INGRESS_MASK,
    [RTK_MACSEC_CIPHERSUITE_GCM_XPN_128] = DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_INGRESS_MASK,
    [RTK_MACSEC_CIPHERSUITE_GCM_XPN_256] = DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_INGRESS_MASK,
};

static const uint32 dva_g_xfromRecordCtxSaltAddr[2][4] =
{
    [EM_RTK_MACSEC_DIRECTION_TX] = 
    {
        [RTK_MACSEC_CIPHERSUITE_GCM_256]     = 0,
        [RTK_MACSEC_CIPHERSUITE_GCM_128]     = 0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_128] = DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_EGRESS_IS0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_256] = DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_EGRESS_IS0,
    },
    [EM_RTK_MACSEC_DIRECTION_RX] = 
    {
        [RTK_MACSEC_CIPHERSUITE_GCM_128]     = 0,
        [RTK_MACSEC_CIPHERSUITE_GCM_256]     = 0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_128] = DVA_MACSEC_SECY_TRANSFORM_XPN_128BIT_INGRESS_IV0,
        [RTK_MACSEC_CIPHERSUITE_GCM_XPN_256] = DVA_MACSEC_SECY_TRANSFORM_XPN_256BIT_INGRESS_IV0,
    }
};
/* clang-format on */

/******************************************************************************/
/* Functions Definition                                                       */
/******************************************************************************/
static inline bool dva_is_macsec_command_done(uint32 unit, uint32 addr, uint32 bit)
{
    uint32 timeout = DVA_MACSEC_INDIRECT_ACCESS_TIMEOUT;
    uint32 data    = 0u;

    (void)dva_reg_read(unit, addr, &data);
    while((timeout > 0u) && ((data & ((uint32)1u << bit)) != 0u))
    {
        (void)dva_reg_read(unit, addr, &data);
        (void)rtk_usecond_delay(1u);
        timeout--;
    }

    return (timeout != 0u);
}

static inline bool dva_is_xpn(uint32 cipherSuite)
{
    return ((cipherSuite == RTK_MACSEC_CIPHERSUITE_GCM_XPN_128) || (cipherSuite == RTK_MACSEC_CIPHERSUITE_GCM_XPN_256));
}

static inline bool dva_is_256bit_sak(uint32 cipherSuite)
{
    return ((cipherSuite == RTK_MACSEC_CIPHERSUITE_GCM_256) || (cipherSuite == RTK_MACSEC_CIPHERSUITE_GCM_XPN_256));
}

static inline uint32 dva_macsec_reg_value_field_set(uint32 regVal, uint32 offset, uint32 len, uint32 fieldVal)
{
    uint32 mask      = (((uint32)1u << len) - (uint32)1u);
    uint32 newRegVal = regVal;

    newRegVal &= (~(mask << offset));
    newRegVal |= ((fieldVal & mask) << offset);

    return newRegVal;
}

static inline uint32 dva_macsec_reg_value_field_get(uint32 regVal, uint32 offset, uint32 len)
{
    uint32 mask = (((uint32)1u << len) - (uint32)1u);

    return ((regVal >> offset) & mask);
}

static RtkApiRet dva_macsec_register_tx_set(uint32 unit, RtkPort sysPort, uint32 addr, uint32 data)
{
    RtkApiRet retCode = RT_ERR_OK;

    uint32 dataAddr    = DVA_MACSEC_TX_DATA_REG_BASE + (PERPORT_REG_OFFSET * sysPort);
    uint32 addrAddr    = DVA_MACSEC_TX_ADDR_REG_BASE + (PERPORT_REG_OFFSET * sysPort);
    uint32 commandAddr = DVA_MACSEC_TX_COMD_REG_BASE + (PERPORT_REG_OFFSET * sysPort);

    CHK_FUN_RET(retCode, dva_reg_write(unit, addrAddr, addr));
    CHK_FUN_RET(retCode, dva_reg_write(unit, dataAddr, data));
    CHK_FUN_RET(retCode, dva_reg_write(unit, commandAddr, ((uint32)1u << DVA_MACSEC_TX_CMD_WRITE)));

    if(false == dva_is_macsec_command_done(unit, commandAddr, DVA_MACSEC_TX_CMD_WRITE))
    {
        dva_esm_event_log_set(DVA_EM_EVENT_MACSEC_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
        retCode = RT_ERR_BUSYWAIT_TIMEOUT;
    }

    return retCode;
}

static RtkApiRet dva_macsec_register_tx_get(uint32 unit, RtkPort sysPort, uint32 addr, uint32 *data)
{
    RtkApiRet retCode = RT_ERR_OK;

    uint32 dataAddr    = DVA_MACSEC_TX_DATA_REG_BASE + (PERPORT_REG_OFFSET * sysPort);
    uint32 addrAddr    = DVA_MACSEC_TX_ADDR_REG_BASE + (PERPORT_REG_OFFSET * sysPort);
    uint32 commandAddr = DVA_MACSEC_TX_COMD_REG_BASE + (PERPORT_REG_OFFSET * sysPort);

    CHK_FUN_RET(retCode, dva_reg_write(unit, addrAddr, addr));
    CHK_FUN_RET(retCode, dva_reg_write(unit, commandAddr, ((uint32)1u << DVA_MACSEC_TX_CMD_READ)));

    if(dva_is_macsec_command_done(unit, commandAddr, DVA_MACSEC_TX_CMD_READ))
    {
        CHK_FUN_RET(retCode, dva_reg_read(unit, dataAddr, data));
    }
    else
    {
        dva_esm_event_log_set(DVA_EM_EVENT_MACSEC_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
        retCode = RT_ERR_BUSYWAIT_TIMEOUT;
    }

    return retCode;
}

static RtkApiRet dva_macsec_register_rx_set(uint32 unit, RtkPort sysPort, uint32 addr, uint32 data)
{
    RtkApiRet retCode = RT_ERR_OK;

    uint32 dataAddr    = DVA_MACSEC_RX_DATA_REG_BASE + (PERPORT_REG_OFFSET * sysPort);
    uint32 addrAddr    = DVA_MACSEC_RX_ADDR_REG_BASE + (PERPORT_REG_OFFSET * sysPort);
    uint32 commandAddr = DVA_MACSEC_RX_COMD_REG_BASE + (PERPORT_REG_OFFSET * sysPort);

    CHK_FUN_RET(retCode, dva_reg_write(unit, addrAddr, addr));
    CHK_FUN_RET(retCode, dva_reg_write(unit, dataAddr, data));
    CHK_FUN_RET(retCode, dva_reg_write(unit, commandAddr, 0x1));

    if(false == dva_is_macsec_command_done(unit, commandAddr, DVA_MACSEC_RX_CMD_WRITE))
    {
        dva_esm_event_log_set(DVA_EM_EVENT_MACSEC_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
        retCode = RT_ERR_BUSYWAIT_TIMEOUT;
    }

    return retCode;
}

static RtkApiRet dva_macsec_register_rx_get(uint32 unit, RtkPort sysPort, uint32 addr, uint32 *data)
{
    RtkApiRet retCode = RT_ERR_OK;

    uint32 dataAddr    = DVA_MACSEC_RX_DATA_REG_BASE + (PERPORT_REG_OFFSET * sysPort);
    uint32 addrAddr    = DVA_MACSEC_RX_ADDR_REG_BASE + (PERPORT_REG_OFFSET * sysPort);
    uint32 commandAddr = DVA_MACSEC_RX_COMD_REG_BASE + (PERPORT_REG_OFFSET * sysPort);

    CHK_FUN_RET(retCode, dva_reg_write(unit, addrAddr, addr));
    CHK_FUN_RET(retCode, dva_reg_write(unit, commandAddr, ((uint32)1u << DVA_MACSEC_RX_CMD_READ)));

    if(dva_is_macsec_command_done(unit, commandAddr, DVA_MACSEC_RX_CMD_READ))
    {
        CHK_FUN_RET(retCode, dva_reg_read(unit, dataAddr, data));
    }
    else
    {
        dva_esm_event_log_set(DVA_EM_EVENT_MACSEC_POLLING_TIMEOUT, DVA_EM_EVENT_STATUS_FAILED);
        retCode = RT_ERR_BUSYWAIT_TIMEOUT;
    }

    return retCode;
}

static bool dva_is_params_enabled(uint32 enableMask, uint32 bit)
{
    return ((enableMask & (1UL << bit)) != 0u);
}

static RtkApiRet dva_macsec_params_range_check(uint32 value, uint32 minValue, uint32 maxValue)
{
    return ((minValue <= value) && (value <= maxValue)) ? RT_ERR_OK : (-RT_ERR_OUT_OF_RANGE);
}

bool dva_drv_secy_helper_is_params_modified(uint32 modifiedMask, uint32 bit)
{
    return ((modifiedMask & (1UL << bit)) != 0u);
}

RtkApiRet dva_drv_secy_helper_vlan_match_rule_parameter_check(const RtkMacsecSaMatchEntry_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;

    do
    {
        /* outer vlan id */
        if(dva_is_params_enabled(params->enableMask, RTK_MACSEC_SA_MATCH_OUTER_VLAN_ID))
        {
            CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->outerVlanid, RTK_MACSEC_MIN_VLAN_ID_VALUE, RTK_MACSEC_MAX_VLAN_ID_VALUE));
        }

        /* inner vlan id */
        if(dva_is_params_enabled(params->enableMask, RTK_MACSEC_SA_MATCH_INNER_VLAN_ID))
        {
            CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->innerVlanid, RTK_MACSEC_MIN_VLAN_ID_VALUE, RTK_MACSEC_MAX_VLAN_ID_VALUE));
        }

        /* outer vlan pcp */
        if(dva_is_params_enabled(params->enableMask, RTK_MACSEC_SA_MATCH_OUTER_VLAN_PCP))
        {
            CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->outerVlanPcp, RTK_MACSEC_MIN_VLAN_PCP_VALUE, RTK_MACSEC_MAX_VLAN_PCP_VALUE));
        }

        /* inner vlan pcp */
        if(dva_is_params_enabled(params->enableMask, RTK_MACSEC_SA_MATCH_INNER_VLAN_PCP))
        {
            CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->innerVlanPcp, RTK_MACSEC_MIN_VLAN_PCP_VALUE, RTK_MACSEC_MAX_VLAN_PCP_VALUE));
        }

    } while(false);

    return retCode;
}

RtkApiRet dva_drv_secy_helper_match_rule_parameter_check(const RtkMacsecSaMatchEntry_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;

    do
    {

        CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->entryPriority, RTK_MACSEC_MIN_ENTRY_PRIORITY_VALUE, RTK_MACSEC_MAX_ENTRY_PRIORITY_VALUE));

        /* flow ctrl idx */
        if(dva_is_params_enabled(params->enableMask, RTK_MACSEC_SA_MATCH_FLOW_CTRL))
        {
            CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->flowCtrlIdx, RTK_MACSEC_MIN_ENTRY_IDX, RTK_MACSEC_MAX_ENTRY_IDX));
        }

        /* source port */
        if(dva_is_params_enabled(params->enableMask, RTK_MACSEC_SA_MATCH_SOURCE_PORT))
        {
            CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->sourcePort, RTK_MACSEC_SOURCE_PORT_COMMON_PORT, RTK_MACSEC_SOURCE_PORT_UNCONTROLLED_PORT));
        }

        /* an */
        if(dva_is_params_enabled(params->enableMask, RTK_MACSEC_SA_MATCH_AN_0) && dva_is_params_enabled(params->enableMask, RTK_MACSEC_SA_MATCH_AN_1))
        {
            /* compare 2 bits of AN */
            CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->an, RTK_MACSEC_MIN_AN_VALUE, RTK_MACSEC_MAX_AN_VALUE));
        }
        else if(dva_is_params_enabled(params->enableMask, RTK_MACSEC_SA_MATCH_AN_0))
        {
            /* only compare 1 bit of AN */
            CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->an, RTK_MACSEC_MIN_AN_0_VALUE, RTK_MACSEC_MAX_AN_0_VALUE));
        }
        else
        {
            /* left empty */
        }

    } while(false);

    return retCode;
}

RtkApiRet dva_drv_secy_helper_flow_control_parameter_check(const RtkMacsecFlowControl_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->validateFrames, RTK_MACSEC_VALID_FRAME_DISABLED, RTK_MACSEC_VALID_FRAME_RESERVED));
        CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->confidentialityOffset, RTK_MACSEC_MIN_CONFIDENTIAL_OFFSET, RTK_MACSEC_MAX_CONFIDENTIAL_OFFSET));
        CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->bypassTagSize, RTK_MACSEC_MIN_BYPASS_TAG_SIZE, RTK_MACSEC_MAX_BYPASS_TAG_SIZE));
        CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->saEntryIdx, RTK_MACSEC_MIN_ENTRY_IDX, RTK_MACSEC_MAX_ENTRY_IDX));
    } while(false);

    return retCode;
}

RtkApiRet dva_drv_secy_helper_sa_entry_parameter_check(uint32 cipherSuite, const RtkMacsecSecurityAssociation_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(cipherSuite, RTK_MACSEC_CIPHERSUITE_GCM_128, RTK_MACSEC_CIPHERSUITE_GCM_NONE));
        CHK_FUN_CONTINUE(retCode, dva_macsec_params_range_check(params->an, RTK_MACSEC_MIN_AN_VALUE, RTK_MACSEC_MAX_AN_VALUE));
    } while(false);

    return retCode;
}

RtkApiRet dva_drv_secy_helper_verify_user_port_and_covert_to_sys_port(RtkPort port, RtkPort *sysPort)
{
    RtkApiRet retCode = RT_ERR_OK;

    if((port >= DVA_RTK_USER_PORT_1ST) && (port <= DVA_RTK_USER_PORT_END))
    {
        *sysPort = dva_g_userPort2sysPort[port];
    }
    else
    {
        retCode = -(RT_ERR_PORT_ID);
    }

    return retCode;
}

/* .d88888b   .d888888     8888ba.88ba   .d888888  d888888P  a88888b. dP     dP      888888ba  dP     dP dP         88888888b */
/* 88.    "' d8'    88     88  `8b  `8b d8'    88     88    d8'   `88 88     88      88    `8b 88     88 88         88        */
/* `Y88888b. 88aaaaa88a    88   88   88 88aaaaa88a    88    88        88aaaaa88a    a88aaaa8P' 88     88 88        a88aaaa    */
/*       `8b 88     88     88   88   88 88     88     88    88        88     88      88   `8b. 88     88 88         88        */
/* d8'   .8P 88     88     88   88   88 88     88     88    Y8.   .88 88     88      88     88 Y8.   .8P 88         88        */
/*  Y88888P  88     88     dP   dP   dP 88     88     dP     Y88888P' dP     dP      dP     dP `Y88888P' 88888888P  88888888P */

RtkApiRet dva_drv_secy_mr_mac_sa_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, const uint8 *macSa)
{
    RtkApiRet            retCode          = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet     = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet     = dva_g_regGet[direction];
    uint32               saMatchLoRegAddr = 0u;
    uint32               saMatchHiRegAddr = 0u;
    uint32               value            = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        saMatchLoRegAddr = DVA_MACSEC_SECY_SAM_MAC_SA_MATCH_LO_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        saMatchHiRegAddr = DVA_MACSEC_SECY_SAM_MAC_SA_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);

        value = (macSa[0] | ((uint32)macSa[1] << 8) | (((uint32)macSa[2] << 16)) | ((uint32)macSa[3] << 24));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, saMatchLoRegAddr, value));

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, saMatchHiRegAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 0u, 16u, (macSa[4] | ((uint32)macSa[5] << 8)));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, saMatchHiRegAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_mac_sa_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *macSa)
{
    RtkApiRet            retCode          = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet     = dva_g_regGet[direction];
    uint32               saMatchLoRegAddr = 0u;
    uint32               saMatchHiRegAddr = 0u;
    uint32               value            = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        saMatchLoRegAddr = DVA_MACSEC_SECY_SAM_MAC_SA_MATCH_LO_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        saMatchHiRegAddr = DVA_MACSEC_SECY_SAM_MAC_SA_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, saMatchLoRegAddr, &value));
        macSa[0] = (uint8)((value & 0x000000FFu) >> 0u);
        macSa[1] = (uint8)((value & 0x0000FF00u) >> 8u);
        macSa[2] = (uint8)((value & 0x00FF0000u) >> 16u);
        macSa[3] = (uint8)((value & 0xFF000000u) >> 24u);

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, saMatchHiRegAddr, &value));
        macSa[4] = (uint8)((value & 0x000000FFu) >> 0u);
        macSa[5] = (uint8)((value & 0x0000FF00u) >> 8u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_ethertype_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 etherType)
{
    RtkApiRet            retCode          = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet     = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet     = dva_g_regGet[direction];
    uint32               saMatchHiRegAddr = 0u;
    uint32               swapValue        = 0u;
    uint32               value            = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        saMatchHiRegAddr = DVA_MACSEC_SECY_SAM_MAC_SA_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, saMatchHiRegAddr, &value));
        swapValue = ((((etherType) & (uint32)0xff00u) >> 8u) | (((etherType) & (uint32)0x00ffu) << 8u));
        value     = dva_macsec_reg_value_field_set(value, 16u, 16u, swapValue);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, saMatchHiRegAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_ethertype_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *etherType)
{
    RtkApiRet            retCode          = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet     = dva_g_regGet[direction];
    uint32               saMatchHiRegAddr = 0u;
    uint32               value            = 0u;
    uint32               swapValue        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        saMatchHiRegAddr = DVA_MACSEC_SECY_SAM_MAC_SA_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, saMatchHiRegAddr, &value));
        value      = dva_macsec_reg_value_field_get(value, 16u, 16u);
        swapValue  = ((((value) & (uint32)0xff00u) >> 8u) | (((value) & (uint32)0x00ffu) << 8u));
        *etherType = swapValue;
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_mac_da_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, const uint8 *macDa)
{
    RtkApiRet            retCode          = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet     = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet     = dva_g_regGet[direction];
    uint32               daMatchLoRegAddr = 0u;
    uint32               daMatchHiRegAddr = 0u;
    uint32               value            = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        daMatchLoRegAddr = DVA_MACSEC_SECY_SAM_MAC_DA_MATCH_LO_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        daMatchHiRegAddr = DVA_MACSEC_SECY_SAM_MAC_DA_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);

        value = (macDa[0] | ((uint32)macDa[1] << 8) | (((uint32)macDa[2] << 16)) | ((uint32)macDa[3] << 24));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, daMatchLoRegAddr, value));

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, daMatchHiRegAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 0u, 16u, (macDa[4] | ((uint32)macDa[5] << 8)));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, daMatchHiRegAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_mac_da_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *macDa)
{
    RtkApiRet            retCode          = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet     = dva_g_regGet[direction];
    uint32               daMatchLoRegAddr = 0u;
    uint32               daMatchHiRegAddr = 0u;
    uint32               value            = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        daMatchLoRegAddr = DVA_MACSEC_SECY_SAM_MAC_DA_MATCH_LO_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        daMatchHiRegAddr = DVA_MACSEC_SECY_SAM_MAC_DA_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, daMatchLoRegAddr, &value));
        macDa[0] = (uint8)((value & 0x000000FFu) >> 0u);
        macDa[1] = (uint8)((value & 0x0000FF00u) >> 8u);
        macDa[2] = (uint8)((value & 0x00FF0000u) >> 16u);
        macDa[3] = (uint8)((value & 0xFF000000u) >> 24u);

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, daMatchHiRegAddr, &value));
        macDa[4] = (uint8)((value & 0x000000FFu) >> 0u);
        macDa[5] = (uint8)((value & 0x0000FF00u) >> 8u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_vlan_id_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 vlanId)
{
    RtkApiRet            retCode          = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet     = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet     = dva_g_regGet[direction];
    uint32               daMatchHiRegAddr = 0u;
    uint32               value            = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        daMatchHiRegAddr = DVA_MACSEC_SECY_SAM_MAC_DA_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, daMatchHiRegAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 16u, 12u, vlanId);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, daMatchHiRegAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_vlan_id_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *vlanId)
{
    RtkApiRet            retCode          = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet     = dva_g_regGet[direction];
    uint32               daMatchHiRegAddr = 0u;
    uint32               value            = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        daMatchHiRegAddr = DVA_MACSEC_SECY_SAM_MAC_DA_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, daMatchHiRegAddr, &value));
        *vlanId = dva_macsec_reg_value_field_get(value, 16u, 12u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_an_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 an)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               miscRegAddr  = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        miscRegAddr = DVA_MACSEC_SECY_SAM_MISC_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, miscRegAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 24u, 2u, an);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, miscRegAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_an_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *an)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               miscRegAddr  = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        miscRegAddr = DVA_MACSEC_SECY_SAM_MISC_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, miscRegAddr, &value));
        *an = dva_macsec_reg_value_field_get(value, 24u, 2u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_prioirty_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 priority)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               miscRegAddr  = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        miscRegAddr = DVA_MACSEC_SECY_SAM_MISC_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, miscRegAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 16u, 4u, priority);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, miscRegAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_prioirty_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *priority)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               miscRegAddr  = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        miscRegAddr = DVA_MACSEC_SECY_SAM_MISC_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, miscRegAddr, &value));
        *priority = dva_macsec_reg_value_field_get(value, 16u, 4u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_source_port_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 sourcePort)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               miscRegAddr  = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        miscRegAddr = DVA_MACSEC_SECY_SAM_MISC_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, miscRegAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 12u, 2u, sourcePort);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, miscRegAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_source_port_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *sourcePort)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               miscRegAddr  = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        miscRegAddr = DVA_MACSEC_SECY_SAM_MISC_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, miscRegAddr, &value));
        *sourcePort = dva_macsec_reg_value_field_get(value, 12u, 2u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_vlan_up_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 vlanUserPriority)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               regAddr      = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        regAddr = DVA_MACSEC_SECY_SAM_MISC_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, regAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 4u, 3u, vlanUserPriority);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, regAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_vlan_up_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *vlanUserPriority)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               regAddr      = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        regAddr = DVA_MACSEC_SECY_SAM_MISC_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, regAddr, &value));
        *vlanUserPriority = dva_macsec_reg_value_field_get(value, 4u, 3u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_inner_vlan_id_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 vlanId)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               regAddr      = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        regAddr = DVA_MACSEC_SECY_SAM_EXT_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, regAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 0u, 12u, vlanId);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, regAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_inner_vlan_id_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *vlanId)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               regAddr      = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        regAddr = DVA_MACSEC_SECY_SAM_EXT_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, regAddr, &value));
        *vlanId = dva_macsec_reg_value_field_get(value, 0u, 12u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_inner_vlan_up_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 vlanUserPriority)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               regAddr      = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        regAddr = DVA_MACSEC_SECY_SAM_EXT_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, regAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 12u, 3u, vlanUserPriority);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, regAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_inner_vlan_up_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *vlanUserPriority)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               regAddr      = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        regAddr = DVA_MACSEC_SECY_SAM_EXT_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, regAddr, &value));
        *vlanUserPriority = dva_macsec_reg_value_field_get(value, 12u, 3u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_sci_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, const uint8 *sci)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               sciLoRegAddr = 0u;
    uint32               sciHiRegAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        sciLoRegAddr = DVA_MACSEC_SECY_SAM_SCI_MATCH_LO_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        sciHiRegAddr = DVA_MACSEC_SECY_SAM_SCI_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);

        value = (sci[0] | ((uint32)sci[1] << 8) | (((uint32)sci[2] << 16)) | ((uint32)sci[3] << 24));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, sciLoRegAddr, value));

        value = (sci[4] | ((uint32)sci[5] << 8) | (((uint32)sci[6] << 16)) | ((uint32)sci[7] << 24));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, sciHiRegAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_sci_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *sci)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               sciLoRegAddr = 0u;
    uint32               sciHiRegAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        sciLoRegAddr = DVA_MACSEC_SECY_SAM_SCI_MATCH_LO_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        sciHiRegAddr = DVA_MACSEC_SECY_SAM_SCI_MATCH_HI_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, sciLoRegAddr, &value));
        sci[0] = (uint8)((value & 0x000000FFu) >> 0u);
        sci[1] = (uint8)((value & 0x0000FF00u) >> 8u);
        sci[2] = (uint8)((value & 0x00FF0000u) >> 16u);
        sci[3] = (uint8)((value & 0xFF000000u) >> 24u);

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, sciHiRegAddr, &value));
        sci[4] = (uint8)((value & 0x000000FFu) >> 0u);
        sci[5] = (uint8)((value & 0x0000FF00u) >> 8u);
        sci[6] = (uint8)((value & 0x00FF0000u) >> 16u);
        sci[7] = (uint8)((value & 0xFF000000u) >> 24u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_flow_ctrl_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 flowCtrlIdx)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               regAddr      = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        regAddr = DVA_MACSEC_SECY_SAM_EXT_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, regAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 16u, 8u, flowCtrlIdx);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, regAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_flow_ctrl_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *flowCtrlIdx)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               regAddr      = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        regAddr = DVA_MACSEC_SECY_SAM_EXT_MATCH_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, regAddr, &value));
        *flowCtrlIdx = dva_macsec_reg_value_field_get(value, 16u, 8u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_mask_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 mask)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               maskRegAddr  = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        maskRegAddr = DVA_MACSEC_SECY_SAM_MASK_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, maskRegAddr, mask));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_mask_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *mask)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               maskRegAddr  = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        maskRegAddr = DVA_MACSEC_SECY_SAM_MASK_BASE + (index * DVA_MACSEC_SECY_SAM_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, maskRegAddr, mask));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_entry_enable_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool enable)
{
    RtkApiRet            retCode        = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet   = dva_g_regSet[direction];
    uint32               entrySetAddr   = DVA_MACSEC_SECY_SAM_ENTRY_SET_BASE;
    uint32               entryClearAddr = DVA_MACSEC_SECY_SAM_ENTRY_CLEAR_BASE;
    uint32               value          = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        value = ((uint32)1u << index);
        if(enable)
        {
            CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, entrySetAddr, value));
        }
        else
        {
            CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, entryClearAddr, value));
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_mr_entry_enable_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool *enable)
{
    RtkApiRet            retCode         = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet    = dva_g_regGet[direction];
    uint32               entryEnableAddr = DVA_MACSEC_SECY_SAM_ENTRY_ENABLE_BASE;
    uint32               value           = 0u;

    if(index < DVA_MACSEC_SECY_SA_MATCH_MAX_ENTRY)
    {
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, entryEnableAddr, &value));

        *enable = ((value & ((uint32)1u << index)) != 0u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

/*  88888888b dP         .88888.  dP   dP   dP     a88888b.  .88888.  888888ba  d888888P  888888ba   .88888.  dP        */
/*  88        88        d8'   `8b 88   88   88    d8'   `88 d8'   `8b 88    `8b    88     88    `8b d8'   `8b 88        */
/* a88aaaa    88        88     88 88  .8P  .8P    88        88     88 88     88    88    a88aaaa8P' 88     88 88        */
/*  88        88        88     88 88  d8'  d8'    88        88     88 88     88    88     88   `8b. 88     88 88        */
/*  88        88        Y8.   .8P 88.d8P8.d8P     Y8.   .88 Y8.   .8P 88     88    88     88     88 Y8.   .8P 88        */
/*  dP        88888888P  `8888P'  8888' Y88'       Y88888P'  `8888P'  dP     dP    dP     dP     dP  `8888P'  88888888P */
RtkApiRet dva_drv_secy_fc_common_params_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 entryIdx, const RtkMacsecFlowControl_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_secy_fc_sa_index_set(unit, sysPort, direction, entryIdx, params->saEntryIdx));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_sa_in_use_set(unit, sysPort, direction, entryIdx, params->saInUse));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_conf_offset_set(unit, sysPort, direction, entryIdx, params->confidentialityOffset));

    return retCode;
}

RtkApiRet dva_drv_secy_fc_common_params_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 entryIdx, RtkMacsecFlowControl_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint32    value   = 0u;

    CHK_FUN_RET(retCode, dva_drv_secy_fc_sa_in_use_get(unit, sysPort, direction, entryIdx, &params->saInUse));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_sa_index_get(unit, sysPort, direction, entryIdx, &value));
    params->saEntryIdx = (uint8)value;
    CHK_FUN_RET(retCode, dva_drv_secy_fc_conf_offset_get(unit, sysPort, direction, entryIdx, &value));
    params->confidentialityOffset = (uint8)value;

    return retCode;
}

RtkApiRet dva_drv_secy_fc_rx_params_set(uint32 unit, RtkPort sysPort, uint8 entryIdx, const RtkMacsecFlowControl_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_secy_fc_rx_rp_set(unit, sysPort, entryIdx, params->replayProtect));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_rx_vf_set(unit, sysPort, entryIdx, params->validateFrames));

    return retCode;
}

RtkApiRet dva_drv_secy_fc_rx_params_get(uint32 unit, RtkPort sysPort, uint8 entryIdx, RtkMacsecFlowControl_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint32    value   = 0u;

    CHK_FUN_RET(retCode, dva_drv_secy_fc_rx_rp_get(unit, sysPort, entryIdx, &params->replayProtect));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_rx_vf_get(unit, sysPort, entryIdx, &value));
    params->validateFrames = (uint8)value;

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_params_set(uint32 unit, RtkPort sysPort, uint8 entryIdx, const RtkMacsecFlowControl_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_pf_set(unit, sysPort, entryIdx, params->protectFrame));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_incl_sci_set(unit, sysPort, entryIdx, params->includeSci));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_use_es_set(unit, sysPort, entryIdx, params->useEs));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_use_scb_set(unit, sysPort, entryIdx, params->useScb));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_cp_set(unit, sysPort, entryIdx, params->confProtect));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_tb_set(unit, sysPort, entryIdx, params->bypassTagSize));

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_params_get(uint32 unit, RtkPort sysPort, uint8 entryIdx, RtkMacsecFlowControl_t *params)
{
    RtkApiRet retCode = RT_ERR_OK;
    uint32    value   = 0u;

    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_pf_get(unit, sysPort, entryIdx, &params->protectFrame));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_incl_sci_get(unit, sysPort, entryIdx, &params->includeSci));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_use_es_get(unit, sysPort, entryIdx, &params->useEs));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_use_scb_get(unit, sysPort, entryIdx, &params->useScb));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_cp_get(unit, sysPort, entryIdx, &params->confProtect));
    CHK_FUN_RET(retCode, dva_drv_secy_fc_tx_tb_get(unit, sysPort, entryIdx, &value));
    params->bypassTagSize = (uint8)value;

    return retCode;
}

RtkApiRet dva_drv_secy_fc_flow_type_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 flowType)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 0u, 2u, flowType);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_fc_flow_type_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *flowType)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *flowType = dva_macsec_reg_value_field_get(value, 0u, 2u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_fc_dest_port_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 srcPort)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 2u, 2u, srcPort);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_fc_dest_port_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *srcPort)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *srcPort = dva_macsec_reg_value_field_get(value, 2u, 2u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_fc_rx_rp_set(uint32 unit, RtkPort sysPort, uint8 index, bool replayProtect)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_rx_get;
    dva_MacsecRegSetFunc macsecRegSet = &dva_macsec_register_rx_set;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 16u, 1u, replayProtect ? 1u : 0u);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_rx_rp_get(uint32 unit, RtkPort sysPort, uint8 index, bool *replayProtect)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_rx_get;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *replayProtect = ((dva_macsec_reg_value_field_get(value, 16u, 1u) == (uint32)1u) ? true : false);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_rx_vf_set(uint32 unit, RtkPort sysPort, uint8 index, uint32 validateFrames)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_rx_get;
    dva_MacsecRegSetFunc macsecRegSet = &dva_macsec_register_rx_set;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 19u, 2u, validateFrames);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_rx_vf_get(uint32 unit, RtkPort sysPort, uint8 index, uint32 *validateFrames)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_rx_get;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *validateFrames = dva_macsec_reg_value_field_get(value, 19u, 2u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_pf_set(uint32 unit, RtkPort sysPort, uint8 index, bool protectFrame)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    dva_MacsecRegSetFunc macsecRegSet = &dva_macsec_register_tx_set;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 16u, 1u, protectFrame ? 1u : 0u);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_pf_get(uint32 unit, RtkPort sysPort, uint8 index, bool *protectFrame)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *protectFrame = ((dva_macsec_reg_value_field_get(value, 16u, 1u) == (uint32)1u) ? true : false);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_incl_sci_set(uint32 unit, RtkPort sysPort, uint8 index, bool includeSci)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    dva_MacsecRegSetFunc macsecRegSet = &dva_macsec_register_tx_set;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 18u, 1u, includeSci ? 1u : 0u);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_incl_sci_get(uint32 unit, RtkPort sysPort, uint8 index, bool *includeSci)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *includeSci = ((dva_macsec_reg_value_field_get(value, 18u, 1u) == (uint32)1u) ? true : false);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_use_es_set(uint32 unit, RtkPort sysPort, uint8 index, bool useEs)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    dva_MacsecRegSetFunc macsecRegSet = &dva_macsec_register_tx_set;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 19u, 1u, useEs ? 1u : 0u);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_use_es_get(uint32 unit, RtkPort sysPort, uint8 index, bool *useEs)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *useEs = ((dva_macsec_reg_value_field_get(value, 19u, 1u) == (uint32)1u) ? true : false);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_use_scb_set(uint32 unit, RtkPort sysPort, uint8 index, bool useScb)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    dva_MacsecRegSetFunc macsecRegSet = &dva_macsec_register_tx_set;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 20u, 1u, useScb ? 1u : 0u);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_use_scb_get(uint32 unit, RtkPort sysPort, uint8 index, bool *useScb)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *useScb = ((dva_macsec_reg_value_field_get(value, 20u, 1u) == (uint32)1u) ? true : false);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_cp_set(uint32 unit, RtkPort sysPort, uint8 index, bool confProtect)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    dva_MacsecRegSetFunc macsecRegSet = &dva_macsec_register_tx_set;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 31u, 1u, confProtect ? 1u : 0u);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_cp_get(uint32 unit, RtkPort sysPort, uint8 index, bool *confProtect)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *confProtect = ((dva_macsec_reg_value_field_get(value, 31u, 1u) == (uint32)1u) ? true : false);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_tb_set(uint32 unit, RtkPort sysPort, uint8 index, uint32 tagBypassSize)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    dva_MacsecRegSetFunc macsecRegSet = &dva_macsec_register_tx_set;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 21u, 2u, tagBypassSize);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_tx_tb_get(uint32 unit, RtkPort sysPort, uint8 index, uint32 *tagBypassSize)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = &dva_macsec_register_tx_get;
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *tagBypassSize = dva_macsec_reg_value_field_get(value, 21u, 2u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_sa_index_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 saIndex)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 8u, 8u, saIndex);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_sa_index_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *saIndex)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *saIndex = dva_macsec_reg_value_field_get(value, 8u, 8u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_sa_in_use_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool saInUse)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 17u, 1u, saInUse ? 1u : 0u);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_sa_in_use_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool *saInUse)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *saInUse = ((dva_macsec_reg_value_field_get(value, 17u, 1u) == (uint32)1u) ? true : false);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_conf_offset_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 confOffset)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 24u, 7u, confOffset);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, flowCtrlAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_fc_conf_offset_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *confOffset)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               flowCtrlAddr = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_FLOW_CTRL_MAX_ENTRY)
    {
        flowCtrlAddr = DVA_MACSEC_SECY_SAM_FLOW_CTRL_BASE + (index * DVA_MACSEC_SECY_FLOW_CTRL_ENTRY_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, flowCtrlAddr, &value));
        *confOffset = dva_macsec_reg_value_field_get(value, 24u, 7u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

/* .dP"Y8    db        888888 88b 88 888888 88""Yb Yb  dP  */
/* `Ybo."   dPYb       88__   88Yb88   88   88__dP  YbdP   */
/* o.`Y8b  dP__Yb      88""   88 Y88   88   88"Yb    8P    */
/* 8bodP' dP""""Yb     888888 88  Y8   88   88  Yb  dP     */

static uint32 dva_secy_get_default_ctrl_data(RtkMacsecDirection_e direction, uint32 cipherSuite)
{
    uint32 ctrlData = 0u;
    switch(cipherSuite)
    {
        case RTK_MACSEC_CIPHERSUITE_GCM_128:
            ctrlData = ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? 0x924BE066u : 0xD24BE06Fu);
            break;
        case RTK_MACSEC_CIPHERSUITE_GCM_256:
            ctrlData = ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? 0x924FE066u : 0xD24FE06Fu);
            break;
        case RTK_MACSEC_CIPHERSUITE_GCM_XPN_128:
            ctrlData = ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? 0xA24BE066u : 0xE24BA0EFu);
            break;
        case RTK_MACSEC_CIPHERSUITE_GCM_XPN_256:
            ctrlData = ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? 0xA24FE066u : 0xE24FA0EFu);
            break;
        case RTK_MACSEC_CIPHERSUITE_GCM_NONE:
            ctrlData = 0x0u;
            break;
        default:
            ctrlData = 0x0u;
            break;
    }

    return ctrlData;
}

RtkApiRet dva_drv_secy_sa_init(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint8 an)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               addr         = 0u;
    uint32               ctrlData     = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        ctrlData = dva_secy_get_default_ctrl_data(direction, cipherSuite);

        if((ctrlData != 0u) && (direction == EM_RTK_MACSEC_DIRECTION_TX))
        {
            ctrlData = dva_macsec_reg_value_field_set(ctrlData, 26u, 2u, an);
        }

        /* Ctrl */
        addr = DVA_MACSEC_SECY_TRANSFORM_128BIT_CTRL + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, ctrlData));

        /* ID */
        addr = DVA_MACSEC_SECY_TRANSFORM_128BIT_ID + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, index));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_an_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *an)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               addr         = 0u;
    uint32               ctrlData     = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        if(direction == EM_RTK_MACSEC_DIRECTION_TX)
        {
            addr = DVA_MACSEC_SECY_TRANSFORM_128BIT_CTRL + (index * DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET);
            CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, &ctrlData));
            value = dva_macsec_reg_value_field_get(ctrlData, 26u, 2u);
            *an   = (uint8)value;
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_key_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, const uint8 *sak, const uint8 *hkey)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               addr         = 0u;
    uint32               i            = 0u;
    uint32               data         = 0u;
    uint32               sakRegSize   = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        /* key 0 ~ 3 or 0 ~ 7 */
        if(dva_is_256bit_sak(cipherSuite))
        {
            sakRegSize = 8u;
        }
        else
        {
            sakRegSize = 4u;
        }

        addr = DVA_MACSEC_SECY_TRANSFORM_128BIT_KEY_0 + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));

        for(i = 0u; i < (sakRegSize * 4u); i += 4u)
        {
            data = (sak[i] | ((uint32)sak[i + 1u] << 8u) | (((uint32)sak[i + 2u] << 16u)) | ((uint32)sak[i + 3u] << 24u));
            CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, data));
            addr += 4u;
        }

        /* hkey 0 ~ 3 */
        for(i = 0u; i < (4u * 4u); i += 4u)
        {
            data = (hkey[i] | ((uint32)hkey[i + 1u] << 8u) | (((uint32)hkey[i + 2u] << 16u)) | ((uint32)hkey[i + 3u] << 24u));
            CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, data));
            addr += 4u;
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_key_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint8 *sak, uint8 *hkey)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               addr         = 0u;
    uint32               i            = 0u;
    uint32               data         = 0u;
    uint32               sakRegSize   = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        /* key 0 ~ 3 or 0 ~ 7 */
        if(dva_is_256bit_sak(cipherSuite))
        {
            sakRegSize = 8u;
        }
        else
        {
            sakRegSize = 4u;
        }

        addr = DVA_MACSEC_SECY_TRANSFORM_128BIT_KEY_0 + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));

        for(i = 0u; i < (sakRegSize * 4u); i += 4u)
        {
            CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, &data));
            sak[i]      = (uint8)((data & 0x000000FFu) >> 0u);
            sak[i + 1u] = (uint8)((data & 0x0000FF00u) >> 8u);
            sak[i + 2u] = (uint8)((data & 0x00FF0000u) >> 16u);
            sak[i + 3u] = (uint8)((data & 0xFF000000u) >> 24u);

            addr += 4u;
        }

        /* hkey 0 ~ 3 */
        for(i = 0u; i < (4u * 4u); i += 4u)
        {
            CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, &data));
            hkey[i]      = (uint8)((data & 0x000000FFu) >> 0u);
            hkey[i + 1u] = (uint8)((data & 0x0000FF00u) >> 8u);
            hkey[i + 2u] = (uint8)((data & 0x00FF0000u) >> 16u);
            hkey[i + 3u] = (uint8)((data & 0xFF000000u) >> 24u);
            addr += 4u;
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_sci_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, const uint8 *sci)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               i            = 0u;
    uint32               addr         = 0u;
    uint32               data         = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        if(dva_is_xpn(cipherSuite) && (EM_RTK_MACSEC_DIRECTION_RX == direction))
        {
            /* XPN Rx does not need sci, just kgnore it */
        }
        else
        {
            addr = dva_g_xfromRecordIv0Addr[direction][cipherSuite] + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));
            for(i = 0u; i < (2u * 4u); i += 4u)
            {
                data = (sci[i] | ((uint32)sci[i + 1u] << 8) | (((uint32)sci[i + 2u] << 16)) | ((uint32)sci[i + 3u] << 24));
                CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, data));
                addr += 4u;
            }
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_sci_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint8 *sci)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               i            = 0u;
    uint32               addr         = 0u;
    uint32               data         = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        if(dva_is_xpn(cipherSuite) && (EM_RTK_MACSEC_DIRECTION_RX == direction))
        {
            /* XPN Rx does not need sci, just kgnore it */
        }
        else
        {
            addr = dva_g_xfromRecordIv0Addr[direction][cipherSuite] + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));
            for(i = 0u; i < (2u * 4u); i += 4u)
            {
                CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, &data));
                sci[i]      = (uint8)((data & 0x000000FFu) >> 0u);
                sci[i + 1u] = (uint8)((data & 0x0000FF00u) >> 8u);
                sci[i + 2u] = (uint8)((data & 0x00FF0000u) >> 16u);
                sci[i + 3u] = (uint8)((data & 0xFF000000u) >> 24u);
                addr += 4u;
            }
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_seq_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint64 seq)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               addr         = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        if((direction <= EM_RTK_MACSEC_DIRECTION_RX) && (cipherSuite <= RTK_MACSEC_CIPHERSUITE_GCM_XPN_256))
        {
            addr = dva_g_xfromRecordSeqAddr[direction][cipherSuite] + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));

            CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, (seq & 0xFFFFFFFFu)));

            if(dva_is_xpn(cipherSuite))
            {
                addr += 4u;
                CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, (seq >> 32u)));
            }
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_seq_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint64 *seq)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               addr         = 0u;
    uint32               low          = 0u;
    uint32               high         = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        if((direction <= EM_RTK_MACSEC_DIRECTION_RX) && (cipherSuite <= RTK_MACSEC_CIPHERSUITE_GCM_XPN_256))
        {
            addr = dva_g_xfromRecordSeqAddr[direction][cipherSuite] + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));
            CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, &low));

            if(dva_is_xpn(cipherSuite))
            {
                addr += 4u;
                CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, &high));
            }

            *seq = (((uint64)high << 32u) | (uint64)low);
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_mask_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint32 mask)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               addr         = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        if(cipherSuite <= RTK_MACSEC_CIPHERSUITE_GCM_XPN_256)
        {
            addr = dva_g_xfromRecordMaskAddr[cipherSuite] + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));
            CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, mask));
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_mask_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint32 *mask)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               addr         = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        if(cipherSuite <= RTK_MACSEC_CIPHERSUITE_GCM_XPN_256)
        {
            addr = dva_g_xfromRecordMaskAddr[cipherSuite] + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));
            CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, mask));
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_sa_ctx_salt_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, const uint8 *salt, const uint8 *ssci)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               addr         = 0u;
    uint32               data         = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        if((direction <= EM_RTK_MACSEC_DIRECTION_RX) && (cipherSuite <= RTK_MACSEC_CIPHERSUITE_GCM_XPN_256))
        {
            addr = dva_g_xfromRecordCtxSaltAddr[direction][cipherSuite] + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));

            data = ((uint32)salt[0] ^ (uint32)ssci[0]) | (((uint32)salt[1] ^ (uint32)ssci[1]) << 8u) | (((uint32)salt[2] ^ (uint32)ssci[2]) << 16u) | (((uint32)salt[3] ^ (uint32)ssci[3]) << 24u);
            CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, data));

            addr += 4u;
            data = (uint32)salt[4] | ((uint32)salt[5] << 8u) | ((uint32)salt[6] << 16u) | ((uint32)salt[7] << 24u);
            CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, data));

            addr += 4u;
            data = (uint32)salt[8] | ((uint32)salt[9] << 8u) | ((uint32)salt[10] << 16u) | ((uint32)salt[11] << 24u);
            CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, data));
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_sa_ctx_salt_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 cipherSuite, uint8 *ctxSalt)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               i            = 0u;
    uint32               addr         = 0u;
    uint32               data         = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        if((direction <= EM_RTK_MACSEC_DIRECTION_RX) && (cipherSuite <= RTK_MACSEC_CIPHERSUITE_GCM_XPN_256))
        {
            addr = dva_g_xfromRecordCtxSaltAddr[direction][cipherSuite] + (index * ((direction == EM_RTK_MACSEC_DIRECTION_TX) ? DVA_MACSEC_SECY_TRANSFORM_ENTRY_TX_OFFSET : DVA_MACSEC_SECY_TRANSFORM_ENTRY_RX_OFFSET));

            for(i = 0u; i < (3u * 4u); i += 4u)
            {
                CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, &data));
                ctxSalt[i]      = (uint8)((data & 0x000000FFu) >> 0u);
                ctxSalt[i + 1u] = (uint8)((data & 0x0000FF00u) >> 8u);
                ctxSalt[i + 2u] = (uint8)((data & 0x00FF0000u) >> 16u);
                ctxSalt[i + 3u] = (uint8)((data & 0xFF000000u) >> 24u);
                addr += 4u;
            }
        }
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

/*  a88888b.  .88888.  dP     dP 888888ba  d888888P  88888888b  888888ba  */
/* d8'   `88 d8'   `8b 88     88 88    `8b    88     88         88    `8b */
/* 88        88     88 88     88 88     88    88    a88aaaa    a88aaaa8P' */
/* 88        88     88 88     88 88     88    88     88         88   `8b. */
/* Y8.   .88 Y8.   .8P Y8.   .8P 88     88    88     88         88     88 */
/*  Y88888P'  `8888P'  `Y88888P' dP     dP    dP     88888888P  dP     dP */

RtkApiRet dva_drv_secy_counter_global_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 counterType, uint64 *value)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               counterAddr  = DVA_MACSEC_SECY_GLOBAL_COUNTER_BASE + (counterType * DVA_MACSEC_SECY_COUNTER_ITEM_OFFSET);
    uint32               lo           = 0u;
    uint32               hi           = 0u;

    CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, counterAddr, &lo));
    CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, counterAddr + 4u, &hi));
    *value = (((uint64)hi << 32u) | (uint64)lo);

    return retCode;
}

RtkApiRet dva_drv_secy_counter_global_clear(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 counterType)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               counterAddr  = DVA_MACSEC_SECY_GLOBAL_COUNTER_BASE + (counterType * DVA_MACSEC_SECY_COUNTER_ITEM_OFFSET);

    CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, counterAddr, 0x0u));
    CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, counterAddr + 4u, 0x0u));

    return retCode;
}

RtkApiRet dva_drv_secy_counter_sa_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 counterType, uint64 *value)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               counterAddr  = 0u;
    uint32               lo           = 0u;
    uint32               hi           = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        counterAddr = DVA_MACSEC_SECY_SA_COUNTER_BASE + (index * DVA_MACSEC_SECY_SA_COUNTER_ENTRY_OFFSET) + (counterType * DVA_MACSEC_SECY_COUNTER_ITEM_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, counterAddr, &lo));
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, counterAddr + 4u, &hi));
        *value = (((uint64)hi << 32u) | (uint64)lo);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_counter_sa_clear(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 counterType)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               counterAddr  = 0u;

    if(index < DVA_MACSEC_SECY_SA_MAX_ENTRY)
    {
        counterAddr = DVA_MACSEC_SECY_SA_COUNTER_BASE + (index * DVA_MACSEC_SECY_SA_COUNTER_ENTRY_OFFSET) + (counterType * DVA_MACSEC_SECY_COUNTER_ITEM_OFFSET);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, counterAddr, 0x0u));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, counterAddr + 4u, 0x0u));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }
    return retCode;
}

RtkApiRet dva_drv_secy_counter_clear_all(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];

    CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, DVA_MACSEC_SECY_COUNT_CONTROL, 0x1u));

    return retCode;
}

RtkApiRet dva_drv_secy_counter_rxgp1_get(uint32 unit, RtkPort sysPort, RtkMacsecRxGlobalCounter_t *rxCounters)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_TRANSFORM_ERROR_PKTS, &rxCounters->transformErrorPkts));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_CTRL, &rxCounters->inPktsCtrl));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_NO_TAG, &rxCounters->inPktsNoTag));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_UNTAGGED, &rxCounters->inPktsUntagged));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_TAGGED, &rxCounters->inPktsTagged));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_BAD_TAG, &rxCounters->inPktsBadTag));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_UNTAGGED_MISS, &rxCounters->inPktsUntaggedMiss));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_NO_SCI, &rxCounters->inPktsNoSci));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_UNKNOW_SCI, &rxCounters->inPktsUnknownSci));

    return retCode;
}

RtkApiRet dva_drv_secy_counter_rxgp2_get(uint32 unit, RtkPort sysPort, RtkMacsecRxGlobalCounter_t *rxCounters)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_CONTROLLED_NOT_PASS, &rxCounters->inConsistCheckControlledNotPass));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_UNCONTROLLED_NOT_PASS, &rxCounters->inConsistCheckUncontrolledNotPass));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_CONTROLLED_PASS, &rxCounters->inConsistCheckControlledPass));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_UNCONTROLLED_PASS, &rxCounters->inConsistCheckUncontrolledPass));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_get(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_OVER_SIZE, &rxCounters->inOverSize));

    return retCode;
}

RtkApiRet dva_drv_secy_counter_rxgp1_clear(uint32 unit, RtkPort sysPort)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_TRANSFORM_ERROR_PKTS));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_CTRL));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_NO_TAG));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_UNTAGGED));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_TAGGED));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_BAD_TAG));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_UNTAGGED_MISS));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_NO_SCI));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_PKTS_UNKNOW_SCI));

    return retCode;
}

RtkApiRet dva_drv_secy_counter_rxgp2_clear(uint32 unit, RtkPort sysPort)
{
    RtkApiRet retCode = RT_ERR_OK;

    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_CONTROLLED_NOT_PASS));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_UNCONTROLLED_NOT_PASS));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_CONTROLLED_PASS));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_CONSIST_CHECK_UNCONTROLLED_PASS));
    CHK_FUN_RET(retCode, dva_drv_secy_counter_global_clear(unit, sysPort, EM_RTK_MACSEC_DIRECTION_RX, DVA_MACSEC_SECY_RX_GLOBAL_IN_OVER_SIZE));

    return retCode;
}

/*  dP""b8  dP"Yb  88b 88 888888 88""Yb  dP"Yb  88         88""Yb    db     dP""b8 88  dP 888888 888888      dP""b8 88        db    .dP"Y8 .dP"Y8 88 888888 88  dP""b8    db    888888 88  dP"Yb  88b 88  */
/* dP   `" dP   Yb 88Yb88   88   88__dP dP   Yb 88         88__dP   dPYb   dP   `" 88odP  88__     88       dP   `" 88       dPYb   `Ybo." `Ybo." 88 88__   88 dP   `"   dPYb     88   88 dP   Yb 88Yb88  */
/* Yb      Yb   dP 88 Y88   88   88"Yb  Yb   dP 88  .o     88"""   dP__Yb  Yb      88"Yb  88""     88       Yb      88  .o  dP__Yb  o.`Y8b o.`Y8b 88 88""   88 Yb       dP__Yb    88   88 Yb   dP 88 Y88  */
/*  YboodP  YbodP  88  Y8   88   88  Yb  YbodP  88ood8     88     dP""""Yb  YboodP 88  Yb 888888   88        YboodP 88ood8 dP""""Yb 8bodP' 8bodP' 88 88     88  YboodP dP""""Yb   88   88  YbodP  88  Y8  */

RtkApiRet dva_drv_secy_cp_da_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint8 *macDa)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               macDaLoAddr  = 0u;
    uint32               macDaHiAddr  = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_CTRL_PACKET_ENTRY)
    {
        macDaLoAddr = DVA_MACSEC_SECY_CP_MAC_DA_MATCH_0 + (index * DVA_MACSEC_SECY_CTRL_PACKET_DA_OFFSET);
        macDaHiAddr = DVA_MACSEC_SECY_CP_MAC_DA_ET_MATCH_0 + (index * DVA_MACSEC_SECY_CTRL_PACKET_DA_OFFSET);

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, macDaLoAddr, &value));
        macDa[0] = (uint8)((value & 0x000000FFu) >> 0u);
        macDa[1] = (uint8)((value & 0x0000FF00u) >> 8u);
        macDa[2] = (uint8)((value & 0x00FF0000u) >> 16u);
        macDa[3] = (uint8)((value & 0xFF000000u) >> 24u);

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, macDaHiAddr, &value));
        macDa[4] = (uint8)((value & 0x000000FFu) >> 0u);
        macDa[5] = (uint8)((value & 0x0000FF00u) >> 8u);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_cp_da_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, const uint8 *macDa)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               macDaLoAddr  = 0u;
    uint32               macDaHiAddr  = 0u;
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_CTRL_PACKET_ENTRY)
    {
        macDaLoAddr = DVA_MACSEC_SECY_CP_MAC_DA_MATCH_0 + (index * DVA_MACSEC_SECY_CTRL_PACKET_DA_OFFSET);
        macDaHiAddr = DVA_MACSEC_SECY_CP_MAC_DA_ET_MATCH_0 + (index * DVA_MACSEC_SECY_CTRL_PACKET_DA_OFFSET);

        value = (macDa[0] | ((uint32)macDa[1] << 8u) | (((uint32)macDa[2] << 16u)) | ((uint32)macDa[3] << 24u));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, macDaLoAddr, value));

        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, macDaHiAddr, &value));
        value = dva_macsec_reg_value_field_set(value, 0u, 16u, (macDa[4] | ((uint32)macDa[5] << 8u)));
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, macDaHiAddr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_cp_da_enable_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool enable)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               value;

    if(index < DVA_MACSEC_SECY_CTRL_PACKET_ENTRY)
    {
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_CP_MATCH_ENABLE, &value));
        if(enable)
        {
            value |= ((uint32)1u << (index + DVA_MACSEC_SECY_CP_MATCH_ENABLE_MAC_DA_BIT));
        }
        else
        {
            value &= ~((uint32)1u << (index + DVA_MACSEC_SECY_CP_MATCH_ENABLE_MAC_DA_BIT));
        }
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, DVA_MACSEC_SECY_CP_MATCH_ENABLE, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_cp_da_enable_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool *enable)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_CTRL_PACKET_ENTRY)
    {
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_CP_MATCH_ENABLE, &value));
        *enable = ((value & ((uint32)1u << (index + DVA_MACSEC_SECY_CP_MATCH_ENABLE_MAC_DA_BIT))) != 0u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_cp_et_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 *etherType)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               addr         = 0u;
    uint32               value        = 0u;
    uint32               swapValue    = 0u;

    if(index < DVA_MACSEC_SECY_CTRL_PACKET_ENTRY)
    {
        addr = DVA_MACSEC_SECY_CP_ET_MATCH_10 + (index * DVA_MACSEC_SECY_CTRL_PACKET_ETHER_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, &value));
        value      = dva_macsec_reg_value_field_get(value, 16u, 16u);
        swapValue  = ((((value) & (uint32)0xff00u) >> 8u) | (((value) & (uint32)0x00ffu) << 8u));
        *etherType = swapValue;
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_cp_et_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, uint32 etherType)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               addr         = 0u;
    uint32               value        = 0u;
    uint32               swapValue    = 0u;

    if(index < DVA_MACSEC_SECY_CTRL_PACKET_ENTRY)
    {
        addr = DVA_MACSEC_SECY_CP_ET_MATCH_10 + (index * DVA_MACSEC_SECY_CTRL_PACKET_ETHER_OFFSET);
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, addr, &value));
        swapValue = ((((etherType) & (uint32)0xff00u) >> 8u) | (((etherType) & (uint32)0x00ffu) << 8u));
        value     = dva_macsec_reg_value_field_set(value, 16u, 16u, swapValue);
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, addr, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_cp_et_enable_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool enable)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_CTRL_PACKET_ENTRY)
    {
        /* entry enable */
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_CP_MATCH_ENABLE, &value));
        if(enable)
        {
            value |= ((uint32)1u << (index + DVA_MACSEC_SECY_CP_MATCH_ENABLE_ETHER_TYPE_BIT));
        }
        else
        {
            value &= ~((uint32)1u << (index + DVA_MACSEC_SECY_CP_MATCH_ENABLE_ETHER_TYPE_BIT));
        }
        CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, DVA_MACSEC_SECY_CP_MATCH_ENABLE, value));
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

RtkApiRet dva_drv_secy_cp_et_enable_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint8 index, bool *enable)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               value        = 0u;

    if(index < DVA_MACSEC_SECY_CTRL_PACKET_ENTRY)
    {
        CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_CP_MATCH_ENABLE, &value));
        *enable = ((value & ((uint32)1u << (index + DVA_MACSEC_SECY_CP_MATCH_ENABLE_ETHER_TYPE_BIT))) != 0u);
    }
    else
    {
        retCode = (-RT_ERR_ENTRY_INDEX);
    }

    return retCode;
}

/* Yb    dP 88        db    88b 88     88""Yb    db    88""Yb .dP"Y8 888888 88""Yb      dP""b8  dP"Yb  88b 88 888888 88""Yb  dP"Yb  88      */
/*  Yb  dP  88       dPYb   88Yb88     88__dP   dPYb   88__dP `Ybo." 88__   88__dP     dP   `" dP   Yb 88Yb88   88   88__dP dP   Yb 88      */
/*   YbdP   88  .o  dP__Yb  88 Y88     88"""   dP__Yb  88"Yb  o.`Y8b 88""   88"Yb      Yb      Yb   dP 88 Y88   88   88"Yb  Yb   dP 88  .o  */
/*    YP    88ood8 dP""""Yb 88  Y8     88     dP""""Yb 88  Yb 8bodP' 888888 88  Yb      YboodP  YbodP  88  Y8   88   88  Yb  YbodP  88ood8  */
RtkApiRet dva_drv_secy_vp_control_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, bool parseQinQ, bool parseStag)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               value        = DVA_MACSEC_DEFAULT_VLAN_PARSER_REG_VALUE;

    if(parseQinQ)
    {
        value |= ((uint32)1u << DVA_MACSEC_SECY_VLAN_PARSER_CONTROL_Q_IN_Q_BIT);
    }
    else
    {
        value &= ~((uint32)1u << DVA_MACSEC_SECY_VLAN_PARSER_CONTROL_Q_IN_Q_BIT);
    }

    if(parseStag)
    {
        value |= ((uint32)1u << DVA_MACSEC_SECY_VLAN_PARSER_CONTROL_PARSE_STAG_BIT);
    }
    else
    {
        value &= ~((uint32)1u << DVA_MACSEC_SECY_VLAN_PARSER_CONTROL_PARSE_STAG_BIT);
    }

    CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, DVA_MACSEC_SECY_VLAN_PARSER_CONTROL, value));

    return retCode;
}

RtkApiRet dva_drv_secy_vp_control_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, bool *parseQinQ, bool *parseStag)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               value        = 0u;

    CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_VLAN_PARSER_CONTROL, &value));

    *parseQinQ = ((value & ((uint32)1u << DVA_MACSEC_SECY_VLAN_PARSER_CONTROL_Q_IN_Q_BIT)) != 0u);
    *parseStag = ((value & ((uint32)1u << DVA_MACSEC_SECY_VLAN_PARSER_CONTROL_PARSE_STAG_BIT)) != 0u);

    return retCode;
}

RtkApiRet dva_drv_secy_vp_qtag_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 qtag)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               value        = 0u;
    uint32               tmp          = 0u;

    CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_VLAN_TAGS_TPID, &value));
    tmp = (((qtag & 0x000000FFu) << 8u) | ((qtag & 0x0000FF00u) >> 8u));

    value = dva_macsec_reg_value_field_set(value, 0u, 16u, tmp);
    CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, DVA_MACSEC_SECY_VLAN_TAGS_TPID, value));

    return retCode;
}

RtkApiRet dva_drv_secy_vp_qtag_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 *qtag)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               value        = 0u;
    uint32               tmp          = 0u;

    CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_VLAN_TAGS_TPID, &value));
    tmp   = dva_macsec_reg_value_field_get(value, 0u, 16u);
    *qtag = (((tmp & 0x000000FFu) << 8u) | ((tmp & 0x0000FF00u) >> 8u));

    return retCode;
}

RtkApiRet dva_drv_secy_vp_stag_set(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 stag)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    dva_MacsecRegSetFunc macsecRegSet = dva_g_regSet[direction];
    uint32               value        = 0u;
    uint32               tmp          = 0u;

    CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_VLAN_TAGS_TPID, &value));
    tmp = (((stag & 0x000000FFu) << 8u) | ((stag & 0x0000FF00u) >> 8u));

    value = dva_macsec_reg_value_field_set(value, 16u, 16u, tmp);
    CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, DVA_MACSEC_SECY_VLAN_TAGS_TPID, value));

    /* write stag2 and stag3 to same value as stag 1*/
    value = dva_macsec_reg_value_field_set(value, 0u, 16u, tmp);
    CHK_FUN_RET(retCode, macsecRegSet(unit, sysPort, DVA_MACSEC_SECY_VLAN_TAGS_TPID2, value));

    return retCode;
}

RtkApiRet dva_drv_secy_vp_stag_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, uint32 *stag)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               value        = 0u;
    uint32               tmp          = 0u;

    CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_VLAN_TAGS_TPID, &value));
    tmp   = dva_macsec_reg_value_field_get(value, 16u, 16u);
    *stag = (((tmp & 0x000000FFu) << 8u) | ((tmp & 0x0000FF00u) >> 8u));

    return retCode;
}

/* debug */
RtkApiRet dva_drv_secy_dbg_vlan_valid_flags_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, dva_MacsecSecyVlanTagValid_t *flags)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               value        = 0u;

    CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_VLAN_PARSER2_DEBUG, &value));
    value = dva_macsec_reg_value_field_get(value, 0u, 4u);

    flags->vlanValid = ((value & (1u << 0u)) != 0u);
    flags->qInQValid = ((value & (1u << 1u)) != 0u);
    flags->stagValid = ((value & (1u << 2u)) != 0u);
    flags->qtagValid = ((value & (1u << 3u)) != 0u);

    return retCode;
}

RtkApiRet dva_drv_secy_dbg_sectag_identified_get(uint32 unit, RtkPort sysPort, RtkMacsecDirection_e direction, dva_MacsecSecySectagIdentified_t *flags)
{
    RtkApiRet            retCode      = RT_ERR_OK;
    dva_MacsecRegGetFunc macsecRegGet = dva_g_regGet[direction];
    uint32               value        = 0u;

    CHK_FUN_RET(retCode, macsecRegGet(unit, sysPort, DVA_MACSEC_SECY_SECTAG_IDENTIFIED_DEBUG, &value));
    value = dva_macsec_reg_value_field_get(value, 0u, 4u);

    flags->macsecClass = (uint8)dva_macsec_reg_value_field_get(value, 0u, 2u);
    flags->secTagValid = ((value & (1u << 2u)) != 0u);
    flags->sciValid    = ((value & (1u << 3u)) != 0u);
    flags->macsecValid = ((value & (1u << 4u)) != 0u);

    return retCode;
}
