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
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_asicdrv_vlan.h"
#include "rtl907xd_va_asicdrv_acl.h"
/**
 * @addtogroup RTK_MODULE_DOT1X DOT1X
 * @{
 */

/**
 * @addtogroup HANDSHAKE_CFG Handshake information configuration
 * @{
 */
/**
 * @addtogroup HANDSHAKE_CFG_ASIC Low Level Driver
 * @brief DOT1X Low Level Driver
 * @{
 */

/**
 * @brief Get port-based DOT1X enable port mask.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pPortMask            Port mask (DVA_RTK_USER_PMAP_1ST to DVA_RTK_USER_PMAP_ALL) \n
 *                                  Port-based DOT1X enabled port mask
 * @retval RT_ERR_OK                Port-based DOT1X enable port mask was gotten
 * @retval -RT_ERR_NULL_POINTER     The "pPortMask" is NULL
 * @retval Others                   Please refer to dva_drv_indirect_sram_2bytes_get()
 */
RtkApiRet dva_dot1x_get_enable_portmask(uint32 unit, RtkPmap *pPortMask)
{
    uint32    regAddr = 0;
    uint16    regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pPortMask == NULL)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Return enabled port mask */
        regAddr = (uint32)DVA_DOT1X_ENABLE_PORTMASK_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_2bytes_get(unit, regAddr, &regData));
        *pPortMask = ((uint32)regData & 0xFFFFu);
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief  Set port-based DOT1X enable port mask.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMask              Port mask (DVA_RTK_USER_PMAP_1ST to DVA_RTK_USER_PMAP_ALL) \n
 *                                  Port-based DOT1X enabled port mask
 * @param[out] None
 * @retval RT_ERR_OK                The DOT1X enable port mask was set.
 * @retval -RT_ERR_PORT_MASK        The "portMask" is out of range
 * @retval Others                   Please refer to dva_drv_indirect_sram_2bytes_get(), \n
 *                                  dva_drv_indirect_sram_2bytes_set()
 */
RtkApiRet dva_dot1x_set_enable_portmask(uint32 unit, RtkPmap portMask)
{
    uint32    regAddr = 0;
    uint16    regData = 0;
    uint16    udpPort = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check input */
        if(0u != (portMask & (~(DVA_RTK_USER_PMAP_ALL))))
        {
            retVal = -RT_ERR_PORT_MASK;
            continue;
        }
        /* Clear the enabled port mask */
        regAddr = (uint32)DVA_DOT1X_ENABLE_PORTMASK_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_2bytes_get(unit, regAddr, &regData));
        regData &= 0x0000u;
        /* Set the enabled port mask */
        regData |= ((uint16)portMask & 0xFFFFu);
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_2bytes_set(unit, regAddr, regData));

        if(0u != portMask)
        {
            CHK_FUN_CONTINUE(retVal, dva_dot1x_get_radius_udp_port(unit, &udpPort));
            /* Config ACL rules */
            CHK_FUN_CONTINUE(retVal, dva_drv_acl_config_dot1x_arp(unit));
            CHK_FUN_CONTINUE(retVal, dva_drv_acl_config_dot1x_radius(unit, udpPort));
            CHK_FUN_CONTINUE(retVal, dva_drv_acl_config_dot1x_eap(unit));
        }
        else
        {
            /* Clear ACL rules */
            CHK_FUN_CONTINUE(retVal, dva_drv_acl_config_clear(unit, DVA_ACL_DOT1X_ARP));
            CHK_FUN_CONTINUE(retVal, dva_drv_acl_config_clear(unit, DVA_ACL_DOT1X_RADIUS));
            CHK_FUN_CONTINUE(retVal, dva_drv_acl_config_clear(unit, DVA_ACL_DOT1X_EAP));
        }

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get the secret key from the switch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] secretKeyBufSize      Size of secret key buffer (0 to 31) \n
 *                                  Size of secret key buffer
 * @param[out] pSecretKey           The secret key (N/A) \n
 *                                  Pointer that points to secret key
 * @retval RT_ERR_OK                The secret key was gotten
 * @retval -RT_ERR_FAILED           The "secretKeyBufSize" is out of range
 * @retval -RT_ERR_PORT_ID          The "port" is out of range
 * @retval -RT_ERR_NULL_POINTER     The pointer of "pSecretKey" is NULL
 * @retval Others                   Please refer to dva_drv_memset(), \n
 *                                  dva_drv_indirect_sram_2bytes_get()
 */
