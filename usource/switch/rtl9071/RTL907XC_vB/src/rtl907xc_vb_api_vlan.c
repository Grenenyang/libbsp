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

#include "rtl907xc_vb_asicdrv_vlan.h"
#include "rtl907xc_vb_asicdrv_svlan.h"
#include "rtl907xc_vb_asicdrv.h"
#include "rtl907xc_vb_api_ext.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"

/**
 * @addtogroup RTK_MODULE_VLAN VLAN
 * @{
 */
/**
 * @addtogroup VLAN_TAG_STATUS_FILTER VLAN Tag Status Filter
 * @{
 */
/**
 * @defgroup VLAN_TAG_STATUS_FILTER_ASIC Low Level Driver
 * @brief VLAN Low Level Driver
 * @{
 */
/**
 * @brief Configure acceptable inner tag frame types for a specific port.
 * @param[in]  unit            Stacking unit (1 to 7) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port            Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                             The physical port number of the switch.
 * @param[in]  acceptFrameType Acceptable frame type (N/A) \n
 *                             Acceptable frame type used to receive packets with the expected VLAN tag status.
 * @param[out] None
 * @retval RT_ERR_OK                      The acceptable frame type of the port was set successfully.
 * @retval -RT_ERR_PORT_ID                The "port" is out of range.
 * @retval -RT_ERR_VLAN_ACCEPT_FRAME_TYPE The "acceptFrameType" is out of range.
 * @retval Others                         Please refer to cvb_drv_vlan_port_acpt_type_set().
 */
RtkApiRet cvb_vlan_port_acpt_type_set(uint32 unit, RtkPort port, VlanAcceptFrameType_e acceptFrameType)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(acceptFrameType >= EM_ACCEPT_FRAME_TYPE_END)
    {
        retVal = -RT_ERR_VLAN_ACCEPT_FRAME_TYPE;
    }
    else
    {
        /* Do nothing */
    }

    /* Set VLAN supported frame type */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_acpt_type_set(unit, port, acceptFrameType));

    return retVal;
}

/**
 * @brief Get acceptable inner tag frame types for a specific port.
 * @param[in]  unit             Stacking unit (1 to 7) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port             Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                              The physical port number of the switch.
 * @param[out] pAcceptFrameType Acceptable frame type (N/A) \n
 *                              Acceptable frame type used to receive packets with the expected VLAN tag status.
 * @retval RT_ERR_OK            The acceptable frame type of the port was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pAcceptFrameType" is NULL.
 * @retval Others               Please refer to cvb_drv_vlan_port_acpt_type_get().
 */
RtkApiRet cvb_vlan_port_acpt_type_get(uint32 unit, RtkPort port, VlanAcceptFrameType_e *pAcceptFrameType)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pAcceptFrameType)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get VLAN supported frame type */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_acpt_type_get(unit, port, pAcceptFrameType));

    return retVal;
}

/**
 * @brief Configure acceptable outer tag frame types for a specific port.
 * @param[in]  unit            Stacking unit (1 to 7) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port            Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                             The physical port number of the switch.
 * @param[in]  acceptFrameType Acceptable frame type (N/A) \n
 *                             Acceptable frame type used to receive packets with the expected VLAN tag status.
 * @param[out] None
 * @retval RT_ERR_OK                      The acceptable frame type of the port was set successfully.
 * @retval -RT_ERR_PORT_ID                The "port" is out of range.
 * @retval -RT_ERR_VLAN_ACCEPT_FRAME_TYPE The "acceptFrameType" is out of range.
 * @retval Others                         Please refer to cvb_drv_svlan_port_acpt_type_set().
 */
RtkApiRet cvb_svlan_port_acpt_type_set(uint32 unit, RtkPort port, VlanAcceptFrameType_e acceptFrameType)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(acceptFrameType >= EM_ACCEPT_FRAME_TYPE_END)
    {
        retVal = -RT_ERR_VLAN_ACCEPT_FRAME_TYPE;
    }
    else
    {
        /* Do nothing */
    }

    /* Set SVLAN supported frame type */
    CHK_FUN_RET(retVal, cvb_drv_svlan_port_acpt_type_set(unit, port, acceptFrameType));

    return retVal;
}

/**
 * @brief Get acceptable outer tag frame types for a specific port.
 * @param[in]  unit             Stacking unit (1 to 7) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port             Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                              The physical port number of the switch.
 * @param[out] pAcceptFrameType Acceptable frame type (N/A) \n
 *                              Acceptable frame type used to receive packets with the expected VLAN tag status.
 * @retval RT_ERR_OK            The acceptable frame type of the port was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pAcceptFrameType" is NULL.
 * @retval Others               Please refer to cvb_drv_svlan_port_acpt_type_get().
 */
RtkApiRet cvb_svlan_port_acpt_type_get(uint32 unit, RtkPort port, VlanAcceptFrameType_e *pAcceptFrameType)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pAcceptFrameType)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get SVLAN supported frame type */
    CHK_FUN_RET(retVal, cvb_drv_svlan_port_acpt_type_get(unit, port, pAcceptFrameType));

    return retVal;
}

/**
 * @brief Configure whether to accept double tag for a specific port.
 * @param[in]  unit            Stacking unit (1 to 7) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port            Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                             The physical port number of the switch.
 * @param[in]  acceptDoubleTag Accept double tag (EM_ACCEPT_DOUBLE_TAG to EM_DENY_DOUBLE_TAG) \n
 *                             Accept frame type indicates whether to permit incoming packets with double tag to enter the switch.
 * @param[out] None
 * @retval RT_ERR_OK     The acceptable double tag type of the port was set successfully.
 * @retval -RT_ERR_INPUT The "acceptDoubleTag" was out of range. \n
 *                       The invalid register access channel was passed into the register access function.
 * @retval Others        Please refer to cvb_drv_vlan_check_valid_user_port(), cvb_drv_indirect_reg_field_get(), cvb_reg_field_set(), cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_vlan_port_acpt_double_tag_set(uint32 unit, RtkPort port, VlanAcceptDoubleTag_e acceptDoubleTag)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    /* Check if the input parameter "port" is valid */
    retVal = cvb_drv_vlan_check_valid_user_port(unit, port);

    /* Set double tag type for the specific port */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    if(RT_ERR_OK == retVal)
    {
        switch(acceptDoubleTag)
        {
            case EM_ACCEPT_DOUBLE_TAG:
                retVal = cvb_reg_field_set(CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTDOUBLETAG_LSP, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTDOUBLETAG_LEN, 1u, &regVal);
                break;
            case EM_DENY_DOUBLE_TAG:
                retVal = cvb_reg_field_set(CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTDOUBLETAG_LSP, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTDOUBLETAG_LEN, 0u, &regVal);
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, regVal));

    return retVal;
}

