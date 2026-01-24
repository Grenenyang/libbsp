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

#ifndef CVB_RTK_ASIC_CB_H_
#define CVB_RTK_ASIC_CB_H_

#include "rtkas_error.h"
#include "rtkas_common_types.h"
#include "rtl907xc_vb_api_cb.h"
#include "rtkas_types.h"
#include "rtl907xc_vb_asicdrv_routing.h"
#include "rtl907xc_vb_table_struct.h"
#include "rtkas_api_cb.h"
/* for Internal channel*/
#define CVB_INTERNALREAD    (0u)
#define CVB_INTERNALWRITE   (1u)
#define CVB_COMPLETE_ACCESS (0u)
#define CVB_EXECUTE_ACCESS  (1u)

#define CVB_SEQRCV_CURDIFF_INERNAL_ADDR   (0xD8u)
#define CVB_SEQRCV_RSTTIME_INERNAL_ADDR   (0x138u)
#define CVB_SEQRCV_TAKEANY_INERNAL_ADDR   (0x152u)
#define CVB_INDIVRCV_RCVNUM_INERNAL_ADDR  (0x60u)
#define CVB_INDIVRCV_RSTTIME_INERNAL_ADDR (0x108u)
#define CVB_INDIVRCV_TAKEANY_INERNAL_ADDR (0x150u)

/*for address string*/
#define CVB_IPV4LEN (4u)
#define CVB_IPV6LEN (16u)
#define CVB_MACLEN  (6u)

#define CVB_IPV6_FIG_BYTES_NUM (16u)
#define CVB_MAC_FIG_BYTES_NUM  (6u)

/* for entry read timeout*/
#define CVB_READTABLETIME (10u)

/*for internal channel*/
#define CVB_INTERNAL_DATAENTRY_BIT63       (63u)
#define CVB_INTERNAL_DATAENTRY_BIT95       (95u)
#define CVB_INTERNAL_PERDATAENTRY_BYTE_NUM (8u)

/*for sid*/
#define CVB_IPV4_FLAG (1U)
#define CVB_IPV6_FLAG (0U)

/* for lut seqidx filed */
#define CVB_MISC_TABLE_SEQRCV_IDX_DATA (0x30u)

typedef struct
{
    uint8 dstMac[6];
    uint8 srcMac[6];

    uint16 ethertype;
    uint8  ethertypeCtrl;

    uint16 vlanId;
    uint8  vlanIdCtrl;

    uint8 ipFlag;

    uint32 srcIp[4];
    uint8  sourceIpCtrl;

    uint32 dstIp[4];
    uint8  dstIpCtrl;

    uint8 dscp;
    uint8 dscpCtrl;

    uint8 nextProtocol;
    uint8 nextProtocolCtrl;

    uint16 srcPort;
    uint8  srcPortCtrl;

    uint16 dstPort;
    uint8  dstPortCtrl;

    uint32 carebits;

    uint16 portMask;
    uint8  streamHandle;

    uint8 isValid;
} cvb_CbSidTabParamIn_t;

typedef struct
{
    uint8  ctrlBitWord;
    uint8  ctrlBitoffset;
    uint8  fieldWord;
    uint16 fieldoffset;
    uint16 fieldmask;
    uint32 carebitmask;
} cvb_CbCarebitInfo_t;

