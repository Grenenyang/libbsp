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

#include "rtl907xd_va_api.h"
#include "rtl907xd_va_api_qos.h"
#include "rtl907xd_va_api_flow_ctrl.h"
#include "rtl907xd_va_asicdrv_flow_ctrl.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_regField_list.h"

/**
 * @addtogroup RTK_MODULE_FLOWCTRL Flow Control
 * @brief Flow Control APIs
 * @{
 */

/**
 * @addtogroup FLOWCTRL_EN Flow Control Status
 * @{
 */

/**
 * @defgroup FLOWCTRL_EN_ASIC Low Level Driver
 * @brief FLOWCTRL Low Level Driver
 * @{
 */

/**
 * @brief Set Flow Control status.
 * @param[in] unit     Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port     Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
                       Port ID
 * @param[in] en       enable (DVA_DISABLED to DVA_ENABLED) \n
                       Flow Control status
 * @retval RT_ERR_OK         The flow control status has been successfully set.
 * @retval -RT_ERR_PORT_ID   The port is invalid.
 * @retval -RT_ERR_ENABLE    The enable (en) value is invalid.
 * @retval Others Please refer to dva_drv_flow_ctrl_ra_bypass_chk(), dva_drv_indirect_reg_field_set() and dva_drv_flow_ctrl_global_set()
 */
RtkApiRet dva_flow_ctrl_en_set(uint32 unit, dva_RtkPort port, dva_RtkEnable en)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0u;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(en >= DVA_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* Calculate the register address */
        regAddr = DVA_PORT0_PROPERTY_CONFIGURE + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]);
    }

    /* Check if RA bypass is disabled */
    CHK_FUN_RET(retVal, dva_drv_flow_ctrl_ra_bypass_chk(unit, port));
    /* Set TX/RX pause of MAC */
    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, regAddr, DVA_PORT0_PROPERTY_CONFIGURE_EN_PAUSE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_PAUSE_LEN, en));
    CHK_FUN_RET(retVal, dva_drv_indirect_reg_field_set(unit, regAddr, DVA_PORT0_PROPERTY_CONFIGURE_EN_ASY_PAUSE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_ASY_PAUSE_LEN, en));
    /* Get global TX/RX pause of RA module */
    CHK_FUN_RET(retVal, dva_drv_flow_ctrl_global_set(unit, port, en));

    return retVal;
}

/**
 * @brief Get Flow Control status.
 * @param[in] unit      Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
                        Port ID
 * @param[out] pEn      enable (N/A) \n
                        Flow Control status
 * @retval RT_ERR_OK              The flow control status has been successfully get.
 * @retval -RT_ERR_PORT_ID        The port is invalid.
 * @retval -RT_ERR_ENABLE         The enable (en) value is invalid.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others Please refer to dva_drv_flow_ctrl_ra_bypass_chk(), dva_reg_read() and dva_reg_field_get()
 */
RtkApiRet dva_flow_ctrl_en_get(uint32 unit, dva_RtkPort port, dva_RtkEnable *pEn)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    asyPauseStatus = 0u;
    uint32    pauseStatus    = 0u;
    uint32    regAddr        = 0u;
    uint32    regVal         = 0u;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pEn)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Calculate the register address */
        regAddr = DVA_PORT0_PROPERTY_CONFIGURE + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]);
    }

    /* Check if RA bypass is disabled */
    CHK_FUN_RET(retVal, dva_drv_flow_ctrl_ra_bypass_chk(unit, port));
    /* Convert flow control status to enable */
    if(RT_ERR_OK == retVal)
    {
        CHK_FUN_RET(retVal, dva_reg_read(unit, regAddr, &regVal));
        /* tx pause en */
        CHK_FUN_RET(retVal, dva_reg_field_get(regAddr, DVA_PORT0_PROPERTY_CONFIGURE_EN_PAUSE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_PAUSE_LEN, &pauseStatus, regVal));
        /* recognize rx pause en */
        CHK_FUN_RET(retVal, dva_reg_field_get(regAddr, DVA_PORT0_PROPERTY_CONFIGURE_EN_ASY_PAUSE_LSP, DVA_PORT0_PROPERTY_CONFIGURE_EN_ASY_PAUSE_LEN, &asyPauseStatus, regVal));

        if((1u == pauseStatus) && (1u == asyPauseStatus))
        {
            *pEn = DVA_ENABLED;
        }
        else
        {
            *pEn = DVA_DISABLED;
        }
    }

    return retVal;
}
/**@}*/ /* FLOWCTRL_EN_ASIC */
/**@}*/ /* FLOWCTRL_EN */