/**
 * @brief Get acceptable double tag for a specific port.
 * @param[in]  unit             Stacking unit (1 to 7) \n
 *                              Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port             Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                              The physical port number of the switch.
 * @param[out] pAcceptDoubleTag Accept double tag (N/A) \n
 *                              Accept frame type indicates whether to permit incoming packets with double tag to enter the switch.
 * @retval RT_ERR_OK     The acceptable double tag type of the port was retrieved successfully.
 * @retval -RT_ERR_INPUT The "pAcceptDoubleTag" was out of range. \n
 *                       The invalid register access channel was passed into the register access function.
 * @retval Others        Please refer to cvb_drv_vlan_check_valid_user_port(), cvb_drv_indirect_reg_field_get(), cvb_reg_field_get().
 */
RtkApiRet cvb_vlan_port_acpt_double_tag_get(uint32 unit, RtkPort port, VlanAcceptDoubleTag_e *pAcceptDoubleTag)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    uint32    acpdtag = 0u;

    /* Check if the input parameter "port" is valid */
    retVal = cvb_drv_vlan_check_valid_user_port(unit, port);

    /* Get double tag type for the specific port */
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTDOUBLETAG_LSP, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTDOUBLETAG_LEN, &acpdtag, regVal));

    if(RT_ERR_OK == retVal)
    {
        switch(acpdtag)
        {
            case 0:
                *pAcceptDoubleTag = EM_DENY_DOUBLE_TAG;
                break;
            case 1u:
                *pAcceptDoubleTag = EM_ACCEPT_DOUBLE_TAG;
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }

    return retVal;
}
/**@}*/ /*VLAN_TAG_STATUS_FILTER_ASIC*/
/**@}*/ /*VLAN_TAG_STATUS_FILTER*/

/**
 * @addtogroup VLAN_ASSIGNMENT VLAN Assignment
 * @{
 */
/**
 * @defgroup VLAN_ASSIGNMENT_ASIC Low Level Driver
 * @brief VLAN Low Level Driver
 * @{
 */
/**
 * @brief Configure the switch not to ignore VLAN tags of incoming packets for a specific port.
 * @param[in]  unit    Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port    Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                     The physical port number of the switch.
 * @param[in]  enabled Disable the ignoring VLAN tag function (CVB_DISABLED to CVB_ENABLED) \n
 *                     The ignoring VLAN tag function indicates whether to force the port-based VLAN instead of the tag-based VLAN to perform forwarding within the switch.
 * @param[out] None
 * @retval RT_ERR_OK       The tag-aware function of the port was set successfully.
 * @retval -RT_ERR_PORT_ID The "port" is out of range.
 * @retval -RT_ERR_ENABLE  The "enabled" was out of range. \n
 *                         The invalid register access channel was passed into the register access function.
 * @retval Others          Please refer to cvb_drv_vlan_port_tag_aware_set().
 */
RtkApiRet cvb_vlan_tag_aware_set(uint32 unit, RtkPort port, cvb_RtkEnable enabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(enabled >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* Do nothing */
    }

    /* Enable or disable VLAN tag aware function for the specific port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_tag_aware_set(unit, port, (uint32)enabled));

    return retVal;
}

/**
 * @brief Get the configuration of the ignoring VLAN tag function for a specific port.
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port     Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                      The physical port number of the switch.
 * @param[out] pEnabled Disable the ignoring VLAN tag function (N/A) \n
 *                      The ignoring VLAN tag function indicates whether to force the port-based VLAN instead of the tag-based VLAN to perform forwarding within the switch.
 * @retval RT_ERR_OK            The tag-aware function of the port was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pEnabled" is NULL.
 * @retval Others               Please refer to cvb_drv_vlan_port_tag_aware_get().
 */
RtkApiRet cvb_vlan_tag_aware_get(uint32 unit, RtkPort port, cvb_RtkEnable *pEnabled)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    enabled = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pEnabled)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get the status VLAN tag aware function for the specific port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_tag_aware_get(unit, port, &enabled));

    if(RT_ERR_OK == retVal)
    {
        if((uint32)CVB_ENABLED == enabled)
        {
            *pEnabled = CVB_ENABLED;
        }
        else if((uint32)CVB_DISABLED == enabled)
        {
            *pEnabled = CVB_DISABLED;
        }
        else
        {
            /* Do nothing */
        }
    }

    return retVal;
}

/**
 * @brief Configure a default VLAN ID and priority of the inner VLAN tag for a specific port.
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port     Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                      The physical port number of the switch.
 * @param[in]  pvid     IEEE 802.1Q Port-based VLAN ID (1 to 0xFFFu) \n
 *                      The VLAN ID for a specific port.
 * @param[in]  priority IEEE 802.1p VLAN priority for the "pvid" (0 to 7) \n
 *                      The VLAN priority for a specific port.
 * @param[out] None
 * @retval RT_ERR_OK             The inner port-based VLAN was set successfully.
 * @retval -RT_ERR_PORT_ID       The "port" is out of range.
 * @retval -RT_ERR_VLAN_VID      The "pvid" was out of range.
 * @retval -RT_ERR_VLAN_PRIORITY The "priority" was out of range.
 * @retval Others                Please refer to cvb_drv_vlan_port_pvid_set().
 */
RtkApiRet cvb_vlan_port_pvid_set(uint32 unit, RtkPort port, cvb_RtkVlan pvid, cvb_VlanPri priority)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if((pvid < 1u) || (pvid > CVB_VLAN_VID_MAX))
    {
        retVal = -RT_ERR_VLAN_VID;
    }
    else if(priority > CVB_VLAN_PRI_MAX)
    {
        retVal = -RT_ERR_VLAN_PRIORITY;
    }
    else
    {
        /* Do nothing */
    }

    /* Set the inner pvid of the port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_pvid_set(unit, port, pvid, priority));

    return retVal;
}

/**
 * @brief Get the default VLAN ID and priority of the inner VLAN tag for a specific port.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                       The physical port number of the switch.
 * @param[out] pPvid     IEEE 802.1Q Port-based VLAN ID (N/A) \n
 *                       The VLAN ID for a specific port.
 * @param[out] pPriority IEEE 802.1p VLAN priority for the "pvid" (N/A) \n
 *                       The VLAN priority for a specific port.
 * @retval RT_ERR_OK            The inner port-based VLAN was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pPvid" or "pPriority" is NULL.
 * @retval Others               Please refer to cvb_drv_vlan_port_pvid_get().
 */