RtkApiRet dva_dot1x_get_secret_key(uint32 unit, RtkPort port, uint32 secretKeyBufSize, uint8 *pSecretKey)
{
    const uint32 regAddrOffset[] = {
        DVA_DOT1X_P1_SECRET_KEY_OFFSET,
        DVA_DOT1X_P2_SECRET_KEY_OFFSET,
        DVA_DOT1X_P3_SECRET_KEY_OFFSET,
        DVA_DOT1X_P4_SECRET_KEY_OFFSET,
        DVA_DOT1X_P5_SECRET_KEY_OFFSET,
        DVA_DOT1X_P6_SECRET_KEY_OFFSET,
        DVA_DOT1X_P7_SECRET_KEY_OFFSET,
        DVA_DOT1X_P8_SECRET_KEY_OFFSET,
        DVA_DOT1X_P9_SECRET_KEY_OFFSET,
        DVA_DOT1X_P10_SECRET_KEY_OFFSET,
        DVA_DOT1X_P11_SECRET_KEY_OFFSET,
        DVA_DOT1X_P12_SECRET_KEY_OFFSET};

    uint32    regAddr = 0;
    uint16    regData = 0;
    uint32    i       = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check port range */
        if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* Check buffer length */
        if(secretKeyBufSize < ((uint32)DVA_DOT1X_SECRET_KEY_LENGTH))
        {
            retVal = -(RT_ERR_FAILED);
            continue;
        }
        /* Check NULL pointer */
        if(pSecretKey == NULL)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get secret key */
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(pSecretKey, 0, secretKeyBufSize));
        regAddr = regAddrOffset[port - 1u];
        for(i = 0; i < ((uint32)DVA_DOT1X_SECRET_KEY_LENGTH / 2u); i++)
        {
            regData = 0;
            retVal  = dva_drv_indirect_sram_2bytes_get(unit, regAddr, &regData);
            regAddr = regAddr + 2u;
            if(RT_ERR_OK != retVal)
            {
                continue;
            }

            pSecretKey[i * 2u]        = (uint8)(regData & 0xFFu);
            pSecretKey[(i * 2u) + 1u] = (uint8)((regData >> 8u) & 0xFFu);
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set secret key in switch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] secretKeyBufSize      Size of secret key buffer (0 to 31) \n
 *                                  Size of secret key buffer
 * @param[in] pSecretKey            The secret key (N/A) \n
 *                                  Pointer that points to secret key
 * @param[out] None
 * @retval RT_ERR_OK                The secret key was set
 * @retval -RT_ERR_PORT_ID          The "port" is out of range
 * @retval -RT_ERR_FAILED           The "secretKeyLength" is out of range
 *                                  The length of "pSecretKey" is out of range
 * @retval -RT_ERR_NULL_POINTER     The pointer of "pSecretKey" is NULL
 * @retval Others                   Please refer to dva_drv_indirect_sram_1byte_set()
 */
RtkApiRet dva_dot1x_set_secret_key(uint32 unit, RtkPort port, uint32 secretKeyLength, const uint8 *pSecretKey)
{
    const uint32 regAddrOffset[] = {
        DVA_DOT1X_P1_SECRET_KEY_OFFSET,
        DVA_DOT1X_P2_SECRET_KEY_OFFSET,
        DVA_DOT1X_P3_SECRET_KEY_OFFSET,
        DVA_DOT1X_P4_SECRET_KEY_OFFSET,
        DVA_DOT1X_P5_SECRET_KEY_OFFSET,
        DVA_DOT1X_P6_SECRET_KEY_OFFSET,
        DVA_DOT1X_P7_SECRET_KEY_OFFSET,
        DVA_DOT1X_P8_SECRET_KEY_OFFSET,
        DVA_DOT1X_P9_SECRET_KEY_OFFSET,
        DVA_DOT1X_P10_SECRET_KEY_OFFSET,
        DVA_DOT1X_P11_SECRET_KEY_OFFSET,
        DVA_DOT1X_P12_SECRET_KEY_OFFSET};

    uint32    regAddr = 0;
    uint8     regData = 0;
    uint32    i       = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check port range */
        if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check buffer length */
        if(secretKeyLength > (uint32)DVA_DOT1X_SECRET_KEY_LENGTH)
        {
            retVal = -(RT_ERR_FAILED);
            continue;
        }

        /* Check NULL pointer */
        if(pSecretKey == NULL)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Check input length */
        if(rtl_strlen((const char *)pSecretKey) > ((uint32)DVA_DOT1X_SECRET_KEY_LENGTH - 1u))
        {
            retVal = -(RT_ERR_FAILED);
            continue;
        }
        /* Set secret key */
        regAddr = regAddrOffset[port - 1u];
        for(i = 0; i < ((uint32)secretKeyLength); i++)
        {
            regData = pSecretKey[i];
            retVal  = dva_drv_indirect_sram_1byte_set(unit, regAddr, regData);
            regAddr = regAddr + 1u;
            if(retVal != RT_ERR_OK)
            {
                continue;
            }
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the switch IP address.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pIpAddr      Switch IP address (0 to 255 for 4 elements)
 * @retval RT_ERR_OK            DOT1X switch IP address was gotten
 * @retval -RT_ERR_NULL_POINTER The "pIpAddr" is NULL
 * @retval Others               Please refer to dva_drv_memset(), \n
 *                              dva_drv_indirect_sram_get(), \n
 *                              dva_drv_memcpy()
 *
 */
RtkApiRet dva_dot1x_get_switch_ip(uint32 unit, uint8 pIpAddr[DVA_IPV4_ADDR_LEN])
{
    uint32    regAddr     = 0;
    uint32    regData     = 0;
    uint8     switchIP[4] = {0};
    RtkApiRet retVal      = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pIpAddr == NULL)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get switch IP[0] switch IP[1] */
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(pIpAddr, 0, DVA_IPV4_ADDR_LEN));
        regAddr = DVA_DOT1X_SWITCH_IP_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_get(unit, regAddr, &regData));

        switchIP[0] = (uint8)(regData & 0xFFu);
        switchIP[1] = (uint8)(regData >> 8u) & 0xFFu;
        switchIP[2] = (uint8)(regData >> 16u) & 0xFFu;
        switchIP[3] = (uint8)(regData >> 24u) & 0xFFu;

        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(pIpAddr, switchIP, DVA_IPV4_ADDR_LEN));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set the switch IP address.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] pIpAddr Switch IP address (0 to 255 for 4 elements) \n
 *                    Switch IP address.
 * @param[out] None
 * @retval RT_ERR_OK DOT1X switch IP was set.
 * @retval -RT_ERR_NULL_POINTER The "pIpAddr" is NULL.
 * @retval Others Please refer to dva_drv_memcpy(), \n
 *                dva_drv_indirect_sram_set().
 */
