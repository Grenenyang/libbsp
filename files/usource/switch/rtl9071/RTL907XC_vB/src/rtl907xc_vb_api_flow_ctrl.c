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

#include "rtl907xc_vb_api.h"
#include "rtl907xc_vb_api_qos.h"
#include "rtl907xc_vb_api_flow_ctrl.h"
#include "rtl907xc_vb_asicdrv_flow_ctrl.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
#include "rtl907xc_vb_reg_list.h"
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_regField_list.h"

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
 * @param[in] port     Port ID (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
                       Port ID
 * @param[in] en       enable (CVB_DISABLED to CVB_ENABLED) \n
                       Flow Control status
 * @retval RT_ERR_OK         The flow control status has been successfully set.
 * @retval -RT_ERR_PORT_ID   The port is invalid.
 * @retval -RT_ERR_ENABLE    The enable (en) value is invalid.
 * @retval Others Please refer to cvb_drv_flow_ctrl_ra_bypass_chk(), cvb_drv_indirect_reg_field_set() and cvb_drv_flow_ctrl_global_set()
 */
RtkApiRet cvb_flow_ctrl_en_set(uint32 unit, cvb_RtkPort port, cvb_RtkEnable en)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0u;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(en >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* do nothing */
    }

    /* Check if RA bypass is disabled */
    CHK_FUN_RET(retVal, cvb_drv_flow_ctrl_ra_bypass_chk(unit, port));

    /* Set flow control enable */
    if(cvb_g_userPort2sysPort[port] <= CVB_FLOWCTRL_STATUS_REG_PORTA_MAX)
    {
        /* System Port 0 to Port 8 */
        regAddr = CVB_PORT0_PROPERTY_CONFIGURE + (CVB_FLOWCTRL_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]);
    }
    else if(cvb_g_userPort2sysPort[port] <= CVB_FLOWCTRL_STATUS_REG_PORTB_MAX)
    {
        /* System Port 9 to Port 12 */
        regAddr = CVB_PORT9_PROPERTY_CONFIGURE + (CVB_FLOWCTRL_REG_OFFSET * ((uint32)cvb_g_userPort2sysPort[port] - CVB_FLOWCTRL_STATUS_REG_PORTB_MIN));
    }
    else if(cvb_g_userPort2sysPort[port] <= CVB_FLOWCTRL_STATUS_REG_PORTC_MAX)
    {
        /* System Port 13 to Port 15 */
        regAddr = CVB_PORT13_PROPERTY_CONFIGURE + (CVB_FLOWCTRL_REG_OFFSET * ((uint32)cvb_g_userPort2sysPort[port] - CVB_FLOWCTRL_STATUS_REG_PORTC_MIN));
    }
    else
    {
        /* do nothing */
    }

    /* Set TX/RX pause of MAC */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, regAddr, CVB_PORT0_PROPERTY_CONFIGURE_P0_EN_PAUSE_LSP, CVB_PORT0_PROPERTY_CONFIGURE_P0_EN_PAUSE_LEN, en));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, regAddr, CVB_PORT0_PROPERTY_CONFIGURE_P0_EN_ASY_PAUSE_LSP, CVB_PORT0_PROPERTY_CONFIGURE_P0_EN_ASY_PAUSE_LEN, en));
    /* Get global TX/RX pause of RA module */
    CHK_FUN_RET(retVal, cvb_drv_flow_ctrl_global_set(unit, port, en));

    return retVal;
}

/**
 * @brief Get Flow Control status.
 * @param[in] unit      Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port      Port ID (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
                        Port ID
 * @param[out] pEn      enable (N/A) \n
                        Flow Control status
 * @retval RT_ERR_OK              The flow control status has been successfully get.
 * @retval -RT_ERR_PORT_ID        The port is invalid.
 * @retval -RT_ERR_ENABLE         The enable (en) value is invalid.
 * @retval -RT_ERR_NULL_POINTER   The input parameter is NULL.
 * @retval Others Please refer to cvb_drv_flow_ctrl_ra_bypass_chk(), cvb_reg_read() and cvb_reg_field_get()
 */