RtkApiRet cvb_vlan_port_pvid_get(uint32 unit, RtkPort port, cvb_RtkVlan *pPvid, cvb_VlanPri *pPriority)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if((NULL == pPvid) || (NULL == pPriority))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get the inner pvid of the port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_pvid_get(unit, port, pPvid, pPriority));

    return retVal;
}

/**
 * @brief Configure a default VLAN ID and priority of the outer VLAN tag for a specific port.
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port     Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                      The physical port number of the switch.
 * @param[in]  pvid     IEEE 802.1Q Port-based VLAN ID (1 to 0xFFFu) \n
 *                      The VLAN ID for a specific port.
 * @param[in]  priority IEEE 802.1p VLAN priority for the "pvid" (0 to 7) \n
 *                      The VLAN priority for a specific port.
 * @param[in]  dei      IEEE 802.1Q Drop eligible indicator (0 to 1) \n
 *                      The VLAN DEI for a specific port.
 * @param[out] None
 * @retval RT_ERR_OK             The outer port-based VLAN was set successfully.
 * @retval -RT_ERR_PORT_ID       The "port" is out of range.
 * @retval -RT_ERR_VLAN_VID      The "pvid" was out of range.
 * @retval -RT_ERR_INPUT         The "priority" was out of range.
 * @retval -RT_ERR_VLAN_PRIORITY The "dei" was out of range.
 * @retval Others                Please refer to cvb_drv_svlan_port_pvid_set().
 */
RtkApiRet cvb_svlan_port_pvid_set(uint32 unit, RtkPort port, cvb_RtkVlan pvid, cvb_VlanPri priority, cvb_VlanDei dei)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if((pvid < 1u) || (pvid > CVB_VLAN_VID_MAX))
    {
        retVal = -RT_ERR_VLAN_VID;
    }
    else if(priority > CVB_VLAN_PRI_MAX)
    {
        retVal = -RT_ERR_VLAN_PRIORITY;
    }
    else if(dei > CVB_VLAN_DEI_MAX)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    /* Set the outer pvid of the port */
    CHK_FUN_RET(retVal, cvb_drv_svlan_port_pvid_set(unit, port, pvid, priority, dei));

    return retVal;
}

/**
 * @brief Get the default VLAN ID, priority, and DEI of the outer VLAN tag for a specific port.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                       The physical port number of the switch.
 * @param[out] pPvid     IEEE 802.1Q Port-based VLAN ID (N/A) \n
 *                       The VLAN ID for a specific port.
 * @param[out] pPriority IEEE 802.1p VLAN priority for the "pvid" (N/A) \n
 *                       The VLAN priority for a specific port.
 * @param[out] pDei      IEEE 802.1Q Drop eligible indicator (N/A) \n
 *                       The VLAN DEI for a specific port.
 * @retval RT_ERR_OK            The outer port-based VLAN was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pPvid", "pPriority", or "pDei" is NULL.
 * @retval Others               Please refer to cvb_drv_svlan_port_pvid_get().
 */
RtkApiRet cvb_svlan_port_pvid_get(uint32 unit, RtkPort port, cvb_RtkVlan *pPvid, cvb_VlanPri *pPriority, cvb_VlanDei *pDei)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if((NULL == pPvid) || (NULL == pPriority) || (NULL == pDei))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get the outer pvid of the port */
    CHK_FUN_RET(retVal, cvb_drv_svlan_port_pvid_get(unit, port, pPvid, pPriority, pDei));

    return retVal;
}
/**@}*/ /*VLAN_ASSIGNMENT_ASIC*/
/**@}*/ /*VLAN_ASSIGNMENT*/

/**
 * @addtogroup INGRESS_VLAN_RETAGGING Ingress VLAN Retagging
 * @{
 */
/**
 * @defgroup INGRESS_VLAN_RETAGGING_ASIC Low Level Driver
 * @brief VLAN Low Level Driver
 * @{
 */
/**
 * @brief Initialize ingress VLAN retagging.
 * @param[in]  unit Stacking unit (1 to 7) \n
 *                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The ingress VLAN retagging function of the ports was initialized successfully.
 * @retval Others    Please refer to cvb_drv_drv_memset(), cvb_drv_vlan_igr_retagging_en_set(), cvb_drv_vlan_igr_retagging_set().
 */
RtkApiRet cvb_vlan_igr_retagging_init(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    port   = 0u;
    uint32    idx    = 0u;
    RtkPmap   zero   = 0u;

    retVal = cvb_drv_drv_memset(&zero, 0, sizeof(zero));

    /* Disable ingress VLAN filter ability for all ports */
    for(port = (RtkPort)CVB_RTK_USER_PORT_1ST; port <= (RtkPort)CVB_RTK_USER_PORT_END; port++)
    {
        CHK_FUN_RET(retVal, cvb_drv_vlan_igr_retagging_en_set(unit, port, CVB_DISABLED));
        if(RT_ERR_OK != retVal)
        {
            break;
        }
    }
    /* Disable ingress VLAN filter ability for the CPU port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_igr_retagging_en_set(unit, (RtkPort)CVB_RTK_CPU_PORT, CVB_DISABLED));

    /* Clear ingress VLAN retagging entry */
    for(idx = 0u; idx < CVB_VLAN_INGRESS_RETAG_TBLSIZE; idx++)
    {
        CHK_FUN_RET(retVal, cvb_drv_vlan_igr_retagging_set(unit, idx, 0u, 0u, zero, 0u));
        if(RT_ERR_OK != retVal)
        {
            break;
        }
    }

    return retVal;
}

/**
 * @brief Set an ingress VLAN retagging entry.
 * @param[in]  unit        Stacking unit (1 to 7) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  idx         Index (0 to 31) \n
 *                         Index of the ingress VLAN retagging table.
 * @param[in]  originalVid Original VLAN ID (0 to 0xFFFu) \n
 *                         Original VLAN ID to be replaced.
 * @param[in]  newVid      New VLAN ID (0 to 0xFFFu) \n
 *                         New VLAN ID after replacement.
 * @param[in]  pbm         Ingress member ports (CVB_RTK_USER_PMAP_1ST to CVB_RTK_USER_PMAP_ALL, CVB_RTK_CPU_PMAP) \n
 *                         Affected ingress port mask.
 * @param[in]  valid       Valid bit (0 to 1) \n
 *                         Indicates whether this table entry is valid.
 * @param[out] None
 * @retval Others Please refer to cvb_drv_vlan_igr_retagging_set().
 */
