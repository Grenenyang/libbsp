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

#include "rtl907xc_vb_api_reset.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
#include "rtl907xc_vb_asicdrv_reset.h"
#include "rtl907xc_vb_reg_list.h"
#include "rtl907xc_vb_regField_list.h"
#include "rtkas_types.h"

/**
 * @addtogroup RTK_MODULE_RESET Reset
 * @brief Reset APIs
 * @{
 */

/**
 * @addtogroup RESET_CHIP_RST Reset chip
 * @{
 */

/**
 * @addtogroup RESET_CHIP_RST_ASIC Low Level Driver
 * @brief Reset Low Level Driver
 * @{
 */

/**
 * @brief Chip reset. \n
 *        The reset is triggered by the software configuration setting to reset the switch. Thus, the system will proceed with the boot-up process.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK   The reset operation was successfully
 * @retval Others      Please refer to cvb_drv_reset_chip_rst()
 * @note  The power-on behavior depends on the PCB board's power configuration. \n
 *        For the complete boot-up flow, please refer to the SDK programming guide and related boot-up documentation.
 */
RtkApiRet cvb_reset_chip_rst(uint32 unit)
{
    RtkApiRet retVal;

    /* Step 1. Trigger the chip reset */
    retVal = cvb_drv_reset_chip_rst(unit);

    return retVal;
}
/**@}*/ /* RESET_CHIP_RST_ASIC */
/**@}*/ /* RESET_CHIP_RST */

/**@}*/ /* Reset */