/**
 * @addtogroup FLOWCTRL_PAGE_CNT Flow Control page count
 * @{
 */

/**
 * @defgroup FLOWCTRL_PAGE_CNT_ASIC Low Level Driver
 * @brief FLOWCTRL Low Level Driver
 * @{
 */

/**
 * @brief Set the page count for a specific queue of a specific port.
 * @param[in] unit        Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port        Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
                          Port ID
 * @param[in] que         Queue ID (0 to QOS_QUE_MAX) \n
                          Queue ID
 * @param[in] pageCnt     Page Count (0 to DVA_FLOWCTRL_PAGE_CNT_MAX) \n
                          Page Count
 * @param[out] None
 * @retval RT_ERR_OK                   The flow control page count has been successfully set.
 * @retval -RT_ERR_PORT_ID             The port is invalid.
 * @retval -RT_ERR_QUEUE_ID            The queue is invalid.
 * @retval -RT_ERR_FLOWCTRL_PAGE_CNT   The page count is invalid.
 * @retval Others Please refer to dva_reg_field_write()
 */
RtkApiRet dva_flow_ctrl_page_cnt_set(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_FlowctrlPageCnt pageCnt)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0x0u;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(pageCnt > DVA_FLOWCTRL_PAGE_CNT_MAX)
    {
        retVal = -RT_ERR_FLOWCTRL_PAGE_CNT;
    }
    else
    {
        regAddr = DVA_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0 + (DVA_FLOWCTRL_REG_OFFSET * (que / DVA_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)) + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]);
    }

    /* Set page count */
    CHK_FUN_RET(retVal, dva_reg_field_write(unit,
                                            regAddr,
                                            DVA_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LSP + (DVA_FLOWCTRL_PAGE_CNT_LSP_OFFSET1 * (que % DVA_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)),
                                            DVA_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LEN,
                                            pageCnt));

    return retVal;
}

/**
 * @brief Get the page count for a specific queue of a specific port.
 * @param[in] unit          Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port          Port ID (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
                            Port ID
 * @param[in] que           Queue ID (0 to QOS_QUE_MAX) \n
 *                          Queue ID
 * @param[out] pPageCnt     Page Count (N/A) \n
                            Page Count
 * @retval RT_ERR_OK                   The flow control page count has been successfully get.
 * @retval -RT_ERR_PORT_ID             The port is invalid.
 * @retval -RT_ERR_QUEUE_ID            The queue is invalid.
 * @retval -RT_ERR_NULL_POINTER        The input parameter is NULL.
 * @retval Others Please refer to dva_reg_field_read()
 */
RtkApiRet dva_flow_ctrl_page_cnt_get(uint32 unit, dva_RtkPort port, dva_QosQueNum que, dva_FlowctrlPageCnt *pPageCnt)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0u;

    if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(NULL == pPageCnt)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        regAddr = DVA_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0 + (DVA_FLOWCTRL_REG_OFFSET * (que / DVA_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)) + (PERPORT_REG_OFFSET * (uint32)dva_g_userPort2sysPort[port]);
    }

    /* Get page count */
    CHK_FUN_RET(retVal, dva_reg_field_read(unit,
                                           regAddr,
                                           DVA_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LSP + (DVA_FLOWCTRL_PAGE_CNT_LSP_OFFSET1 * (que % DVA_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)),
                                           DVA_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LEN,
                                           pPageCnt));

    return retVal;
}
/**@}*/ /* FLOWCTRL_PAGE_CNT_ASIC*/
/**@}*/ /* FLOWCTRL_PAGE_CNT */

/**@}*/ /* Flow Control */
