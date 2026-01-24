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

#ifndef CVB_ASICDRV_LUT_H__
#define CVB_ASICDRV_LUT_H__

#include "rtkas_common_types.h"
#include "rtkas_error.h"

#include "rtl907xc_vb_api_l2.h"

#define CVB_LUT_INVALID_FID (0xFu)
#define CVB_LUT_FID_MASK    (0xFu)
#define CVB_LUT_UNTAG_CVID  (0xFFFu)

#define CVB_LUT_DATA0_MASK (0x03FFFFFFu)
#define CVB_LUT_DATA1_MASK (0xFFFFFFFFu)
#define CVB_LUT_DATA2_MASK (0x000FFFFFu)

#define CVB_LUTCAM_DATA0_MASK (0x03FFFFFFu)
#define CVB_LUTCAM_DATA1_MASK (0xFFFFFFFFu)
#define CVB_LUTCAM_DATA2_MASK (0x1FFFFFFFu)

#define CVB_LUTCAM_INDEX_START (0u)
#define CVB_LUTCAM_INDEX_END   (cvb_table_list[CVB_LUT_BCAM].size)

#define CVB_TBL_GROUP_DATA0_MASK (0x0001FFFFu) /* portmask */
#define CVB_TBL_GROUP_DATA1_MASK (0x0FFFFFFFu) /* group ip */

#define CVB_TBL_MAX_BURST_ENTRYNUM (4096u) /* The max entry num for burst read. refer to the "entryNum" field name */

#define CVB_HASH_WITH_CVID  (0u)
#define CVB_HASH_WITH_FID   (1u)
#define CVB_HASH_METHOD_END (2u)

#define CVB_LUT_8WAY_FIRST   (0u)
#define CVB_LUT_8WAY_SECOND  (1u)
#define CVB_LUT_8WAY_THIRD   (2u)
#define CVB_LUT_8WAY_FOURTH  (3u)
#define CVB_LUT_8WAY_FIFTH   (4u)
#define CVB_LUT_8WAY_SIXTH   (5u)
#define CVB_LUT_8WAY_SEVENTH (6u)
#define CVB_LUT_8WAY_EIGHTH  (7u)
#define CVB_LUT_8WAY_END     (8u)

#define CVB_GLOBAL_HASHALG_ADDR (0x00197a1du)

typedef enum
{
    CVB_EM_LUT_ASIC_UNICAST        = 0u,
    CVB_EM_LUT_ASIC_MULTI          = 1u,
    CVB_EM_LUT_ASIC_IPMULTI        = 2u,
    CVB_EM_LUT_ASIC_INVALID_ENTRY  = 3u,
    CVB_EM_LUT_ASIC_ENTRY_TYPE_END = 4u
} cvb_LutAsicEntryType_e;

typedef enum
{
    CVB_EM_LUT_FLUSH_ALL       = 0u,
    CVB_EM_LUT_FLUSH_PORT_ONLY = 1u,
    CVB_EM_LUT_FLUSH_PORT_CVID = 2u,
    CVB_EM_LUT_FLUSH_PORT_FID  = 3u,
    CVB_EM_LUT_FLUSH_END       = 4u
} cvb_LutFlushMode_e;

/*address table lookup entry*/
typedef struct
{
    uint8  eavPriEn;
    uint8  eavPri;
    uint8  fid;
    uint8  bstatic;
    uint8  auth;
    uint8  dablk;
    uint8  sablk;
    uint8  spa;
    uint16 age;
    uint8  l3entry;
    uint8  mac[6];
    uint16 vid;

    uint8 ulfid; /* 1: hash with fid, 0: hash with cvid */
} cvb_LutAsicUniParam_t;

typedef struct
{
    uint8  eavPriEn;
    uint8  eavPri;
    uint8  fid;
    uint32 mbr;
    uint8  vleaky; /* crsvlan */
    uint8  l3entry;
    uint8  mac[6];
    uint16 vid; /* ppi_fid */
} cvb_LutAsicMultiParam_t;

typedef struct
{
    uint8  eavPriEn;
    uint8  eavPri;
    uint8  fid;
    uint32 mbr;
    uint8  vleaky; /* crsvlan */
    uint8  l3entry;
    uint8  sip[4];
    uint8  dip[4];
} cvb_LutAsicIpMultiParam_t;

typedef struct
{
    uint8  dip[4];
    uint32 portmask;
} cvb_LutAsicGroupParam_t;

