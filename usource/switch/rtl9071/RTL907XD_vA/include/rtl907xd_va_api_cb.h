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

#ifndef DVA_RTK_API_CB_H_
#define DVA_RTK_API_CB_H_

/********* Include Header *********/
#include "rtl907xd_va_asicdrv_routing.h"
#include "rtkas_api_cb.h"
/********* Definition *********/
/* for entry range */
#define DVA_FRER_L4PORT_RANGE_CHKTBL_INSTANCE_NUM_MAX (4u)
#define DVA_FRER_SEQRCV_INSTANCE_NUM_MAX              (48U)
#define DVA_FRER_MISC_INSTANCE_NUM_MAX                (96U)
#define DVA_FRER_INDIVRCV_INSTANCE_NUM_MAX            (96U)
#define DVA_FRER_RH_INSTANCE_MAX                      (96U)
#define DVA_FRER_EGIF_INSTANCE                        (96U)
#define DVA_FRER_OIL_INSTANCE_NUM_MAX                 (96U)
#define DVA_FRER_RH_TABLE_OFFSET                      (DVA_RH_TABLE_MAX - DVA_FRER_RH_INSTANCE_MAX)
#define DVA_FRER_OIL_TABLE_OFFSET                     (DVA_OIL_TABLE_MAX - DVA_FRER_OIL_INSTANCE_NUM_MAX)
#define DVA_FRER_EGIF_TABLE_OFFSET                    (DVA_EGIF_TABLE_MAX - DVA_FRER_EGIF_INSTANCE)

/*for global control*/
#define DVA_FRER_INT_STATUS_BIT_31TH (31u)
#define DVA_FRER_INT_STATUS_BIT_47TH (47u)

/****for SID entry*****/
#define DVA_FRER_VID_MAX                     (0xFFFu)
#define DVA_FRER_USER_PMSK_MAX               (0x1FFEu)
#define DVA_FERE_SID_VALIDFIELD_WORD_NUM     (6u)
#define DVA_FERE_SID_DATAFIELD_WORDS_CNT     (15u)
#define DVA_FERE_SID_CAREFIELD_LAST_WORD_NUM (30u)
#define DVA_FRER_VIDPRI_MAX                  (7u)
#define DVA_FRER_L4PORT_RANGE_FILED_MAX      (0xFu)

/*****for SeqRCV entry*****/
/* SeqRCV entry parameter max*/
#define DVA_FRER_HISLEN_MAX   (0x100u)
#define DVA_FRER_HISLEN_MIN   (0x2u)
#define DVA_FRER_LATHPATH_MIN (2u)
#define DVA_FRER_LATHPATH_MAX (96u)
#define DVA_LATE_ERR_OCCUR    (1u)
#define DVA_LATE_ERR_CLEAR    (0u)
#define DVA_VECTOR            (0U)
#define DVA_MATCH             (1U)

/*****for activeSID entry*****/
/*active sid parameter max*/
#define DVA_FRER_PCP_MAX            (0x7u)
#define DVA_ACTIVE_RHTBL_FIELD_MAX  (((uint16)1u << 13u) | (DVA_FRER_OIL_INSTANCE_NUM_MAX - 1u))
#define DVA_ACTIVE_RHTBL_FIELDS_NUM (12u)

/*****for Internal channel*****/

#define DVA_STREAMID_CNTPERSTERAM_INERNAL_ADDR (0x0u)
#define DVA_SEQRCV_CNTDROP_INERNAL_ADDR        (0x78u)
#define DVA_SEQRCV_CNTPASS_INERNAL_ADDR        (0xA8u)
#define DVA_STREAMID_CNTPERPORT_INERNAL_ADDR   (0x153u)

/*****for timer range*****/
#define DVA_FRER_LATENT_TEST_TIMER_MAX   (0xFFFFFFFFu)
#define DVA_FRER_LATENT_RETEST_TIMER_MAX (0xFFFFFFFFu)
#define DVA_FRER_INDIVRCV_TIMER_MAX      (0xFFFFFFFFu)
/*****for interrupt range*****/
#define DVA_FRER_INTERRUPT_MASK_MAX (0xFFFFFFFFFFFFu)

/*****for counter range*****/
#define DVA_FRER_SID_STREAM_CNT_MAX  (0xFFFFFFFFFFFFFFFFu)
#define DVA_FRER_SID_PORT_CNT_MAX    (0xFFFFFFFFFFFFFFFFu)
#define DVA_FRER_SEQRCV_PASS_CNT_MAX (0xFFFFFFFFFFFFFFFFu)
#define DVA_FRER_SEQRCV_DROP_CNT_MAX (0xFFFFFFFFFFFFFFFFu)

/*****for split dpm mode*****/
#define DVA_FRER_SPLIT_DPM_OVERWRITE (0u)
#define DVA_FRER_SPLIT_DPM_EXTEND    (1u)

/********* Structure *********/
typedef struct
{
    uint32 val[31]; /**< Value of passive SID entry (N/A) */
} dva_CbSidTabEntry_t;

typedef struct
{
    uint32 val[3]; /**< Value of MISC entry (N/A) */
} dva_CbLutEntry_t;

typedef struct
{
    uint32 val[3]; /**< Value of sequence recovery entry (N/A) */
} dva_CbSeqRcvTabEntry_t;