RtkApiRet cvb_vlan_igr_retagging_set(uint32      unit,
                                     uint32      idx,
                                     cvb_RtkVlan originalVid,
                                     cvb_RtkVlan newVid,
                                     RtkPmap     pbm,
                                     uint8       valid)
{
    /* Configure the ingress VLAN retagging entry */
    return cvb_drv_vlan_igr_retagging_set(unit, idx, originalVid, newVid, pbm, valid);
}

/**
 * @brief Get an ingress VLAN retagging entry.
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  idx          Index (0 to 31) \n
 *                          Index of the ingress VLAN retagging table.
 * @param[out] pOriginalVid Original VLAN ID (N/A) \n
 *                          Original VLAN ID to be replaced.
 * @param[out] pNewVid      New VLAN ID (N/A) \n
 *                          New VLAN ID after replacement.
 * @param[out] pPbm         Ingress member ports (N/A) \n
 *                          Affected ingress port mask.
 * @param[out] pValid       Valid bit (N/A) \n
 *                          Indicates whether this table entry is valid.
 * @retval Others Please refer to cvb_drv_vlan_igr_retagging_get().
 */
RtkApiRet cvb_vlan_igr_retagging_get(uint32       unit,
                                     uint32       idx,
                                     cvb_RtkVlan *pOriginalVid,
                                     cvb_RtkVlan *pNewVid,
                                     RtkPmap     *pPbm,
                                     uint8       *pValid)
{
    /* Get the ingress VLAN retagging entry */
    return cvb_drv_vlan_igr_retagging_get(unit, idx, pOriginalVid, pNewVid, pPbm, pValid);
}

/**
 * @brief Set ingress VLAN retagging status.
 * @param[in]  unit   Stacking unit (1 to 7) \n
 *                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port   Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                    The physical port number of the switch.
 * @param[in]  enable Enable the ingress retagging function (0 to 1) \n
 *                    The ingress retagging indicates whether to replace the "original_vid" with the "new_vid" to perform forwarding within the switch.
 * @param[out] None
 * @retval Others Please refer to cvb_drv_vlan_igr_retagging_en_set().
 */
RtkApiRet cvb_vlan_igr_retagging_en_set(uint32 unit, RtkPort port, cvb_RtkEnable enable)
{
    /* Enable or disable ingress VLAN retagging function */
    return cvb_drv_vlan_igr_retagging_en_set(unit, port, enable);
}

/**
 * @brief Get ingress VLAN retagging status.
 * @param[in]  unit    Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port    Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                     The physical port number of the switch.
 * @param[out] pEnable Enable the ingress retagging function (N/A) \n
 *                     The ingress retagging indicates whether to replace the "original_vid" with the "new_vid" to perform forwarding within the switch.
 * @retval Others Please refer to cvb_drv_vlan_igr_retagging_en_get().
 */
RtkApiRet cvb_vlan_igr_retagging_en_get(uint32 unit, RtkPort port, cvb_RtkEnable *pEnable)
{
    /* Get the status of the ingress VLAN retagging function */
    return cvb_drv_vlan_igr_retagging_en_get(unit, port, pEnable);
}
/**@}*/ /*INGRESS_VLAN_RETAGGING_ASIC*/
/**@}*/ /*INGRESS_VLAN_RETAGGING*/

/**
 * @addtogroup VLAN_TABLE_LOOKUP VLAN Table Lookup
 * @{
 */
/**
 * @defgroup VLAN_TABLE_LOOKUP_ASIC Low Level Driver
 * @brief VLAN Low Level Driver
 * @{
 */
/**
 * @brief Initialize the VLAN table.
 * @param[in]  unit Stacking unit (1 to 7) \n
 *                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The VLAN table was initialized successfully.
 * @retval Others    Please refer to cvb_drv_drv_memset(), cvb_drv_vlan_flush_set(), cvb_drv_vlan_tbl_entry_set(), cvb_drv_vlan_port_tag_aware_set(), \n
 *                   cvb_drv_vlan_port_acpt_type_set(), cvb_drv_vlan_port_pvid_set(), cvb_drv_vlan_egr_tag_status_set(), cvb_drv_vlan_port_igr_filter_set().
 */
