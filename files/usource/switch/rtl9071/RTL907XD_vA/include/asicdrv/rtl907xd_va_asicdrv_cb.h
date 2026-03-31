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

#ifndef DVA_RTK_ASIC_CB_H_
#define DVA_RTK_ASIC_CB_H_

#include "rtkas_error.h"
#include "rtkas_common_types.h"
#include "rtl907xd_va_api_cb.h"
#include "rtkas_types.h"
#include "rtl907xd_va_api_routing.h"
#include "rtl907xd_va_table_struct.h"
#include "rtkas_api_cb.h"
/* for Internal channel*/
#define DVA_INTERNALREAD    (0u)
#define DVA_INTERNALWRITE   (1u)
#define DVA_COMPLETE_ACCESS (0u)
#define DVA_EXECUTE_ACCESS  (1u)

#define DVA_SEQRCV_CURDIFF_INERNAL_ADDR   (0xD8u)
#define DVA_SEQRCV_RSTTIME_INERNAL_ADDR   (0x138u)
#define DVA_SEQRCV_TAKEANY_INERNAL_ADDR   (0x152u)
#define DVA_INDIVRCV_RCVNUM_INERNAL_ADDR  (0x60u)
#define DVA_INDIVRCV_RSTTIME_INERNAL_ADDR (0x108u)
#define DVA_INDIVRCV_TAKEANY_INERNAL_ADDR (0x150u)

#define DVA_IPV6_FIG_BYTES_NUM (16u)
#define DVA_MAC_FIG_BYTES_NUM  (6u)

/*for internal channel*/
#define DVA_INTERNAL_DATAENTRY_BIT63       (63u)
#define DVA_INTERNAL_DATAENTRY_BIT95       (95u)
#define DVA_INTERNAL_PERDATAENTRY_BYTE_NUM (8u)

/*for sid*/
#define DVA_IPV4_FLAG (1U)
#define DVA_IPV6_FLAG (0U)

typedef struct
{
    uint8 dstMac[6];
    uint8 srcMac[6];

    uint16 ethertype;
    uint8  ethertypeCtrl;

    uint16 vlanId;
    uint8  vlanIdCtrl;

    uint8 vlanPri;

    uint8 ipFlag;

    uint32 srcIp[4];
    uint8  sourceIpCtrl;

    uint32 dstIp[4];
    uint8  dstIpCtrl;

    uint8 dscp;
    uint8 dscpCtrl;

    uint8 nextProtocol;
    uint8 nextProtocolCtrl;

    uint8 l4PortChkTblMap;

    uint16 srcPort;
    uint8  srcPortCtrl;

    uint16 dstPort;
    uint8  dstPortCtrl;

    uint32 carebits;

    uint16 portMask;
    uint8  streamHandle;

    uint8 isValid;
} dva_CbSidTabParamIn_t;

typedef struct
{
    uint8  ctrlBitWord;
    uint8  ctrlBitoffset;
    uint32 ctrlBitmask;
    uint8  fieldWord;
    uint16 fieldoffset;
    uint16 fieldmask;
    uint32 carebitmask;

} dva_CbSidParamGetInfo_t;

void      dva_drv_cb_gen_ipv6array(uint32 *ipaddrentry, uint8 *ipaddrarray);
RtkApiRet dva_drv_cb_actsid_param2egifparam(CbActiveEgifTblParm_t *actegifParm, dva_RtEgIfTblParm_t *egifTableParm);
RtkApiRet dva_drv_cb_misc_check_enable_status(CbLutParam_t *lutTabentry);
RtkApiRet dva_drv_cb_misc_check_value_range(CbLutParam_t *lutTabentry);
RtkApiRet dva_drv_cb_sid_param2entry(dva_CbSidTabEntry_t *const sidTabentry, dva_CbSidTabParamIn_t *const sidTabParam);
RtkApiRet dva_drv_cb_sid_entry2param(dva_CbSidTabEntry_t *const sidTabentry, dva_CbSidTabParamIn_t *const sidTabParam);
RtkApiRet dva_drv_cb_sidentry_set(uint32 unit, const uint32 index, CbSidparam_t *const sidParam);
RtkApiRet dva_drv_cb_sidentry_get(uint32 unit, uint32 index, CbSidparam_t *sidParam);
RtkApiRet dva_drv_cb_lut_param2entry(dva_CbLutEntry_t *lutEntry, CbLutParam_t *lutParam);
RtkApiRet dva_drv_cb_lut_entry2param(dva_CbLutEntry_t *const lutEntry, CbLutParam_t *const lutParam);
RtkApiRet dva_drv_cb_lutentry_set(uint32 unit, uint32 index, CbLutParam_t *lutParam);
RtkApiRet dva_drv_cb_lutentry_get(uint32 unit, uint32 index, CbLutParam_t *lutParam);
RtkApiRet dva_drv_cb_indivrcventry_set(uint32 unit, uint32 index, CbIndivParam_t *indivParam);
RtkApiRet dva_drv_cb_indivrcventry_get(uint32 unit, uint32 index, CbIndivParam_t *indivParam);
RtkApiRet dva_drv_cb_seqrcvtab_param2entry(dva_CbSeqRcvTabEntry_t *const seqrRcvEntry, CbSeqRcvTabParam_t *const seqRcvparam);
RtkApiRet dva_drv_cb_seqrcvtab_entry2param(dva_CbSeqRcvTabEntry_t *const seqrRcvEntry, CbSeqRcvTabParam_t *const seqRcvparam);
RtkApiRet dva_drv_cb_egress_seqrcv_intf_chk(uint32 index, CbSeqRcvTabParam_t *seqTabentry);
RtkApiRet dva_drv_cb_seqrcvtab_entry_set(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *const seqParam);
RtkApiRet dva_drv_cb_seqrcvtab_entry_get(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *const seqParam);
RtkApiRet dva_drv_cb_rtag_set(uint32 unit, uint16 rtag);
RtkApiRet dva_drv_cb_rtag_get(uint32 unit, uint32 *rtag);
RtkApiRet dva_drv_cb_seqrcv_cnt_set(uint32 unit, const uint32 index, uint64 cntPass, uint64 cntDrop);
RtkApiRet dva_drv_cb_actsid_egiftbl_intf_chk(uint32 egifIndex, CbActiveEgifTblParm_t *actegifParm);
#endif /*_RTL906X_ASICDRV_CB_H_*/