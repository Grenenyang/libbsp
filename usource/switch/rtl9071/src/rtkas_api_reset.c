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
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_api_reset.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_reset.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_reset.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_reset.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_reset.h"
#endif

/**
 * @defgroup RTK_MODULE_RESET Reset
 * @brief Reset API. This component is used to reset the chip.
 * @{
 */

/**
 * @defgroup RESET_CHIP_RST Reset chip
 * @brief Reset chip
 * @{
 */

/**
 * @brief Chip reset. \n
 *        The reset is triggered by the software configuration setting to reset the switch. Thus, the system will proceed with the boot-up process. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 *            Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK             The reset operation was successfully
 * @retval -RT_ERR_NOT_SUPPORT   Not supported.
 * @retval Others Please refer to dv1_reset_chip_rst(), dva_reset_chip_rst(), cvb_reset_chip_rst(), cva_reset_chip_rst().
 * @note  The power-on behavior depends on the PCB board's power configuration. \n
 *        For the complete boot-up flow, please refer to the SDK programming guide and related boot-up documentation.
 */
RtkApiRet rtk_reset_chip_rst(UnitChip_t unitChip)
{
    RtkApiRet ret;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reset_chip_rst(unitChip.unit);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reset_chip_rst(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reset_chip_rst(unitChip.unit);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reset_chip_rst(unitChip.unit);
            break;
#endif
        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}
/**@}*/ /* RESET_CHIP_RST */

/**@}*/ /* Reset */