RtkApiRet cvb_vlan_init(uint32 unit)
{
    RtkApiRet          retVal = RT_ERR_OK;
    uint32             port   = 0u;
    cvb_VlanTblParam_t vlanEntry;

    retVal = cvb_drv_drv_memset(&vlanEntry, 0, sizeof(vlanEntry));

    /* Clear the VLAN table */
    CHK_FUN_RET(retVal, cvb_drv_vlan_flush_set(unit));

    /* Set default vlan 1 */
    vlanEntry.statefulChkMask = 0u;
    /* Set a default VLAN 1 with all ports, but excluding the CPU port */
    vlanEntry.mbr   = (uint32)CVB_RTK_SYS_PMAP_ALL;
    vlanEntry.utag  = (uint32)(CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP);
    vlanEntry.fid   = 0u;
    vlanEntry.ulfid = (uint8)EM_VLAN_SVL;
    vlanEntry.vbfwd = (uint8)EM_ALE_BASED_FWD;
    vlanEntry.valid = (uint8)1u;
    CHK_FUN_RET(retVal, cvb_drv_vlan_tbl_entry_set(unit, 1u, &vlanEntry));

    /* Set the default VLAN configuration for all ports */
    for(port = (RtkPort)CVB_RTK_USER_PORT_1ST; port <= (RtkPort)CVB_RTK_USER_PORT_END; port++)
    {
        CHK_FUN_RET(retVal, cvb_drv_vlan_port_tag_aware_set(unit, port, (uint32)TRUE));
        CHK_FUN_RET(retVal, cvb_drv_vlan_port_acpt_type_set(unit, port, EM_ACCEPT_FRAME_TYPE_ALL));
        CHK_FUN_RET(retVal, cvb_drv_vlan_port_pvid_set(unit, port, 1u, 0u));
        CHK_FUN_RET(retVal, cvb_drv_vlan_egr_tag_status_set(unit, port, EM_TAG_STATUS_WITH_TAG, EM_TAG_STATUS_WITH_TAG));
        CHK_FUN_RET(retVal, cvb_drv_vlan_port_igr_filter_set(unit, port, TRUE));
    }
    /* Set the default VLAN configuration for the CPU port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_tag_aware_set(unit, (RtkPort)CVB_RTK_CPU_PORT, (uint32)TRUE));
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_acpt_type_set(unit, (RtkPort)CVB_RTK_CPU_PORT, EM_ACCEPT_FRAME_TYPE_ALL));
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_pvid_set(unit, (RtkPort)CVB_RTK_CPU_PORT, 1u, 0u));
    CHK_FUN_RET(retVal, cvb_drv_vlan_egr_tag_status_set(unit, (RtkPort)CVB_RTK_CPU_PORT, EM_TAG_STATUS_WITH_TAG, EM_TAG_STATUS_WITH_TAG));
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_igr_filter_set(unit, (RtkPort)CVB_RTK_CPU_PORT, TRUE));

    return retVal;
}

/**
 * @brief Add or modify a VLAN entry in the VLAN table.
 * @param[in]  unit       Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  vid        VLAN ID (0 to 0xFFFu) \n
 *                        VLAN identifier, which is also the index of the VLAN table.
 * @param[in]  pVlanEntry VLAN entry (N/A) \n
 *                        The related configuration of VLAN. For detailed items, please refer to "VlanEntry_t".
 * @param[out] None
 * @retval RT_ERR_OK            The VLAN entry was added or modified successfully.
 * @retval -RT_ERR_VLAN_VID     The "vid" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pVlanEntry" is NULL.
 * @retval -RT_ERR_L2_FID       The "fid" of the VLAN entry is out of range.
 * @retval -RT_ERR_INPUT        The "ulfid" or "vbfwd" of the VLAN entry is out of range.
 * @retval Others               Please refer to cvb_drv_drv_memset(), cvb_drv_vlan_entry2tblparam(), cvb_drv_vlan_tbl_entry_set().
 */
RtkApiRet cvb_vlan_set(uint32 unit, cvb_RtkVlan vid, VlanEntry_t *pVlanEntry)
{
    RtkApiRet          retVal = RT_ERR_OK;
    cvb_VlanTblParam_t vlanEntry;

    /* Check if the input parameters are valid */
    if(vid > CVB_VLAN_VID_MAX)
    {
        retVal = -RT_ERR_VLAN_VID;
    }
    else if(NULL == pVlanEntry)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(pVlanEntry->fid > CVB_VLAN_FID_MAX)
    {
        retVal = -RT_ERR_L2_FID;
    }
    else if(pVlanEntry->ulfid >= EM_VLAN_IVLSVL_END)
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(pVlanEntry->vbfwd >= EM_VLAN_FWDMODE_END)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&vlanEntry, 0, sizeof(vlanEntry)));
    /* Convert the elements of the VLAN entry to the table parameters */
    CHK_FUN_RET(retVal, cvb_drv_vlan_entry2tblparam(unit, pVlanEntry, &vlanEntry));
    vlanEntry.valid = 1u;

    /* Set VLAN table entry */
    CHK_FUN_RET(retVal, cvb_drv_vlan_tbl_entry_set(unit, vid, &vlanEntry));

    return retVal;
}

/**
 * @brief Get a VLAN entry.
 * @param[in]  unit       Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  vid        VLAN ID (0 to 0xFFFu) \n
 *                        VLAN identifier, which is also the index of the VLAN table.
 * @param[out] pVlanEntry VLAN entry (N/A) \n
 *                        The related configuration of VLAN. For detailed items, please refer to "VlanEntry_t".
 * @retval RT_ERR_OK            The VLAN entry was retrieved successfully.
 * @retval -RT_ERR_VLAN_VID     The "vid" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pVlanEntry" is NULL.
 * @retval Others               Please refer to cvb_drv_drv_memset(), cvb_drv_vlan_tbl_entry_get(), cvb_drv_vlan_tblparam2entry().
 */
RtkApiRet cvb_vlan_get(uint32 unit, cvb_RtkVlan vid, VlanEntry_t *pVlanEntry)
{
    RtkApiRet          retVal = RT_ERR_OK;
    cvb_VlanTblParam_t vlanEntry;

    /* Check if the input parameters are valid */
    if(vid > CVB_VLAN_VID_MAX)
    {
        retVal = -RT_ERR_VLAN_VID;
    }
    else if((NULL == pVlanEntry))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&vlanEntry, 0, sizeof(vlanEntry)));
    /* Get VLAN table entry */
    CHK_FUN_RET(retVal, cvb_drv_vlan_tbl_entry_get(unit, vid, &vlanEntry));

    if(RT_ERR_OK == retVal)
    {
        if(vlanEntry.valid == 0u)
        {
            retVal = -RT_ERR_VLAN_ENTRY_NOT_FOUND;
        }
        else
        {
            /* Convert the table parameters to the elements of the VLAN entry */
            retVal = cvb_drv_vlan_tblparam2entry(unit, &vlanEntry, pVlanEntry);
        }
    }

    return retVal;
}

/**
 * @brief Destroy a VLAN entry in the VLAN table.
 * @param[in]  unit Stacking unit (1 to 7) \n
 *                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  vid  VLAN ID (0 to 0xFFFu) \n
 *                  VLAN identifier, which is also the index of the VLAN table.
 * @param[out] None
 * @retval RT_ERR_OK        The VLAN entry was destroyed successfully.
 * @retval -RT_ERR_VLAN_VID The "vid" is out of range.
 * @retval Others           Please refer to cvb_drv_drv_memset(), cvb_drv_vlan_tbl_entry_get(), cvb_drv_vlan_tbl_entry_set().
 */
RtkApiRet cvb_vlan_destroy(uint32 unit, cvb_RtkVlan vid)
{
    RtkApiRet          retVal = RT_ERR_OK;
    cvb_VlanTblParam_t vlanEntry;

    /* Check if the input parameters are valid */
    if(vid > CVB_VLAN_VID_MAX)
    {
        retVal = -RT_ERR_VLAN_VID;
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&vlanEntry, 0, sizeof(vlanEntry)));
    /* Get VLAN table entry */
    CHK_FUN_RET(retVal, cvb_drv_vlan_tbl_entry_get(unit, vid, &vlanEntry));

    if((RT_ERR_OK == retVal) && (vlanEntry.valid == 0u))
    {
        retVal = -RT_ERR_VLAN_ENTRY_NOT_FOUND;
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&vlanEntry, 0, sizeof(vlanEntry)));
    /* Set VLAN table entry */
    CHK_FUN_RET(retVal, cvb_drv_vlan_tbl_entry_set(unit, vid, &vlanEntry));

    return retVal;
}