RtkApiRet dva_dot1x_set_switch_ip(uint32 unit, const uint8 pIpAddr[DVA_IPV4_ADDR_LEN])
{
    uint32    regAddr                     = 0;
    uint32    regData                     = 0;
    uint32    ownIP                       = 0;
    uint8     switchIP[DVA_IPV4_ADDR_LEN] = {0};
    RtkApiRet retVal                      = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pIpAddr == NULL)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(switchIP, pIpAddr, DVA_IPV4_ADDR_LEN));
        /* Set switch IP */
        regAddr = DVA_DOT1X_SWITCH_IP_OFFSET;
        regData = (((uint32)switchIP[3] << 24u) | ((uint32)switchIP[2] << 16u)
                   | ((uint32)switchIP[1] << 8u) | (uint32)switchIP[0]);
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_set(unit, regAddr, regData));

        ownIP = (((uint32)switchIP[0] << 24u) | ((uint32)switchIP[1] << 16u)
                 | ((uint32)switchIP[2] << 8u) | (uint32)switchIP[3]);
        CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, DVA_SWITCH_OWN_IP_ADDRESS, ownIP));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the RADIUS server IP address in the switch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pIpAddr IP address (0 to 255 for 4 elements) \n
 * @retval RT_ERR_OK RADIUS server IP address was gotten.
 * @retval -RT_ERR_NULL_POINTER The pointer of "pIpAddr" is NULL.
 * @retval Others Please refer to dva_drv_memset(), \n
 *                dva_drv_indirect_sram_get(), \n
 *                dva_drv_memcpy().
 */
RtkApiRet dva_dot1x_get_radius_server_ip(uint32 unit, uint8 pIpAddr[DVA_IPV4_ADDR_LEN])
{
    uint32    regAddr                        = 0;
    uint32    regData                        = 0;
    uint8     radiusSrvIP[DVA_IPV4_ADDR_LEN] = {0};
    RtkApiRet retVal                         = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pIpAddr == NULL)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get RADIUS server IP */
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(pIpAddr, 0, DVA_IPV4_ADDR_LEN));

        regAddr = (uint32)DVA_DOT1X_RADIUS_SERVER_IP_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_get(unit, regAddr, &regData));

        radiusSrvIP[0] = (uint8)(regData & 0xFFu);
        radiusSrvIP[1] = ((uint8)(regData >> 8u) & 0xFFu);
        radiusSrvIP[2] = ((uint8)(regData >> 16u) & 0xFFu);
        radiusSrvIP[3] = ((uint8)(regData >> 24u) & 0xFFu);

        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(pIpAddr, radiusSrvIP, DVA_IPV4_ADDR_LEN));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set the RADIUS server IP address in the switch.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] pIpAddr IP address (0 to 255 for 4 elements) \n
 *                    Pointer that points to the RADIUS server IP address string.
 * @param[out] None
 * @retval RT_ERR_OK RADIUS server IP was set.
 * @retval -RT_ERR_NULL_POINTER The pointer of "pIpAddr" is NULL.
 * @retval Others Please refer to dva_drv_memcpy(), \n
 *                dva_drv_indirect_sram_set().
 */
RtkApiRet dva_dot1x_set_radius_server_ip(uint32 unit, const uint8 pIpAddr[DVA_IPV4_ADDR_LEN])
{
    uint32    regAddr                        = 0;
    uint32    regData                        = 0;
    uint8     radiusSrvIP[DVA_IPV4_ADDR_LEN] = {0};
    RtkApiRet retVal                         = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pIpAddr == NULL)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(radiusSrvIP, pIpAddr, DVA_IPV4_ADDR_LEN));
        /* Set RADIUS server IP */
        regAddr = DVA_DOT1X_RADIUS_SERVER_IP_OFFSET;
        regData = (((uint32)radiusSrvIP[3] << 24u) | ((uint32)radiusSrvIP[2] << 16u)
                   | ((uint32)radiusSrvIP[1] << 8u) | (uint32)radiusSrvIP[0]);
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_set(unit, regAddr, regData));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get RADIUS UDP port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pPortNum         Port number (0 to 0xFFFF) \n
                                UDP port number
 * @retval RT_ERR_OK            RADIUS UDP port was gotten
 * @retval -RT_ERR_NULL_POINTER The "pPortNum" is NULL
 * @retval Others               Please refer to dva_drv_indirect_sram_2bytes_get()
 *
 */
RtkApiRet dva_dot1x_get_radius_udp_port(uint32 unit, uint16 *pPortNum)
{
    uint32    regAddr = 0;
    uint16    regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pPortNum == NULL)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get RADIUS UDP port */
        regAddr = DVA_DOT1X_RADIUS_UDP_PORT_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_2bytes_get(unit, regAddr, &regData));
        *pPortNum = regData;
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set RADIUS UDP port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portNum   Port number (0 to 0xFFFF) \n
                        UDP port number
 * @param[out] None
 * @retval RT_ERR_OK    RADIUS UDP port was set
 * @retval Others       Please refer to dva_drv_indirect_sram_2bytes_set()
 */
RtkApiRet dva_dot1x_set_radius_udp_port(uint32 unit, uint16 portNum)
{
    uint32    regAddr = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Set RADIUS UDP port */
        regAddr = DVA_DOT1X_RADIUS_UDP_PORT_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_2bytes_set(unit, regAddr, portNum));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get the switch port mask connected to the RADIUS server.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pPortMask            Port mask (0x1 to DVA_RTK_USER_PMAP_ALL) \n
 *                                  Port mask connected to the RADIUS server
 * @retval RT_ERR_OK                Port mask was gotten
 * @retval -RT_ERR_NULL_POINTER     The "pPortMask" is NULL
 * @retval Others                   Please refer to dva_drv_indirect_sram_2bytes_get()
 */
RtkApiRet dva_dot1x_get_radius_server_portmask(uint32 unit, RtkPmap *pPortMask)
{
    uint32    regAddr = 0;
    uint16    regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pPortMask == NULL)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get RADIUS server port mask */
        regAddr = DVA_DOT1X_RADIUS_SERVER_PORT_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_2bytes_get(unit, regAddr, &regData));

        *pPortMask = (uint32)regData;
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set the switch port mask connected to the RADIUS server.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMask          Port mask (0x1 to DVA_RTK_USER_PMAP_ALL) \n
 *                              Port mask connected to the RADIUS server
 * @param[out] None
 * @retval RT_ERR_OK            Port mask was set
 * @retval -RT_ERR_PORT_MASK    The "portMask" is out of range
 * @retval Others               Please refer to dva_drv_indirect_sram_2bytes_set()
 */
