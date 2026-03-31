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

#ifndef CVA_ASICDRV_LUT_H__
#define CVA_ASICDRV_LUT_H__

#include "rtkas_common_types.h"
#include "rtkas_error.h"

#include "rtl907xc_va_api_l2.h"

#define CVA_LUT_INVALID_FID (0xFu)
#define CVA_LUT_FID_MASK    (0xFu)
#define CVA_LUT_UNTAG_CVID  (0xFFFu)

#define CVA_LUT_DATA0_MASK (0x03FFFFFFu)
#define CVA_LUT_DATA1_MASK (0xFFFFFFFFu)
#define CVA_LUT_DATA2_MASK (0x000FFFFFu)

#define CVA_LUTCAM_DATA0_MASK (0x03FFFFFFu)
#define CVA_LUTCAM_DATA1_MASK (0xFFFFFFFFu)
#define CVA_LUTCAM_DATA2_MASK (0x1FFFFFFFu)

#define CVA_LUTCAM_INDEX_START (0u)
#define CVA_LUTCAM_INDEX_END   (cva_table_list[CVA_LUT_BCAM].size)

#define CVA_TBL_GROUP_DATA0_MASK (0x0001FFFFu) /* portmask */
#define CVA_TBL_GROUP_DATA1_MASK (0x0FFFFFFFu) /* group ip */

#define CVA_TBL_MAX_BURST_ENTRYNUM (4096u) /* The max entry num for burst read. refer to the "entryNum" field name */

#define CVA_HASH_WITH_CVID  (0u)
#define CVA_HASH_WITH_FID   (1u)
#define CVA_HASH_METHOD_END (2u)

#define CVA_LUT_8WAY_FIRST   (0u)
#define CVA_LUT_8WAY_SECOND  (1u)
#define CVA_LUT_8WAY_THIRD   (2u)
#define CVA_LUT_8WAY_FOURTH  (3u)
#define CVA_LUT_8WAY_FIFTH   (4u)
#define CVA_LUT_8WAY_SIXTH   (5u)
#define CVA_LUT_8WAY_SEVENTH (6u)
#define CVA_LUT_8WAY_EIGHTH  (7u)
#define CVA_LUT_8WAY_END     (8u)

#define CVA_GLOBAL_HASHALG_ADDR (0x00192269u)

typedef enum
{
    CVA_EM_LUT_ASIC_UNICAST        = 0u,
    CVA_EM_LUT_ASIC_MULTI          = 1u,
    CVA_EM_LUT_ASIC_IPMULTI        = 2u,
    CVA_EM_LUT_ASIC_INVALID_ENTRY  = 3u,
    CVA_EM_LUT_ASIC_ENTRY_TYPE_END = 4u
} cva_LutAsicEntryType_e;

typedef enum
{
    CVA_EM_LUT_FLUSH_ALL       = 0u,
    CVA_EM_LUT_FLUSH_PORT_ONLY = 1u,
    CVA_EM_LUT_FLUSH_PORT_CVID = 2u,
    CVA_EM_LUT_FLUSH_PORT_FID  = 3u,
    CVA_EM_LUT_FLUSH_END       = 4u
} cva_LutFlushMode_e;

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
} cva_LutAsicUniParam_t;

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
} cva_LutAsicMultiParam_t;

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
} cva_LutAsicIpMultiParam_t;

typedef struct
{
    uint8  dip[4];
    uint32 portmask;
} cva_LutAsicGroupParam_t;

typedef struct
{
    cva_LutAsicEntryType_e    type;
    cva_LutAsicUniParam_t     uni;   /* unicast parameter      */
    cva_LutAsicMultiParam_t   multi; /* multicast parameter    */
    cva_LutAsicIpMultiParam_t ip;    /* ip multicast parameter */

    /* entry number in lut */
    uint16 index;
} cva_LutAsicParam_t;

