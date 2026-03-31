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

#ifndef CVA_ASICDRV_ROUTING_H_
#define CVA_ASICDRV_ROUTING_H_

#include "rtkas_common_types.h"
#include "rtkas_error.h"

#define CVA_INDR_NUM_RESERVED (6u)
#define CVA_RH_TABLE_MAX      (2048u)
#define CVA_OIL_TABLE_MAX     (8192u)
#define CVA_EGIF_TABLE_MAX    (160u)
typedef struct
{
    uint32 val[9]; /* DATAREG, but if val[N], N>6, assign val[N+1] for INDR_NUM_6 reserved */
} cva_RtRhTbl_t;

typedef struct
{
    uint16 portRhIdx[15];
} cva_RtRhTblParm_t;

/*
    Outgoing Interface List Table
*/
typedef struct
{
    uint32 val[1];
} cva_tblAccess_OIL_TABLE_t;

typedef struct
{
    uint8  ttlChk;
    uint8  ttlDec;
    uint8  egrIfIdx;
    uint16 oilNext;
} cva_RtOilTblParm_t;

/*
    Egress IF Table
*/
typedef struct
{
    uint32 val[3];
} cva_RtEgIfTbl_t;

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
} cva_RtEgIfTblParm_t;

EXTERN_API RtkApiRet cva_drv_rt_rhtbl_get(uint32 unit, uint32 entryNum, cva_RtRhTblParm_t *rhTblParm);
EXTERN_API RtkApiRet cva_drv_rt_rhtbl_set(uint32 unit, uint32 entryNum, const cva_RtRhTblParm_t *rhTblParm);
EXTERN_API RtkApiRet cva_drv_rt_oiltbl_get(uint32 unit, uint32 entryNum, cva_RtOilTblParm_t *oilTblParm);
EXTERN_API RtkApiRet cva_drv_rt_oiltbl_set(uint32 unit, uint32 entryNum, const cva_RtOilTblParm_t *oilTblParm);
EXTERN_API RtkApiRet cva_drv_rt_egriftbl_get(uint32 unit, uint32 entryNum, cva_RtEgIfTblParm_t *egIfTblParm);
EXTERN_API RtkApiRet cva_drv_rt_egriftbl_set(uint32 unit, uint32 entryNum, const cva_RtEgIfTblParm_t *egIfTblParm);
#endif /*CVA_ASICDRV_ROUTING_H_*/