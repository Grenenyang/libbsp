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

#ifndef RTKAS_API_CB_H__
#define RTKAS_API_CB_H__
/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common.h"
/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint32 cb_lattest_timer;
typedef uint32 cb_latrst_timer;
typedef uint64 cb_intMask;
typedef uint64 cb_counter;
/******************************************************************************/
/* Enumeration                                                                */
/******************************************************************************/

/******************************************************************************/
/* Struct                                                                     */
/******************************************************************************/
/* Structure of passive stream identification entry */
typedef struct
{
    uint8 dstMac[6]; /**< 8-bit array of MAC address (0x0 to 0xFF for 6 elements) */
    uint8 srcMac[6]; /**< 8-bit array of MAC address (0x0 to 0xFF for 6 elements) */

    uint16 ethertype; /**< The ethertype that used to identify (0 to 0xFFFF) */
    uint16 vlanId;    /**< The vlan ID that used to identify (0 to 4095) */

    uint8 ipFlag; /**< Indicate IP type that used to identify (0 to 1) */

    uint8 dstIp[16]; /**< 8-bit array of IP address (0x0 to 0xFF for 16 elements) */
    uint8 srcIp[16]; /**< 8-bit array of IP address (0x0 to 0xFF for 16 elements) */

    uint8 dscp;         /**< That used to identify the stream (0 to 0xFF) */
    uint8 nextProtocol; /**< The IP next protocol parameter used to match UDP,TCP,SCTP (0 to 0xFF) */

    uint16 srcPort; /**< Specifies the TCP or UDP Source Port that that used to match (0 to 0xFFFF) */
    uint16 dstPort; /**< Specifies the TCP or UDP Destination Port that used to match (0 to 0xFFFF) */

    uint32 carebits;     /**< Indicates the carefields for passive stream identification (0 to 0x3FF) */
    uint16 portMask;     /**< The entry is instantiated on one or more ports (0x1 to 0xFFE) */
    uint8  streamHandle; /**< Stream handle (0 to 95) */
    uint8  isValid;      /**< Entry valid (0 to 1) */

#if defined(RTL_907XD_VA)
    uint8 vlanPri;         /**< The vlan priority that used to identify (0 to 7) */
    uint8 l4PortChkTblMap; /**< L4Port range checking table index maskbits mapping (0 to 0xF,0 indicate not to use range checking) */
#endif

} CbSidparam_t;

/* Structure of CB MISC entry */
typedef struct
{
    uint8  indRcvIdx;   /**< Individual recovery entry index (0 to 95) */
    uint8  indRcvEn;    /**< Individual recovery enable bit (0 to 1) */
    uint8  seqRcvIdx;   /**< Sequence recovery entry index (0 to 47) */
    uint8  seqRcvEn;    /**< Sequence recovery enable bit (0 to 1) */
    uint16 splitDPM;    /**< Destination port mask of splitting function (0x1 to 0xFFE) */
    uint8  splitEn;     /**< Splitting function enable (0 to 1) */
    uint16 genSeqNum;   /**< Generation Sequence number (0 to 65535) */
    uint8  genSeqNumEn; /**< Generation Sequence number enable (0 to 1) */
    uint16 activeIdx;   /**< Active Sequence Identification index (0 to 2047) */
    uint8  activeEn;    /**< Active Sequence Identification enable (0 to 1) */
    uint8  encEn;       /**< Encode enable at egress port (0 to 1) */
} CbLutParam_t;

/* Structure of individual recovery entry */
typedef struct
{
    uint16 rcvnum;  /**< Recovery Sequence number (0 to 65535) */
    uint32 rstload; /**< IndRcv reset timer load value, unit in us (0 to 0xFFFFFFFF) */
    uint32 rsttime; /**< IndRcv reset timer,unit in us (0 to 0xFFFFFFFF) */
    uint8  takeany; /**< IndRcv takeany bit (0 to 1) */
    uint8  valid;   /**< Entry valid (0 to 1) */
} CbIndivParam_t;