typedef struct
{
    cvb_LutAsicEntryType_e    type;
    cvb_LutAsicUniParam_t     uni;   /* unicast parameter      */
    cvb_LutAsicMultiParam_t   multi; /* multicast parameter    */
    cvb_LutAsicIpMultiParam_t ip;    /* ip multicast parameter */

    /* entry number in lut */
    uint16 index;
} cvb_LutAsicParam_t;

typedef struct
{
    uint32 val[3];
} cvb_LutTblAsicEntry_t; /* l2 talbe or L2 BCAM */

typedef struct
{
    uint32 val[2];
} cvb_LutTblGroupEntry_t;

typedef struct
{
    uint16 vid;
    uint16 portmask;
    uint8  fid;
} cvb_LutFlushParam_t;

typedef struct
{
    uint32 defPortNum;
    uint32 portNum;
    uint32 defMac;
    uint8  mac[6];
    uint32 defFid;
    uint8  ulfid; /* 0: hash with vid, 1: hash with fid */
    uint32 fid;
    uint32 vid;
    uint32 startIdx;
} cvb_LutSearchParam_t;

extern uint32 cvb_g_lutTime;

EXTERN_API RtkApiRet cvb_drv_lut_hash0_unicast_index_get(uint32 unit, const cvb_LutAsicUniParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cvb_drv_lut_hash1_unicast_index_get(uint32 unit, const cvb_LutAsicUniParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cvb_drv_lut_hash0_multicast_index_get(uint32 unit, const cvb_LutAsicMultiParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cvb_drv_lut_hash1_multicast_index_get(uint32 unit, const cvb_LutAsicMultiParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cvb_drv_lut_hash0_ip_index_get(uint32 unit, const cvb_LutAsicIpMultiParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cvb_drv_lut_hash1_ip_index_get(uint32 unit, const cvb_LutAsicIpMultiParam_t *const param, const uint8 idx, uint16 *const index);

EXTERN_API RtkApiRet cvb_drv_lut_entry_delete_input_check(const uint8 *const mac, uint16 vid, uint16 fid, uint8 ulfid);
EXTERN_API RtkApiRet cvb_drv_lut_entry_delete_param_init(const uint8 *const mac, uint16 vid, uint16 fid, uint8 ulfid, cvb_LutSearchParam_t *const pParam, cvb_LutAsicMultiParam_t *const pMultiParam, cvb_LutAsicParam_t *const pLutParam);
EXTERN_API uint32    cvb_drv_lut_multi_entry_check(const uint8 *const mac, const uint16 vid, const uint16 fid, const cvb_LutAsicParam_t *const pLutParam);
EXTERN_API RtkApiRet cvb_drv_lut_entry_delete(uint32 unit, const uint16 index);
EXTERN_API RtkApiRet cvb_drv_lut_entry_get(uint32 unit, const uint32 hashAlg, const uint16 index, cvb_LutAsicParam_t *const param);
EXTERN_API RtkApiRet cvb_drv_lut_entry_burst_get(uint32 unit, const uint32 hashAlg, const uint16 index, cvb_LutAsicParam_t *const param, uint32 entryNum);
EXTERN_API RtkApiRet cvb_drv_lut_entry_update_age(uint32 unit, const uint8 *const macAddr, const uint32 agecnt);
EXTERN_API RtkApiRet cvb_drv_lut_ageunit_get(uint32 unit, uint32 *const ageunit);
EXTERN_API RtkApiRet cvb_drv_lut_ageunit_set(uint32 unit, const uint32 ageunit);

EXTERN_API RtkApiRet cvb_drv_lut_uni_find_index(uint32 unit, const uint32 hashAlg, const cvb_LutAsicUniParam_t *const param, uint16 *pIndex);
EXTERN_API RtkApiRet cvb_drv_lut_uni_entry_add(uint32 unit, const cvb_LutAsicUniParam_t *const param);
EXTERN_API RtkApiRet cvb_drv_lut_uni_entry_set(uint32 unit, const uint32 hashAlg, const cvb_LutAsicUniParam_t *const param, const uint8 idx);
EXTERN_API RtkApiRet cvb_drv_lut_uni_entry_get(uint32 unit, const uint32 hashAlg, const uint16 index, cvb_LutAsicUniParam_t *const param);
EXTERN_API RtkApiRet cvb_drv_lut_uni_entry_force_set(uint32 unit, const uint16 index, const cvb_LutAsicUniParam_t *const param);

EXTERN_API RtkApiRet cvb_drv_lut_multi_entry_input_check(const uint8 *const pMacAddr, const cvb_VlanFid fid, const LutParam_t *const pLutEntry);
EXTERN_API RtkApiRet cvb_drv_lut_multi_entry_param_init(const uint8 *const pMacAddr, const cvb_VlanFid fid, cvb_LutAsicMultiParam_t *const pMultiParam);
EXTERN_API RtkApiRet cvb_drv_lut_multicast_index_get(uint32 unit, const uint32 hashAlg, const cvb_LutAsicMultiParam_t *const pMultiParam, const uint8 idx, uint16 *const pIndex);
EXTERN_API RtkApiRet cvb_drv_lut_multi_find_index(uint32 unit, const uint32 ulfid, const uint32 hashAlg, const cvb_LutAsicMultiParam_t *const param, uint16 *pIndex);
EXTERN_API RtkApiRet cvb_drv_lut_multi_entry_add(uint32 unit, const cvb_LutAsicMultiParam_t *const param);

EXTERN_API RtkApiRet cvb_drv_lut_ip_multi_index_get(uint32 unit, const uint32 hashAlg, const cvb_LutAsicIpMultiParam_t *const param, const uint8 idx, uint16 *const pIndex);
EXTERN_API RtkApiRet cvb_drv_lut_ip_multi_entry_add(uint32 unit, const cvb_LutAsicIpMultiParam_t *const param);
EXTERN_API RtkApiRet cvb_drv_lut_ip_multi_entry_input_check(const uint8 *const sip, const uint8 *const dip);
EXTERN_API RtkApiRet cvb_drv_lut_ip_multi_entry_param_init(const uint8 *const sip, const uint8 *const dip, cvb_LutAsicIpMultiParam_t *const pIpMultiParam, cvb_LutAsicParam_t *const pLutParam);
EXTERN_API uint32    cvb_drv_lut_ip_multi_param_cmp(const cvb_LutAsicIpMultiParam_t *const pIpMultiParam, const cvb_LutAsicParam_t *const pRetparam);

EXTERN_API RtkApiRet cvb_drv_lut_lookup_miss_ctrl_input_check(const RtkPort port, const uint8 type, const uint8 action);

EXTERN_API RtkApiRet cvb_drv_lut_group_entry_set(uint32 unit, const cvb_LutAsicGroupParam_t *const param, const uint8 idx);
EXTERN_API RtkApiRet cvb_drv_lut_group_entry_get(uint32 unit, const uint16 index, cvb_LutAsicGroupParam_t *const param);

EXTERN_API RtkApiRet cvb_drv_lut_cam_entry_set(uint32 unit, const uint16 index, const cvb_LutAsicParam_t *const param);
EXTERN_API RtkApiRet cvb_drv_lut_cam_entry_get(uint32 unit, const uint16 index, cvb_LutAsicParam_t *const param);

EXTERN_API RtkApiRet cvb_drv_lut_flush(uint32 unit, const cvb_LutFlushMode_e mode, const cvb_LutFlushParam_t *const param);
EXTERN_API RtkApiRet cvb_drv_lut_search(uint32 unit, const cvb_LutSearchParam_t *const param, uint32 *const found, uint16 *const hitaddr);

EXTERN_API RtkApiRet cvb_drv_lut_hash_algo_get(uint32 unit, uint32 *const hashAlg);
EXTERN_API RtkApiRet cvb_drv_lut_hash_algo_set(uint32 unit, const uint32 hashAlg);

EXTERN_API RtkApiRet cvb_drv_lut_multi_ulfid_get(uint32 unit, uint32 *const ulfid);
EXTERN_API RtkApiRet cvb_drv_lut_multi_ulfid_set(uint32 unit, const uint32 ulfid);

EXTERN_API RtkApiRet cvb_drv_lut_bkt_full_trap(uint32 unit, const uint32 enable);
EXTERN_API RtkApiRet cvb_drv_lut_lru_enable(uint32 unit, const uint32 enable);

EXTERN_API RtkApiRet cvb_drv_lut_sys_param2user_param(const cvb_LutAsicParam_t *const sysParam, LutParam_t *const userParam);
#endif /* CVB_ASICDRV_LUT_H__ */