/**
 * @brief Configure whether to enable the outer tag parser ability.
 * @param[in]  unit   Stacking unit (1 to 7) \n
 *                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port   Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                    The physical port number of the switch.
 * @param[in]  enable Enable (0 to 1) \n
 *                    The outer tag parser ability if the "enable" is equal to CVB_ENABLED.
 * @param[out] None
 * @retval RT_ERR_OK       The outer tag parser ability was set successfully.
 * @retval -RT_ERR_PORT_ID The "port" is out of range.
 * @retval -RT_ERR_ENABLE  The "enable" is out of range.
 * @retval Others          Please refer to cvb_drv_svlan_pkt_parser_en_set().
 */
RtkApiRet cvb_svlan_port_pkt_parser_en_set(uint32 unit, RtkPort port, cvb_RtkEnable enable)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(enable >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* Do nothing */
    }

    /* Enable or disable the outer tag parser ability */
    CHK_FUN_RET(retVal, cvb_drv_svlan_pkt_parser_en_set(unit, port, (uint32)enable));

    return retVal;
}

/**
 * @brief Get whether to enable the outer tag parser ability.
 * @param[in]  unit    Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port    Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                     The physical port number of the switch.
 * @param[out] pEnable Enable (N/A) \n
 *                     The outer tag parser ability if the "enable" is equal to CVB_ENABLED.
 * @retval RT_ERR_OK            The outer tag parser ability was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pEnable" is NULL.
 * @retval -RT_ERR_FAILED       The outer tag parser ability retrieval failed.
 * @retval Others               Please refer to cvb_drv_svlan_pkt_parser_en_get().
 */
RtkApiRet cvb_svlan_port_pkt_parser_en_get(uint32 unit, RtkPort port, cvb_RtkEnable *pEnable)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    enable = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pEnable)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get the status of the outer tag parser ability */
    CHK_FUN_RET(retVal, cvb_drv_svlan_pkt_parser_en_get(unit, port, &enable));

    if(RT_ERR_OK == retVal)
    {
        switch(enable)
        {
            case 0:
                *pEnable = CVB_DISABLED;
                break;
            case 1:
                *pEnable = CVB_ENABLED;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }

    return retVal;
}

/**
 * @brief Configure whether forwarding is based on outer VID or inner VID.
 * @param[in]  unit   Stacking unit (1 to 7) \n
 *                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port   Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                    The physical port number of the switch.
 * @param[in]  enable Enable (0 to 1) \n
 *                    The forwarding is based on outer VID if the "enable" is equal to CVB_ENABLED.
 * @param[out] None
 * @retval RT_ERR_OK       The forwarding method was set successfully.
 * @retval -RT_ERR_PORT_ID The "port" is out of range.
 * @retval -RT_ERR_ENABLE  The "enable" is out of range.
 * @retval Others          Please refer to cvb_drv_svlan_fwd_base_ovid_set().
 */
RtkApiRet cvb_svlan_fwd_base_ovid_set(uint32 unit, RtkPort port, cvb_RtkEnable enable)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(enable >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* Do nothing */
    }
    /* Set forward type */
    CHK_FUN_RET(retVal, cvb_drv_svlan_fwd_base_ovid_set(unit, port, (uint32)enable));

    return retVal;
}

/**
 * @brief Get whether forwarding is based on outer VID or inner VID.
 * @param[in]  unit    Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port    Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                     The physical port number of the switch.
 * @param[out] pEnable Enable (N/A) \n
 *                     The forwarding is based on outer VID if the "enable" is equal to CVB_ENABLED.
 * @retval RT_ERR_OK            The forwarding method was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pEnable" is NULL.
 * @retval -RT_ERR_FAILED       The forwarding method retrieval failed.
 * @retval Others               Please refer to cvb_drv_svlan_fwd_base_ovid_get().
 */
RtkApiRet cvb_svlan_fwd_base_ovid_get(uint32 unit, RtkPort port, cvb_RtkEnable *pEnable)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    enable = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pEnable)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get forward type */
    CHK_FUN_RET(retVal, cvb_drv_svlan_fwd_base_ovid_get(unit, port, &enable));
    if(RT_ERR_OK == retVal)
    {
        switch(enable)
        {
            case 0:
                *pEnable = CVB_DISABLED;
                break;
            case 1:
                *pEnable = CVB_ENABLED;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }

    return retVal;
}
/**@}*/ /*VLAN_TABLE_LOOKUP_ASIC*/
/**@}*/ /*VLAN_TABLE_LOOKUP*/

/**
 * @addtogroup INGRESS_VLAN_FILTER Ingress VLAN Filter
 * @{
 */
/**
 * @defgroup INGRESS_VLAN_FILTER_ASIC Low Level Driver
 * @brief VLAN Low Level Driver
 * @{
 */
/**
 * @brief Configure ingress VLAN filter for a specific port.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                       The physical port number of the switch.
 * @param[in]  igrFilter Ingress filter (0 to 1) \n
 *                       The status of the ingress filter function.
 * @param[out] None
 * @retval RT_ERR_OK       The ingress VLAN filter ability of the port was set successfully.
 * @retval -RT_ERR_PORT_ID The "port" is out of range.
 * @retval -RT_ERR_ENABLE  The "igrFilter" is out of range.
 * @retval Others          Please refer to cvb_drv_vlan_port_igr_filter_set().
 */
RtkApiRet cvb_vlan_port_igr_filter_en_set(uint32 unit, RtkPort port, cvb_RtkEnable igrFilter)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(igrFilter >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* Do nothing */
    }

    /* Enable or disable ingress VLAN filter ability for the specific port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_igr_filter_set(unit, port, (uint32)igrFilter));

    return retVal;
}

/**
 * @brief Get the status of the ingress VLAN filter for a specific port.
 * @param[in]  unit       Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                        The physical port number of the switch.
 * @param[out] pIgrFilter Ingress filter (N/A) \n
 *                        The status of the ingress filter function.
 * @retval RT_ERR_OK            The ingress VLAN filter ability of the port was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pIgrFilter" is NULL.
 * @retval Others               Please refer to cvb_drv_vlan_port_igr_filter_get().
 */
RtkApiRet cvb_vlan_port_igr_filter_en_get(uint32 unit, RtkPort port, cvb_RtkEnable *pIgrFilter)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    enabled = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIgrFilter)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get the status of ingress VLAN filter ability for the specific port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_igr_filter_get(unit, port, &enabled));

    if(RT_ERR_OK == retVal)
    {
        if(0u == enabled)
        {
            *pIgrFilter = CVB_DISABLED;
        }
        else if(1u == enabled)
        {
            *pIgrFilter = CVB_ENABLED;
        }
        else
        {
            /* Do nothing */
        }
    }

    return retVal;
}
/**@}*/ /*INGRESS_VLAN_FILTER_ASIC*/
/**@}*/ /*INGRESS_VLAN_FILTER*/