/* Structure of sequence recovery entry */
typedef struct
{
    uint16 hisLen; /**< History length (2 to 256) */
    uint16 hisIdx; /**< The history head index (0 to 511) */
    uint16 rcvNum; /**< Recovery Sequence number (0 to 0xFFFF) */
    uint8  takeno; /**< The flag that the Entry will receive no sequence number frame (0 to 1) */
    uint8  alg;    /**< Algorithm choose (0 to 1) */
    uint8  valid;  /**< Entry valid (0 to 1) */

    uint64 expDiff;  /**< Expected Latent error difference (0 to 0xFFFFFFFFFFFFFFFF) */
    uint32 rstLoad;  /**< SeqRcv reset timer load value, unit in us (0 to 0xFFFFFFFF) */
    uint8  latPaths; /**< The number of member streams (2 to 96) */
    uint8  laten;    /**< Latent error enable status (0 to 1) */

    int64  cntDiff; /**< Current difference of latent error (-2^63 to 2^63-1) */
    uint32 rsttime; /**< SeqRcv reste timer, unit in us (0 to 0xFFFFFFFF) */
    uint8  takeany; /**< SeqRcv takeany bit (0 to 1) */
} CbSeqRcvTabParam_t;

/* Structure of CB replication header entry */
typedef struct
{
    uint16 portRhIdx[15];
} CbActsidRhTblParm_t;

/* Structure of CB outgoing interface entry */
typedef struct
{
    uint8  egifIdx;      /**< Outgoing interface index(0u to 95 */
    uint8  oilnextValid; /**< valid bit(0u to 1u)*/
    uint16 nextOilIdx;   /**< next OIL entry index(0u to 95)*/
} CbActsidOiltblParm_t;

/* Structure of CB egress interface entry */
typedef struct
{
    uint8  saReplaceEn;  /**< SA replace enable field (0 to 1), only one of saReplaceEn and daReplaceEn can be enabled */
    uint8  daReplaceEn;  /**< DA replace enable field (0 to 1), only one of saReplaceEn and daReplaceEn can be enabled */
    uint8  vidReplaceEn; /**< VID replace enable field (0 to 1) */
    uint8  pcpReplaceEn; /**< PCP replace enable field (0 to 1) */
    uint8  mac[6];       /**< 8-bit array of MAC address (0x0 to 0xFF for 6 elements) */
    uint16 vid;          /**< VID used for replacement (0 to 4095) */
    uint8  pcp;          /**< pcp used for replacement (0 to 7) */

} CbActiveEgifTblParm_t;

/* Structure of CB sequence recovery history entry */
typedef struct
{
    uint32 val[17]; /**< Value of sequence history entry (N/A) */
} CbSeqHistoryEntry_t;

/**
 * @brief This structure represents the enable status of the R-tag parser for a specified port.
 *
 * This structure contains the switch port number and its enable status of the R-tag parser.
 */
typedef struct
{
    RtkPort   port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable status; /**< [in,out] R-tag parser enable state: 0 means disabled, 1 means enabled (0 to 1) */

} CbParseRtagEnabledInfo_t;

/**
 * @brief This structure represents the enable status of CB (FRER).
 *
 * This structure contains the enable status of CB (FRER).
 */
typedef struct
{
    RtkEnable status; /**< [in,out] CB function enable status: 0 means disabled, 1 means enabled (0 to 1) */

} CbEnabledInfo_t;

/**
 * @brief This structure represents the enable status of stream identification.
 *
 * This structure contains the enable status of stream identification.
 */
typedef struct
{
    RtkEnable status; /**< [in,out] Stream identification enable status: 0 means disabled, 1 means enabled (0 to 1) */

} CbSidEnabledInfo_t;

/**
 * @brief This structure represents the reset information for a specified stream identification.
 *
 * This structure includes the index of a specific stream identification.
 */
typedef struct
{
    uint32 index; /**< [in] Passive stream identification entry index: There are <CHIPID>_FRER_SID_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_SID_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_SID_INSTANCE_NUM_MAX-1u) */

} CbIngressPassiveSidResetInfo_t;

/**
 * @brief This structure represents the reset information for a specified individual recovery entry.
 *
 * This structure includes the index of a specific individual recovery entry.
 */
typedef struct
{
    uint32 index; /**< [in] IndRcv entry index: There are <CHIPID>_FRER_INDIVRCV_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_INDIVRCV_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_INDIVRCV_INSTANCE_NUM_MAX-1u) */

} CbIngressIndrcvResetInfo_t;

/**
 * @brief This structure represents the reset information for a specified sequence recovery entry.
 *
 * This structure includes the index of a specific sequence recovery entry.
 */
typedef struct
{
    uint32 index; /**< [in] Sequence recovery entry index: There are <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1u) */

} CbEgressSeqrcvResetInfo_t;