RtkApiRet dva_dot1x_set_radius_server_portmask(uint32 unit, RtkPmap portMask)
{
    uint32    regAddr = 0;
    uint16    regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check input */
        if(0u != ((uint16)portMask & (~(DVA_RTK_USER_PMAP_ALL))))
        {
            retVal = -RT_ERR_PORT_MASK;
            continue;
        }
        /* Set RADIUS server port mask */
        regAddr = DVA_DOT1X_RADIUS_SERVER_PORT_OFFSET;
        regData = (uint16)portMask;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_2bytes_set(unit, regAddr, regData));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Set the method for searching the RADIUS server.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] status Status (0 to 2) \n
 *                   The method of searching for a RADIUS server.
 * @param[out] None
 * @retval RT_ERR_OK The method of searching for a RADIUS server was set.
 * @retval -RT_ERR_FAILED The "status" is out of range.
 * @retval Others Please refer to dva_drv_indirect_sram_1byte_get(), \n
 *                dva_drv_indirect_sram_1byte_set().
 */
RtkApiRet dva_dot1x_set_arp_port_select(uint32 unit, uint32 status)
{
    uint32    regAddr = 0;
    uint8     regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check the input value */
        if((status != DVA_ARP_TO_ALLPORT) && (status != DVA_ARP_TO_DOT1X_CONFIG) && (status != DVA_ARP_TO_RADIUS_CONFIG))
        {
            retVal = -(RT_ERR_FAILED);
            continue;
        }
        /* Clear the data */
        regAddr = (uint32)DVA_DOT1X_SET_ARP_PORT_SELECT;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_get(unit, regAddr, &regData));

        regData &= 0x00u;

        /* Set the status */
        if(status == DVA_ARP_TO_DOT1X_CONFIG)
        {
            regData |= 0x2u;
        }
        else if(status == DVA_ARP_TO_RADIUS_CONFIG)
        {
            regData |= 0x1u;
        }
        else if(status == DVA_ARP_TO_ALLPORT)
        {
            regData &= 0x0u;
        }
        else
        {
            /* Do nothing */
        }
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_set(unit, regAddr, regData));

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the method used for searching the RADIUS server.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pStatus                  Status (0 to 2) \n
 *                                      The method of searching RADIUS server
 * @retval RT_ERR_OK                    The method of searching RADIUS server was gotten
 * @retval -RT_ERR_NULL_POINTER         The "pStatus" is NULL
 * @retval Others                       Please refer to dva_drv_indirect_sram_1byte_get()
 */
RtkApiRet dva_dot1x_get_arp_port_select(uint32 unit, uint32 *pStatus)
{
    uint32    regAddr = 0;
    uint8     regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pStatus == NULL)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get the select */
        regAddr = DVA_DOT1X_SET_ARP_PORT_SELECT;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_get(unit, regAddr, &regData));

        if((regData & 0xFFu) == 0x1u)
        {
            *pStatus = DVA_ARP_TO_RADIUS_CONFIG;
        }
        else if((regData & 0xFFu) == 0x2u)
        {
            *pStatus = DVA_ARP_TO_DOT1X_CONFIG;
        }
        else if((regData & 0xFFu) == 0x0u)
        {
            *pStatus = DVA_ARP_TO_ALLPORT;
        }
        else
        {
            /* Do nothing */
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the dynamic flag to trigger the DOT1X configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The dynamic flag was set.
 * @retval Others Please refer to dva_drv_indirect_sram_1byte_get(), \n
 *                dva_drv_indirect_sram_1byte_set().
 */
RtkApiRet dva_dot1x_set_dynamic_reset_flag(uint32 unit)
{
    uint32    regAddr = 0;
    uint8     regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Clear the data */
        regAddr = (uint32)DVA_DOT1X_DYNAMIC_RESET_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_get(unit, regAddr, &regData));

        regData &= 0x00u;
        /* Set flag */
        regData |= 0x1u;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_set(unit, regAddr, regData));

    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* HANDSHAKE_CFG_ASIC */
/**@}*/ /* HANDSHAKE_CFG */

/**
 * @defgroup DOT1X_AGING_TIME DOT1X Aging Time
 * @{
 */
/**
 * @addtogroup AGING_ASIC Low Level Driver
 * @brief DOT1X Low Level Driver
 * @{
 */
/**
 * @brief Get the enable status of the aging function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pStatus                  Status (0 to 1) \n
 *                                      The enable status of aging function
 * @retval RT_ERR_OK                    Enable status of the aging function was gotten
 * @retval -RT_ERR_NULL_POINTER         The "pStatus" is NULL
 * @retval Others                       Please refer to dva_drv_indirect_sram_1byte_get()
 */
RtkApiRet dva_dot1x_get_aging_time_enable(uint32 unit, uint32 *pStatus)
{
    uint32    regAddr = 0;
    uint8     regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pStatus == NULL)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get status */
        regAddr = DVA_DOT1X_AGING_ENABLE_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_get(unit, regAddr, &regData));

        if((regData & 0xFFu) == 0x1u)
        {
            *pStatus = (uint32)DVA_ENABLED;
        }
        else
        {
            *pStatus = (uint32)DVA_DISABLED;
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set the enable status of the aging function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] status Status (0 to 1) \n
 *                   The enable status of the aging function.
 * @param[out] None
 * @retval RT_ERR_OK Enable status of the aging function was set.
 * @retval -RT_ERR_FAILED The "status" is out of range.
 * @retval Others Please refer to dva_drv_indirect_sram_1byte_get(), \n
 *                dva_drv_indirect_sram_1byte_set().
 */
