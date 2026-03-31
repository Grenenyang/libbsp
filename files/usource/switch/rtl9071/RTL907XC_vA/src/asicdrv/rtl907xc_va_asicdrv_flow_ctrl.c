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

#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_api_qos.h"
#include "rtl907xc_va_api_flow_ctrl.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#include "rtl907xc_va_asicdrv_flow_ctrl.h"

/**
 * @brief Check if RA bypass is disabled.
 * @param[in] unit     Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port     Port ID (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
                       Port ID
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FLOWCTRL_RA_BYPASS
 * @retval Others Please refer to cva_reg_field_write() and RTL906X_STK_SDS_REG_SET()
 */
RtkApiRet cva_drv_flow_ctrl_ra_bypass_chk(uint32 unit, cva_RtkPort port)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0x0u;
    uint32    regVal  = 0x0u;

    if(port > CVA_RTK_USER_PORT_END)
    {
        retVal = -RT_ERR_PORT_ID;
    }

    /* Check if RA bypass is disabled */
    if(RT_ERR_OK == retVal)
    {
        regAddr = CVA_PORT0_L2_MISC_TX_RA_REG1 + (CVA_FLOWCTRL_PORT_OFFSET * (uint32)cva_g_userPort2sysPort[port]);
        CHK_FUN_RET(retVal, cva_drv_indirect_reg_field_get(unit, regAddr, CVA_PORT0_L2_MISC_TX_RA_REG1_RG_H2E_BYPASS_LSP, CVA_PORT0_L2_MISC_TX_RA_REG1_RG_H2E_BYPASS_LEN, &regVal));
        if((RT_ERR_OK == retVal) && (CVA_FLOWCTRL_RA_BYPASS_EN == regVal))
        {
            retVal = -RT_ERR_FLOWCTRL_RA_BYPASS;
        }
    }

    return retVal;
}

/**
 * @brief Set global flow ctrl EnRx/Tx_FC
 * @param[in] unit     Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port     Port ID (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
                       Port ID
 * @param[in] en       enable (CVA_DISABLED to CVA_ENABLED) \n
 *                     Flow Control global status
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 * @retval Others Please refer to cva_reg_read(), cva_reg_field_set() and cva_reg_write()
 */
RtkApiRet cva_drv_flow_ctrl_global_set(uint32 unit, cva_RtkPort port, cva_RtkEnable en)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0x0u;

    if(port > CVA_RTK_USER_PORT_END)
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else
    {
        regAddr = CVA_PORT0_L2_MISC + (CVA_FLOWCTRL_PORT_OFFSET * (uint32)cva_g_userPort2sysPort[port]);
    }

    /* Global recognize RX pause enable */
    CHK_FUN_RET(retVal, cva_drv_indirect_reg_field_set(unit, regAddr, CVA_PORT0_L2_MISC_ENRX_FC_LSP, CVA_PORT0_L2_MISC_ENRX_FC_LEN, en));
    /* Global TX pause enable */
    CHK_FUN_RET(retVal, cva_drv_indirect_reg_field_set(unit, regAddr, CVA_PORT0_L2_MISC_ENTX_FC_LSP, CVA_PORT0_L2_MISC_ENTX_FC_LEN, en));

    return retVal;
}