/**
 * @brief This structure represents the reset information for a specified MISC entry.
 *
 * This structure includes the index of a specific MISC entry.
 */
typedef struct
{
    uint32 index; /**< [in] LUT entry index: There are <CHIPID>_FRER_MISC_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_MISC_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_MISC_INSTANCE_NUM_MAX-1u) */

} CbMiscResetInfo_t;

/**
 * @brief This structure represents the information for the latent error test timer.
 *
 * This structure includes the value of the latent error test timer.
 */
typedef struct
{
    cb_lattest_timer lattimvalue; /**< [in,out] Latent error test timer (0u to <CHIPID>_FRER_LATENT_TEST_TIMER_MAX) */
} CbLatenTimerInfo_t;

/**
 * @brief This structure represents the information for the latent error retest timer.
 *
 * This structure includes the value of the latent error retest timer.
 */
typedef struct
{
    cb_latrst_timer latrsttimvalue; /**< [in,out] Latent error reset time (0u to <CHIPID>_FRER_LATENT_RETEST_TIMER_MAX) */
} CbLatenResetTimerInfo_t;

/**
 * @brief This structure represents the latent error interrupt enable state.
 *
 * This structure includes the state of the latent error interrupt enable mechanism.
 */
typedef struct
{
    RtkEnable status; /**< [in,out] Interrupt enable status: 0 means disabled, 1 means enabled (0 to 1)*/

} CbInterruptEnableInfo_t;

/**
 * @brief This structure represents the latent error interrupt flag information.
 *
 * This structure includes the information related to the latent error interrupt flag.
 */
typedef struct
{
    uint32 flag; /**< [out] Interrupt flag: 0 indicates no global interrupt, 1 indicates a global interrupt (0 to 1) */

} CbInterruptFlagInfo_t;

/**
 * @brief This structure represents the mapping mask for entries where a latent error interrupt has occurred.
 *
 * This structure includes the mapping mask information for entries that have experienced a latent error interrupt.
 */
typedef struct
{
    cb_intMask status; /**< [out] Latent error status (0u to <CHIPID>_FRER_INTERRUPT_MASK_MAX) */

} CbInterruptMaskInfo_t;

/**
 * @brief This structure represents whether a specific sequence recovery entry has experienced a latent error interrupt.
 *
 * This structure includes the index of the specific sequence recovery entry and the status of the latent error.
 */
typedef struct
{
    uint32 index;  /**< [in] Sequence recovery entry index: There are <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1u) */
    uint32 status; /**< [out] Latent error status: 0 indicates no latent error, 1 indicates a latent error (0 to 1) **/

} CbInterruptPerStateInfo_t;

/**
 * @brief This structure represents the configuration information of a stream identification.
 *
 * This structure includes the index of the stream identification and its packets match rules.
 */
typedef struct
{
    uint32       index;         /**< [in] Entry index: There are <CHIPID>_FRER_SID_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_SID_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_SID_INSTANCE_NUM_MAX-1u) */
    CbSidparam_t sidentryParam; /**< [in,out] Passive stream identification entry parameters (N/A) */

} CbIngressPassiveSidInfo_t;

/**
 * @brief This structure represents the match counter information for a specific stream identification.
 *
 * This structure includes the index of the stream identification and its match counter value.
 */
typedef struct
{
    uint32     index;       /**< [in] Entry index: There are <CHIPID>_FRER_SID_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_SID_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_SID_INSTANCE_NUM_MAX-1u) */
    cb_counter identityNum; /**< [out] Per-stream identification counter value (0u to <CHIPID>_FRER_SID_STREAM_CNT_MAX) */

} CbIngressPerstreamIdcntInfo_t;
/**
 * @brief This structure represents the counter for a specific port match stream identification.
 *
 * This structure includes the port number and its match counter value.
 */
typedef struct
{
    RtkPort    port;        /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    cb_counter identityNum; /**< [out] Per-port identification counter value (0u to <CHIPID>_FRER_SID_PORT_CNT_MAX) */

} CbIngressPerportIdcntInfo_t;

/**
 * @brief This structure represents the configuration information of a MISC entry.
 *
 * This structure includes the index of the MISC entry and information about it, including the recovery function, sequence number generation function, \n
 *        stream splitting function, active stream identification function, and encode function.
 */