RtkApiRet dva_dot1x_set_aging_time_enable(uint32 unit, uint32 status)
{
    uint32    regAddr = 0;
    uint8     regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check input value */
        if((status != (uint32)DVA_ENABLED) && (status != (uint32)DVA_DISABLED))
        {
            retVal = -(RT_ERR_FAILED);
            continue;
        }
        /* Clear the data */
        regAddr = DVA_DOT1X_AGING_ENABLE_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_get(unit, regAddr, &regData));

        regData &= 0x00u;
        /* Set the status */
        if(status == (uint32)DVA_ENABLED)
        {
            regData |= 0x1u;
        }
        else
        {
            regData |= 0x0u;
        }

        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_set(unit, regAddr, regData));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get the value of aging time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pAgingTime           Aging Time (0 to 0xFFFF) \n
 *                                  The value of aging timer
 * @retval RT_ERR_OK                The value of aging time was gotten
 * @retval -RT_ERR_NULL_POINTER     The "pAgingTime" is NULL
 * @retval Others                   Please refer to dva_drv_indirect_sram_2bytes_get()
 */
RtkApiRet dva_dot1x_get_aging_time(uint32 unit, uint16 *pAgingTime)
{
    uint32    regAddr = 0;
    uint16    regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pAgingTime == NULL)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get aging time */
        regAddr = DVA_DOT1X_AGING_TIME_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_2bytes_get(unit, regAddr, &regData));

        *pAgingTime = regData;
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set the value of aging time.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] agingTime         Aging Time (0 to 0xFFFF) \n
 *                              The value of aging timer
 * @param[out] None
 * @retval RT_ERR_OK            The value of aging time was set
 * @retval Others               Please refer to dva_drv_indirect_sram_2bytes_set()
 */
RtkApiRet dva_dot1x_set_aging_time(uint32 unit, uint16 agingTime)
{
    uint32    regAddr = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Set aging time */
        regAddr = DVA_DOT1X_AGING_TIME_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_2bytes_set(unit, regAddr, agingTime));

    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* AGING_ASIC */
/**@}*/ /* DOT1X_AGING_TIME */

/**
 * @addtogroup FORCE_UNAUTH Force unauth
 * @{
 */
/**
 * @addtogroup FORCE_UNAUTH_ASIC Low Level Driver
 * @brief DOT1X Low Level Driver
 * @{
 */

/**
 * @brief Set the enable status of the force unauthorized function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] status Status (0 to 1) \n
 *                   The enable status of the force unauthorized function.
 * @param[out] None
 * @retval RT_ERR_OK Enable status of the force unauthorized function was set.
 * @retval -RT_ERR_FAILED The "status" is out of range.
 * @retval Others Please refer to dva_drv_indirect_sram_1byte_get(), \n
 *                dva_drv_indirect_sram_1byte_set().
 */
RtkApiRet dva_dot1x_set_force_unauth_enable(uint32 unit, uint32 status)
{
    uint32    regAddr = 0;
    uint8     regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check the input value */
        if((status != (uint32)DVA_ENABLED) && (status != (uint32)DVA_DISABLED))
        {
            retVal = -(RT_ERR_FAILED);
            continue;
        }
        /* Clear the data */
        regAddr = (uint32)DVA_DOT1X_FORCE_UNAUTH_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_get(unit, regAddr, &regData));

        regData &= 0x00u;
        /* Set the status */
        if(status == (uint32)DVA_ENABLED)
        {
            regData |= 0x1u;
        }

        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_set(unit, regAddr, regData));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get the enable status of force unauthorized function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pStatus                 Status (0 to 1) \n
 *                                     The enable status of force unauthorized function
 * @retval RT_ERR_OK                   The enable status of force unauthorized function was gotten
 * @retval -RT_ERR_NULL_POINTER        The "pStatus" is NULL
 * @retval Others                      Please refer to dva_drv_indirect_sram_1byte_get()
 */
RtkApiRet dva_dot1x_get_force_unauth_enable(uint32 unit, uint32 *pStatus)
{
    uint32    regAddr = 0;
    uint8     regData = 0;
    RtkApiRet retVal  = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(pStatus == NULL)
        {
            retVal = -(RT_ERR_NULL_POINTER);
            continue;
        }
        /* Get the status */
        regAddr = DVA_DOT1X_FORCE_UNAUTH_OFFSET;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_sram_1byte_get(unit, regAddr, &regData));

        if(regData == 0x1u)
        {
            *pStatus = (uint32)DVA_ENABLED;
        }
        else
        {
            *pStatus = (uint32)DVA_DISABLED;
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* FORCE_UNAUTH_ASIC */
/**@}*/ /* FORCE_UNAUTH */

/**
 * @addtogroup DOT1X_RESET DOT1X register reset
 * @{
 */
/**
 * @addtogroup DOT1X_RESET_ASIC Low Level Driver
 * @brief DOT1X Low Level Driver
 * @{
 */

/**
 * @brief Reset each port to the default state.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK        The DOT1X register was reset to default value
 * @retval Others           Please refer to dva_reg_write()
 */
RtkApiRet dva_dot1x_hw_reset(uint32 unit)
{
    uint32    portId;
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Initialize the guest VLAN control register */
        CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_DEFAULTVAL));
        CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL1, DVA_GUEST_VLAN_GLOBAL_CONTROL1_DEFAULTVAL));
        /* Initialize the DOT1X network access related control register */
        CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, DVA_DOT1X_NETWORK_ACCESS_RELATED_CONTROL, DVA_DOT1X_NETWORK_ACCESS_RELATED_CONTROL_DEFAULTVAL));

        for(portId = DVA_RTK_SYS_PORT_1ST; portId <= DVA_RTK_SYS_PORT_ID_MAX; portId++)
        {
            /* Initialize the port# based DOT1X control register */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_BASED_DOT1X_CONTROL + (portId * 0x800u), 0, 32u, DVA_PORT0_BASED_DOT1X_CONTROL_DEFAULTVAL));
            /* Initialize the port# egress DOT1X control register */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_EGRESS_DOT1X_CONTROL + (portId * 0x800u), 0, 32u, DVA_PORT0_EGRESS_DOT1X_CONTROL_DEFAULTVAL));
            /* Initialize the port# MAC based DOT1X control register */
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_MAC_BASED_DOT1X_CONTROL + (portId * 0x800u), 0, 32u, DVA_PORT0_MAC_BASED_DOT1X_CONTROL_DEFAULTVAL));
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* DOT1X_RESET_ASIC */
/**@}*/ /* DOT1X_RESET */

