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

#ifndef DVA_ASICDRV_TCAM_H__
#define DVA_ASICDRV_TCAM_H__

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/********* Boundary *********/
#define DVA_TCAM_PIE_BLOCK_BOUND         (8u)
#define DVA_TCAM_PIE_TCAM_NUM_BOUND      (DVA_TCAM_PIE_TCAM1)
#define DVA_TCAM_NETROUTE_TCAM_NUM_BOUND (DVA_TCAM_NETROUTE_TCAM5)
#define DVA_TCAM_INGIF_TCAM_NUM_BOUND    (DVA_TCAM_INGIF_TCAM1)
#define DVA_TCAM_SID_TCAM_NUM_BOUND      (DVA_TCAM_SID_TCAM3)

/********* SRAM Config *********/
#define DVA_TCAM_SRAM_NETROUTE_ADDR (0x010FF20C)

/********* PIE TCAM *********/
#define DVA_TCAM_PIE_TCAM0           (0u)
#define DVA_TCAM_PIE_TCAM1           (1u)
#define DVA_TCAM_PIE_TCAM_NUM        ((DVA_TCAM_PIE_TCAM1) + (1u))
#define DVA_TCAM_PIE_BLOCK_ENTRY_NUM (64u)

/********* NetRoute TCAM *********/
#define DVA_TCAM_NETROUTE_TCAM0    (0u)
#define DVA_TCAM_NETROUTE_TCAM1    (1u)
#define DVA_TCAM_NETROUTE_TCAM2    (2u)
#define DVA_TCAM_NETROUTE_TCAM3    (3u)
#define DVA_TCAM_NETROUTE_TCAM4    (4u)
#define DVA_TCAM_NETROUTE_TCAM5    (5u)
#define DVA_TCAM_NETROUTE_TCAM_NUM ((DVA_TCAM_NETROUTE_TCAM5) + (1u))

/********* INGIF TCAM *********/
#define DVA_TCAM_INGIF_TCAM0    (0u)
#define DVA_TCAM_INGIF_TCAM1    (1u)
#define DVA_TCAM_INGIF_TCAM_NUM ((DVA_TCAM_INGIF_TCAM1) + (1u))

/********* SID TCAM *********/
#define DVA_TCAM_SID_TCAM0    (0u)
#define DVA_TCAM_SID_TCAM1    (1u)
#define DVA_TCAM_SID_TCAM2    (2u)
#define DVA_TCAM_SID_TCAM3    (3u)
#define DVA_TCAM_SID_TCAM_NUM ((DVA_TCAM_SID_TCAM3) + (1u))

/********* Register Offset *********/
#define DVA_TCAM_CTRL_REG_INTERVAL_NUM (3u)
#define DVA_TCAM_4_BYTE_REG            (4u)
#define DVA_TCAM_PIE_BLOCK_REG_OFFSET  (0x18u)

/********* TCAM Data Register Number *********/
/* 31 is the max number of dataReg(DVA_STREAM_IDENTIFICATION_TABLE) */
#define DVA_TCAM_DATA_REG_MAX      (DVA_TCAM_SID_DATA_REG)
#define DVA_TCAM_PIE_DATA_REG      (13u)
#define DVA_TCAM_NETROUTE_DATA_REG (23u)
#define DVA_TCAM_INGIF_DATA_REG    (9u)
#define DVA_TCAM_SID_DATA_REG      (31u)

/********* Valid Offset *********/
#define DVA_TCAM_VALID_OFFSET          (6u)
#define DVA_TCAM_NETROUTE_VALID_OFFSET (7u) /* NETROUTE_VALID is in the data register 7 */
#define DVA_TCAM_ACL_PIE_MASK_OFFSET   (7u)
/* before valid bit */
#define DVA_TCAM_CB_SID_MASK_OFFSET_BF_VALID (16u)
/* after valid bit */
#define DVA_TCAM_CB_SID_MASK_OFFSET_AF_VALID (15u)
/* before valid bit */
#define DVA_TCAM_RT_NETWORK_MASK_OFFSET_BF_VALID (12u)
/* after valid bit */
#define DVA_TCAM_RT_NETWORK_MASK_OFFSET_AF_VALID (11u)

/* Interrupt */
#define DVA_TCAM_IRQ_RESTORE               (0u)
#define DVA_TCAM_IRQ_CLEAR                 (1u)
#define DVA_TCAM_ERROR_ACL_CB_ROUTE_EN_MSK (7u)       /* Bit[0]: ACL, Bit[1]: CB, BIT[2]: Route (Level 3) */
#define DVA_TCAM_ERROR_ACL_EN_MSK          (0x3FFFFu) /* Bit[17:0], Length = 18 */
#define DVA_TCAM_ERROR_CB_EN_MSK           (0xFu)     /* Bit[3:0], Length = 4 */
#define DVA_TCAM_ERROR_ROUTE_EN_MSK        (0xFFu)    /* INGIF Bit[1:0], Length = 2, Val = 0x3; NETROUTE Bit[7:2], Length = 6, Val = 0xFC */

/********* Internal Define *********/
#define DVA_TCAM_SAFETY_ENABLE_BIT (0u)
#define DVA_TCAM_WRITE_RETRY_TIME  (3u)

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
typedef struct
{
    uint32        valid; /* For making image mode, if valid = 0, it means no entry needs to be calculated */
    uint32        addr;
    const uint32 *data;
} dva_drv_TcamEntryInfo;

typedef struct
{
    uint32 val[DVA_TCAM_PIE_TCAM_NUM];
} dva_drv_TcamPieCrcVal_t;

typedef struct
{
    uint32 val[DVA_TCAM_NETROUTE_TCAM_NUM];
} dva_drv_TcamNetRouteCrcVal_t;

typedef struct
{
    uint32 val[DVA_TCAM_INGIF_TCAM_NUM];
} dva_drv_TcamIngifCrcVal_t;

typedef struct
{
    uint32 val[DVA_TCAM_SID_TCAM_NUM];
} dva_drv_TcamSidCrcVal_t;

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet dva_drv_tcam_check(uint32 unit, const dva_table_list_t table, const uint32 addr, const uint32 *const data);
#ifdef RTK_INTERNAL_DEV
EXTERN_API RtkApiRet dva_drv_tcam_write_internal(uint32 unit, dva_table_list_t table, uint32 addr, const uint32 *data);
#else
EXTERN_API RtkApiRet dva_drv_tcam_write(uint32 unit, dva_table_list_t table, uint32 addr, const uint32 *data);
#endif /* RTK_INTERNAL_DEV */
EXTERN_API RtkApiRet dva_drv_tcam_set_pie_entry(uint32 unit, uint32 addr, const uint32 *data);
EXTERN_API RtkApiRet dva_drv_tcam_set_netroute_entry(uint32 unit, uint32 addr, const uint32 *data);
EXTERN_API RtkApiRet dva_drv_tcam_set_ingif_entry(uint32 unit, uint32 addr, const uint32 *data);
EXTERN_API RtkApiRet dva_drv_tcam_set_sid_entry(uint32 unit, uint32 addr, const uint32 *data);
#endif /* DVA_ASICDRV_TCAM_H__ */