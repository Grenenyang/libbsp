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

#ifndef RTKAS_API_FIR_H__
#define RTKAS_API_FIR_H__

#include "rtkas_api_routing.h"

/********* Definition *********/
#define FIR_SESSION_DIR_1_WAY (0u)
#define FIR_SESSION_DIR_2_WAY (1u)
#define L4_HDR_FMT_TCP        (0u)
#define L4_HDR_FMT_UDP        (1u)
#define L4_HDR_FMT_BOTH       (2u)
#define EGIF_TABLE_MAX        (64u)

/********* Enumeration ********/

/********* Structure **********/
typedef struct
{
    /* External */
    uint16 maxTcpConnection; /**< Maximum of active TCP connection (0 to 1024) */
    uint16 maxUdpConnection; /**< Maximum of active UDP connection (0 to 1024) */
    uint16 maxHalfOpenNum;   /**< Maximum of active TCP half-open session (0 to 1024) */
    uint32 ageOutTime;       /**< Threshold(sec) of age out the entry (60 to 0x3FFFFFFF) */
    uint32 tcpCloseWait;     /**< Threshold(sec) of age out the entry (0 to 5) */
    /* Internal */
    uint16 halfOpenNum;   /**< Current number of TCP half-open session  (0 to 1024) */
    uint16 tcpSessionNum; /**< Current number of TCP session  (0 to 1024) */
    uint16 udpSessionNum; /**< Current number of UDP session  (0 to 1024) */
} FirGlbCfg_t;

typedef struct
{
    uint8             sip[IPV6_ADDR_LEN];              /**< Array of source IP (0x0 to 0xFF for 16 elements) */
    uint8             dip[IPV6_ADDR_LEN];              /**< Array of destination IP (0x0 to 0xFF for 16 elements) */
    uint16            sp;                              /**< TCP/UDP source port (0x0 to 0xFFFF) */
    uint16            dp;                              /**< TCP/UDP destination port (0x0 to 0xFFFF) */
    uint8             valid;                           /**< Valid field (DISABLED to ENABLED) */
    uint8             spValid;                         /**< Valid field of source port (DISABLED to ENABLED) */
    uint8             dpValid;                         /**< Valid field of destination port (DISABLED to ENABLED) */
    uint8             dir;                             /**< Indicate the direction of the rule (FIR_SESSION_DIR_1_WAY to FIR_SESSION_DIR_2_WAY) */
    uint8             protocol;                        /**< Protocol of the rule (L4_HDR_FMT_TCP to L4_HDR_FMT_BOTH) */
    RtHostEntryType_e ipType;                          /**< Ip tpye of the rule (EM_RT_UC4 to EM_RT_MC6) */
    uint8             fwdL2Bind;                       /**< L2 Binding status of forward direction(DISABLED to ENABLED) */
    uint8             backL2Bind;                      /**< L2 Binding status of backward direction(DISABLED to ENABLED) */
    uint16            fwdL2;                           /**< Binded L2 index of forward direction(0 to 4095) */
    uint16            backL2;                          /**< Binded L2 index of backward direction(0 to 4095) */
    uint8             fwdEgrIf;                        /**< Binded egress interface of forward direction(0 to 63) */
    uint8             backEgrIf;                       /**< Binded egress interface of backward direction(0 to 63) */
    uint32            egrBitmap[EGIF_TABLE_MAX / 32u]; /**< Egress(DISABLED to ENABLED) */
} FirRuleTblEntry_t;

typedef struct
{
    uint8 enable; /**< Enable/Disable stateful feature (DISABLED to ENABLED) */
} FirEnable_t;

typedef struct
{
    uint8             index;     /**< A rule entry index (0 to 255) */
    FirRuleTblEntry_t ruleEntry; /**< A rule entry data (N/A) */
} FirRuleTblGet_t;

/********* API Declaration **********/
EXTERN_API RtkApiRet rtk_fir_enable(UnitChip_t unitChip, const FirEnable_t *param);
EXTERN_API RtkApiRet rtk_fir_glb_cfg_set(UnitChip_t unitChip, const FirGlbCfg_t *glbCfg);
EXTERN_API RtkApiRet rtk_fir_glb_cfg_get(UnitChip_t unitChip, FirGlbCfg_t *glbCfg);
EXTERN_API RtkApiRet rtk_fir_rule_tbl_set(UnitChip_t unitChip, const FirRuleTblEntry_t *ruleEntry);
EXTERN_API RtkApiRet rtk_fir_rule_tbl_get(UnitChip_t unitChip, FirRuleTblGet_t *param);
#endif /* RTKAS_API_FIR_H__ */