typedef struct
{
    uint32       index;       /**< [in] Entry index: There are <CHIPID>_FRER_MISC_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_MISC_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_MISC_INSTANCE_NUM_MAX-1u) */
    CbLutParam_t lutTabParam; /**< [in,out] MISC table entry parameters (N/A) */

} CbMiscInfo_t;

/**
 * @brief This structure represents the configuration information of an individual recovery entry.
 *
 * This structure includes the index of the individual recovery entry and status of individual recovery function.
 */
typedef struct
{
    uint32         index;       /**< [in] Entry index: There are <CHIPID>_FRER_INDIVRCV_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_INDIVRCV_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_INDIVRCV_INSTANCE_NUM_MAX-1u) */
    CbIndivParam_t indTabParam; /**< [in,out] Individual recovery table entry parameters (N/A) */

} CbIngressIndrcvInfo_t;

/**
 * @brief This structure represents the configuration information of a sequence recovery entry.
 *
 * This structure includes the index of the sequence recovery entry and status of sequence recovery function.
 */
typedef struct
{
    uint32             index;       /**< [in] Entry index: There are <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1u) */
    CbSeqRcvTabParam_t seqTabParam; /**< [in,out] Sequence recovery table entry parameters (N/A) */

} CbEgressSeqrcvInfo_t;

/**
 * @brief This structure represents a counter for recording the number of discarded packets for a specific sequence recovery entry.
 *
 * This structure contains the index of the sequence recovery entry and the count of packets that have been discarded.
 */
typedef struct
{
    uint32     index;   /**< [in] Entry index: There are <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1u) */
    cb_counter dropcnt; /**< [out] The counter value of dropped packets (0u to <CHIPID>_FRER_SEQRCV_DROP_CNT_MAX) */

} CbEgressSeqrcvDropcntInfo_t;

/**
 * @brief This structure represents a counter that records the number of packets passed through a specified sequence recovery entry.
 *
 * This structure contains the index of the sequence recovery entry and the count of packets that have passed through it.
 */
typedef struct
{
    uint32     index;   /**< [in] Entry index: There are <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1u) */
    cb_counter passcnt; /**< [out] The counter value of passed packets (0u to <CHIPID>_FRER_SEQRCV_PASS_CNT_MAX) */

} CbEgressSeqrcvPasscntInfo_t;

/**
 * @brief This structure records the historical information of a specified sequence recovery entry.
 *
 * This structure contains the index of the sequence recovery entry and its historical information.
 */
typedef struct
{
    uint32              index;           /**< [in] Entry index: There are <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_SEQRCV_INSTANCE_NUM_MAX-1u) */
    CbSeqHistoryEntry_t seqHistoryEntry; /**< [in,out] Sequence recovery history entry (N/A) */

} CbEgressSeqhistoryInfo_t;

/**
 * @brief This structure represents the configuration information of a replication header entry.
 *
 * This structure includes the index of the replication header entry and the entry information.It specifies which port's outgoing packets need to be modified, \n
 * and specifies the corresponding outgoing interface list start index.
 */
typedef struct
{
    uint32              index;       /**< [in] Entry index: There are <CHIPID>_FRER_RH_INSTANCE_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_RH_INSTANCE_MAX-1. (0u to <CHIPID>_FRER_RH_INSTANCE_MAX-1u) */
    CbActsidRhTblParm_t rhEntryParm; /**< [in,out] Replication header entry parameters (N/A) */

} CbActsidRhtblInfo_t;

/**
 * @brief This structure represents the configuration information of an outgoing interface list entry.
 *
 * This structure includes the index of the outgoing interface list entry and the entry information.It contains an index pointing to \n
 * next outgoing interface list entry and another index pointing to the corresponding egress interface table.
 */
typedef struct
{
    uint32               index;        /**< [in] Entry index: There are <CHIPID>_FRER_OIL_INSTANCE_NUM_MAX instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_OIL_INSTANCE_NUM_MAX-1. (0u to <CHIPID>_FRER_OIL_INSTANCE_NUM_MAX-1u) */
    CbActsidOiltblParm_t oilEntryParm; /**< [in,out] Outgoing interface list entry parameters (N/A) */

} CbActsidOiltblInfo_t;

/**
 * @brief This structure represents the configuration information of an egress interface entry.
 *
 * This structure includes the index of the egress interface entry and the entry information.It specifies the contents of the packet to be modified.
 */
