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

#ifndef DVA_RTK_API_MIRROR_H__
#define DVA_RTK_API_MIRROR_H__

#include "rtl907xd_va_api.h"
#include "rtkas_api_mirror.h"

#define DVA_MIRROR_FUNCTION_OFFSET (0x2Cu)

EXTERN_API RtkApiRet dva_mirror_port_based_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringPort, RtkPmap mirroredIngressPortmask, RtkPmap mirroredEgressPortmask, uint32 morg);

EXTERN_API RtkApiRet dva_mirror_port_based_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkPmap *pMirroredIngressPortmask, RtkPmap *pMirroredEgressPortmask, uint32 *pMorg);

EXTERN_API RtkApiRet dva_mirror_flow_based_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringPort, RtkPmap mirroredIngressPortmask, RtkPmap mirroredEgressPortmask, uint32 morg);

EXTERN_API RtkApiRet dva_mirror_flow_based_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkPmap *pMirroredIngressPortmask, RtkPmap *pMirroredEgressPortmask, uint32 *pMorg);

EXTERN_API RtkApiRet dva_mirror_filter_set(uint32 unit, uint8 mirrorNo, RtkEnable enable);

EXTERN_API RtkApiRet dva_mirror_filter_get(uint32 unit, uint8 mirrorNo, RtkEnable *pEnable);

EXTERN_API RtkApiRet dva_mirror_sample_rate_set(uint32 unit, uint8 mirrorNo, uint32 samples);

EXTERN_API RtkApiRet dva_mirror_sample_rate_get(uint32 unit, uint8 mirrorNo, uint32 *pSamples);

EXTERN_API RtkApiRet dva_rspan_tx_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringPort, RtkRspanTxtagAction_e action, RtkRspanTxtag_t tag);

EXTERN_API RtkApiRet dva_rspan_tx_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkRspanTxtagAction_e *pAction, RtkRspanTxtag_t *pTag);

EXTERN_API RtkApiRet dva_rspan_rx_set(uint32 unit, uint8 mirrorNo, RtkPmap mirroredPortmask, RtkEnable enable, uint16 vid);

EXTERN_API RtkApiRet dva_rspan_rx_get(uint32 unit, uint8 mirrorNo, RtkPmap *pMirroredPortmask, RtkEnable *pEnable, uint16 *pVid);

EXTERN_API RtkApiRet dva_mirror_reset(uint32 unit, uint8 mirrorNo);

#endif /* DVA_RTK_API_MIRROR_H__ */