EXTERN_API void      cvb_drv_cb_gen_ipv6array(uint32 *ipaddrentry, uint8 *ipaddrarray);
EXTERN_API RtkApiRet cvb_drv_cb_read_table(uint32 unit, cvb_table_list_t tableType, uint32 tableAddress, uint32 *rEntry, uint32 *errorEntry, uint64 cmpLen);
EXTERN_API RtkApiRet cvb_drv_cb_actsid_egiftbl_checkif(uint32 index, CbActiveEgifTblParm_t *actegifParm);
EXTERN_API RtkApiRet cvb_drv_cb_actsid_param2egifparam(CbActiveEgifTblParm_t *actegifParm, cvb_RtEgIfTblParm_t *egifTableParm);
EXTERN_API RtkApiRet cvb_drv_cb_misc_check_enable_status(CbLutParam_t *lutTabentry);
EXTERN_API RtkApiRet cvb_drv_cb_misc_check_value_range(CbLutParam_t *lutTabentry);
EXTERN_API RtkApiRet cvb_drv_cb_sid_param2entry(cvb_CbSidTabEntry_t *const sidTabentry, cvb_CbSidTabParamIn_t *const sidTabParam);
EXTERN_API RtkApiRet cvb_drv_cb_sid_entry2param(cvb_CbSidTabEntry_t *const sidTabentry, cvb_CbSidTabParamIn_t *const sidTabParam);
EXTERN_API RtkApiRet cvb_drv_cb_sidentry_set(uint32 unit, const uint32 index, CbSidparam_t *const sidParam);
EXTERN_API RtkApiRet cvb_drv_cb_sidentry_get(uint32 unit, uint32 index, CbSidparam_t *sidParam);
EXTERN_API RtkApiRet cvb_drv_cb_lut_param2entry(cvb_CbLutEntry_t *lutEntry, CbLutParam_t *lutParam);
EXTERN_API RtkApiRet cvb_drv_cb_lut_entry2param(cvb_CbLutEntry_t *const lutEntry, CbLutParam_t *const lutParam);
EXTERN_API RtkApiRet cvb_drv_cb_lutentry_set(uint32 unit, uint32 index, CbLutParam_t *lutParam);
EXTERN_API RtkApiRet cvb_drv_cb_lutentry_get(uint32 unit, uint32 index, CbLutParam_t *lutParam);
EXTERN_API RtkApiRet cvb_drv_cb_indivrcventry_set(uint32 unit, uint32 index, CbIndivParam_t *indivParam);
EXTERN_API RtkApiRet cvb_drv_cb_indivrcventry_get(uint32 unit, uint32 index, CbIndivParam_t *indivParam);
EXTERN_API RtkApiRet cvb_drv_cb_seqrcvtab_param2entry(cvb_CbSeqRcvTabEntry_t *const seqrRcvEntry, CbSeqRcvTabParam_t *const seqRcvparam);
EXTERN_API RtkApiRet cvb_drv_cb_seqrcvtab_entry2param(cvb_CbSeqRcvTabEntry_t *const seqrRcvEntry, CbSeqRcvTabParam_t *const seqRcvparam);
EXTERN_API RtkApiRet cvb_drv_cb_egress_seqrcv_checkif(uint32 index, CbSeqRcvTabParam_t *seqTabentry);
EXTERN_API RtkApiRet cvb_drv_cb_seqrcvtab_entry_set(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *const seqParam);
EXTERN_API RtkApiRet cvb_drv_cb_seqrcvtab_entry_get(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *const seqParam);
EXTERN_API RtkApiRet cvb_drv_cb_rtag_set(uint32 unit, uint16 rtag);
EXTERN_API RtkApiRet cvb_drv_cb_rtag_get(uint32 unit, uint32 *rtag);
EXTERN_API RtkApiRet cvb_drv_cb_seqrcv_cnt_set(uint32 unit, const uint32 index, uint64 cntPass, uint64 cntDrop);
EXTERN_API RtkApiRet cvb_drv_asic_cb_read_internal_channel(uint32 unit, uint32 index, uint32 addr, uint8 *data, uint8 byteNum);
EXTERN_API RtkApiRet cvb_drv_cb_write_8bits(uint32 unit, uint32 index, uint32 regAddr, uint32 *regLsp, uint32 regLen, uint32 data);
EXTERN_API void      cvb_drv_cb_sid_gen_wildcare_control(cvb_CbSidTabEntry_t *const sidTabentry, cvb_CbSidTabParamIn_t *const sidTabParam, uint16 *careBitsandCtrl,
                                                         uint32 selectField, uint8 entryWord, uint16 carabitmask, uint8 ctrlBitOffset);
EXTERN_API RtkApiRet cvb_drv_cb_gen_ipv6entry(CbSidparam_t *sidentryParam, cvb_CbSidTabParamIn_t *sidTabentry);

#endif /*_RTL906X_ASICDRV_CB_H_*/