typedef struct
{
    uint32                index;       /**< [in] Entry index: There are <CHIPID>_FRER_EGIF_INSTANCE instances, with the index starting at 0. Therefore, the maximum index is <CHIPID>_FRER_EGIF_INSTANCE-1. (0u to <CHIPID>_FRER_EGIF_INSTANCE-1u) */
    CbActiveEgifTblParm_t actegifParm; /**< [in,out] Egress interface entry parameters (N/A) */

} CbActsidEgiftblInfo_t;

/**
 * @brief This structure records the configuration information for the mode of splitting the destination port mask.
 *
 * This structure contains the configuration mode for splitting the destination port mask.
 */
typedef struct
{
    uint32 splitDpmMode; /**< [in,out] The mode of splitting the destination port mask (<CHIPID>_FRER_SPLIT_DPM_OVERWRITE to <CHIPID>_FRER_SPLIT_DPM_EXTEND) */
} CbSplitDpmModeInfo_t;
/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_cb_parse_rtag_enabled_set(UnitChip_t unitChip, CbParseRtagEnabledInfo_t *cbParseRtagEnabledInfo);
EXTERN_API RtkApiRet rtk_cb_parse_rtag_enabled_get(UnitChip_t unitChip, CbParseRtagEnabledInfo_t *cbParseRtagEnabledInfo);
EXTERN_API RtkApiRet rtk_cb_enabled_set(UnitChip_t unitChip, CbEnabledInfo_t *cbEnabledInfo);
EXTERN_API RtkApiRet rtk_cb_enabled_get(UnitChip_t unitChip, CbEnabledInfo_t *cbEnabledInfo);
EXTERN_API RtkApiRet rtk_cb_sid_enabled_set(UnitChip_t unitChip, CbSidEnabledInfo_t *cbSidEnabledInfo);
EXTERN_API RtkApiRet rtk_cb_sid_enabled_get(UnitChip_t unitChip, CbSidEnabledInfo_t *cbSidEnabledInfo);

EXTERN_API RtkApiRet rtk_cb_ingress_passive_sid_reset(UnitChip_t unitChip, CbIngressPassiveSidResetInfo_t *cbIngressPassiveSidReset);
EXTERN_API RtkApiRet rtk_cb_ingress_indrcv_reset(UnitChip_t unitChip, CbIngressIndrcvResetInfo_t *cbIngressIndrcvReset);
EXTERN_API RtkApiRet rtk_cb_egress_seqrcv_reset(UnitChip_t unitChip, CbEgressSeqrcvResetInfo_t *cbEgressSeqrcvReset);
EXTERN_API RtkApiRet rtk_cb_misc_reset(UnitChip_t unitChip, CbMiscResetInfo_t *cbMiscReset);
EXTERN_API RtkApiRet rtk_cb_reset(UnitChip_t unitChip);

EXTERN_API RtkApiRet rtk_cb_laten_timer_set(UnitChip_t unitChip, CbLatenTimerInfo_t *cbLatenTimerInfo);
EXTERN_API RtkApiRet rtk_cb_laten_timer_get(UnitChip_t unitChip, CbLatenTimerInfo_t *cbLatenTimerInfo);
EXTERN_API RtkApiRet rtk_cb_laten_reset_timer_set(UnitChip_t unitChip, CbLatenResetTimerInfo_t *cbLatenResetTimerInfo);
EXTERN_API RtkApiRet rtk_cb_laten_reset_timer_get(UnitChip_t unitChip, CbLatenResetTimerInfo_t *cbLatenResetTimerInfo);
EXTERN_API RtkApiRet rtk_cb_interrupt_enable_get(UnitChip_t unitChip, CbInterruptEnableInfo_t *cbInterruptEnableInfo);
EXTERN_API RtkApiRet rtk_cb_interrupt_enable_set(UnitChip_t unitChip, CbInterruptEnableInfo_t *cbInterruptEnableInfo);
EXTERN_API RtkApiRet rtk_cb_interrupt_flag_get(UnitChip_t unitChip, CbInterruptFlagInfo_t *cbInterruptFlagInfo);
EXTERN_API RtkApiRet rtk_cb_interrupt_mask_get(UnitChip_t unitChip, CbInterruptMaskInfo_t *cbInterruptMaskInfo);
EXTERN_API RtkApiRet rtk_cb_interrupt_state_clear(UnitChip_t unitChip, CbInterruptPerStateInfo_t *cbInterruptStateClear);
EXTERN_API RtkApiRet rtk_cb_interrupt_state_get(UnitChip_t unitChip, CbInterruptPerStateInfo_t *cbInterruptStateInfo);

