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

#include "rtkas_api_ext.h"
#include "rtkas_api_port_isolation.h"

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_api.h"
#endif

RtkApiRet dv1_isolation_set_single(void);

/**
 * @addtogroup PORTISO_EXAMPLE Port Isolation Example
 * @{
 */
/**
 * @defgroup RTL907DV1_PORTISO_EXAMPLE RTL907XD_V1 Port Isolation Example
 * @{
 */
/**
 * @defgroup RTL907DV1_PORTISO_SIN_EXAMPLE Single Isolation Example
 * @brief This demonstrates how to set single port isolation.
 *
 * @image html single.png
 *
 * @section Purpose
 * This demonstrates how to set single port isolation.
 *
 * @section Description
 * Choose port 1 to demonstrate APIs.
 * Set the Port Isolation Mask to prevent one port from transmitting packets to another port. \n
 * <> Set the port isolation mask for Port 1 \n
 * Set the corresponding bit (Port 2) of the Port Isolation Mask for Port 1, then Port 1 cannot send packets to Port 2, but Port 1 can still receive packets from Port 2. \n
 * <> Transmit packets from Port 1 to Port 2 \n
 * Port 2 could not receive packets from Port 1. \n
 * <> Transmit packets from Port 2 to Port 1 \n
 * Transmit packet to Port 1 from Port 2, check if Port 1 received the packet or not.
 *
 * @{
 */
RtkApiRet dv1_isolation_set_single(void)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t uintChip;
    uintChip.unit = DV1_UNIT;
    uintChip.chip = CHIP_907XD_V1;
    PortIsoInfo_t isoSet, isoGet;

    do
    {
        /* Step 1. Set Port Isolation Mask for Port 1 */
        isoSet.port     = (RtkPort)DV1_RTK_USER_PORT_1ST;
        isoSet.portmask = (RtkPmap)DV1_RTK_USER_PMAP_2ND;
        CHK_FUN_CONTINUE(ret, rtk_port_isolation_set(uintChip, &isoSet));

        /* Step 2. Get Port Isolation Mask from Port 1 */
        isoGet.port = (RtkPort)DV1_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(ret, rtk_port_isolation_get(uintChip, &isoGet));

        /* Expected Behavior:
         *     1. the obtained isolation port mask is expected;
         *     2. Port 1 cannot receive any packet from Port 2;
         */
        if(isoGet.portmask != isoSet.portmask)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907DV1_PORTISO_SIN_EXAMPLE */
/**@}*/ /* RTL907DV1_PORTISO_EXAMPLE */
/**@}*/ /* PORTISO_EXAMPLE */