/**
 * @addtogroup INGRESS_FILTER Ingress Port-based DOT1X Filter
 * @{
 */
/**
 * @addtogroup INGRESS_FILTER_ASIC Low Level Driver
 * @brief DOT1X Low Level Driver
 * @{
 */

/**
 * @brief Set the action configuration for unauthorized packets.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] pktType               Packet type (DVA_EM_DOT1X_PKT_INTAG to DVA_EM_DOT1X_PKT_OUTUNTAG) \n
 *                                  The type of packet
 * @param[in] unauthAction          Unauthorized packet action (DVA_EM_DOT1X_ACTION_DROP to DVA_EM_DOT1X_ACTION_GUESTVLAN) \n
 *                                  Indicate unauthorized packet action
 * @param[out] None
 * @retval RT_ERR_OK                DOT1X unauthorized action configuration was set
 * @retval -RT_ERR_INPUT            Input wrong parameters to sub function
 * @retval Others                   Please refer to dva_reg_field_write()
 */
RtkApiRet dva_dot1x_unauth_packet_oper_set(uint32 unit, uint32 pktType, uint32 unauthAction)
{

    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Set DOT1X unauth action configuration */
        if((pktType > DVA_DOT1X_PKT_END) || (unauthAction > DVA_DOT1X_ACTION_END))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if(DVA_DOT1X_PKT_INTAG == pktType)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_INTAGDOT1XUNAUTHBH_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_INTAGDOT1XUNAUTHBH_LEN, unauthAction));
        }
        if(DVA_DOT1X_PKT_INUNTAG == pktType)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_INUNTAGDOT1XUNAUTHBH_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_INUNTAGDOT1XUNAUTHBH_LEN, unauthAction));
        }
        if(DVA_DOT1X_PKT_OUTTAG == pktType)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_OUTTAGDOT1XUNAUTHBH_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_OUTTAGDOT1XUNAUTHBH_LEN, unauthAction));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_OUTUNTAGDOT1XUNAUTHBH_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_OUTUNTAGDOT1XUNAUTHBH_LEN, unauthAction));
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get the action configuration for unauthorized packets.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] pktType                   Packet type (DVA_EM_DOT1X_PKT_INTAG to DVA_EM_DOT1X_PKT_OUTUNTAG) \n
 *                                      The type of packet
 * @param[out] pUnauthAction            Unauthorized packet action (DVA_EM_DOT1X_ACTION_DROP to DVA_EM_DOT1X_ACTION_GUESTVLAN) \n
 *                                      Indicate unauthorized packet action
 * @retval RT_ERR_OK                    DOT1X unauthorized action configuration was gotten
 * @retval -RT_ERR_NULL_POINTER         The "pUnauthAction" is NULL
 * @retval -RT_ERR_INPUT                The "pktType" is out of range
 * @retval Others                       Please refer to dva_reg_field_read()
 */
RtkApiRet dva_dot1x_unauth_packet_oper_get(uint32 unit, uint32 pktType, uint32 *pUnauthAction)
{
    uint32    val;
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(NULL == pUnauthAction)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(pktType > DVA_DOT1X_PKT_END)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Get DOT1X unauth action configuration */
        if(DVA_DOT1X_PKT_INTAG == pktType)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_INTAGDOT1XUNAUTHBH_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_INTAGDOT1XUNAUTHBH_LEN, &val));
        }
        if(DVA_DOT1X_PKT_INUNTAG == pktType)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_INUNTAGDOT1XUNAUTHBH_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_INUNTAGDOT1XUNAUTHBH_LEN, &val));
        }
        if(DVA_DOT1X_PKT_OUTTAG == pktType)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_OUTTAGDOT1XUNAUTHBH_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_OUTTAGDOT1XUNAUTHBH_LEN, &val));
        }
        if(DVA_DOT1X_PKT_OUTUNTAG == pktType)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_OUTUNTAGDOT1XUNAUTHBH_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_OUTUNTAGDOT1XUNAUTHBH_LEN, &val));
        }
        *pUnauthAction = val;
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get port-based DOT1X authorized status.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                      Port number (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
                                        Port Number
 * @param[out] pPortAuth                Authentication status (0 to 1) \n
                                        Authentication status of DOT1X port
 * @retval RT_ERR_OK                    Port-based DOT1X authorized status was gotten
 * @retval -RT_ERR_PORT_ID              The "port" is out of range
 * @retval -RT_ERR_NULL_POINTER         The "pPortAuth" is NULL
 * @retval Others                       Please refer to dva_drv_indirect_reg_field_get()
 */
RtkApiRet dva_dot1x_portbased_auth_get(uint32 unit, RtkPort port, uint32 *pPortAuth)
{
    uint32    val;
    RtkApiRet retVal = RT_ERR_OK;
    uint32    wPort  = 0u;
    do
    {
        /* Check port validity */
        if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == pPortAuth)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* UserPort2SysPort */
        wPort = dva_g_userPort2sysPort[port];
        /* Get the port# based DOT1X control register */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_BASED_DOT1X_CONTROL + (wPort * 0x800u), DVA_PORT0_BASED_DOT1X_CONTROL_DOT1XPORTAUTH_LSP, DVA_PORT0_BASED_DOT1X_CONTROL_DOT1XPORTAUTH_LEN, &val));
        if((uint32)DVA_DOT1X_UNAUTH == val)
        {
            *pPortAuth = DVA_DOT1X_UNAUTH;
        }
        else if((uint32)DVA_DOT1X_AUTH == val)
        {
            *pPortAuth = DVA_DOT1X_AUTH;
        }
        else
        {
            /* Intentionally left blank */
        }

    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set DOT1X guest VLAN entry configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] vid Guest VLAN ID (0 to 4095) \n
 *                DOT1X guest VLAN ID.
 * @param[in] pVlanEntry Guest VLAN struct member pointer \n
 * @param[out] None
 * @retval RT_ERR_OK DOT1X guest VLAN configuration was set.
 * @retval -RT_ERR_VLAN_VID The "vid" is out of range.
 * @retval -RT_ERR_NULL_POINTER The "pVlanEntry" is NULL.
 * @retval -RT_ERR_DOT1X_GUEST_VLAN_ENTRY_EXIST DOT1X guest VLAN entry exists.
 * @retval Others Please refer to dva_drv_memset(), \n
 *                dva_reg_field_write(), \n
 *                dva_drv_vlan_entry2tblparam(), \n
 *                dva_drv_vlan_tbl_entry_get(), \n
 *                dva_drv_vlan_tbl_entry_set().
 */