typedef struct
{
    uint32 val[3];
} cva_LutTblAsicEntry_t; /* l2 talbe or L2 BCAM */

typedef struct
{
    uint32 val[2];
} cva_LutTblGroupEntry_t;

typedef struct
{
    uint16 vid;
    uint16 portmask;
    uint8  fid;
} cva_LutFlushParam_t;

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
} cva_LutSearchParam_t;

extern uint32 cva_g_lutTime;

EXTERN_API RtkApiRet cva_drv_lut_hash0_unicast_index_get(uint32 unit, const cva_LutAsicUniParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cva_drv_lut_hash1_unicast_index_get(uint32 unit, const cva_LutAsicUniParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cva_drv_lut_hash0_multicast_index_get(uint32 unit, const cva_LutAsicMultiParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cva_drv_lut_hash1_multicast_index_get(uint32 unit, const cva_LutAsicMultiParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cva_drv_lut_hash0_ip_index_get(uint32 unit, const cva_LutAsicIpMultiParam_t *const param, const uint8 idx, uint16 *const index);
EXTERN_API RtkApiRet cva_drv_lut_hash1_ip_index_get(uint32 unit, const cva_LutAsicIpMultiParam_t *const param, const uint8 idx, uint16 *const index);

EXTERN_API RtkApiRet cva_drv_lut_entry_delete_input_check(const uint8 *const mac, uint16 vid, uint16 fid, uint8 ulfid);
EXTERN_API RtkApiRet cva_drv_lut_entry_delete_param_init(const uint8 *const mac, uint16 vid, uint16 fid, uint8 ulfid, cva_LutSearchParam_t *const pParam, cva_LutAsicMultiParam_t *const pMultiParam, cva_LutAsicParam_t *const pLutParam);
EXTERN_API uint32    cva_drv_lut_multi_entry_check(const uint8 *const mac, const uint16 vid, const uint16 fid, const cva_LutAsicParam_t *const pLutParam);
EXTERN_API RtkApiRet cva_drv_lut_entry_delete(uint32 unit, const uint16 index);
EXTERN_API RtkApiRet cva_drv_lut_entry_get(uint32 unit, const uint32 hashAlg, const uint16 index, cva_LutAsicParam_t *const param);
EXTERN_API RtkApiRet cva_drv_lut_entry_burst_get(uint32 unit, const uint32 hashAlg, const uint16 index, cva_LutAsicParam_t *const param, uint32 entryNum);
EXTERN_API RtkApiRet cva_drv_lut_entry_update_age(uint32 unit, const uint8 *const macAddr, const uint32 agecnt);
EXTERN_API RtkApiRet cva_drv_lut_ageunit_get(uint32 unit, uint32 *const ageunit);
EXTERN_API RtkApiRet cva_drv_lut_ageunit_set(uint32 unit, const uint32 ageunit);

EXTERN_API RtkApiRet cva_drv_lut_uni_find_index(uint32 unit, const uint32 hashAlg, const cva_LutAsicUniParam_t *const param, uint16 *pIndex);
EXTERN_API RtkApiRet cva_drv_lut_uni_entry_add(uint32 unit, const cva_LutAsicUniParam_t *const param);
EXTERN_API RtkApiRet cva_drv_lut_uni_entry_set(uint32 unit, const uint32 hashAlg, const cva_LutAsicUniParam_t *const param, const uint8 idx);
EXTERN_API RtkApiRet cva_drv_lut_uni_entry_get(uint32 unit, const uint32 hashAlg, const uint16 index, cva_LutAsicUniParam_t *const param);
EXTERN_API RtkApiRet cva_drv_lut_uni_entry_force_set(uint32 unit, const uint16 index, const cva_LutAsicUniParam_t *const param);

EXTERN_API RtkApiRet cva_drv_lut_multi_entry_input_check(const uint8 *const pMacAddr, const cva_VlanFid fid, const LutParam_t *const pLutEntry);
EXTERN_API RtkApiRet cva_drv_lut_multi_entry_param_init(const uint8 *const pMacAddr, const cva_VlanFid fid, cva_LutAsicMultiParam_t *const pMultiParam);
EXTERN_API RtkApiRet cva_drv_lut_multicast_index_get(uint32 unit, const uint32 hashAlg, const cva_LutAsicMultiParam_t *const pMultiParam, const uint8 idx, uint16 *const pIndex);
EXTERN_API RtkApiRet cva_drv_lut_multi_find_index(uint32 unit, const uint32 ulfid, const uint32 hashAlg, const cva_LutAsicMultiParam_t *const param, uint16 *pIndex);
EXTERN_API RtkApiRet cva_drv_lut_multi_entry_add(uint32 unit, const cva_LutAsicMultiParam_t *const param);

EXTERN_API RtkApiRet cva_drv_lut_ip_multi_index_get(uint32 unit, const uint32 hashAlg, const cva_LutAsicIpMultiParam_t *const param, const uint8 idx, uint16 *const pIndex);
EXTERN_API RtkApiRet cva_drv_lut_ip_multi_entry_add(uint32 unit, const cva_LutAsicIpMultiParam_t *const param);
EXTERN_API RtkApiRet cva_drv_lut_ip_multi_entry_input_check(const uint8 *const sip, const uint8 *const dip);
EXTERN_API RtkApiRet cva_drv_lut_ip_multi_entry_param_init(const uint8 *const sip, const uint8 *const dip, cva_LutAsicIpMultiParam_t *const pIpMultiParam, cva_LutAsicParam_t *const pLutParam);
EXTERN_API uint32    cva_drv_lut_ip_multi_param_cmp(const cva_LutAsicIpMultiParam_t *const pIpMultiParam, const cva_LutAsicParam_t *const pRetparam);

EXTERN_API RtkApiRet cva_drv_lut_lookup_miss_ctrl_input_check(const RtkPort port, const uint8 type, const uint8 action);

EXTERN_API RtkApiRet cva_drv_lut_group_entry_set(uint32 unit, const cva_LutAsicGroupParam_t *const param, const uint8 idx);
EXTERN_API RtkApiRet cva_drv_lut_group_entry_get(uint32 unit, const uint16 index, cva_LutAsicGroupParam_t *const param);

EXTERN_API RtkApiRet cva_drv_lut_cam_entry_set(uint32 unit, const uint16 index, const cva_LutAsicParam_t *const param);
EXTERN_API RtkApiRet cva_drv_lut_cam_entry_get(uint32 unit, const uint16 index, cva_LutAsicParam_t *const param);

EXTERN_API RtkApiRet cva_drv_lut_flush(uint32 unit, const cva_LutFlushMode_e mode, const cva_LutFlushParam_t *const param);
EXTERN_API RtkApiRet cva_drv_lut_search(uint32 unit, const cva_LutSearchParam_t *const param, uint32 *const found, uint16 *const hitaddr);

EXTERN_API RtkApiRet cva_drv_lut_hash_algo_get(uint32 unit, uint32 *const hashAlg);
EXTERN_API RtkApiRet cva_drv_lut_hash_algo_set(uint32 unit, const uint32 hashAlg);

EXTERN_API RtkApiRet cva_drv_lut_multi_ulfid_get(uint32 unit, uint32 *const ulfid);
EXTERN_API RtkApiRet cva_drv_lut_multi_ulfid_set(uint32 unit, const uint32 ulfid);

EXTERN_API RtkApiRet cva_drv_lut_bkt_full_trap(uint32 unit, const uint32 enable);
EXTERN_API RtkApiRet cva_drv_lut_lru_enable(uint32 unit, const uint32 enable);

EXTERN_API RtkApiRet cva_drv_lut_sys_param2user_param(const cva_LutAsicParam_t *const sysParam, LutParam_t *const userParam);
#endif /* CVA_ASICDRV_LUT_H__ */
