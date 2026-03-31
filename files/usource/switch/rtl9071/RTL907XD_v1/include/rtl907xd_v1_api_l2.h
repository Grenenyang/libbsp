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

#ifndef DV1_RTK_API_L2_H__
#define DV1_RTK_API_L2_H__

#include "rtl907xd_v1_api.h"
#include "rtl907xd_v1_api_vlan.h"
#include "rtkas_api_l2.h"

#define DV1_LM_L2_UNICAST   (0u)
#define DV1_LM_L2_BROADCAST (1u)
#define DV1_LM_L2_MULTICAST (2u)
#define DV1_LM_L3_MULTICAST (3u)
#define DV1_LM_V4_MULTICAST (4u)
#define DV1_LM_V6_MULTICAST (5u)
#define DV1_LM_TYPE_END     (6u)

#define DV1_LM_ACTION_DROP        (0u)
#define DV1_LM_ACTION_TRAP_TO_CPU (1u)
#define DV1_LM_ACTION_FLOOD       (2u)
#define DV1_LM_ACTION_END         (3u)

#define DV1_LUT_TABLE_INDEX_MAX (4095u)
#define DV1_LUT_VID_MAX         (0xFFFu)
#define DV1_LUT_FID_MAX         (14u)
#define DV1_LUT_ULFID_MAX       (1u)
#define DV1_LUT_ENABLE_MAX      (1u)
#define DV1_LUT_RECORD_IDX_MAX  (5u)

#define DV1_LUT_BUCKET_KEY_LEN (7u)

#define DV1_SOURCE_ADDRESS_MOVE_RECORD_IDX_OFFSET (8u)

EXTERN_API RtkApiRet dv1_lut_hash_algo_get(uint32 unit, uint32 *pAlgo);

EXTERN_API RtkApiRet dv1_lut_mac_lrn_en(uint32 unit, RtkPmap portmask, uint8 enable);

EXTERN_API RtkApiRet dv1_lut_lookup_miss_ctrl(uint32 unit, RtkPort port, uint8 type, uint8 action);

EXTERN_API RtkApiRet dv1_lut_uni_entry_add(uint32 unit, const uint8 *const mac, uint16 vid, uint8 port, uint16 fid, uint8 ulfid);

EXTERN_API RtkApiRet dv1_lut_multi_entry_add(uint32 unit, const uint8 *const mac, dv1_RtkVlan vid, RtkPmap portmask, dv1_VlanFid fid);

EXTERN_API RtkApiRet dv1_lut_ip_multi_entry_add(uint32 unit, const uint8 *const sip, const uint8 *const dip, const RtkPmap portmask);

EXTERN_API RtkApiRet dv1_lut_entry_delete(uint32 unit, const uint8 *const mac, uint16 vid, uint16 fid, uint8 ulfid);

EXTERN_API RtkApiRet dv1_lut_ip_multi_entry_delete(uint32 unit, const uint8 *const sip, const uint8 *const dip);

EXTERN_API RtkApiRet dv1_lut_entry_get(uint32 unit, const uint16 index, LutParam_t *const pLutEntry);

EXTERN_API RtkApiRet dv1_lut_cam_entry_get(uint32 unit, const uint16 index, LutParam_t *const pLutEntry);

EXTERN_API RtkApiRet dv1_lut_entry_search(uint32 unit, const uint8 *const pMacAddr, LutParam_t *const pLutEntry);

EXTERN_API RtkApiRet dv1_lut_multi_entry_get(uint32 unit, const uint8 *const pMacAddr, const dv1_VlanFid fid, LutParam_t *const pLutEntry);

EXTERN_API RtkApiRet dv1_lut_ip_multi_entry_get(uint32 unit, const uint8 *const sip, const uint8 *const dip, RtkPmap *const portmask);

EXTERN_API RtkApiRet dv1_lut_sa_move_legal_port_get(uint32 unit, RtkPort port, RtkPmap *pPortmask);

EXTERN_API RtkApiRet dv1_lut_sa_move_legal_port_set(uint32 unit, RtkPort port, RtkPmap portmask);

EXTERN_API RtkApiRet dv1_lut_sa_move_legal_port_get_act(uint32 unit, RtkPort port, LegalPortAct_e *pLegalPortAct);

EXTERN_API RtkApiRet dv1_lut_sa_move_legal_port_set_act(uint32 unit, RtkPort port, LegalPortAct_e legalPortAct);

EXTERN_API RtkApiRet dv1_lut_sa_move_illegal_port_get_act(uint32 unit, RtkPort port, IllegalPortAct_e *pIllegalPortAct);

EXTERN_API RtkApiRet dv1_lut_sa_move_illegal_port_set_act(uint32 unit, RtkPort port, IllegalPortAct_e IllegalPortAct);

EXTERN_API RtkApiRet dv1_lut_sa_move_cpu_port_get(uint32 unit, RtkPmap *pPortmask);

EXTERN_API RtkApiRet dv1_lut_sa_move_cpu_port_set(uint32 unit, RtkPmap portmask);

EXTERN_API RtkApiRet dv1_lut_legal_sa_move_record_en(uint32 unit, RtkPort port, uint8 enable);

EXTERN_API RtkApiRet dv1_lut_illegal_sa_move_record_en(uint32 unit, RtkPort port, uint8 enable);

EXTERN_API RtkApiRet dv1_lut_legal_sa_move_get_record(uint32 unit, uint8 idx, SaMoveRecord_t *pMoveRecord);

EXTERN_API RtkApiRet dv1_lut_illegal_sa_move_get_record(uint32 unit, uint8 idx, SaMoveRecord_t *pMoveRecord);

#endif /*DV1_RTK_API_L2_H__ */