RtkApiRet dva_dot1x_guest_vlan_entry_set(uint32 unit, RtkVlan vid, VlanEntry_t *pVlanEntry)
{
    RtkApiRet          retVal = RT_ERR_OK;
    dva_VlanTblParam_t vlanParam;
    dva_VlanTblParam_t vlanParamTemp;
    do
    {
        /* Check if the input parameters are valid */
        if(vid > DVA_VLAN_VID_MAX)
        {
            /* Set retVal to -RT_ERR_VLAN_VID */
            retVal = -RT_ERR_VLAN_VID;
            continue;
        }
        /* Check if the input parameters are valid */
        if((NULL == pVlanEntry))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Initialize vlanEntry */
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&vlanParam, 0, sizeof(dva_VlanTblParam_t)));
        /* Initialize vlanEntryTemp */
        CHK_FUN_CONTINUE(retVal, dva_drv_memset(&vlanParamTemp, 0, sizeof(dva_VlanTblParam_t)));
        /* Set guest VLAN ID: set the guest VLAN global control register1 to vid */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_GVLANID_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_GVLANID_LEN, vid));
        /* Convert VLAN entry to VLAN parameters */
        CHK_FUN_RET(retVal, dva_drv_vlan_entry2tblparam(unit, pVlanEntry, &vlanParam));
        /* Get VLAN table entry: call dva_drv_vlan_tbl_entry_get() with "vid" to get vlanEntry */
        CHK_FUN_CONTINUE(retVal, dva_drv_vlan_tbl_entry_get(unit, vid, &vlanParamTemp));
        /* Check if vlanEntry is invalid */
        if(0u == vlanParamTemp.valid)
        {
            /* Set VLAN entry valid to 1 */
            vlanParam.valid = 1u;
            /* Set VLAN entry X to trigger HW to set guest VLAN vid entry */
            CHK_FUN_CONTINUE(retVal, dva_drv_vlan_tbl_entry_set(unit, vid, &vlanParam));
        }
        else
        {
            /* vlanEntry is valid */
            /* Check if vlanParamTemp is equal to vlanParam */
            if(0 == rtl_memcmp(&vlanParamTemp, &vlanParam, sizeof(dva_VlanTblParam_t)))
            {
                /* Set VLAN entry valid to 1 */
                vlanParam.valid = 1u;
                /* Set VLAN entry vid again to trigger HW to set guest VLAN vid entry */
                CHK_FUN_CONTINUE(retVal, dva_drv_vlan_tbl_entry_set(unit, vid, &vlanParam));
            }
            else
            {
                /* Set retVal to -RT_ERR_DOT1X_GUEST_VLAN_ENTRY_EXIST(=0x0011000A) */
                retVal = -RT_ERR_DOT1X_GUEST_VLAN_ENTRY_EXIST;
                /* Loop continue */
                continue;
            }
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get DOT1X guest VLAN entry configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pVid VLAN ID (0 to 4095) \n
 *                  DOT1X guest VLAN ID.
 * @param[out] pVlanEntry Guest VLAN struct member pointer \n
 * @retval RT_ERR_OK DOT1X guest VLAN configuration was gotten.
 * @retval -RT_ERR_NULL_POINTER The pointer of "pVid" or "pVlanEntry" is NULL.
 * @retval Others Please refer to dva_drv_memset(), \n
 *                dva_reg_field_read(), \n
 *                dva_drv_vlan_tbl_entry_get(), \n
 *                dva_drv_vlan_tblparam2entry().
 */
RtkApiRet dva_dot1x_guest_vlan_entry_get(uint32 unit, RtkVlan *pVid, VlanEntry_t *pVlanEntry)
{
    RtkApiRet          retVal = RT_ERR_OK;
    dva_VlanTblParam_t vlanParam;
    do
    {
        /* Check if the input parameters are valid */
        if((NULL == pVlanEntry))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Initialize pVlanEntryTemp */
        (void)dva_drv_memset(&vlanParam, 0, sizeof(dva_VlanTblParam_t));
        /* Get the guest VLAN global control register1 */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL0, DVA_GUEST_VLAN_GLOBAL_CONTROL0_GVLANID_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL0_GVLANID_LEN, pVid));

        /* Get VLAN entry vid again to trigger HW to set guest VLAN vid entry */
        CHK_FUN_CONTINUE(retVal, dva_drv_vlan_tbl_entry_get(unit, *pVid, &vlanParam));
        /* Check if VLAN entry valid is 0 */
        if(vlanParam.valid == 0u)
        {
            /* Set retVal to -RT_ERR_VLAN_ENTRY_NOT_FOUND */
            retVal = -RT_ERR_VLAN_ENTRY_NOT_FOUND;
            continue;
        }
        else
        {
            /* Convert the table parameters to the elements of the VLAN entry */
            retVal = dva_drv_vlan_tblparam2entry(unit, &vlanParam, pVlanEntry);
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get DOT1X CPU port mask.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] cpuPort               CPU port mask (0x1 to DVA_RTK_USER_PMAP_ALL) \n
 *                                   CPU port mask when packets trapped
 * @retval RT_ERR_OK                 DOT1X CPU port mask was set
 * @retval -RT_ERR_NULL_POINTER      The "cpuPort" is NULL
 * @retval Others                    Please refer to dva_reg_field_read()
 */
RtkApiRet dva_dot1x_cpuportmask_get(uint32 unit, RtkPmap *cpuPort)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(NULL == cpuPort)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get the guest VLAN global control register1 */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL1, DVA_GUEST_VLAN_GLOBAL_CONTROL1_DOT1XCPUMASK_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL1_DOT1XCPUMASK_LEN, cpuPort));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set DOT1X CPU port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] cpuPort                   CPU port mask (0x1 to DVA_RTK_USER_PMAP_ALL) \n
 *                                      CPU port mask when packets trapped
 * @param[out] None
 * @retval RT_ERR_OK                    DOT1X CPU port configuration was gotten
 * @retval -RT_ERR_PORT_MASK            The "cpuPort" is out of range
 * @retval Others                       Please refer to dva_reg_field_write()
 *
 */
