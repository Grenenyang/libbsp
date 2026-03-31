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

#include "rtkas_types.h"
#include "rtkas_system_porting.h"
#include "rtl907xd_v1_asicdrv_stacking.h"
#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtl907xd_v1_api_stacking.h"
#include "rtkas_api_stacking.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup STK_EXAMPLE Stacking Example
 * @{
 */

/**
 * @addtogroup RTL907DV1_STACKING_EXAMPLE RTL907XD_V1 Stacking Example
 * @{
 */

/**
 * @addtogroup RTL907DV1_STACKING_FLASHLESS_EXAMPLE Stacking Flashless Example
 * @brief <b>Stacking Flashless Update Slave</b>
 *
 * @section Purpose
 *   Load stacking slave in flashless mode.
 *
 * @section Description
 *   This example constructs stacking in flashless mode. \n
 *   The example updates the slave unit 2 with drvTest_stacking_fw.bin and drvTest_stacking_cfg.bin. (firmware and configuration bin are the same if firmware bin is not secure bin) \n
 *   Connect the enabled stacking port between master DUT and slave DUT. \n
 *   Reboot slave DUT before master DUT. \n
 *   Refer to example eth_flashless_update to update master DUT via Ethernet \n
 *
 * @image html stacking/stacking_flashless_update.jpg
 *
 * @{
 */
RtkApiRet dv1_stacking_flashless_update(void);
RtkApiRet dv1_stacking_flashless_update(void)
{
    RtkApiRet                      retVal      = RT_ERR_OK;
    uint32                         slaveUnit   = 2u;
    const int8                    *fwFileName  = "image/RTL907XD_v1/stacking/drvTest_stacking_fw.bin";
    const int8                    *cfgFileName = "image/RTL907XD_v1/stacking/drvTest_stacking_cfg.bin";
    UnitChip_t                     unitChip;
    StackingFlashlessUpdateSlave_t stackingFlashlessUpdateSlave;

    unitChip.chip = CHIP_907XD_V1;

    /* Step 1. Update slave DUT */
    (void)drv_memset(&stackingFlashlessUpdateSlave, 0, sizeof(StackingFlashlessUpdateSlave_t));
    stackingFlashlessUpdateSlave.slaveUnitDV1 = slaveUnit;
    stackingFlashlessUpdateSlave.fwFileName   = fwFileName;
    stackingFlashlessUpdateSlave.cfgFileName  = cfgFileName;
    CHK_FUN_RET(retVal, rtk_stacking_flashless_update_slave(unitChip, &stackingFlashlessUpdateSlave));
    /* Expected Behaviour: The slave stacking related OTP has been programmed */

    return retVal;
}

/**@}*/ /* RTL907DV1_STACKING_FLASHLESS_EXAMPLE */
/**@}*/ /* RTL907DV1_STACKING_EXAMPLE */
/**@}*/ /* STK_EXAMPLE */
/**@}*/ /* EXAMPLE */