/**
 * @addtogroup EGRESS_TAG_STATUS_DECISION Egress Tag Status Decision
 * @{
 */
/**
 * @defgroup EGRESS_TAG_STATUS_DECISION_ASIC Low Level Driver
 * @brief VLAN Low Level Driver
 * @{
 */
/**
 * @brief Configure inner tag status for a specific egress port.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                       The physical port number of the switch.
 * @param[in]  tagStatus Inner tag status of the egress port (0 to 2) \n
 *                       Transmit with or without inner VLAN tag for the packets received from UNI/NNI port.
 * @param[out] None
 * @retval RT_ERR_OK       The egress tag status was set successfully.
 * @retval -RT_ERR_PORT_ID The "port" is out of range.
 * @retval -RT_ERR_INPUT   The "tagStatus" is out of range.
 * @retval Others          Please refer to cvb_drv_vlan_egr_tag_status_set().
 */
RtkApiRet cvb_vlan_port_egr_tag_status_set(uint32 unit, RtkPort port, VlanTxTagStatus_e tagStatus)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(tagStatus >= EM_TAG_STATUS_END)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    /* Set egress port CVLAN tag status */
    CHK_FUN_RET(retVal, cvb_drv_vlan_egr_tag_status_set(unit, port, tagStatus, tagStatus));

    return retVal;
}

/**
 * @brief Get inner tag status for a specific egress port.
 * @param[in]  unit       Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                        The physical port number of the switch.
 * @param[out] pTagStatus Inner tag status of the egress port (N/A) \n
 *                        Transmit with or without inner VLAN tag for the packets received from UNI/NNI port.
 * @retval RT_ERR_OK            The egress tag status was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pTagStatus" is NULL.
 * @retval Others               Please refer to cvb_drv_vlan_egr_tag_status_get().
 */
RtkApiRet cvb_vlan_port_egr_tag_status_get(uint32 unit, RtkPort port, VlanTxTagStatus_e *pTagStatus)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pTagStatus)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get egress port CVLAN tag status */
    CHK_FUN_RET(retVal, cvb_drv_vlan_egr_tag_status_get(unit, port, pTagStatus, pTagStatus));

    return retVal;
}

/**
 * @brief Configure outer tag status for a specific egress port.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                       The physical port number of the switch.
 * @param[in]  tagStatus Outer tag status of the egress port (0 to 2) \n
 *                       Transmit with or without outer VLAN tag for the packets received from UNI/NNI port.
 * @param[out] None
 * @retval RT_ERR_OK       The egress tag status was set successfully.
 * @retval -RT_ERR_PORT_ID The "port" is out of range.
 * @retval -RT_ERR_INPUT   The "tagStatus" is out of range.
 * @retval Others          Please refer to cvb_drv_svlan_port_egr_stat_set().
 */
RtkApiRet cvb_svlan_port_egr_tag_status_set(uint32 unit, RtkPort port, VlanTxTagStatus_e tagStatus)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(tagStatus >= EM_TAG_STATUS_END)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    /* Set egress port SVLAN tag status */
    CHK_FUN_RET(retVal, cvb_drv_svlan_port_egr_stat_set(unit, port, tagStatus, tagStatus));

    return retVal;
}

/**
 * @brief Get outer tag status for a specific egress port.
 * @param[in]  unit       Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                        The physical port number of the switch.
 * @param[out] pTagStatus Outer tag status of the egress port (N/A) \n
 *                        Transmit with or without outer VLAN tag for the packets received from UNI/NNI port.
 * @retval RT_ERR_OK            The egress tag status was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pTagStatus" is NULL.
 * @retval Others               Please refer to cvb_drv_svlan_port_egr_stat_get().
 */
RtkApiRet cvb_svlan_port_egr_tag_status_get(uint32 unit, RtkPort port, VlanTxTagStatus_e *pTagStatus)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pTagStatus)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get egress port SVLAN tag status */
    CHK_FUN_RET(retVal, cvb_drv_svlan_port_egr_stat_get(unit, port, pTagStatus, pTagStatus));

    return retVal;
}
/**@}*/ /*EGRESS_TAG_STATUS_DECISION_ASIC*/
/**@}*/ /*EGRESS_TAG_STATUS_DECISION*/

/**
 * @addtogroup EGRESS_VLAN_FILTER Egress VLAN Filter
 * @{
 */
/**
 * @defgroup EGRESS_VLAN_FILTER_ASIC Low Level Driver
 * @brief VLAN Low Level Driver
 * @{
 */
/**
 * @brief Configure egress VLAN filter for a specific port.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                       The physical port number of the switch.
 * @param[in]  egrFilter Egress filter (0 to 1) \n
 *                       The status of the VLAN egress function.
 * @param[out] None
 * @retval RT_ERR_OK       The egress VLAN filter ability of the port was set successfully.
 * @retval -RT_ERR_PORT_ID The "port" is out of range.
 * @retval -RT_ERR_ENABLE  The "egrFilter" is out of range.
 * @retval Others          Please refer to cvb_drv_vlan_port_egr_filter_set().
 */
RtkApiRet cvb_vlan_port_egr_filter_en_set(uint32 unit, RtkPort port, cvb_RtkEnable egrFilter)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(egrFilter >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    else
    {
        /* Do nothing */
    }

    /* Enable or disable egress VLAN filter ability for the specific port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_egr_filter_set(unit, port, (uint32)egrFilter));

    return retVal;
}

/**
 * @brief Get egress VLAN filter status for a specific port.
 * @param[in]  unit       Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port       Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                        The physical port number of the switch.
 * @param[out] pEgrFilter Egress filter (N/A) \n
 *                        The status of the VLAN egress function.
 * @retval RT_ERR_OK            The egress VLAN filter ability of the port was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pEgrFilter" is NULL.
 * @retval Others               Please refer to cvb_drv_vlan_port_egr_filter_get().
 */
RtkApiRet cvb_vlan_port_egr_filter_en_get(uint32 unit, RtkPort port, cvb_RtkEnable *pEgrFilter)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    enabled = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pEgrFilter)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get the status of egress VLAN filter ability for the specific port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_port_egr_filter_get(unit, port, &enabled));

    if(RT_ERR_OK == retVal)
    {
        if((uint32)CVB_ENABLED == enabled)
        {
            *pEgrFilter = CVB_ENABLED;
        }
        else if((uint32)CVB_DISABLED == enabled)
        {
            *pEgrFilter = CVB_DISABLED;
        }
        else
        {
            /* Do nothing */
        }
    }

    return retVal;
}
/**@}*/ /*EGRESS_VLAN_FILTER_ASIC*/
/**@}*/ /*EGRESS_VLAN_FILTER*/

