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

#include "rtkas_common_types.h"
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtkas_error.h"
#include "rtkas_api_ethernet_access.h"
#include "rtkas_api_sys_boot.h"
#include "rtkas_api_interface.h"
#include "rtl907xd_va_asicdrv_port.h"
#include "rtkas_api_phy.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dva_eth_fe_flashless_update_flow(UnitChip_t unitChip);
/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup FLASHLESS Flashless Example
 * @{
 */

/**
 * @addtogroup RTL907D_VA_FLASHLESS_EXAMPLE RTL907XD_VA Flashless Example
 * @{
 */
/**
 * @addtogroup RTL907D_VA_FLASHLESS_ETH_FE_EXAMPLE Flashless Eth Update Example via FE
 * @brief <b>Flashless Ethernet Access update flow via FE</b>
 *
 * @section Purpose
 *   External update firmware and configuration via FE port.\n\n
 *
 * @section Description
 *   This example demonstrates how to configure FE port and then update the firmware and configuration via the ethernet access on FE port in flashless mode.
 *
 * @image html flashless/eth_flashless.jpg
 *
 * @{
 */
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dva_eth_fe_flashless_update_flow(UnitChip_t unitChip)
{
    const int8       *fileName   = "image/RTL907XD_vA/flashless/P7_FE_10_GENP7FL.bin";
    RtkApiRet         ret        = RT_ERR_OK;
    RtkPort           configPort = DVA_RTK_USER_PORT_7TH;
    SysCheckFl_t      sysCheckFl;
    SysEthPortRxDis_t sysEthPortRxDis;
    SysEthUpdateFw_t  sysEthUpdateFw;
    SysEthUpdateCfg_t sysEthUpdateCfg;
    PortFeConfigSet_t portFeConfigSet = {.userPort = DVA_RTK_USER_PORT_7TH, .portMode = EM_MODE_EXT_FAST_ETHERNET, .anEnable = 0u, .speed = PHY_BMCR_SPEED_10M};

    do
    {
        /* Step 1. Ethernet access flashless update only supports unicast. Please make sure DMAC is set correctly */
        /* Step 2. The link partner connects with port 7 FE of the switch */
        /* Step 3. Check flashless mode. If wanting to bypass eth link checking, set byPassEth to 1 */
        sysCheckFl.byPassEth = 1u;
        CHK_FUN_CONTINUE(ret, rtk_sys_check_flashless_mode(unitChip, &sysCheckFl));

        /* Step 4. Configure the FE interface to FE 10M*/
        CHK_FUN_CONTINUE(ret, rtk_port_fe_config_set(unitChip, &portFeConfigSet));
        /* Wait FE link up*/
        (void)g_fp_udelay(3000000);

        /* Step 5. Set port7 as configured port and disable other ports rx_en */
        sysEthPortRxDis.userPort = configPort;
        CHK_FUN_CONTINUE(ret, rtk_sys_eth_disable_non_configure_port_rx(unitChip, &sysEthPortRxDis));

        /* Step 6. Update firmware and configuration
         * At this stage, users can manually configure the switch or use rtk_sys_eth_update_config to
         * load config after the firmware is updated.
         */
        /* Update firmware */
        sysEthUpdateFw.fileName = fileName;
        CHK_FUN_CONTINUE(ret, rtk_sys_eth_update_firmware(unitChip, &sysEthUpdateFw));
        /* Leave firmware stage and enter config stage */
        CHK_FUN_CONTINUE(ret, rtk_sys_eth_leave_fw_to_cfg(unitChip));
        sysEthUpdateCfg.fileName = fileName;
        /* Update config if needed*/
        CHK_FUN_CONTINUE(ret, rtk_sys_eth_update_config(unitChip, &sysEthUpdateCfg));
        /* Step 7. Leave flashless*/
        CHK_FUN_CONTINUE(ret, rtk_sys_leave_flashless(unitChip));
        /* Step 8. Check leave flashless is successful*/
        CHK_FUN_CONTINUE(ret, rtk_sys_check_leave_flashless(unitChip));
    } while(0u == 1u);

    /* Expected Behaviour:
     *   Check the reg 0x4A1B004C is the same as the image version.
     *   The check function return value "ret" must return RT_ERR_OK.
     */

    return ret;
}
/**@}*/ /* RTL907D_VA_FLASHLESS_ETH_EXAMPLE */

/**@}*/ /* RTL907D_VA_FLASHLESS_EXAMPLE */
/**@}*/ /* FLASHLESS */
/**@}*/ /* EXAMPLE */
