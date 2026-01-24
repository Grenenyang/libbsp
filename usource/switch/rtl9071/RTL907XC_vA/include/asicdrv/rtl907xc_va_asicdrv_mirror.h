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

#ifndef CVA_ASICDRV_MIRROR_H_
#define CVA_ASICDRV_MIRROR_H_

#include "rtkas_common_types.h"
#include "rtl907xc_va_api.h"
#include "rtl907xc_va_asicdrv.h"
#include "rtl907xc_va_api_mirror.h"

EXTERN_API RtkApiRet cva_drv_mirror_port_based_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringSysPort, RtkPmap mirroredIngressSysPortmask, RtkPmap mirroredEgressSysPortMask, uint32 morg);
EXTERN_API RtkApiRet cva_drv_mirror_port_based_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkPmap *pMirroredIngressPortmask, RtkPmap *pMirroredEgressPortmask, uint32 *pMorg);
EXTERN_API RtkApiRet cva_drv_mirror_flow_based_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringSysPort, RtkPmap mirroredIngressSysPortmask, RtkPmap mirroredEgressSysPortMask, uint32 morg);
EXTERN_API RtkApiRet cva_drv_mirror_flow_based_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkPmap *pMirroredIngressPortmask, RtkPmap *pMirroredEgressPortmask, uint32 *pMorg);
EXTERN_API RtkApiRet cva_drv_mirror_rspan_tx_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringSysPort, RtkRspanTxtagAction_e action, RtkRspanTxtag_t tag);
EXTERN_API RtkApiRet cva_drv_mirror_rspan_tx_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkRspanTxtagAction_e *pAction, RtkRspanTxtag_t *pTag);
EXTERN_API RtkApiRet cva_drv_mirror_rspan_rx_set(uint32 unit, uint8 mirrorNo, RtkPmap mirroredSysProtmask, RtkEnable enable, uint16 vid);
EXTERN_API RtkApiRet cva_drv_mirror_rspan_rx_get(uint32 unit, uint8 mirrorNo, RtkPmap *pMirroredPortmask, RtkEnable *pEnable, uint16 *pVid);

#endif /*#ifndef CVA_ASICDRV_MIRROR_H_*/