/**
 * @addtogroup EGRESS_VLAN_RETAGGING Egress VLAN Retagging
 * @{
 */
/**
 * @defgroup EGRESS_VLAN_RETAGGING_ASIC Low Level Driver
 * @brief VLAN Low Level Driver
 * @{
 */
/**
 * @brief Initialize egress VLAN retagging.
 * @param[in]  unit Stacking unit (1 to 7) \n
 *                  Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The egress VLAN retagging function of the ports was initialized successfully.
 * @retval Others    Please refer to cvb_drv_drv_memset(), cvb_drv_vlan_egr_retagging_en_set(), cvb_drv_vlan_egress_retagging_set().
 */
RtkApiRet cvb_vlan_egr_retagging_init(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    port   = 0u;
    uint32    idx    = 0u;
    RtkPmap   zero   = 0u;

    retVal = cvb_drv_drv_memset(&zero, 0, sizeof(zero));

    /* Disable egress VLAN filter ability for all ports */
    for(port = (RtkPort)CVB_RTK_USER_PORT_1ST; port <= (RtkPort)CVB_RTK_USER_PORT_END; port++)
    {
        CHK_FUN_RET(retVal, cvb_drv_vlan_egr_retagging_en_set(unit, port, CVB_DISABLED));
        if(RT_ERR_OK != retVal)
        {
            break;
        }
    }
    /* Disable egress VLAN filter ability for the CPU port */
    CHK_FUN_RET(retVal, cvb_drv_vlan_egr_retagging_en_set(unit, (RtkPort)CVB_RTK_CPU_PORT, CVB_DISABLED));

    /* Clear egress VLAN retagging entry */
    for(idx = 0u; idx < CVB_VLAN_EGRESS_RETAG_TBLSIZE; idx++)
    {
        CHK_FUN_RET(retVal, cvb_drv_vlan_egress_retagging_set(unit, idx, 0u, 0u, zero, 0u));
        if(RT_ERR_OK != retVal)
        {
            break;
        }
    }

    return retVal;
}

/**
 * @brief Set an egress VLAN retagging entry.
 * @param[in]  unit        Stacking unit (1 to 7) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  idx         Index (0 to 31) \n
 *                         Index of the egress VLAN retagging table.
 * @param[in]  originalVid Original VLAN ID (0 to 0xFFFu) \n
 *                         Original VLAN ID to be replaced.
 * @param[in]  newVid      New VLAN ID (0 to 0xFFFu) \n
 *                         New VLAN ID after replacement.
 * @param[in]  pbm         Egress member ports (CVB_RTK_USER_PMAP_1ST to CVB_RTK_USER_PMAP_ALL, CVB_RTK_CPU_PMAP) \n
 *                         Affected egress port mask.
 * @param[in]  valid       Valid bit (0 to 1) \n
 *                         Indicates whether this table entry is valid.
 * @param[out] None
 * @retval Others Please refer to cvb_drv_vlan_egress_retagging_set().
 */
RtkApiRet cvb_vlan_egr_retagging_set(uint32      unit,
                                     uint32      idx,
                                     cvb_RtkVlan originalVid,
                                     cvb_RtkVlan newVid,
                                     RtkPmap     pbm,
                                     uint8       valid)
{
    /* Configure the egress VLAN retagging entry */
    return cvb_drv_vlan_egress_retagging_set(unit, idx, originalVid, newVid, pbm, valid);
}

/**
 * @brief Get an egress VLAN retagging entry.
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  idx          Index (0 to 31) \n
 *                          Index of the egress VLAN retagging table.
 * @param[out] pOriginalVid Original VLAN ID (N/A) \n
 *                          Original VLAN ID to be replaced.
 * @param[out] pNewVid      New VLAN ID (N/A) \n
 *                          New VLAN ID after replacement.
 * @param[out] pPbm         Egress member ports (N/A) \n
 *                          Affected egress port mask.
 * @param[out] pValid       Valid bit (N/A) \n
 *                          Indicates whether this table entry is valid.
 * @retval Others Please refer to cvb_drv_vlan_egress_retagging_get().
 */
RtkApiRet cvb_vlan_egr_retagging_get(uint32       unit,
                                     uint32       idx,
                                     cvb_RtkVlan *pOriginalVid,
                                     cvb_RtkVlan *pNewVid,
                                     RtkPmap     *pPbm,
                                     uint8       *pValid)
{
    /* Get the egress VLAN retagging entry */
    return cvb_drv_vlan_egress_retagging_get(unit, idx, pOriginalVid, pNewVid, pPbm, pValid);
}

/**
 * @brief Set egress VLAN retagging status.
 * @param[in]  unit   Stacking unit (1 to 7) \n
 *                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port   Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                    The physical port number of the switch.
 * @param[in]  enable Enable the egress retagging function (0 to 1) \n
 *                    The egress retagging indicates whether to replace the "original_vid" with the "new_vid" to perform forwarding within the switch.
 * @param[out] None
 * @retval Others Please refer to cvb_drv_vlan_egr_retagging_en_set().
 */
RtkApiRet cvb_vlan_egr_retagging_en_set(uint32 unit, RtkPort port, cvb_RtkEnable enable)
{
    /* Enable or disable egress VLAN retagging function */
    return cvb_drv_vlan_egr_retagging_en_set(unit, port, enable);
}

/**
 * @brief Get egress VLAN retagging status.
 * @param[in]  unit    Stacking unit (1 to 7) \n
 *                     Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port    Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                     The physical port number of the switch.
 * @param[out] pEnable Enable the egress retagging function (N/A) \n
 *                     The egress retagging indicates whether to replace the "original_vid" with the "new_vid" to perform forwarding within the switch.
 * @retval Others Please refer to cvb_drv_vlan_egr_retagging_en_get().
 */
RtkApiRet cvb_vlan_egr_retagging_en_get(uint32 unit, RtkPort port, cvb_RtkEnable *pEnable)
{
    /* Get the status of the egress VLAN retagging function */
    return cvb_drv_vlan_egr_retagging_en_get(unit, port, pEnable);
}
/**@}*/ /*EGRESS_VLAN_RETAGGING_ASIC*/
/**@}*/ /*EGRESS_VLAN_RETAGGING*/
/**@}*/ /*VLAN*/
