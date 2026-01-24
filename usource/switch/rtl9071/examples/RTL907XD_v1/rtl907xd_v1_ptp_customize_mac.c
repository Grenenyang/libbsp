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

#include "rtkas_api_ptp_ext.h"
/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup PTP PTP Example
 * @{
 */
/**
 * @defgroup RTL907XDV1_PTP_EXAMPLE RTL907XD_V1 PTP Example
 * @{
 */

/**
 *  @defgroup RTL907XDV1_PTP_CUSTOMIZE__MAC_EXAMPLE PTP customize mac Example
 *  @brief <b>Customized Destination MAC Address</b> \n
 *  @section Purpose
 *           This example demonstrates how to customize the PTP destination address to 01-80-C2-11-22-33. \n\n
 *  @section Description
 *           If there is a requirement to customize the PTP destination address, the switch can support this customization. \n
 *           Note that this destination address customization must be applied to all PTP devices (GM, Bridge, End point) in the environment to ensure successful time synchronization. \n
 *           The default destination MAC address for PTP messages is 01-80-C2-00-00-0E. \n
 *  @{
 */

RtkApiRet dv1_ptp_customize_mac(void);

RtkApiRet dv1_ptp_customize_mac(void)
{
    uint8      port                        = 0u;
    uint8      USER_MAC_ADDR[MAC_ADDR_LEN] = {0x01u, 0x80u, 0xc2u, 0x11u, 0x22u, 0x33u};
    RtkApiRet  ret                         = RT_ERR_OK;
    RtkApiRet  apiresult                   = RT_ERR_OK;
    uint32     callAPIerrorFlag            = 0u;
    UnitChip_t unitChip;

    unitChip.chip = CHIP_907XD_V1;
    unitChip.unit = DV1_UNIT;

    PtpGlenable_t            ptpGlenable;
    PtpCustomDamac01Enable_t ptpCustomDamac01Enable = {0u};
    PtpCustomDamac01_t       ptpCustomDamac01       = {0u};

    do
    {
        /* Step 1. Disable PTP to configure. */
        ptpGlenable.glenable = 0u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_glenable(unitChip, &ptpGlenable));

        /* Step 2. Set internal port. */
        ptpCustomDamac01Enable.port   = DV1_RTK_CPU_PORT;
        ptpCustomDamac01Enable.enable = 1u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_damac01_enable_custom(unitChip, &ptpCustomDamac01Enable));
        ptpCustomDamac01.port     = DV1_RTK_CPU_PORT;
        ptpCustomDamac01.damac[0] = USER_MAC_ADDR[0];
        ptpCustomDamac01.damac[1] = USER_MAC_ADDR[1];
        ptpCustomDamac01.damac[2] = USER_MAC_ADDR[2];
        ptpCustomDamac01.damac[3] = USER_MAC_ADDR[3];
        ptpCustomDamac01.damac[4] = USER_MAC_ADDR[4];
        ptpCustomDamac01.damac[5] = USER_MAC_ADDR[5];
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_custom_damac01(unitChip, &ptpCustomDamac01));

        for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_12TH; port++)
        {
            /* Step 3. Enable Setting RX DMAC. */
            ptpCustomDamac01Enable.port   = port;
            ptpCustomDamac01Enable.enable = 1u;
            apiresult                     = rtk_ptp_set_damac01_enable_custom(unitChip, &ptpCustomDamac01Enable);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint32)1u << 0);
            }
            /* Step 4. Set RX DMAC to customized MAC 01:80:c2:11:22:33 */
            ptpCustomDamac01.port     = port;
            ptpCustomDamac01.damac[0] = USER_MAC_ADDR[0];
            ptpCustomDamac01.damac[1] = USER_MAC_ADDR[1];
            ptpCustomDamac01.damac[2] = USER_MAC_ADDR[2];
            ptpCustomDamac01.damac[3] = USER_MAC_ADDR[3];
            ptpCustomDamac01.damac[4] = USER_MAC_ADDR[4];
            ptpCustomDamac01.damac[5] = USER_MAC_ADDR[5];
            apiresult                 = rtk_ptp_set_custom_damac01(unitChip, &ptpCustomDamac01);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint32)1u << 1);
            }
        }
        if(0u != callAPIerrorFlag)
        {
            ret = -RT_ERR_FAILED;
            continue;
        }

        /* Step 5. Enable PTP. */
        ptpGlenable.glenable = 1u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_glenable(unitChip, &ptpGlenable));

        /* Step 6 Reinit protocol configuration. */
        CHK_FUN_CONTINUE((ret), rtk_ptp_reinitprotocol(unitChip));
    } while(0u == 1u);

    /* Expected Behaviour:
     * 1. Check clock stable on DUT. clkstable: > 0 means stable. 0 means unstable.
     * - Check it by API rtk_ptp_clkstable_get(uint32 domain, uint8 *clkstable).
     * 2. Sync/Follow_Up from DUT should have DMAC 01:80:c2:11:22:33
     * */

    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XDV1_PTP_EXAMPLE */
/**@}*/ /* RTL907XDV1_PTP_CUSTOMIZE__MAC_EXAMPLE */