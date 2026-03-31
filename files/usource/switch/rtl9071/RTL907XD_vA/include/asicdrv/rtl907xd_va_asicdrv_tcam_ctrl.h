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

#ifndef DVA_ASICDRV_TCAM_CTRL_H__
#define DVA_ASICDRV_TCAM_CTRL_H__

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet dva_drv_tcam_get_safety(uint32 unit, uint32 *pEnable);
EXTERN_API RtkApiRet dva_drv_tcam_set_safety(uint32 unit, uint32 enable);
EXTERN_API RtkApiRet dva_drv_tcam_get_pie_check_en(uint32 unit, uint32 block, uint32 tcamNum, uint32 *pEnable);
EXTERN_API RtkApiRet dva_drv_tcam_set_pie_check_en(uint32 unit, uint32 block, uint32 tcamNum, uint32 enable);
EXTERN_API RtkApiRet dva_drv_tcam_set_pie_all_check_en(uint32 unit, uint32 enable);
EXTERN_API RtkApiRet dva_drv_tcam_get_pie_golden_crc(uint32 unit, uint32 block, uint32 tcamNum, uint32 *pGoldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_set_pie_golden_crc(uint32 unit, uint32 block, uint32 tcamNum, uint32 goldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_set_pie_block_golden_crc(uint32 unit, uint32 block, dva_drv_TcamPieCrcVal_t *pGoldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_get_netroute_check_en(uint32 unit, uint32 tcamNum, uint32 *pEnable);
EXTERN_API RtkApiRet dva_drv_tcam_set_netroute_check_en(uint32 unit, uint32 tcamNum, uint32 enable);
EXTERN_API RtkApiRet dva_drv_tcam_set_netroute_all_check_en(uint32 unit, uint32 enable);
EXTERN_API RtkApiRet dva_drv_tcam_get_netroute_golden_crc(uint32 unit, uint32 tcamNum, uint32 *pGoldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_set_netroute_golden_crc(uint32 unit, uint32 tcamNum, uint32 goldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_set_netroute_all_golden_crc(uint32 unit, dva_drv_TcamNetRouteCrcVal_t *pGoldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_get_ingif_check_en(uint32 unit, uint32 tcamNum, uint32 *pEnable);
EXTERN_API RtkApiRet dva_drv_tcam_set_ingif_check_en(uint32 unit, uint32 tcamNum, uint32 enable);
EXTERN_API RtkApiRet dva_drv_tcam_set_ingif_all_check_en(uint32 unit, uint32 enable);
EXTERN_API RtkApiRet dva_drv_tcam_get_ingif_golden_crc(uint32 unit, uint32 tcamNum, uint32 *pGoldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_set_ingif_golden_crc(uint32 unit, uint32 tcamNum, uint32 goldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_set_ingif_all_golden_crc(uint32 unit, dva_drv_TcamIngifCrcVal_t *pGoldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_get_sid_check_en(uint32 unit, uint32 tcamNum, uint32 *pEnable);
EXTERN_API RtkApiRet dva_drv_tcam_set_sid_check_en(uint32 unit, uint32 tcamNum, uint32 enable);
EXTERN_API RtkApiRet dva_drv_tcam_set_sid_all_check_en(uint32 unit, uint32 enable);
EXTERN_API RtkApiRet dva_drv_tcam_get_sid_golden_crc(uint32 unit, uint32 tcamNum, uint32 *pGoldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_set_sid_golden_crc(uint32 unit, uint32 tcamNum, uint32 goldenCrc);
EXTERN_API RtkApiRet dva_drv_tcam_set_sid_all_golden_crc(uint32 unit, dva_drv_TcamSidCrcVal_t *pGoldenCrc);
#endif /* DVA_ASICDRV_TCAM_CTRL_H__ */