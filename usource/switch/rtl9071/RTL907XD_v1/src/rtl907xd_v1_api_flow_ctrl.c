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

#include "rtl907xd_v1_api.h"
#include "rtl907xd_v1_api_qos.h"
#include "rtl907xd_v1_api_flow_ctrl.h"
#include "rtl907xd_v1_asicdrv_flow_ctrl.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_regField_list.h"

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
 * @param[in] port     Port ID (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
                       Port ID
 * @param[in] en       Enable (DV1_DISABLED to DV1_ENABLED) \n
                       Flow Control status
 * @retval RT_ERR_OK         The flow control status has been successfully set.
 * @retval -RT_ERR_PORT_ID   The port is invalid.
 * @retval -RT_ERR_ENABLE    The enable (en) value is invalid.
 * @retval Others Please refer to dv1_drv_flow_ctrl_ra_bypass_chk(), dv1_drv_indirect_reg_field_set() and dv1_drv_flow_ctrl_global_set()
 */
RtkApiRet dv1_flow_ctrl_en_set(uint32 unit, dv1_RtkPort port, dv1_RtkEnable en)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0u;

    if((port < DV1_RTK_USER_PORT_1ST) || (port > DV1_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(en >= DV1_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* do nothing */
    }

    /* Check if RA bypass is disabled */
    CHK_FUN_RET(retVal, dv1_drv_flow_ctrl_ra_bypass_chk(unit, port));

    /* Set flow control enable */
    if(dv1_g_userPort2sysPort[port] <= DV1_FLOWCTRL_STATUS_REG_PORTA_MAX)
    {
        /* System Port 0 to Port 8 */
        regAddr = DV1_PORT0_PROPERTY_CONFIGURE + (DV1_FLOWCTRL_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]);
    }
    else if(dv1_g_userPort2sysPort[port] <= DV1_FLOWCTRL_STATUS_REG_PORTB_MAX)
    {
        /* System Port 9 to Port 12 */
        regAddr = DV1_PORT9_PROPERTY_CONFIGURE + (DV1_FLOWCTRL_REG_OFFSET * ((uint32)dv1_g_userPort2sysPort[port] - DV1_FLOWCTRL_STATUS_REG_PORTB_MIN));
    }
    else if(dv1_g_userPort2sysPort[port] <= DV1_FLOWCTRL_STATUS_REG_PORTC_MAX)
    {
        /* System Port 13 to Port 15 */
        regAddr = DV1_PORT13_PROPERTY_CONFIGURE + (DV1_FLOWCTRL_REG_OFFSET * ((uint32)dv1_g_userPort2sysPort[port] - DV1_FLOWCTRL_STATUS_REG_PORTC_MIN));
    }
    else
    {
        /* do nothing */
    }

    /* Set TX/RX pause of MAC */
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, regAddr, DV1_PORT0_PROPERTY_CONFIGURE_P0_EN_PAUSE_LSP, DV1_PORT0_PROPERTY_CONFIGURE_P0_EN_PAUSE_LEN, en));
    CHK_FUN_RET(retVal, dv1_drv_indirect_reg_field_set(unit, regAddr, DV1_PORT0_PROPERTY_CONFIGURE_P0_EN_ASY_PAUSE_LSP, DV1_PORT0_PROPERTY_CONFIGURE_P0_EN_ASY_PAUSE_LEN, en));
    /* Get global TX/RX pause of RA module */
    CHK_FUN_RET(retVal, dv1_drv_flow_ctrl_global_set(unit, port, en));

    return retVal;
}

/**
 * @brief Get Flow Control status.
 * @param[in] unit      Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
                        Port ID
 * @param[out] pEn      Enable \n
                        Flow Control status
 * @retval RT_ERR_OK              The flow control status has been successfully get.
 * @retval -RT_ERR_PORT_ID        The port is invalid.
 * @retval -RT_ERR_ENABLE         The enable (en) value is invalid.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others Please refer to dv1_drv_flow_ctrl_ra_bypass_chk(), dv1_reg_read() and dv1_reg_field_get()
 */