RtkApiRet cvb_flow_ctrl_en_get(uint32 unit, cvb_RtkPort port, cvb_RtkEnable *pEn)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    asyPauseStatus = 0u;
    uint32    pauseStatus    = 0u;
    uint32    regAddr        = 0u;
    uint32    regVal         = 0u;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
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
    CHK_FUN_RET(retVal, cvb_drv_flow_ctrl_ra_bypass_chk(unit, port));

    /* Get flow control status */
    if(cvb_g_userPort2sysPort[port] <= CVB_FLOWCTRL_STATUS_REG_PORTA_MAX)
    {
        /* System Port 0 to Port 8 */
        regAddr = CVB_PORT0_PROPERTY_CONFIGURE + (CVB_FLOWCTRL_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]);
    }
    else if(cvb_g_userPort2sysPort[port] <= CVB_FLOWCTRL_STATUS_REG_PORTB_MAX)
    {
        /* System Port 9 to Port 12 */
        regAddr = CVB_PORT9_PROPERTY_CONFIGURE + (CVB_FLOWCTRL_REG_OFFSET * ((uint32)cvb_g_userPort2sysPort[port] - CVB_FLOWCTRL_STATUS_REG_PORTB_MIN));
    }
    else if(cvb_g_userPort2sysPort[port] <= CVB_FLOWCTRL_STATUS_REG_PORTC_MAX)
    {
        /* System Port 13 to Port 15 */
        regAddr = CVB_PORT13_PROPERTY_CONFIGURE + (CVB_FLOWCTRL_REG_OFFSET * ((uint32)cvb_g_userPort2sysPort[port] - CVB_FLOWCTRL_STATUS_REG_PORTC_MIN));
    }
    else
    {
        /* do nothing */
    }

    /* Convert flow control status to enable */
    if(RT_ERR_OK == retVal)
    {
        CHK_FUN_RET(retVal, cvb_reg_read(unit, regAddr, &regVal));
        /* tx pause en */
        CHK_FUN_RET(retVal, cvb_reg_field_get(regAddr, CVB_PORT0_PROPERTY_CONFIGURE_P0_EN_PAUSE_LSP, CVB_PORT0_PROPERTY_CONFIGURE_P0_EN_PAUSE_LEN, &pauseStatus, regVal));
        /* recognize rx pause en */
        CHK_FUN_RET(retVal, cvb_reg_field_get(regAddr, CVB_PORT0_PROPERTY_CONFIGURE_P0_EN_ASY_PAUSE_LSP, CVB_PORT0_PROPERTY_CONFIGURE_P0_EN_ASY_PAUSE_LEN, &asyPauseStatus, regVal));

        if((1u == pauseStatus) && (1u == asyPauseStatus))
        {
            *pEn = CVB_ENABLED;
        }
        else
        {
            *pEn = CVB_DISABLED;
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
 * @param[in] port        Port ID (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
                          Port ID
 * @param[in] que         Queue ID (0 to QOS_QUE_MAX) \n
                          Queue ID
 * @param[in] pageCnt     Page Count (0 to CVB_FLOWCTRL_PAGE_CNT_MAX) \n
                          Page Count
 * @param[out] None
 * @retval RT_ERR_OK                   The flow control page count has been successfully set.
 * @retval -RT_ERR_PORT_ID             The port is invalid.
 * @retval -RT_ERR_QUEUE_ID            The queue is invalid.
 * @retval -RT_ERR_FLOWCTRL_PAGE_CNT   The page count is invalid.
 * @retval Others Please refer to cvb_reg_field_write()
 */
RtkApiRet cvb_flow_ctrl_page_cnt_set(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_FlowctrlPageCnt pageCnt)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0x0u;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(que > QOS_QUE_MAX)
    {
        retVal = -RT_ERR_QUEUE_ID;
    }
    else if(pageCnt > CVB_FLOWCTRL_PAGE_CNT_MAX)
    {
        retVal = -RT_ERR_FLOWCTRL_PAGE_CNT;
    }
    else
    {
        regAddr = CVB_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0 + (CVB_FLOWCTRL_REG_OFFSET * (que / CVB_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)) + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]);
    }

    /* Set page count */
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit,
                                            regAddr,
                                            CVB_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LSP + (CVB_FLOWCTRL_PAGE_CNT_LSP_OFFSET1 * (que % CVB_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)),
                                            CVB_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LEN,
                                            pageCnt));

    return retVal;
}

/**
 * @brief Get the page count for a specific queue of a specific port.
 * @param[in] unit          Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port          Port ID (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
                            Port ID
 * @param[in] que           Queue ID (0 to QOS_QUE_MAX) \n
 *                          Queue ID
 * @param[out] pPageCnt     Page Count (N/A) \n
 *                          Page Count
 * @retval RT_ERR_OK                   The flow control page count has been successfully get.
 * @retval -RT_ERR_PORT_ID             The port is invalid.
 * @retval -RT_ERR_QUEUE_ID            The queue is invalid.
 * @retval -RT_ERR_NULL_POINTER        The input parameter is NULL.
 * @retval Others Please refer to cvb_reg_field_read()
 */
RtkApiRet cvb_flow_ctrl_page_cnt_get(uint32 unit, cvb_RtkPort port, cvb_QosQueNum que, cvb_FlowctrlPageCnt *pPageCnt)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0u;

    if((port < CVB_RTK_USER_PORT_1ST) || (port > CVB_RTK_USER_PORT_END))
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
        regAddr = CVB_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0 + (CVB_FLOWCTRL_REG_OFFSET * (que / CVB_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)) + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]);
    }

    /* Get page count */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit,
                                           regAddr,
                                           CVB_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LSP + (CVB_FLOWCTRL_PAGE_CNT_LSP_OFFSET1 * (que % CVB_FLOWCTRL_PAGE_CNT_LSP_OFFSET2)),
                                           CVB_PORT0_PAGE_COUNT_LIMIT_FOR_TX_QUEUE_CONTORL0_TXQ0MAXPAGECNT_LEN,
                                           pPageCnt));

    return retVal;
}
/**@}*/ /* FLOWCTRL_PAGE_CNT_ASIC*/
/**@}*/ /* FLOWCTRL_PAGE_CNT */

/**@}*/ /* Flow Control */
