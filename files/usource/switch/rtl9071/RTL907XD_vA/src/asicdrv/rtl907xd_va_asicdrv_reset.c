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

#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_asicdrv_reset.h"

/**
 * @brief Chip reset. \n
 *        The reset is triggered by the software configuration setting to reset the switch. Thus, the system will proceed with the boot-up process.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK   The reset operation was successfully
 * @retval Others      Please refer to dva_drv_indirect_reg_field_set(), dva_drv_hw_indirect_get(), dva_drv_hw_indirect_set(), dva_reg_write() and usecond_delay()
 * @note  The power-on behavior depends on the PCB board's power configuration. \n
 *        For the complete boot-up flow, please refer to the SDK programming guide and related boot-up documentation.
 */
RtkApiRet dva_drv_reset_chip_rst(uint32 unit)
{
    RtkApiRet retVal;
    uint32    regVal;

    /* Enable the chip reset */
    retVal = dva_reg_field_write(unit, DVA_DESIGN_FOR_MEASTUMENT, DVA_DESIGN_FOR_MEASTUMENT_OP_FSM_PHYRSTB_EN_LSP, DVA_DESIGN_FOR_MEASTUMENT_OP_FSM_PHYRSTB_EN_LEN, 1);

    /* Set the scope of the chip reset to all circuits/registers */
    CHK_FUN_RET(retVal, dva_drv_hw_indirect_get(unit, DVA_RESET_GATED_ADDR, &regVal));
    if(RT_ERR_OK == retVal)
    {
        regVal |= ((uint32)0x1u << DVA_RESET_GATED_BIT);
    }
    CHK_FUN_RET(retVal, dva_drv_hw_indirect_set(unit, DVA_RESET_GATED_ADDR, regVal));

    /* Trigger the chip reset */
    CHK_FUN_RET(retVal, dva_drv_hw_indirect_get(unit, DVA_RESET_CHIP_ADDR, &regVal));
    if(RT_ERR_OK == retVal)
    {
        regVal |= ((uint32)0x1u << DVA_RESET_CHIP_BIT);
    }
    CHK_FUN_RET(retVal, dva_reg_write(unit, DVA_RESET_CHIP_ADDR, regVal));

    /* Waiting for the reset to be done */
    CHK_FUN_RET(retVal, rtk_usecond_delay(DVA_RESET_TIME_US));

    return retVal;
}