EXTERN_API RtkApiRet rtk_cb_ingress_passive_sid_set(UnitChip_t unitChip, CbIngressPassiveSidInfo_t *cbIngressPassiveSidInfo);
EXTERN_API RtkApiRet rtk_cb_ingress_passive_sid_get(UnitChip_t unitChip, CbIngressPassiveSidInfo_t *cbIngressPassiveSidInfo);
EXTERN_API RtkApiRet rtk_cb_ingress_perstream_idcnt_get(UnitChip_t unitChip, CbIngressPerstreamIdcntInfo_t *cbIngressPerstreamIdcntInfo);
EXTERN_API RtkApiRet rtk_cb_ingress_perport_idcnt_get(UnitChip_t unitChip, CbIngressPerportIdcntInfo_t *cbIngressPerportIdcntInfo);

EXTERN_API RtkApiRet rtk_cb_misc_set(UnitChip_t unitChip, CbMiscInfo_t *cbMiscInfo);
EXTERN_API RtkApiRet rtk_cb_misc_get(UnitChip_t unitChip, CbMiscInfo_t *cbMiscInfo);
EXTERN_API RtkApiRet rtk_cb_splitdpm_mode_set(UnitChip_t unitChip, CbSplitDpmModeInfo_t *cbSplitDpmModeInfo);
EXTERN_API RtkApiRet rtk_cb_splitdpm_mode_get(UnitChip_t unitChip, CbSplitDpmModeInfo_t *cbSplitDpmModeInfo);

EXTERN_API RtkApiRet rtk_cb_ingress_indrcv_set(UnitChip_t unitChip, CbIngressIndrcvInfo_t *cbIngressIndrcvInfo);
EXTERN_API RtkApiRet rtk_cb_ingress_indrcv_get(UnitChip_t unitChip, CbIngressIndrcvInfo_t *cbIngressIndrcvInfo);

EXTERN_API RtkApiRet rtk_cb_egress_seqrcv_set(UnitChip_t unitChip, CbEgressSeqrcvInfo_t *cbEgressSeqrcvInfo);
EXTERN_API RtkApiRet rtk_cb_egress_seqrcv_get(UnitChip_t unitChip, CbEgressSeqrcvInfo_t *cbEgressSeqrcvInfo);
EXTERN_API RtkApiRet rtk_cb_egress_seqrcv_dropcnt_get(UnitChip_t unitChip, CbEgressSeqrcvDropcntInfo_t *cbEgressSeqrcvDropcntInfo);
EXTERN_API RtkApiRet rtk_cb_egress_seqrcv_passcnt_get(UnitChip_t unitChip, CbEgressSeqrcvPasscntInfo_t *cbEgressSeqrcvPasscntInfo);
EXTERN_API RtkApiRet rtk_cb_egress_seqhistory_get(UnitChip_t unitChip, CbEgressSeqhistoryInfo_t *cbEgressSeqhistoryInfo);

EXTERN_API RtkApiRet rtk_cb_actsid_rhtbl_set(UnitChip_t unitChip, CbActsidRhtblInfo_t *cbActsidRhtblInfo);
EXTERN_API RtkApiRet rtk_cb_actsid_rhtbl_get(UnitChip_t unitChip, CbActsidRhtblInfo_t *cbActsidRhtblInfo);
EXTERN_API RtkApiRet rtk_cb_actsid_oiltbl_set(UnitChip_t unitChip, CbActsidOiltblInfo_t *cbActsidOiltblInfo);
EXTERN_API RtkApiRet rtk_cb_actsid_oiltbl_get(UnitChip_t unitChip, CbActsidOiltblInfo_t *cbActsidOiltblInfo);
EXTERN_API RtkApiRet rtk_cb_actsid_egiftbl_set(UnitChip_t unitChip, CbActsidEgiftblInfo_t *cbActsidEgiftblInfo);
EXTERN_API RtkApiRet rtk_cb_actsid_egiftbl_get(UnitChip_t unitChip, CbActsidEgiftblInfo_t *cbActsidEgiftblInfo);

#endif /* RTKAS_API_CB_H__ */
