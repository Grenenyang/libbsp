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

#ifndef DVA_ASICDRV_TCAM_IRQ_H__
#define DVA_ASICDRV_TCAM_IRQ_H__

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet dva_drv_tcam_get_pie_fail_irpt(uint32 unit, uint32 block, uint32 tcamNum, uint32 *pFail);
EXTERN_API RtkApiRet dva_drv_tcam_get_pie_clr_irpt(uint32 unit, uint32 block, uint32 tcamNum, uint32 *pClear);
EXTERN_API RtkApiRet dva_drv_tcam_set_pie_clr_irpt(uint32 unit, uint32 block, uint32 tcamNum, uint32 clear);
EXTERN_API RtkApiRet dva_drv_tcam_get_acl_ip(uint32 unit, uint32 *pIP);
EXTERN_API RtkApiRet dva_drv_tcam_get_acl_ie(uint32 unit, uint32 *pIE);
EXTERN_API RtkApiRet dva_drv_tcam_set_acl_ie(uint32 unit, uint32 ie);
EXTERN_API RtkApiRet dva_drv_tcam_get_netroute_fail_irpt(uint32 unit, uint32 tcamNum, uint32 *pFail);
EXTERN_API RtkApiRet dva_drv_tcam_get_netroute_clr_irpt(uint32 unit, uint32 tcamNum, uint32 *pClear);
EXTERN_API RtkApiRet dva_drv_tcam_set_netroute_clr_irpt(uint32 unit, uint32 tcamNum, uint32 clear);
EXTERN_API RtkApiRet dva_drv_tcam_get_ingif_fail_irpt(uint32 unit, uint32 tcamNum, uint32 *pFail);
EXTERN_API RtkApiRet dva_drv_tcam_get_ingif_clr_irpt(uint32 unit, uint32 tcamNum, uint32 *pClear);
EXTERN_API RtkApiRet dva_drv_tcam_set_ingif_clr_irpt(uint32 unit, uint32 tcamNum, uint32 clear);
EXTERN_API RtkApiRet dva_drv_tcam_get_route_ip(uint32 unit, uint32 *pIP);
EXTERN_API RtkApiRet dva_drv_tcam_get_route_ie(uint32 unit, uint32 *pIE);
EXTERN_API RtkApiRet dva_drv_tcam_set_route_ie(uint32 unit, uint32 ie);
EXTERN_API RtkApiRet dva_drv_tcam_get_sid_fail_irpt(uint32 unit, uint32 tcamNum, uint32 *pFail);
EXTERN_API RtkApiRet dva_drv_tcam_get_sid_clr_irpt(uint32 unit, uint32 tcamNum, uint32 *pClear);
EXTERN_API RtkApiRet dva_drv_tcam_set_sid_clr_irpt(uint32 unit, uint32 tcamNum, uint32 clear);
EXTERN_API RtkApiRet dva_drv_tcam_get_cb_ip(uint32 unit, uint32 *pIP);
EXTERN_API RtkApiRet dva_drv_tcam_get_cb_ie(uint32 unit, uint32 *pIE);
EXTERN_API RtkApiRet dva_drv_tcam_set_cb_ie(uint32 unit, uint32 ie);
#endif /* DVA_ASICDRV_TCAM_CRC_H__ */