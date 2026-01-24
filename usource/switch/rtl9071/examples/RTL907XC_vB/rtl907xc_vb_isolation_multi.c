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

#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_api.h"
#endif

RtkApiRet cvb_isolation_set_multi(void);

/**
 * @addtogroup RTL907CVB_PORTISO_EXAMPLE
 * @{
 */
/**
 * @defgroup RTL907CVB_PORTISO_MUL_EXAMPLE Multiple Isolation Example
 * @brief This demonstrates how to set multiple port isolation.
 *
 * @image html multi.png
 *
 * @section Purpose
 * This demonstrates how to set multiple port isolation.
 *
 * @section Description
 * Choose port 1 and port 2 to demonstrate APIs.
 * Set over two Port Isolation Masks to prevent assigned ports from transmitting packets to each other. \n
 * <> Set multiple port isolation masks for Port 1 and Port 2 \n
 * Set the corresponding bit (Port 2) of the Port Isolation Mask for Port 1. \n
 * Set the corresponding bit (Port 1) of the Port Isolation Mask for Port 2. \n
 * After setting the Port Isolation Mask of both ports, they can’t send packets to each other. \n
 * <> Transmit packets to Port 1 from Port 2 \n
 * Transmit packets to Port 1 from Port 2, check that Port 1 can’t receive packets from Port 2. \n
 * <> Transmit packets to Port 2 from Port 1 \n
 * Transmit packets to Port 2 from Port 1, check that Port 2 can’t receive packets from Port 1.
 *
 * @{
 */
RtkApiRet cvb_isolation_set_multi(void)
{
    RtkApiRet  ret = RT_ERR_OK;
    UnitChip_t uintChip;
    uintChip.unit = CVB_UNIT;
    uintChip.chip = CHIP_907XC_VB;
    PortIsoInfo_t isoSet1, isoSet2;
    PortIsoInfo_t isoGet1, isoGet2;

    do
    {
        /* Step 1-1. Set Port Isolation Mask for Port 1 */
        isoSet1.port     = (RtkPort)CVB_RTK_USER_PORT_1ST;
        isoSet1.portmask = (RtkPmap)CVB_RTK_USER_PMAP_2ND;
        CHK_FUN_CONTINUE(ret, rtk_port_isolation_set(uintChip, &isoSet1));
        /* Step 1-2. Get Port Isolation Mask from Port 1 */
        isoGet1.port = (RtkPort)CVB_RTK_USER_PORT_1ST;
        CHK_FUN_CONTINUE(ret, rtk_port_isolation_get(uintChip, &isoGet1));
        /* Expected Behavior:
         *     1. the obtained isolation port mask is expected;
         *     2. Port 1 cannot receive any packet from Port 2;
         */
        if(isoGet1.portmask != isoSet1.portmask)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 2-1. Set Port Isolation Mask for Port 2 */
        isoSet2.port     = (RtkPort)CVB_RTK_USER_PORT_2ND;
        isoSet2.portmask = (RtkPmap)CVB_RTK_USER_PMAP_1ST;
        CHK_FUN_CONTINUE(ret, rtk_port_isolation_set(uintChip, &isoSet2));
        /* Step 2-2. Get Port Isolation Mask from Port 2 */
        isoGet2.port = (RtkPort)CVB_RTK_USER_PORT_2ND;
        CHK_FUN_CONTINUE(ret, rtk_port_isolation_get(uintChip, &isoGet2));
        /* Expected Behavior:
         *     1. the obtained isolation port mask is expected;
         *     2. Port 2 cannot receive any packet from Port 1;
         */
        if(isoGet2.portmask != isoSet2.portmask)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907CVB_PORTISO_MUL_EXAMPLE */
/**@}*/ /* RTL907CVB_PORTISO_EXAMPLE */