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
#include "rtkas_api_spi_flash.h"
#include "rtkas_api_sys_boot.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup FLASHLESS Flashless Example
 * @{
 */
/**
 * @defgroup RTL907D_V1_FLASHLESS_EXAMPLE RTL907XD_V1 Flashless Example
 * @{
 */
/**
 * @defgroup RTL907D_V1_FLASHLESS_REGIF_EXAMPLE Flashless Regif Update Example
 * @brief <b>Flashless Reg-Interface (I2C/SPI/SMI) update flow</b>
 *
 * @section Purpose
 *   External update firmware and configuration.
 *
 * @section Description
 *   This example demonstrates how to update the firmware and configuration via the reg interface in flashless mode.
 *
 * @image html flashless/regif_flashless.jpg
 *
 *  @{
 */

/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_sys_flashless_update_flow(UnitChip_t unitChip);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_sys_flashless_update_flow(UnitChip_t unitChip)
{
    const int8    *fileName = "image/RTL907XD_v1/flashless/drvTest_flashless_testcase0.bin";
    RtkApiRet      ret      = RT_ERR_OK;
    SysCheckFl_t   sysCheckFl;
    SysUpdateFw_t  sysUpdateFw;
    SysUpdateCfg_t sysUpdateCfg;
    do
    {
        /* Step 1. Check flashless mode. Set byPassEth to 1 to skip link checking */
        sysCheckFl.byPassEth = 1u;
        CHK_FUN_CONTINUE(ret, rtk_sys_check_flashless_mode(unitChip, &sysCheckFl));

        /* Step 2. Update firmware and configuration
         * At this stage, users can manually configure the switch or use rtk_sys_update_config to
         * load the config after the firmware is updated.
         */
        /* Update firmware*/
        sysUpdateFw.fileName = fileName;
        CHK_FUN_CONTINUE(ret, rtk_sys_update_firmware(unitChip, &sysUpdateFw));
        /* Leave firmware stage and enter config stage */
        CHK_FUN_CONTINUE(ret, rtk_sys_leave_fw_to_cfg(unitChip));
        /* Update config if needed*/
        sysUpdateCfg.fileName = fileName;
        CHK_FUN_CONTINUE(ret, rtk_sys_update_config(unitChip, &sysUpdateCfg));
        /* Step 3. Leave flashless */
        CHK_FUN_CONTINUE(ret, rtk_sys_leave_flashless(unitChip));
        /* Step 4. Check if leaving flashless is successful */
        CHK_FUN_CONTINUE(ret, rtk_sys_check_leave_flashless(unitChip));

    } while(0u == 1u);

    /* Expected Behaviour:
     *   Check the reg 0x4A1B004C is the same as image version.
     *   The check function return value "ret" must return RT_ERR_OK.
     */

    return ret;
}
/**@}*/ /* RTL907D_V1_FLASHLESS_REGIF_EXAMPLE */
/**@}*/ /* RTL907D_V1_FLASHLESS_EXAMPLE */
/**@}*/ /* FLASHLESS */
/**@}*/ /* EXAMPLE */