/********* Function Declaration *********/
EXTERN_API RtkApiRet dva_cb_parse_rtag_enabled_set(uint32 unit, RtkPort port, RtkEnable on);
EXTERN_API RtkApiRet dva_cb_parse_rtag_enabled_get(uint32 unit, RtkPort port, RtkEnable *on);
EXTERN_API RtkApiRet dva_cb_enabled_set(uint32 unit, RtkEnable status);
EXTERN_API RtkApiRet dva_cb_enabled_get(uint32 unit, RtkEnable *status);
EXTERN_API RtkApiRet dva_cb_sid_enabled_set(uint32 unit, RtkEnable status);
EXTERN_API RtkApiRet dva_cb_sid_enabled_get(uint32 unit, RtkEnable *status);
EXTERN_API RtkApiRet dva_cb_misc_set(uint32 unit, uint32 index, CbLutParam_t *lutTabentry);
EXTERN_API RtkApiRet dva_cb_misc_get(uint32 unit, uint32 index, CbLutParam_t *lutTabentry);

EXTERN_API RtkApiRet dva_cb_laten_timer_set(uint32 unit, uint32 lattimvalue);
EXTERN_API RtkApiRet dva_cb_laten_timer_get(uint32 unit, uint32 *lattimvalue);
EXTERN_API RtkApiRet dva_cb_laten_reset_timer_set(uint32 unit, uint32 latrsttimvalue);
EXTERN_API RtkApiRet dva_cb_laten_reset_timer_get(uint32 unit, uint32 *latrsttimvalue);

EXTERN_API RtkApiRet dva_cb_splitdpm_mode_set(uint32 unit, uint32 splitDpmMode);
EXTERN_API RtkApiRet dva_cb_splitdpm_mode_get(uint32 unit, uint32 *splitDpmMode);

EXTERN_API RtkApiRet dva_cb_interrupt_enable_set(uint32 unit, RtkEnable status);
EXTERN_API RtkApiRet dva_cb_interrupt_enable_get(uint32 unit, RtkEnable *status);
EXTERN_API RtkApiRet dva_cb_interrupt_flag_get(uint32 unit, uint32 *flag);
EXTERN_API RtkApiRet dva_cb_interrupt_mask_get(uint32 unit, uint64 *status);
EXTERN_API RtkApiRet dva_cb_interrupt_state_clear(uint32 unit, uint32 index);
EXTERN_API RtkApiRet dva_cb_interrupt_state_get(uint32 unit, uint32 index, uint32 *status);

EXTERN_API RtkApiRet dva_cb_ingress_passive_sid_set(uint32 unit, uint32 index, CbSidparam_t *sidentryParam);
EXTERN_API RtkApiRet dva_cb_ingress_passive_sid_get(uint32 unit, uint32 index, CbSidparam_t *sidentryParam);
EXTERN_API RtkApiRet dva_cb_ingress_indrcv_set(uint32 unit, uint32 index, CbIndivParam_t *indivTabentry);
EXTERN_API RtkApiRet dva_cb_ingress_indrcv_get(uint32 unit, uint32 index, CbIndivParam_t *indivTabentry);
EXTERN_API RtkApiRet dva_cb_egress_seqrcv_set(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *seqTabentry);
EXTERN_API RtkApiRet dva_cb_egress_seqrcv_get(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *seqTabentry);
EXTERN_API RtkApiRet dva_cb_actsid_rhtbl_set(uint32 unit, uint32 rhIndex, const CbActsidRhTblParm_t *rhEntryParm);
EXTERN_API RtkApiRet dva_cb_actsid_rhtbl_get(uint32 unit, uint32 rhIndex, CbActsidRhTblParm_t *rhEntryParm);
EXTERN_API RtkApiRet dva_cb_actsid_oiltbl_set(uint32 unit, uint32 oilIndex, CbActsidOiltblParm_t *oilEntryParm);
EXTERN_API RtkApiRet dva_cb_actsid_oiltbl_get(uint32 unit, uint32 oilIndex, CbActsidOiltblParm_t *oilEntryParm);
EXTERN_API RtkApiRet dva_cb_actsid_egiftbl_set(uint32 unit, uint32 egifIndex, CbActiveEgifTblParm_t *actegifParm);
EXTERN_API RtkApiRet dva_cb_actsid_egiftbl_get(uint32 unit, uint32 egifIndex, CbActiveEgifTblParm_t *actegifParm);

EXTERN_API RtkApiRet dva_cb_ingress_perstream_idcnt_get(uint32 unit, uint32 index, uint64 *identityNum);
EXTERN_API RtkApiRet dva_cb_ingress_perport_idcnt_get(uint32 unit, RtkPort port, uint64 *identityNum);
EXTERN_API RtkApiRet dva_cb_egress_seqrcv_dropcnt_get(uint32 unit, uint32 index, uint64 *dropcnt);
EXTERN_API RtkApiRet dva_cb_egress_seqrcv_passcnt_get(uint32 unit, uint32 index, uint64 *passcnt);

EXTERN_API RtkApiRet dva_cb_ingress_passive_sid_reset(uint32 unit, uint32 index);
EXTERN_API RtkApiRet dva_cb_ingress_indrcv_reset(uint32 unit, uint32 index);
EXTERN_API RtkApiRet dva_cb_egress_seqrcv_reset(uint32 unit, uint32 index);
EXTERN_API RtkApiRet dva_cb_misc_reset(uint32 unit, uint32 index);
EXTERN_API RtkApiRet dva_cb_reset(uint32 unit);
EXTERN_API RtkApiRet dva_cb_egress_seqhistory_get(uint32 unit, uint32 index, CbSeqHistoryEntry_t *seqHistoryEntry);

#endif /* RTK_API_CB_H__ */
