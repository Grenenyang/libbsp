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

#ifndef DVA_ASICDRV_TCAM_CRC_H__
#define DVA_ASICDRV_TCAM_CRC_H__

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet dva_drv_tcam_pie_crc_calculate(const uint32 *data, uint32 entryNum, const dva_drv_TcamPieCrcVal_t *curCrc, dva_drv_TcamPieCrcVal_t *goldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_get_pie_table_crc(uint32 unit, uint32 block, dva_drv_TcamEntryInfo *updateEntry, dva_drv_TcamPieCrcVal_t *crc);
EXTERN_API RtkApiRet dva_drv_tcam_netroute_crc_calculate(const uint32 *data, uint32 entryNum, const dva_drv_TcamNetRouteCrcVal_t *curCrc, dva_drv_TcamNetRouteCrcVal_t *goldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_get_netroute_table_crc(uint32 unit, dva_drv_TcamEntryInfo *updateEntry, dva_drv_TcamNetRouteCrcVal_t *crc);
EXTERN_API RtkApiRet dva_drv_tcam_ingif_crc_calculate(const uint32 *data, uint32 entryNum, const dva_drv_TcamIngifCrcVal_t *curCrc, dva_drv_TcamIngifCrcVal_t *goldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_get_ingif_table_crc(uint32 unit, dva_drv_TcamEntryInfo *updateEntry, dva_drv_TcamIngifCrcVal_t *crc);
EXTERN_API RtkApiRet dva_drv_tcam_sid_crc_calculate(const uint32 *data, uint32 entryNum, const dva_drv_TcamSidCrcVal_t *curCrc, dva_drv_TcamSidCrcVal_t *goldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_get_sid_table_crc(uint32 unit, dva_drv_TcamEntryInfo *updateEntry, dva_drv_TcamSidCrcVal_t *crc);
#endif /* DVA_ASICDRV_TCAM_CRC_H__ */