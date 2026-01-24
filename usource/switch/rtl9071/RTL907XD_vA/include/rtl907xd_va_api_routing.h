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

#ifndef DVA_RTK_API_ROUTING_H__
#define DVA_RTK_API_ROUTING_H__

/********* Include Header *********/
#include "rtkas_api_routing.h"
#include "rtl907xd_va_api.h"

/********* Definition *********/

/********* Enumeration *********/

/********* Structure *********/
typedef struct
{
    uint16 portRhIdx[15];
} dva_RtRhTblParm_t;

typedef struct
{
    uint8  ttlChk;
    uint8  ttlDec;
    uint8  egrIfIdx;
    uint16 oilNext;
} dva_RtOilTblParm_t;

typedef struct
{
    uint32 macId31to0;
    uint16 macId47to32;
    uint8  macFmtSel;
    uint8  macReplace;
    uint16 vid;
    uint8  vidReplace;
    uint8  pcp;
    uint8  pcpReplace;
    uint8  ipMtuIdx;
    uint8  ip6MtuIdx;
    uint8  ipMcTtl;
    uint8  ip6McHl;
    uint8  icmpRedirChkEn;
    uint8  icmpRedirAct;
    uint8  rpfChkEn;
    uint8  rpfFailAct;
} dva_RtEgIfTblParm_t;

/********* Function Declaration *********/
EXTERN_API RtkApiRet dva_drv_rt_egriftbl_get(uint32 unit, uint32 entryNum, dva_RtEgIfTblParm_t *egIfTblParm);
EXTERN_API RtkApiRet dva_drv_rt_egriftbl_set(uint32 unit, uint32 entryNum, const dva_RtEgIfTblParm_t *egIfTblParm);
EXTERN_API RtkApiRet dva_drv_rt_oiltbl_get(uint32 unit, uint32 entryNum, dva_RtOilTblParm_t *oilTblParm);
EXTERN_API RtkApiRet dva_drv_rt_oiltbl_set(uint32 unit, uint32 entryNum, const dva_RtOilTblParm_t *oilTblParm);
EXTERN_API RtkApiRet dva_drv_rt_rhtbl_get(uint32 unit, uint32 entryNum, dva_RtRhTblParm_t *rhTblParm);
EXTERN_API RtkApiRet dva_drv_rt_rhtbl_set(uint32 unit, uint32 entryNum, const dva_RtRhTblParm_t *rhTblParm);
EXTERN_API RtkApiRet dva_route_disable(uint32 unit, uint8 disable);
EXTERN_API RtkApiRet dva_route_rng_chk_set(uint32 unit, uint8 enable);
EXTERN_API RtkApiRet dva_route_tbl_flush(uint32 unit);
EXTERN_API RtkApiRet dva_route_rttbl_set(uint32 unit, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet dva_route_rttbl_get(uint32 unit, RtTbl_t *rtEntry);
EXTERN_API RtkApiRet dva_route_intf_set(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet dva_route_intf_get(uint32 unit, RtIntfCfg_t *intfCfg);
EXTERN_API RtkApiRet dva_route_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo);
EXTERN_API RtkApiRet dva_route_uc_set(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet dva_route_uc_get(uint32 unit, RtUcEntry_t *ucEntry);
EXTERN_API RtkApiRet dva_route_mc_set(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet dva_route_mc_get(uint32 unit, RtMcEntry_t *mcEntry);
EXTERN_API RtkApiRet dva_route_host_entry_typelist_get(uint32 unit, RtHostTypeList_t *typeList);
#endif /* DVA_RTK_API_ROUTING_H__ */