RtkApiRet dva_dot1x_cpuportmask_set(uint32 unit, RtkPmap cpuPort)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input range */
        if(0u != (((uint16)(cpuPort)) & (~(DVA_RTK_USER_PMAP_ALL))))
        {
            retVal = -RT_ERR_PORT_MASK;
            continue;
        }
        /* Set the guest VLAN global control register1 */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_GUEST_VLAN_GLOBAL_CONTROL1, DVA_GUEST_VLAN_GLOBAL_CONTROL1_DOT1XCPUMASK_LSP, DVA_GUEST_VLAN_GLOBAL_CONTROL1_DOT1XCPUMASK_LEN, cpuPort));
    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* INGRESS_FILTER_ASIC */
/**@}*/ /* INGRESS_FILTER */

/**
 * @addtogroup CONTROL_DIRECTION Port-based DOT1X control direction
 * @{
 */
/**
 * @addtogroup CONTROL_DIRECTION_ASIC Low Level Driver
 * @brief DOT1X Low Level Driver
 * @{
 */

/**
 * @brief Set port-based DOT1X  control direction configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                      Port number (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
                                        Port Number
 * @param[in] portDirection             Control direction (DVA_EM_BOTH to DVA_EM_DIR_IN) \n
                                        Port-based DOT1X  control direction
 * @param[out] None
 * @retval RT_ERR_OK                    Port-based DOT1X  operational direction configuration was set
 * @retval RT_ERR_PORT_ID               The "port" is out of range
 * @retval RT_ERR_DOT1X_PORTBASEDOPDIR  The "portDirection" is out of range
 * @retval Others                       Please refer to dva_drv_indirect_reg_field_set()
 */
RtkApiRet dva_dot1x_portbased_direction_set(uint32 unit, RtkPort port, uint32 portDirection)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    wPort  = 0u;
    do
    {
        /* Check port validity */
        if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check input */
        if(DVA_DOT1X_DIRECTION_END < portDirection)
        {
            retVal = -(RT_ERR_DOT1X_PORTBASEDOPDIR);
            continue;
        }
        /* UserPort2SysPort */
        wPort = dva_g_userPort2sysPort[port];
        /* Set the port# egress DOT1X control register */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_EGRESS_DOT1X_CONTROL + (wPort * 0x800u), DVA_PORT0_EGRESS_DOT1X_CONTROL_DOT1XOPDIR_LSP, DVA_PORT0_EGRESS_DOT1X_CONTROL_DOT1XOPDIR_LEN, portDirection));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief  Get port-based DOT1X control direction configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                          Port number (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END) \n
                                            Port Number
 * @param[out] pPortDirection               Control direction (DVA_EM_BOTH to DVA_EM_DIR_IN) \n
                                            Port-based DOT1X control direction
 * @retval RT_ERR_OK                        Port-based DOT1X control direction configuration was gotten.
 * @retval -RT_ERR_PORT_ID                  The "port" is out of range
 * @retval -RT_ERR_NULL_POINTER             The "pPortDirection" is NULL
 * @retval Others                           Please refer to dva_drv_indirect_reg_field_get()
 */
RtkApiRet dva_dot1x_portbased_direction_get(uint32 unit, RtkPort port, uint32 *pPortDirection)
{
    uint32    val;
    RtkApiRet retVal = RT_ERR_OK;
    uint32    wPort  = 0;
    do
    {
        /* Check port validity */
        if((port < DVA_RTK_USER_PORT_1ST) || (port > DVA_RTK_USER_PORT_END))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }
        /* Check input */
        if(NULL == pPortDirection)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* UserPort2SysPort */
        wPort = dva_g_userPort2sysPort[port];
        /* Get the port# egress DOT1X control register */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_EGRESS_DOT1X_CONTROL + (wPort * 0x800u), DVA_PORT0_EGRESS_DOT1X_CONTROL_DOT1XOPDIR_LSP, DVA_PORT0_EGRESS_DOT1X_CONTROL_DOT1XOPDIR_LEN, &val));

        *pPortDirection = val;

    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* CONTROL_DIRECTION_ASIC */
/**@}*/ /* CONTROL_DIRECTION */
/**@}*/ /* DOT1X */