RtkApiRet dv1_flow_ctrl_en_get(uint32 unit, dv1_RtkPort port, dv1_RtkEnable *pEn)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    asyPauseStatus = 0u;
    uint32    pauseStatus    = 0u;
    uint32    regAddr        = 0u;
    uint32    regVal         = 0u;

    if((port < DV1_RTK_USER_PORT_1ST) || (port > DV1_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pEn)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    /* Check if RA bypass is disabled */
    CHK_FUN_RET(retVal, dv1_drv_flow_ctrl_ra_bypass_chk(unit, port));

    /* Get flow control status */
    if(dv1_g_userPort2sysPort[port] <= DV1_FLOWCTRL_STATUS_REG_PORTA_MAX)
    {
        /* System Port 0 to Port 8 */
        regAddr = DV1_PORT0_PROPERTY_CONFIGURE + (DV1_FLOWCTRL_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]);
    }
    else if(dv1_g_userPort2sysPort[port] <= DV1_FLOWCTRL_STATUS_REG_PORTB_MAX)
    {
        /* System Port 9 to Port 12 */
        regAddr = DV1_PORT9_PROPERTY_CONFIGURE + (DV1_FLOWCTRL_REG_OFFSET * ((uint32)dv1_g_userPort2sysPort[port] - DV1_FLOWCTRL_STATUS_REG_PORTB_MIN));
    }
    else if(dv1_g_userPort2sysPort[port] <= DV1_FLOWCTRL_STATUS_REG_PORTC_MAX)
    {
        /* System Port 13 to Port 15 */
        regAddr = DV1_PORT13_PROPERTY_CONFIGURE + (DV1_FLOWCTRL_REG_OFFSET * ((uint32)dv1_g_userPort2sysPort[port] - DV1_FLOWCTRL_STATUS_REG_PORTC_MIN));
    }
    else
    {
        /* do nothing */
    }

    /* Convert flow control status to enable */
    if(RT_ERR_OK == retVal)
    {
        CHK_FUN_RET(retVal, dv1_reg_read(unit, regAddr, &regVal));
        /* tx pause en */
        CHK_FUN_RET(retVal, dv1_reg_field_get(regAddr, DV1_PORT0_PROPERTY_CONFIGURE_P0_EN_PAUSE_LSP, DV1_PORT0_PROPERTY_CONFIGURE_P0_EN_PAUSE_LEN, &pauseStatus, regVal));
        /* recognize rx pause en */
        CHK_FUN_RET(retVal, dv1_reg_field_get(regAddr, DV1_PORT0_PROPERTY_CONFIGURE_P0_EN_ASY_PAUSE_LSP, DV1_PORT0_PROPERTY_CONFIGURE_P0_EN_ASY_PAUSE_LEN, &asyPauseStatus, regVal));

        if((1u == pauseStatus) && (1u == asyPauseStatus))
        {
            *pEn = DV1_ENABLED;
        }
        else
        {
            *pEn = DV1_DISABLED;
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
 * @param[in] port        Port ID (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
                          Port ID
 * @param[in] que         Queue ID (0 to QOS_QUE_MAX) \n
                          Queue ID
 * @param[in] pageCnt     Page Count (0 to DV1_FLOWCTRL_PAGE_CNT_MAX) \n
                          Page Count
 * @param[out] None
 * @retval RT_ERR_OK                   The flow control page count has been successfully set.
 * @retval -RT_ERR_PORT_ID             The port is invalid.
 * @retval -RT_ERR_QUEUE_ID            The queue is invalid.
 * @retval -RT_ERR_FLOWCTRL_PAGE_CNT   The page count is invalid.
 * @retval Others Please refer to dv1_reg_field_write()
 */
RtkApiRet dv1_flow_ctrl_page_cnt_set(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_FlowctrlPageCnt pageCnt)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0x0u;

    if((port < DV1_RTK_USER_PORT_1ST) || (port > DV1_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(pageCnt > DV1_FLOWCTRL_PAGE_CNT_MAX)
    {
        retVal = -RT_ERR_FLOWCTRL_PAGE_CNT;
    }
    else
    {
        regAddr = DV1_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0 + (DV1_FLOWCTRL_REG_OFFSET * (que / DV1_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)) + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]);
    }

    /* Set page count */
    CHK_FUN_RET(retVal, dv1_reg_field_write(unit,
                                            regAddr,
                                            DV1_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LSP + (DV1_FLOWCTRL_PAGE_CNT_LSP_OFFSET1 * (que % DV1_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)),
                                            DV1_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LEN,
                                            pageCnt));

    return retVal;
}

/**
 * @brief Get the page count for a specific queue of a specific port.
 * @param[in] unit          Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port          Port ID (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
                            Port ID
 * @param[in] que           Queue ID (0 to QOS_QUE_MAX) \n
 *                          Queue ID
 * @param[out] pPageCnt     Page Count (N/A) \n
 *                          Page Count
 * @retval RT_ERR_OK                   The flow control page count has been successfully get.
 * @retval -RT_ERR_PORT_ID             The port is invalid.
 * @retval -RT_ERR_QUEUE_ID            The queue is invalid.
 * @retval -RT_ERR_NULL_POINTER        The input parameter is NULL.
 * @retval Others Please refer to dv1_reg_field_read()
 */
RtkApiRet dv1_flow_ctrl_page_cnt_get(uint32 unit, dv1_RtkPort port, dv1_QosQueNum que, dv1_FlowctrlPageCnt *pPageCnt)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0u;

    if((port < DV1_RTK_USER_PORT_1ST) || (port > DV1_RTK_USER_PORT_END))
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
        regAddr = DV1_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0 + (DV1_FLOWCTRL_REG_OFFSET * (que / DV1_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)) + (PERPORT_REG_OFFSET * (uint32)dv1_g_userPort2sysPort[port]);
    }

    /* Get page count */
    CHK_FUN_RET(retVal, dv1_reg_field_read(unit,
                                           regAddr,
                                           DV1_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LSP + (DV1_FLOWCTRL_PAGE_CNT_LSP_OFFSET1 * (que % DV1_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)),
                                           DV1_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LEN,
                                           pPageCnt));

    return retVal;
}
/**@}*/ /* FLOWCTRL_PAGE_CNT_ASIC*/
/**@}*/ /* FLOWCTRL_PAGE_CNT */

/**@}*/ /* Flow Control */
