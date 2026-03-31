/*
 * Copyright (c) 2024 Realtek Semiconductor Corp. All rights reserved.
 *
 * SPDX-License-Identifier: LicenseRef-Realtek-Proprietary
 *
 * This software component is confidential and proprietary to Realtek
 * Semiconductor Corp. Disclosure, reproduction, redistribution, in whole
 * or in part, of this work and its derivatives without express permission
 * is prohibited.
 */

#include "rtl907xc_va_asicdrv_phy.h"
#include "rtl907xc_va_asicdrv_opfsm.h"
#include "rtl907xc_va_api_opfsm.h"
#include "rtkas_api_opfsm.h"

/**
 * @addtogroup RTK_MODULE_OPFSM Sleep and Wakeup
 * @{
 */

/* The mapping from MAC port to reg address of local host command mode
 *                                    local cmd reg addr of op index    p0      p1      p2      p3      p4      p5      p8      p9      --    p6      p7      --  */
static const uint32 cva_g_sysPortToLocalCmd[CVA_RTK_SYS_PORT_ID_MAX] = {0x920u, 0x922u, 0x924u, 0x926u, 0x928u, 0xA4Cu, 0xA52u, 0xA54u, 0x0u, 0xA4Eu, 0xA50u, 0x0u};
/* The mapping from MAC port to reg address of port px wakeup config
 *                                   local px cmd reg addr of op index    p0      p1      p2      p3      p4      p5      p8      p9      --    p6      p7      --  */
static const uint32 cva_g_sysPortToLocalPxCmd[CVA_RTK_SYS_PORT_ID_MAX] = {0x826u, 0x828u, 0x82Au, 0x82Cu, 0x82Eu, 0x830u, 0x836u, 0x838u, 0x0u, 0x832u, 0x834u, 0x0u};
/* For switch Wake-out Forwarding. The mapping from API parameter "configIdx" to GPIO index */
static const uint8 cva_g_wakeoConfigIdxToGpioIdx[2] = {OP_WAKEO_GPIOB1_ID, OP_WAKEO_GPIOB2_ID};

/**
 * @addtogroup OPFSM_SLEEP_PRECONFIG_SLPCAP PHY Sleep Capability
 * @{
 */

/**
 * @defgroup OPFSM_SLEEP_PRECONFIG_SLPCAP_ASIC Low Level Driver
 * @brief Sleep capability configuration Low Level Driver
 * @{
 */

/**
 * @brief Retrieve the port map associated with the specified sleep capability.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sleepCap Sleep capability (DISABLE or ENABLE).\n
 *                     The specified sleep capability.
 * @param[out] pPortMap Pointer to the sleep capability port map (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      The port map with its physical interface configured to the specific sleep capability.\n
 *                      The valid bits in *pPortMap are:
 *                      - bit1: Set port1 with the physical interface index phy0 (1000BASE-T1)
 *                      - bit2: Set port2 with the physical interface index phy1 (1000BASE-T1)
 *                      - bit3: Set port3 with the physical interface index phy2 (1000BASE-T1)
 *                      - bit4: Set port4 with the physical interface index phy3 (1000BASE-T1)
 *                      - bit5: Set port5 with the physical interface index phy4 (1000BASE-T1)
 *                      - bit6: Set port6 with the physical interface index phy5 (1000BASE-T1)
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer *pPortMap is NULL.
 * @retval Other error codes as defined in cva_reg_read().
 *
 * @note The PHY of port7, port8, port9, port10, port11, and port12 do not support sleep capability configuration.
 *
 * @see cva_op_set_phy_sleep_cap(uint32 unit, RtkPmap portMap, uint8 sleepCapOn).
 */
RtkApiRet cva_op_get_phy_sleep_cap(uint32 unit, RtkEnable sleepCap, RtkPmap *pPortMap)
{
    uint32    regOffset = 0u;
    uint32    regValue  = 0u;
    RtkPort   userPort  = 0u;
    RtkApiRet retVal    = RT_ERR_OK;
    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        else
        {
            *pPortMap = 0x0u;
        }
        /* Check input parameters */
        if((CVA_ENABLED != sleepCap) && (CVA_DISABLED != sleepCap))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* get sleep capability of all TC10-compatible ports */
        for(userPort = CVA_OP_T1_USER_PORT_MIN; userPort <= CVA_RTK_USER_PORT_WITHT1PHY_ID_MAX; userPort++)
        {
            /* check the PHY type of each port */
#ifdef CVA_OP_RTPGE_USER_PORTMAP
            if(0x0u != (CVA_OP_RTPGE_USER_PORTMAP & (1u << userPort)))
            {
                /* get 1000Base-T1 sleep capability */
                regOffset = ((uint32)(cva_g_sysPortToOcpAddr[cva_g_userPort2sysPort[userPort]] | CVA_OCP_SLP_CAP_GIGAPHY)) << 1u;
            }
#endif
#ifdef CVA_OP_BWPHY_USER_PORTMAP
            else if(0x0u != (CVA_OP_BWPHY_USER_PORTMAP & (1u << userPort)))
            {
                /* get 100Base-T1 sleep capability */
                regOffset = ((uint32)(cva_g_sysPortToOcpAddr[cva_g_userPort2sysPort[userPort]] | CVA_OCP_SLP_CAP_100T1)) << 1u;
            }
#endif
            else
            {
                continue;
            }
            /* Read the bit0 of the PHY's sleep capability register */
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, regOffset, &regValue));
            if(sleepCap == (regValue & 0x1u))
            {
                *pPortMap |= ((RtkPort)1u << userPort);
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Enable or disable the sleep capability of the PHY for ports 1 to 6.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap Sleep capability port map (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    The port map for configuring the physical interface sleep capability of selected ports.\n
 *                    The valid bits in portMap are:
 *                    - bit1: Set port1 with the physical interface index phy0 (1000BASE-T1)
 *                    - bit2: Set port2 with the physical interface index phy1 (1000BASE-T1)
 *                    - bit3: Set port3 with the physical interface index phy2 (1000BASE-T1)
 *                    - bit4: Set port4 with the physical interface index phy3 (1000BASE-T1)
 *                    - bit5: Set port5 with the physical interface index phy4 (1000BASE-T1)
 *                    - bit6: Set port6 with the physical interface index phy5 (1000BASE-T1)
 * @param[in] sleepCap Sleep capability setting (DISABLE or ENABLE).\n
 *                     Enable or disable the sleep capability.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input port map or sleepCap is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_phy_1000mt1_phy_sleep_cap_set(), and cva_drv_phy_100mt1_phy_sleep_cap_set().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot be in sleep mode.
 */
RtkApiRet cva_op_set_phy_sleep_cap(uint32 unit, RtkPmap portMap, RtkEnable sleepCap)
{
    uint32    regOffset    = 0u;
    uint32    regOAMOffset = 0u;
    RtkPmap   dstPmap      = 0u;
    RtkPort   userPortId   = 0u;
    RtkApiRet retVal       = RT_ERR_OK;
    do
    {
        /* Check input parameters */
        if((CVA_ENABLED != sleepCap) && (CVA_DISABLED != sleepCap))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Check the ports in the input user port map are all with T1-PHY interface */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Search the user port to be configured according to the input portMap */
        for(userPortId = CVA_OP_T1_USER_PORT_MIN; userPortId <= CVA_RTK_USER_PORT_WITHT1PHY_ID_MAX; userPortId++)
        {
            if(0u == (((uint32)1u << userPortId) & portMap))
            {
                continue;
            }

            /* Set the PHY's sleep capability register according to the type is 1000Base-T1 or 100Base-T1*/
#ifdef CVA_OP_RTPGE_USER_PORTMAP
            if(0x0u != (CVA_OP_RTPGE_USER_PORTMAP & (1u << userPortId)))
            {
                /* 1000Base-T1 */
                regOffset    = ((uint32)(cva_g_sysPortToOcpAddr[cva_g_userPort2sysPort[userPortId]] | CVA_OCP_SLP_CAP_GIGAPHY)) << 1u;
                regOAMOffset = ((uint32)(cva_g_sysPortToOcpAddr[cva_g_userPort2sysPort[userPortId]] | CVA_OCP_OAM_CAP_GIGAPHY)) << 1u;
                CHK_FUN_CONTINUE(retVal, cva_drv_phy_1000mt1_phy_sleep_cap_set(unit, regOffset, regOAMOffset, sleepCap));
            }
#endif
#ifdef CVA_OP_BWPHY_USER_PORTMAP
            else if(0x0u != (CVA_OP_BWPHY_USER_PORTMAP & (1u << userPortId)))
            {
                /* 100Base-T1 */
                regOffset = ((uint32)(cva_g_sysPortToOcpAddr[cva_g_userPort2sysPort[userPortId]] | CVA_OCP_SLP_CAP_100T1)) << 1u;
                CHK_FUN_CONTINUE(retVal, cva_drv_phy_100mt1_phy_sleep_cap_set(unit, regOffset, sleepCap));
            }
#endif
            else
            {
                continue;
            }
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_SLEEP_PRECONFIG_SLPCAP_ASIC */
/**@}*/ /* OPFSM_SLEEP_PRECONFIG_SLPCAP */

/**
 * @addtogroup OPFSM_SLEEP_PRECONFIG_ACCEPT_SLP_CAP The accept sleep request function configuration
 * @{
 */

/**
 * @defgroup OPFSM_SLEEP_PRECONFIG_ACCEPT_SLP_CAP_ASIC Low Level Driver
 * @brief The accept sleep request function configuration Low Level Driver
 * @{
 */

/**
 * @brief Set specific ports (AcceptSleepCmdMsk) to have the permission to accept received sleep requests.
 *        By default, all ports are not allowed to accept received sleep requests, and all ports will reject sleep requests from the link partner.\n
 *        To accept a sleep request, first ensure the port link status is active and the port's sleep capability is enabled.
 *        After a port is enabled to accept sleep capability, it can perform a remote sleep handshake with the link partner after receiving a sleep request from the link partner,
 *        and the port's local OPFSM state will change to OP_LOCAL_SLEEP after the sleep handshake is successfully completed.\n
 *        The accept sleep capability configuration only takes effect on the receive side.
 *        For example, if a port is not configured to accept received sleep requests and the sleep capability is enabled (by API set_cva_op_phy_sleep_cap),
 *        this port will reject the sleep request from the link partner, but it can still send sleep requests to the link partner and eventually enter local sleep mode.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map indicating selected ports in AcceptSleepCmdMsk to be configured (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Each bit indicates a port in AcceptSleepCmdMsk to be configured with the permission to accept sleep requests:
 *                    - bit1: Select port1
 *                    - bit2: Select port2
 *                    - bit3: Select port3
 *                    - bit4: Select port4
 *                    - bit5: Select port5
 *                    - bit6: Select port6
 * @param[in] cfgValue Set value (DISABLE or ENABLE).\n
 *                     Enable or disable the accept sleep capability.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input AcceptSleepCmdMsk or cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_indirect_sram_2bytes_get(), and cva_drv_indirect_sram_2bytes_set().
 *
 * @note References:
 *       - cva_op_set_phy_sleep_cap(uint32 unit, RtkPmap portMap, RtkEnable sleepCapOn)
 *       - cva_op_set_txsleep_portmap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue)
 */
RtkApiRet cva_op_set_accept_sleep_req_cap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue)
{
    uint16    value16 = 0u;
    RtkPmap   dstPmap = 0u;
    RtkPort   phyId   = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((CVA_ENABLED != cfgValue) && (CVA_DISABLED != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Get PHY index mask showing the accept sleep capability setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_get(unit, CVA_OP_ACCEPT_SLEEP_CAP, &value16));

        /* Update the accept sleep capability setting */
        for(phyId = 0u; phyId <= CVA_RTK_PHY_ID_MAX; phyId++)
        {
            if(0u == (((uint16)0x1u << phyId) & ((uint16)dstPmap)))
            {
                continue;
            }

            if(CVA_ENABLED == cfgValue)
            {
                /* Set bit value from 0 to 1 */
                value16 = value16 | (((uint16)(0x1u)) << phyId);
            }
            else
            {
                /* Set bit value from 1 to 0 */
                value16 = value16 & ((((uint16)(0x1u)) << phyId) ^ 0xFFFFu);
            }
        }
        /* Write back the accept sleep capability setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_set(unit, CVA_OP_ACCEPT_SLEEP_CAP, value16));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the port map (AcceptSleepCmdMsk) of ports that are enabled or disabled to accept received sleep requests based on cfgValue.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] cfgValue Config value (DISABLE or ENABLE).\n
 *                     Enable or disable the accept sleep capability.
 * @param[out] pPortMap Pointer to the AcceptSleepCmdMsk (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Returns the port map indicating ports enabled to accept received sleep requests:
 *                      - bit1: If bit value is 1, AcceptSleepCmdMsk includes port1
 *                      - bit2: If bit value is 1, AcceptSleepCmdMsk includes port2
 *                      - bit3: If bit value is 1, AcceptSleepCmdMsk includes port3
 *                      - bit4: If bit value is 1, AcceptSleepCmdMsk includes port4
 *                      - bit5: If bit value is 1, AcceptSleepCmdMsk includes port5
 *                      - bit6: If bit value is 1, AcceptSleepCmdMsk includes port6
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer *pPortMap is NULL.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert() and cva_drv_indirect_sram_2bytes_get().
 *
 * @note Reference:
 *       - cva_op_set_accept_sleep_req_cap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue).
 */
RtkApiRet cva_op_get_accept_sleep_req_cap(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap)
{
    uint16    value16 = 0u;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get PHY index mask showing the accept sleep capability setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_get(unit, CVA_OP_ACCEPT_SLEEP_CAP, &value16));
        /* Return the corresponding configuration value based on whether the parameter cfgValue is enable or disable */
        if(CVA_DISABLED == cfgValue)
        {
            value16 = ~value16;
        }
        srcPmap = (RtkPmap)value16;
        srcPmap &= CVA_RTK_T1PHY_PMAP_ALL;
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(srcPmap, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (uint16)dstPmap;
    } while(0u == 1u);
    return retVal;
}

/**@}*/ /* OPFSM_SLEEP_PRECONFIG_ACCEPT_SLP_CAP_ASIC */
/**@}*/ /* OPFSM_SLEEP_PRECONFIG_ACCEPT_SLP_CAP */

/**
 * @addtogroup OPFSM_SLEEP_PRECONFIG_SLP_FWD The Sleep forwarding function configuration
 * @{
 */
/**
 * @defgroup OPFSM_SLEEP_PRECONFIG_SLP_FWD_ASIC Low Level Driver
 * @brief The sleep request forwarding function configuration Low Level Driver
 * @{
 */

/**
 * @brief Set specific ports (TXSleepPortMsk) to transmit sleep requests when any other port on the switch receives a sleep request.
 *        This API only affects the port's capability of transmitting a sleep request.\n
 *        Regardless of whether the port's capability to accept sleep requests is on or off, the ports in the TXSleepPortMsk will transmit sleep requests when any other port receives a sleep request.\n
 *        The port that receives the sleep request will not send the sleep request to its link partner again.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map indicating selected ports in TXSleepPortMsk to be configured (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Each bit indicates a port in TXSleepPortMsk to be configured to enable transmitting a sleep request when any other port receives a sleep request:
 *                    - bit1: Select port1
 *                    - bit2: Select port2
 *                    - bit3: Select port3
 *                    - bit4: Select port4
 *                    - bit5: Select port5
 *                    - bit6: Select port6
 * @param[in] cfgValue Set value (DISABLE or ENABLE).\n
 *                     Enable or disable the port to transmit sleep requests when any other port receives a sleep request.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input TXSleepPortMsk or cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_indirect_sram_2bytes_get(), and cva_drv_indirect_sram_2bytes_set().
 *
 * @note Reference:
 *       - cva_op_set_accept_sleep_req_cap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue).
 */
RtkApiRet cva_op_set_txsleep_portmap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue)
{
    uint16    value16 = 0u;
    RtkPmap   dstPmap = 0u;
    RtkPort   phyId   = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((CVA_ENABLED != cfgValue) && (CVA_DISABLED != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Get PHY index mask showing the transmitting sleep capability setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_get(unit, CVA_OP_TXSLEEP_PORTMASK, &value16));
        /* Update the PHY transmitting sleep capability setting in the dstPmap */
        for(phyId = 0u; phyId <= CVA_RTK_PHY_ID_MAX; phyId++)
        {
            /* check if the PHY index contained in input parameter portMap */
            if(0u == (((uint16)0x1u << phyId) & ((uint16)dstPmap)))
            {
                continue;
            }

            if(CVA_ENABLED == cfgValue)
            {
                /* Set bit value from 0 to 1 */
                value16 = value16 | (((uint16)(0x1u)) << phyId);
            }
            else
            {
                /* Set bit value from 1 to 0 */
                value16 = value16 & ((((uint16)(0x1u)) << phyId) ^ 0xFFFFu);
            }
        }

        /* Write back the transmitting sleep capability setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_set(unit, CVA_OP_TXSLEEP_PORTMASK, value16));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the transmitting sleep request port map (TXSleepPortMsk).\n
 *        The transmitting sleep request port map (TXSleepPortMsk) shows the ports that will transmit sleep requests when another port receives a sleep request.\n
 *        For example, if bit1 and bit2 in the transmit sleep request port map are set:
 *        - When port1 receives a sleep request, port2 will transmit the sleep request to its link partner.
 *        - When port3 receives a sleep request, port1 and port2 will transmit the sleep request to their link partners.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] cfgValue Set value (DISABLE or ENABLE).\n
 *                     Enable or disable the transmitting sleep request.
 * @param[out] pPortMap Pointer to the TXSleepPortMsk (Range: 0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Returns the port map indicating ports that transmit sleep requests when another port receives a sleep request:
 *                      - bit1: If bit value is 1, TXSleepPortMsk includes port1
 *                      - bit2: If bit value is 1, TXSleepPortMsk includes port2
 *                      - bit3: If bit value is 1, TXSleepPortMsk includes port3
 *                      - bit4: If bit value is 1, TXSleepPortMsk includes port4
 *                      - bit5: If bit value is 1, TXSleepPortMsk includes port5
 *                      - bit6: If bit value is 1, TXSleepPortMsk includes port6
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer *pPortMap is NULL.
 * @retval Other error codes as defined in cva_drv_indirect_sram_2bytes_get() and cva_drv_t1phy_pmap_convert().
 *
 * @note Reference:
 *       - cva_op_set_txsleep_portmap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue).
 */
RtkApiRet cva_op_get_txsleep_portmap(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap)
{
    uint16    value16 = 0u;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get PHY index mask showing the current transmitting sleep capability setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_get(unit, CVA_OP_TXSLEEP_PORTMASK, &value16));
        /* Return the corresponding configuration value based on whether the parameter cfgValue is enable or disable */
        if(CVA_DISABLED == cfgValue)
        {
            value16 = ~value16;
        }
        srcPmap = (RtkPmap)value16;
        srcPmap &= CVA_RTK_T1PHY_PMAP_ALL;
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(srcPmap, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (uint16)dstPmap;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_SLEEP_PRECONFIG_SLP_FWD_ASIC */
/**@}*/ /* OPFSM_SLEEP_PRECONFIG_SLP_FWD */

/**
 * @addtogroup OPFSM_SLEEP_PRECONFIG_WHOLECHIP_SLP The Configuration related to Whole Chip Sleep Mode
 * @{
 */
/**
 * @defgroup OPFSM_SLEEP_PRECONFIG_WHOLECHIP_SLP_ASIC Low Level Driver
 * @brief The ASIC Configuration of Whole Chip Sleep
 * @{
 */

/**
 * @brief Allow or disallow the switch to enter WHOLE_CHIP_SLEEP mode.\n
 *        If the switch is allowed to enter WHOLE_CHIP_SLEEP mode, when any port of switch receives a sleep request and enters OP_LOCAL_SLEEP mode,\n
 *        the switch will check if all the ports in the WholeChipSleepMask are in OP_LOCAL_SLEEP mode.\n
 *        If the switch is allowed to enter WHOLE_CHIP_SLEEP mode and all the ports in the WholeChipSleepMask are in OP_LOCAL_SLEEP mode,\n
 *        the switch will then enter WHOLE_CHIP_SLEEP mode.
 *
 * @param[in] unit Stacking unit (1 to 7).
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] wholeChipOn WholeChipSleep configuration (DISABLE or ENABLE).\n
 *                        Set the whole chip sleep enable bit:
 *                        - ENABLE: Disallow entering WHOLE_CHIP_SLEEP mode
 *                        - DISABLE: Allow entering WHOLE_CHIP_SLEEP mode
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input value of wholeChipOn is invalid.
 * @retval Other error codes as defined in cva_drv_indirect_sram_2bytes_get() and cva_drv_indirect_sram_2bytes_set().
 *
 * @note Reference:
 *       - cva_op_set_wholechip_sleep_portmap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue).
 */
RtkApiRet cva_op_set_en_wholechip_sleep(uint32 unit, RtkEnable wholeChipOn)
{
    uint16    value16 = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((CVA_ENABLED != wholeChipOn) && (CVA_DISABLED != wholeChipOn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Get the bit[10] control the enabling of whole chip sleep setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_get(unit, CVA_OP_ACCEPT_SLEEP_CAP, &value16));
        if(CVA_ENABLED == wholeChipOn)
        {
            value16 = value16 | (((uint16)0x1u) << OP_WHOLE_SLEEP_CTRL_BIT);
        }
        else
        {
            value16 = value16 & ((((uint16)0x1u) << OP_WHOLE_SLEEP_CTRL_BIT) ^ 0xFFFFu);
        }
        /* Write the updated value back to the register */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_set(unit, CVA_OP_ACCEPT_SLEEP_CAP, value16));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the WholeChipSleep configuration for allowing or disallowing the switch to enter WHOLE_CHIP_SLEEP mode.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pWholeChipOn Pointer to the WholeChipSleep configuration (DISABLE or ENABLE).\n
 *                          Gets the setting of enabling or disabling whole chip sleep:
 *                          - DISABLE: Disallow entering WHOLE_CHIP_SLEEP mode
 *                          - ENABLE: Allow entering WHOLE_CHIP_SLEEP mode
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer *pWholeChipOn is NULL.
 * @retval Other error codes as defined in cva_drv_indirect_sram_2bytes_get().
 *
 * @note Reference:
 *       - cva_op_set_en_wholechip_sleep(uint32 unit, RtkEnable wholeChipOn).
 */
RtkApiRet cva_op_get_en_wholechip_sleep(uint32 unit, RtkEnable *pWholeChipOn)
{
    uint16    value16 = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pWholeChipOn)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get the bit[10] control the enabling of whole chip sleep setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_get(unit, CVA_OP_ACCEPT_SLEEP_CAP, &value16));
        /* The bit[10] of address CVA_OP_ACCEPT_SLEEP_CAP is the sleep enable bit */
        value16       = (value16 >> OP_WHOLE_SLEEP_CTRL_BIT) & 0x1u;
        *pWholeChipOn = (RtkEnable)value16;
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the WholeChipSleepMask, which is the port map referred to by the whole chip sleep feature.\n
 *        The switch will enter WHOLE_CHIP_SLEEP mode if all the ports in the WholeChipSleepMask are in OP_LOCAL_SLEEP mode.\n
 *        The default value of WholeChipSleepMask is 0, meaning no port is in the WholeChipSleepMask.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map indicating selected ports in WholeChipSleepMask to be configured (0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Set WholeChipSleepMask to configure the ports referred to by the whole chip sleep feature.\n
 *                    Each bit indicates a port in WholeChipSleepMask to be configured to enable transmitting a sleep request when any other port receives a sleep request:
 *                    - bit1: Select port1
 *                    - bit2: Select port2
 *                    - bit3: Select port3
 *                    - bit4: Select port4
 *                    - bit5: Select port5
 *                    - bit6: Select port6
 * @param[in] cfgValue Whole chip sleep port setting (DISABLE or ENABLE).\n
 *                     The value of the whole chip sleep setting:
 *                     - DISABLE: Don't care if the port is in sleep state
 *                     - ENABLE: Care if the port is in sleep state
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input cfgValue or the input port map is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_reg_read(), and cva_reg_write().
 *
 * @note When any port of switch receives a sleep request and enters OP_LOCAL_SLEEP mode,
 *       the switch will check the WholeChipSleepMask to see if all the ports in it are in OP_LOCAL_SLEEP mode.
 *       If the ports in the WholeChipSleepMask are all in OP_LOCAL_SLEEP mode, switch will enter WHOLE_CHIP_SLEEP mode.
 *       The WholeChipSleepMask cannot include ports 7, 8, 9, 10, 11, and 12.
 *
 * @note References:
 *       - cva_op_set_en_wholechip_sleep(uint32 unit, RtkEnable wholeChipOn)
 *       - cva_op_set_phy_sleep_cap(uint32 unit, RtkPmap portMap, RtkEnable sleepCap)
 *       - cva_op_set_accept_sleep_req_cap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue)
 *       - cva_op_set_txsleep_portmap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue)
 *       - cva_op_port_send_sleep_request(uint32 unit, RtkPmap portMap)
 */
RtkApiRet cva_op_set_wholechip_sleep_portmap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue)
{
    uint32    regValue = 0u;
    RtkPmap   dstPmap  = 0u;
    RtkPort   phyId    = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((CVA_ENABLED != cfgValue) && (CVA_DISABLED != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Read the whole chip sleep register */
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_OP_WHOLE_CHIP_SLEEP_MASK, &regValue));
        /* Get valid bits */
        regValue &= (CVA_RTK_T1PHY_PMAP_ALL);
        /* Update the whole chip sleep setting in the dstPmap */
        for(phyId = 0u; phyId <= CVA_RTK_PHY_ID_MAX; phyId++)
        {
            /* check if the PHY index contained in input parameter portMap */
            if(0u == (((uint32)0x1u << phyId) & dstPmap))
            {
                continue;
            }
            if(CVA_ENABLED == cfgValue)
            {
                regValue = regValue | ((uint32)(0x1u) << phyId);
            }
            else
            {
                regValue = regValue & (((uint32)(0x1u) << phyId) ^ CVA_REG_DATA_MASK);
            }
        }

        /* Write the updated PHY index mask back to the whole chip sleep register */
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, CVA_OP_WHOLE_CHIP_SLEEP_MASK, regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the WholeChipSleepMask, which is a port map referred to by the whole chip sleep feature.\n
 *        The switch will enter WHOLE_CHIP_SLEEP mode if the ports configured in the WholeChipSleepMask are all in OP_LOCAL_SLEEP mode.\n
 *        The default value of WholeChipSleepMask is 0, meaning no port is in the WholeChipSleepMask.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] cfgValue Set value (DISABLE or ENABLE).\n
 *                     Indicates whether ports belong to the WholeChipSleepMask:
 *                     - DISABLE: Not belongs to
 *                     - ENABLE: Belongs to
 * @param[out] pPortMap Pointer to the WholeChipSleepMask (0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Definition of each bit in WholeChipSleepMask:
 *                      - bit1: If bit value is 1, WholeChipSleepMask includes port1
 *                      - bit2: If bit value is 1, WholeChipSleepMask includes port2
 *                      - bit3: If bit value is 1, WholeChipSleepMask includes port3
 *                      - bit4: If bit value is 1, WholeChipSleepMask includes port4
 *                      - bit5: If bit value is 1, WholeChipSleepMask includes port5
 *                      - bit6: If bit value is 1, WholeChipSleepMask includes port6
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer *pPortMap is NULL.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert() and cva_reg_read().
 *
 * @note Reference:
 *       - cva_op_set_wholechip_sleep_portmap(uint32 unit, RtkPmap portMap, RtkEnable cfgValue).
 */
RtkApiRet cva_op_get_wholechip_sleep_portmap(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap)
{
    uint32    wholechipmsk_tmp = 0u;
    RtkPmap   dstPmap          = 0u;
    RtkApiRet retVal           = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Read the whole chip sleep register */
        CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, CVA_OP_WHOLE_CHIP_SLEEP_MASK, &wholechipmsk_tmp));
        if(CVA_ENABLED == cfgValue)
        {
            wholechipmsk_tmp = (wholechipmsk_tmp & CVA_RTK_T1PHY_PMAP_ALL);
        }
        else
        {
            wholechipmsk_tmp = (uint32)((~wholechipmsk_tmp) & CVA_RTK_T1PHY_PMAP_ALL);
        }
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)wholechipmsk_tmp, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = dstPmap;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_SLEEP_PRECONFIG_WHOLECHIP_SLP_ASIC */
/**@}*/ /* OPFSM_SLEEP_PRECONFIG_WHOLECHIP_SLP */

/**
 * @addtogroup OPFSM_WAKE_PRECONFIG_LWAKE_EDGE Lwake edge detection selection function
 * @{
 */
/**
 * @defgroup OPFSM_WAKE_PRECONFIG_LWAKE_EDGE_ASIC Low Level Driver
 * @brief The ASIC Configuration of Lwake edge detection selection function
 * @{
 */

/**
 * @brief Set the signal edge of LWAKE.\n
 *        When switch is in global sleep mode, it can be woken up when detecting the configured LWAKE signal edge on the WAKE pin.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] lwakeEdge The LWAKE signal edge (OP_LWAKE_RISING_EDGE, OP_LWAKE_FALLING_EDGE).\n
 *                      Set the LWAKE signal edge detection type:
 *                      - OP_LWAKE_RISING_EDGE: Rising edge detection
 *                      - OP_LWAKE_FALLING_EDGE: Falling edge detection
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input lwakeEdge is invalid.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get() and cva_drv_hw_indirect_set().
 *
 * @note The default is set as rising edge detection.
 */
RtkApiRet cva_op_set_lwake_edge_sel(uint32 unit, uint16 lwakeEdge)
{
    uint32    regValue = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Get the current lwake edge setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_LWAKE_EDGE << 1u), &regValue));
        if(OP_LWAKE_RISING_EDGE == lwakeEdge)
        {
            /* Set bit11 to 0 */
            regValue = regValue & (((uint32)(0x1u) << 11u) ^ CVA_REG_DATA_MASK);
        }
        else if(OP_LWAKE_FALLING_EDGE == lwakeEdge)
        {
            /* Set bit11 to 1 */
            regValue = regValue | ((uint32)(0x1u) << 11u);
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Write back the current lwake edge setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (CVA_OP_LWAKE_EDGE << 1u), regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the signal edge of LWAKE.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[out] pLwakeEdge The LWAKE signal edge (OP_LWAKE_RISING_EDGE, OP_LWAKE_FALLING_EDGE).\n
 *                        Returns the LWAKE edge setting:
 *                        - OP_LWAKE_RISING_EDGE: Rising edge detection
 *                        - OP_LWAKE_FALLING_EDGE: Falling edge detection
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer pLwakeEdge is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get().
 *
 * @note Reference:
 *       - cva_op_set_lwake_edge_sel(uint32 unit, uint16 lwakeEdge).
 */
RtkApiRet cva_op_get_lwake_edge_sel(uint32 unit, uint16 *pLwakeEdge)
{
    uint32    regValue = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pLwakeEdge)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get the current lwake edge setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_LWAKE_EDGE << 1), &regValue));
        regValue    = (regValue >> 11u) & (uint32)(0x1u);
        *pLwakeEdge = (uint16)(regValue);
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_WAKE_PRECONFIG_LWAKE_EDGE_ASIC */
/**@}*/ /* OPFSM_WAKE_PRECONFIG_LWAKE_EDGE */

/**
 * @addtogroup OPFSM_WAKE_PRECONFIG_LWAKE_SCOPE Lwake scope selection function
 * @{
 */
/**
 * @defgroup OPFSM_WAKE_PRECONFIG_LWAKE_SCOPE_ASIC Low Level Driver
 * @brief The ASIC Configuration of Lwake scope selection function
 * @{
 */

/**
 * @brief Set Specific ports can be configured to enable or disable wakeup from sleep mode upon detecting a local wakeup event.\n
 *        When the RTL907x_C detects a local wakeup event, the ports in the lwake_wake_port port map will wake up after detecting an lwake event and might initiate a remote wakeup to its link partner.\n
 *        By default, all ports have the lwake_wake_port functionality enabled.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] portMap lwake_wake_port port map (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Set port map to configure the ports that will send a remote wakeup after detecting a local wakeup event.\n
 *                    Each bit indicates a port in the lwake_wake_port port map:
 *                    - bit1: Select port1
 *                    - bit2: Select port2
 *                    - bit3: Select port3
 *                    - bit4: Select port4
 *                    - bit5: Select port5
 *                    - bit6: Select port6
 * @param[in] cfgValue Set value (DISABLE or ENABLE).\n
 *                     Enable or disable local wakeup to trigger specific ports to send a remote wakeup:
 *                     - DISABLE: Disable
 *                     - ENABLE: Enable
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input lwake_wake_port port map or the input cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_hw_indirect_get(), and cva_drv_hw_indirect_set().
 */
RtkApiRet cva_op_set_lwake_wake_port(uint32 unit, RtkPmap portMap, RtkEnable cfgValue)
{
    uint32    regValue = 0u;
    RtkPmap   dstPmap  = 0u;
    RtkPort   phyId    = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((CVA_ENABLED != cfgValue) && (CVA_DISABLED != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Get the current local wakeup to remote wakeup setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_LWAKE_TO_RWAKE << 1u), &regValue));
        /* Get valid bits */
        regValue &= CVA_RTK_T1PHY_PMAP_ALL;
        /* Update the current local wakeup to remote wakeup setting according to the input */
        for(phyId = 0u; phyId <= CVA_RTK_PHY_ID_MAX; phyId++)
        {
            if(0u == (((uint32)0x1u << phyId) & dstPmap))
            {
                continue;
            }

            if(CVA_ENABLED == cfgValue)
            {
                regValue = regValue | ((uint32)(0x1u) << phyId);
            }
            else
            {
                regValue = regValue & (((uint32)(0x1u) << phyId) ^ CVA_REG_DATA_MASK);
            }
        }

        /* Write the updated local wakeup to remote wakeup setting back to the register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (CVA_LWAKE_TO_RWAKE << 1), regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the lwake_wake_port port map.\n
 *        When the RTL907x_C detects a local wakeup event, the ports in the lwake_wake_port port map will wake up after detecting an lwake event and might initiate a remote wakeup to its link partner.\n
 *        By default, all ports have the lwake_wake_port functionality enabled.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[out] pPortMap Return lwake_wake_port port map (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      - bit1: If the bit value is 1, port1 enables local wake to remote wake setting
 *                      - bit2: If the bit value is 1, port2 enables local wake to remote wake setting
 *                      - bit3: If the bit value is 1, port3 enables local wake to remote wake setting
 *                      - bit4: If the bit value is 1, port4 enables local wake to remote wake setting
 *                      - bit5: If the bit value is 1, port5 enables local wake to remote wake setting
 *                      - bit6: If the bit value is 1, port6 enables local wake to remote wake setting
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer pPortMap is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get() and cva_drv_t1phy_pmap_convert().
 *
 * @note Reference:
 *       - cva_op_set_lwake_wake_port(uint32 unit, RtkPmap portMap, uint8 cfgValue).
 */
RtkApiRet cva_op_get_lwake_wake_port(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap)
{
    uint32    value32 = 0u;
    RtkPmap   dstPmap = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get the current local wakeup to remote wakeup setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_LWAKE_TO_RWAKE << 1), &value32));
        if(CVA_ENABLED == cfgValue)
        {
            value32 &= (CVA_RTK_T1PHY_PMAP_ALL);
        }
        else
        {
            value32 = ((~value32) & CVA_RTK_T1PHY_PMAP_ALL);
        }
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)value32, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (dstPmap & 0xFFFFu);
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_WAKE_PRECONFIG_LWAKE_SCOPE_ASIC */
/**@}*/ /* OPFSM_WAKE_PRECONFIG_LWAKE_SCOPE */

/**
 * @addtogroup OPFSM_WAKE_PRECONFIG_RWAKE_TX_CAP Rwake transmission capability function
 * @{
 */
/**
 * @defgroup OPFSM_WAKE_PRECONFIG_RWAKE_TX_CAP_ASIC Low Level Driver
 * @brief The ASIC Configuration of Rwake transmission capability function
 * @{
 */

/**
 * @brief Enable or disable the capability of specific ports to transmit WUP/WUR.\n
 *        By default, all ports enable transmitting WUP/WUR.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map indicating the setting of WUP/WUR transmitting (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Each bit in the port map indicates a port to be configured for WUP/WUR transmitting capability:
 *                    - bit1: Select port1
 *                    - bit2: Select port2
 *                    - bit3: Select port3
 *                    - bit4: Select port4
 *                    - bit5: Select port5
 *                    - bit6: Select port6
 * @param[in] cfgValue Set value (DISABLE or ENABLE).\n
 *                     Enable or disable the transmitting WUP/WUR capability:
 *                     - DISABLE: Disable transmit WUP/WUR capability
 *                     - ENABLE: Enable transmit WUP/WUR capability
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input port map or cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_hw_indirect_get(), and cva_drv_hw_indirect_set().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot transmit WUP/WUR.
 */
RtkApiRet cva_op_set_en_wupr_tx(uint32 unit, RtkPmap portMap, RtkEnable cfgValue)
{
    uint32    regValue = 0u;
    RtkPmap   dstPmap  = 0u;
    RtkPort   phyId    = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((CVA_ENABLED != cfgValue) && (CVA_DISABLED != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Get the current transmitting WUP/WUR remote wakeup setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_EN_WUPR_TX << 1u), &regValue));
        /* Get valid bits */
        regValue &= CVA_RTK_T1PHY_PMAP_ALL;
        /* Update the current setting according to the input */
        for(phyId = 0u; phyId <= CVA_RTK_PHY_ID_MAX; phyId++)
        {
            if(0u == (((uint32)0x1u << phyId) & dstPmap))
            {
                continue;
            }

            if(CVA_ENABLED == cfgValue)
            {
                regValue = regValue | ((uint32)(0x1u) << phyId);
            }
            else
            {
                regValue = regValue & (((uint32)(0x1u) << phyId) ^ CVA_REG_DATA_MASK);
            }
        }
        /* Write the updated current transmitting WUP/WUR remote wakeup setting back to the register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (CVA_EN_WUPR_TX << 1u), regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the port map indicating the current port's capability of transmitting WUP/WUR remote wakeup.\n
 *        By default, all ports enable transmitting WUP/WUR.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pPortMap Pointer to the port mask setting of WUP/WUR transmitting (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Returns the port map indicating the capability of transmitting WUP/WUR for each port:
 *                      - bit1: If bit value is 1, port1 enables WUP/WUR transmitting
 *                      - bit2: If bit value is 1, port2 enables WUP/WUR transmitting
 *                      - bit3: If bit value is 1, port3 enables WUP/WUR transmitting
 *                      - bit4: If bit value is 1, port4 enables WUP/WUR transmitting
 *                      - bit5: If bit value is 1, port5 enables WUP/WUR transmitting
 *                      - bit6: If bit value is 1, port6 enables WUP/WUR transmitting
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer *pPortMap is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get() and cva_drv_t1phy_pmap_convert().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot configure the WUP/WUR transmitting capability.
 *
 * @note Reference:
 *       - cva_op_set_en_wupr_tx(uint32 unit, RtkPmap portMap, RtkEnable cfgValue)
 */
RtkApiRet cva_op_get_en_wupr_tx(uint32 unit, RtkEnable cfgValue, RtkPmap *pPortMap)
{
    uint32    value32 = 0u;
    RtkPmap   dstPmap = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get the current transmitting WUP/WUR remote wakeup setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_EN_WUPR_TX << 1), &value32));
        if(CVA_ENABLED == cfgValue)
        {
            value32 &= (CVA_RTK_T1PHY_PMAP_ALL);
        }
        else
        {
            value32 = ((~value32) & CVA_RTK_T1PHY_PMAP_ALL);
        }
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)value32, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (dstPmap & 0xFFFFu);
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_WAKE_PRECONFIG_RWAKE_TX_CAP_ASIC */
/**@}*/ /* OPFSM_WAKE_PRECONFIG_RWAKE_TX_CAP */

/**
 * @addtogroup OPFSM_WAKE_PRECONFIG_DIS_RWAKE_RX_DET Rwake detect function
 * @{
 */
/**
 * @defgroup OPFSM_WAKE_PRECONFIG_DIS_RWAKE_RX_DET_ASIC Low Level Driver
 * @brief The ASIC Configuration of Rwake detect function
 * @{
 */

/**
 * @brief Disable the remote wakeup detection of selected ports.\n
 *        If a port disables remote wakeup detection, it cannot accept remote wakeup requests initialized by the link partner.\n
 *        By default, all ports allow remote wakeup detection.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map indicating selected ports (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Each bit in the port map indicates a port to disable remote wakeup detection:
 *                    - bit1: Select port1
 *                    - bit2: Select port2
 *                    - bit3: Select port3
 *                    - bit4: Select port4
 *                    - bit5: Select port5
 *                    - bit6: Select port6
 * @param[in] cfgValue Set value (OP_ENABLE_RWAKE or OP_DISABLE_RWAKE).\n
 *                     Enable or disable remote wakeup detection:
 *                     - OP_ENABLE_RWAKE: Enable remote wakeup detection for selected ports
 *                     - OP_DISABLE_RWAKE: Disable remote wakeup detection for selected ports
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input port map or cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_hw_indirect_get(), and cva_drv_hw_indirect_set().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot configure remote wakeup detection.
 */
RtkApiRet cva_op_set_disable_rwake_rx(uint32 unit, RtkPmap portMap, uint8 cfgValue)
{
    uint32    regValue = 0u;
    RtkPmap   dstPmap  = 0u;
    RtkPort   phyId    = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((OP_ENABLE_RWAKE != cfgValue) && (OP_DISABLE_RWAKE != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Get the current disable remote wakeup detection setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_DIS_RWAKE << 1u), &regValue));
        /* Get valid bits */
        regValue &= CVA_RTK_T1PHY_PMAP_ALL;
        /* Update the current disable remote wakeup detection setting */
        for(phyId = 0u; phyId <= CVA_RTK_PHY_ID_MAX; phyId++)
        {
            if(0u == (((uint32)0x1u << phyId) & dstPmap))
            {
                continue;
            }
            if(OP_DISABLE_RWAKE == cfgValue)
            {
                regValue = regValue | ((uint32)(0x1u) << phyId);
            }
            else
            {
                regValue = regValue & (((uint32)(0x1u) << phyId) ^ CVA_REG_DATA_MASK);
            }
        }
        /* Write the current disable remote wakeup detection setting back to the register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (CVA_OP_DIS_RWAKE << 1), regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the port map indicating the current port's remote wakeup detection setting.\n
 *        By default, all ports allow remote wakeup detection, so get_cva_op_disable_rwake_rx will return 0.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pPortMap Pointer to the remote wakeup detection setting (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Returns the port map indicating the remote wakeup setting of ports:
 *                      - bit1: Get the value of port1 disable rwake setting
 *                      - bit2: Get the value of port2 disable rwake setting
 *                      - bit3: Get the value of port3 disable rwake setting
 *                      - bit4: Get the value of port4 disable rwake setting
 *                      - bit5: Get the value of port5 disable rwake setting
 *                      - bit6: Get the value of port6 disable rwake setting
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer *pPortMap is NULL.
 * @retval -RT_ERR_FAILED if the register interface access function returns errors.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get() and cva_drv_t1phy_pmap_convert().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot configure remote wakeup detection.
 *
 * @note Reference:
 *       - cva_op_set_disable_rwake_rx(uint32 unit, RtkPmap portMap, uint8 cfgValue)
 */
RtkApiRet cva_op_get_disable_rwake_rx(uint32 unit, RtkPmap *pPortMap)
{
    uint32    value32 = 0u;
    RtkPmap   dstPmap = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get the current disable remote wakeup detection setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_DIS_RWAKE << 1), &value32));
        /* Get valid bits */
        value32 &= CVA_RTK_T1PHY_PMAP_ALL;
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)value32, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (dstPmap & 0xFFFFu);
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_WAKE_PRECONFIG_DIS_RWAKE_RX_DET_ASIC */
/**@}*/ /* OPFSM_WAKE_PRECONFIG_DIS_RWAKE_RX_DET */

/**
 * @addtogroup OPFSM_WAKEUP_FWD_RWAKE_FORWARD Rwake forwarding function
 * @{
 */
/**
 * @defgroup OPFSM_WAKEUP_FWD_RWAKE_FORWARD_ASIC Low Level Driver
 * @brief The ASIC Configuration of Rwake forwarding function
 * @{
 */

/**
 * @brief Get the remote wake-up transmission capability of the selected ports.\n
 *        When the ports in the specified port map (portMap) receive a remote wakeup,\n
 *        the selected port (portSel) will wakeup its link partner if the rwake tx capability is enabled.\n
 *        The default port map setting of a port is all zero.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portSel Port ID (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_6TH).\n
 *                    User port index of the selected port.
 * @param[out] pPortMap Port mask indicating forwarding remote wake setting (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Port mask of forwarding capability of the selected port.
 *                      - bit0: port1
 *                      - bit1: port2
 *                      - bit2: port3
 *                      - bit3: port4
 *                      - bit4: port5
 *                      - bit5: port6
 *                      If the bit value is 0: Disable forwarding remote wake of the selected port\n
 *                      If the bit value is 1: Enable forwarding remote wake of the selected port
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer pPortMap is NULL.
 * @retval -RT_ERR_INPUT if the input portSel is invalid.
 * @retval Other error codes as defined in cva_drv_pmap_convert() and cva_drv_hw_indirect_get().
 *
 * @note Reference:
 *       - cva_op_set_portmap_fwd_rwake_to_port(uint32 unit, RtkPort portSel, RtkPmap portMap, RtkEnable cfgValue).
 */
RtkApiRet cva_op_set_portmap_fwd_rwake_to_port(uint32 unit, RtkPort portSel, RtkPmap portMap, RtkEnable cfgValue)
{
    uint32    regLocal    = 0u;
    uint32    regValOp1   = 0u;
    uint32    regValOp2   = 0u;
    uint32    regValOpMsk = 0u;
    uint32    regValue    = 0u;
    RtkPmap   dstPmap     = 0u;
    RtkPort   phyId       = 0u;
    RtkApiRet retVal      = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((CVA_ENABLED != cfgValue) && (CVA_DISABLED != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((CVA_RTK_USER_PORT_1ST > portSel) || (CVA_RTK_USER_PORT_6TH < portSel))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if(portMap != (portMap & CVA_RTK_USER_PMAP_WITHT1PHY))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        regLocal = cva_g_sysPortToLocalPxCmd[cva_g_userPort2sysPort[portSel]];
        /* Get the current forward pxcfg setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (regLocal << 1u), &regValue));
        for(phyId = 0u; phyId < (uint32)(cva_g_userPort2T1phy[portSel]); phyId++)
        {
            regValOpMsk = regValOpMsk << 1u;
            regValOpMsk |= 1u;
        }
        regValOp1 = (regValue << 1u) & (regValOpMsk ^ 0xFFFFu);
        regValOp2 = regValue & regValOpMsk;
        regValue  = (regValOp1 | regValOp2) & (((uint32)0x1u << cva_g_userPort2T1phy[portSel]) ^ 0x0000ffffu);
        /* Update the current setting according to the input */
        for(phyId = 0u; phyId <= CVA_RTK_PHY_ID_MAX; phyId++)
        {
            if(0u == (((uint32)0x1u << phyId) & dstPmap))
            {
                continue;
            }

            if(CVA_ENABLED == cfgValue)
            {
                regValue = regValue | ((uint32)(0x1u) << phyId);
            }
            else
            {
                regValue = regValue & (((uint32)(0x1u) << phyId) ^ CVA_REG_DATA_MASK);
            }
        }
        regValOp1 = regValue >> ((uint32)cva_g_userPort2T1phy[portSel] + 1u);
        regValOp2 = regValue & regValOpMsk;
        regValue  = (regValOp1 << (uint32)cva_g_userPort2T1phy[portSel]) | regValOp2;
        /* Write back the current forward pxcfg setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (regLocal << 1u), regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the remote wake-up transmission capability of the selected ports.\n
 *        When the ports in the specified port map (portMap) receive a remote wakeup,\n
 *        the selected port (portSel) will wakeup its link partner if the rwake tx capability is enabled.\n
 *        The default port map setting of a port is all zero.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portSel Port ID (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_6TH).\n
 *                    User port index of the selected port.
 * @param[out] pPortMap Port mask indicating forwarding remote wake setting (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Port mask of forwarding capability of the selected port.
 *                      - bit0: port1
 *                      - bit1: port2
 *                      - bit2: port3
 *                      - bit3: port4
 *                      - bit4: port5
 *                      - bit5: port6
 *                      If the bit value is 0: Disable forwarding remote wake of the selected port\n
 *                      If the bit value is 1: Enable forwarding remote wake of the selected port
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the pointer pPortMap is NULL.
 * @retval -RT_ERR_INPUT if the input portSel is invalid.
 * @retval Other error codes as defined in cva_drv_pmap_convert() and cva_drv_hw_indirect_get().
 *
 * @note Reference:
 *       - cva_op_set_portmap_fwd_rwake_to_port(uint32 unit, RtkPort portSel, RtkPmap portMap, RtkEnable cfgValue).
 */
RtkApiRet cva_op_get_portmap_fwd_rwake_to_port(uint32 unit, RtkPort portSel, RtkPmap *pPortMap)
{
    uint32    value32     = 0u;
    uint32    regValOp1   = 0u;
    uint32    regValOp2   = 0u;
    uint32    regValOpMsk = 0u;
    uint32    regLocal    = 0u;
    uint8     phyId       = 0u;
    RtkPmap   dstPmap     = 0u;
    RtkApiRet retVal      = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        if((CVA_RTK_USER_PORT_1ST > portSel) || (CVA_RTK_USER_PORT_6TH < portSel))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* get the reg offset of forward pxcfg setting */
        regLocal = cva_g_sysPortToLocalPxCmd[cva_g_userPort2sysPort[portSel]];
        /* Get the current forward pxcfg setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, ((uint32)regLocal << 1u), &value32));

        for(phyId = 0u; phyId < (uint32)(cva_g_userPort2T1phy[portSel]); phyId++)
        {
            regValOpMsk = regValOpMsk << 1u;
            regValOpMsk |= 1u;
        }
        regValOp1 = (value32 << 1u) & (regValOpMsk ^ 0xFFFFu);
        regValOp2 = value32 & regValOpMsk;
        value32   = (regValOp1 | regValOp2) & (((uint32)0x1u << cva_g_userPort2T1phy[portSel]) ^ 0xFFFFu);
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)value32, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (dstPmap & 0xFFFFu);
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_WAKEUP_FWD_RWAKE_FORWARD_ASIC */
/**@}*/ /* OPFSM_WAKEUP_FWD_RWAKE_FORWARD */

/**
 * @addtogroup OPFSM_WAKEUP_FWD_WAKEO Wake-out forwarding function
 * @{
 */
/**
 * @defgroup OPFSM_WAKEUP_FWD_WAKEO_ASIC Low Level Driver
 * @brief The ASIC Configuration of Wake-out forwarding function
 * @{
 */

/**
 * @brief Set wake-out forwarding port map indicating the ports that will trigger wake-out forwarding on the specific GPIO PIN upon receiving a WUP or WUR.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] configIdx Wake-out forwarding port map config index (0 or 1).
 *                      - 0: Config Index = 0
 *                      - 1: Config Index = 1
 * @param[in] portMap Wake-out forwarding port map (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).
 *                    Each bit indicates a port in the wake-out forwarding port map of the selected config index:\n
 *                    - bit0: Select port1
 *                    - bit1: Select port2
 *                    - bit2: Select port3
 *                    - bit3: Select port4
 *                    - bit4: Select port5
 *                    - bit5: Select port6
 * @param[in] cfgValue Set value (DISABLE or ENABLE).
 *                     - ENABLE: After receiving remote wakeup, DO trigger wake-out forwarding on the selected GPIO PIN
 *                     - DISABLE: After receiving remote wakeup, DO NOT trigger wake-out forwarding on the selected GPIO PIN
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input config index, port map, or cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_indirect_sram_2bytes_get(), and cva_drv_indirect_sram_2bytes_set().
 *
 * @note In switch, Config Index 0 is for GPIOB1, and Config Index 1 is for GPIOB2.
 */
RtkApiRet cva_op_set_wakeo_portmap(uint32 unit, uint8 configIdx, RtkPmap portMap, RtkEnable cfgValue)
{
    uint32    cfgAddr = 0u;
    uint16    value16 = 0u;
    uint8     gpioId  = 0u;
    RtkPmap   dstPmap = 0u;
    RtkPort   phyId   = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* check input parameters */
        if((CVA_ENABLED != cfgValue) && (CVA_DISABLED != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* get the designated WakeO Configuration Index according to the parameter configIdx */
        if((0u == configIdx) || (1u == configIdx))
        {
            gpioId = cva_g_wakeoConfigIdxToGpioIdx[configIdx];
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* get the reg address of configIdx */
        if(OP_WAKEO_GPIOB1_ID == gpioId)
        {
            cfgAddr = CVA_OP_WAKEO_GPIOB1_PORTS;
        }
        if(OP_WAKEO_GPIOB2_ID == gpioId)
        {
            cfgAddr = CVA_OP_WAKEO_GPIOB2_PORTS;
        }
        /* Get the wake-out forwarding setting GPIO index */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_get(unit, cfgAddr, &value16));

        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        for(phyId = 0u; phyId <= CVA_RTK_PHY_ID_MAX; phyId++)
        {
            if(0u == (((uint32)0x1u << phyId) & dstPmap))
            {
                continue;
            }

            if(CVA_ENABLED == cfgValue)
            {
                value16 = value16 | (((uint16)(0x1u)) << phyId);
            }
            else
            {
                value16 = value16 & ((((uint16)(0x1u)) << phyId) ^ 0xFFFFu);
            }
        }
        /* set the updated wake-out forwarding configuration */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_set(unit, cfgAddr, value16));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the wake-out forwarding port map indicating the ports that will trigger wake-out forwarding on the specific GPIO PIN upon receiving a WUP or WUR.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] configIdx The wake-out forwarding portmask config index (0 or 1).\n
 *                      Selected wake-out forwarding portmask config index:
 *                      - 0: Config Index = 0
 *                      - 1: Config Index = 1
 * @param[in] cfgValue Config value (DISABLE or ENABLE).\n
 *                     Enable or disable the WakeO capability.
 * @param[out] pPortMap Wake-out forwarding port map (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      The port map indicating whether a port enabled wake-out forwarding on the selected GPIO:
 *                      - bit0: If the bit value is 1, port1 enabled wake-out forwarding on the selected GPIO
 *                      - bit1: If the bit value is 1, port2 enabled wake-out forwarding on the selected GPIO
 *                      - bit2: If the bit value is 1, port3 enabled wake-out forwarding on the selected GPIO
 *                      - bit3: If the bit value is 1, port4 enabled wake-out forwarding on the selected GPIO
 *                      - bit4: If the bit value is 1, port5 enabled wake-out forwarding on the selected GPIO
 *                      - bit5: If the bit value is 1, port6 enabled wake-out forwarding on the selected GPIO
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer pPortMap is NULL.
 * @retval -RT_ERR_INPUT if the input config index is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert() and cva_drv_indirect_sram_2bytes_get().
 *
 * @note In switch, Config Index 0 is for GPIOB1, and Config Index 1 is for GPIOB2.
 *
 * @note Reference:
 *       - cva_op_set_wakeo_portmap(uint32 unit, uint8 configIdx, RtkPmap portMap, uint16 cfgValue).
 */
RtkApiRet cva_op_get_wakeo_portmap(uint32 unit, uint8 configIdx, RtkEnable cfgValue, RtkPmap *pPortMap)
{
    uint16    value16 = 0u;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;
    uint8     gpioId  = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* get the designated WakeO Configuration Index according to the parameter configIdx */
        if((0u == configIdx) || (1u == configIdx))
        {
            gpioId = cva_g_wakeoConfigIdxToGpioIdx[configIdx];
        }
        else
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* get the reg address of configIdx */
        if(OP_WAKEO_GPIOB1_ID == gpioId)
        {
            /* get wake-out forwarding configuration of GPIOB1 */
            CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_get(unit, CVA_OP_WAKEO_GPIOB1_PORTS, &value16));
        }
        if(OP_WAKEO_GPIOB2_ID == gpioId)
        {
            /* get wake-out forwarding configuration of GPIOB2 */
            CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_2bytes_get(unit, CVA_OP_WAKEO_GPIOB2_PORTS, &value16));
        }
        /* Return the corresponding configuration value based on whether the parameter cfgValue is enable or disable */
        if(CVA_DISABLED == cfgValue)
        {
            value16 = ~value16;
        }
        srcPmap = (RtkPmap)value16;
        srcPmap &= CVA_RTK_T1PHY_PMAP_ALL;
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(srcPmap, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = dstPmap;
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the wake-out forwarding pulse width setting on the selected GPIO.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] cfgValue The wake-out forwarding pulse width setting (OP_WAKEO_PULSETYPE_45US, OP_WAKEO_PULSETYPE_500MS).\n
 *                     - OP_WAKEO_PULSETYPE_45US:  45 us
 *                     - OP_WAKEO_PULSETYPE_500MS: 500 ms
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_indirect_sram_1byte_set().
 */
RtkApiRet cva_op_set_wakeo_pulse(uint32 unit, uint16 cfgValue)
{
    uint8     sramValue = 0u;
    RtkApiRet retVal    = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((OP_WAKEO_PULSETYPE_45US != cfgValue) && (OP_WAKEO_PULSETYPE_500MS != cfgValue) && (OP_WAKEO_PULSETYPE_UNKNOWN != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Update wake-out forwarding pulse setting */
        sramValue = (uint8)cfgValue;
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_1byte_set(unit, CVA_OP_WAKEO_PULSE_CFG, sramValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the wake-out forwarding pulse width setting on the selected GPIO.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[out] pCfgValue The wake-out forwarding pulse width setting (OP_WAKEO_PULSETYPE_45US, OP_WAKEO_PULSETYPE_500MS).
 *                       - OP_WAKEO_PULSETYPE_45US: 45 us
 *                       - OP_WAKEO_PULSETYPE_500MS: 500 ms
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pCfgValue is NULL.
 * @retval Other error codes as defined in cva_drv_indirect_sram_1byte_get().
 *
 * @note Reference:
 *       - cva_op_set_wakeo_pulse(uint32 unit, uint16 cfgValue).
 */
RtkApiRet cva_op_get_wakeo_pulse(uint32 unit, uint16 *pCfgValue)
{
    uint8     sramValue = 0u;
    RtkApiRet retVal    = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pCfgValue)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get the wake-out forwarding pulse setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_1byte_get(unit, CVA_OP_WAKEO_PULSE_CFG, &sramValue));
        *pCfgValue = (uint16)sramValue;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_WAKEUP_FWD_WAKEO_ASIC */
/**@}*/ /* OPFSM_WAKEUP_FWD_WAKEO */

/**
 * @addtogroup OPFSM_OP_STATE_GLOBAL Global state function
 * @{
 */

/**
 * @defgroup OPFSM_OP_STATE_GLOBAL_ASIC Low Level Driver
 * @brief The ASIC Configuration of global OPFSM state
 * @{
 */

/**
 * @brief Get the global OPFSM state of the switch.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pState Pointer to the global OPFSM state.\n
 *                    The global OPFSM state of switch can be one of the following:
 *                    - 0x20: OP_GLOB_STANDBY
 *                    - 0x30: OP_GLOB_NORMAL
 *                    - 0x31: OP_GLOB_SAFETY
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer *pState is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get().
 */
RtkApiRet cva_op_get_global_state(uint32 unit, uint16 *pState)
{
    uint32    regValue = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pState)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get the global OPFSM state by reading the last 8 bits of the specific register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OCP_MR1 << 1u), &regValue));
        *pState = (uint16)(regValue & 0xFFu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the global OPFSM state of switch.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] state Global OPFSM state (0x20, 0x30, 0x40).\n
 *                  The global OPFSM state value to be configured:
 *                  - 0x20: OP_GLOB_STANDBY
 *                  - 0x30: OP_GLOB_NORMAL
 *                  - 0x40: OP_GLOB_WHOLE_CHIP_SLEEP
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input global state is invalid.
 * @retval -RT_ERR_FAILED if the register interface access function returns errors.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get() and cva_drv_hw_indirect_set().
 *
 * @note Use cases:
 *       - cva_op_set_global_state(unit, OP_GLOB_STANDBY); Set global OPFSM state to STANDBY mode.
 *       - cva_op_set_global_state(unit, OP_GLOB_NORMAL); Set global OPFSM state to NORMAL mode.
 *       - cva_op_set_global_state(unit, OP_GLOB_WHOLE_CHIP_SLEEP); Set global OPFSM state to SLEEP mode.
 *       - Users cannot set the global OPFSM state to OP_GLOB_RESET or OP_GLOB_SAFETY.
 */
RtkApiRet cva_op_set_global_state(uint32 unit, uint16 state)
{
    uint32    regValue  = 0u;
    uint32    globValue = 0u;
    RtkApiRet retVal    = RT_ERR_OK;

    do
    {
        /* Get the current global OPFSM state by reading the specific register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_GLOB_CMD << 1u), &regValue));
        switch(state)
        {
            case OP_GLOB_WHOLE_CHIP_SLEEP:
                globValue = 0x14;
                break;
            case OP_GLOB_STANDBY:
                globValue = 0x12;
                break;
            case OP_GLOB_NORMAL:
                globValue = 0x13;
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        /* Set the global OPFSM state by writing the value to the specific register */
        regValue = (regValue & 0xffe0u) | globValue;
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (CVA_OP_GLOB_CMD << 1u), regValue));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_OP_STATE_GLOBAL_ASIC */
/**@}*/ /* OPFSM_OP_STATE_GLOBAL */

/**
 * @addtogroup OPFSM_OP_STATE_LOCAL Local state function
 * @{
 */
/**
 * @defgroup OPFSM_OP_STATE_LOCAL_ASIC Low Level Driver
 * @brief The ASIC Configuration of local OPFSM state
 * @{
 */
/**
 * @brief Get the local OPFSM state of the selected port.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port ID (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_6TH).\n
 *                   The user port index of the selected port.
 * @param[out] pState Pointer to the local OPFSM state.\n
 *                    The local OPFSM state of the selected port can be one of the following:
 *                    - OP_LOCAL_SLEEP
 *                    - OP_LOCAL_STANDBY
 *                    - OP_LOCAL_NORMAL
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer *pState is NULL.
 * @retval -RT_ERR_INPUT if the input port ID is invalid.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get().
 */
RtkApiRet cva_op_get_port_local_state(uint32 unit, RtkPort portId, uint16 *pState)
{
    uint32    regLocal = 0u;
    uint32    regValue = 0u;
    RtkApiRet retVal   = RT_ERR_OK;
    /* The mapping from the phy idx to local state OCP addr:         phy0    phy1    phy2    phy3    phy4    phy5    phy6    phy7    phy8    phy9    phy10   phy11 */
    const uint32 phyIdx2opLocalStateAddr[CVA_RTK_SYS_PORT_ID_MAX] = {0xB12u, 0xB14u, 0xB16u, 0xB18u, 0xB1Au, 0xB4Eu, 0xB50u, 0xB52u, 0xB54u, 0xB56u, 0xB58u, 0xB5Au};

    do
    {
        /* Check input parameters */
        if(NULL == pState)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Check input port index is valid or not */
        if((CVA_OP_T1_USER_PORT_MIN > portId) || (CVA_OP_T1_USER_PORT_MAX < portId))
        {
            retVal  = -RT_ERR_INPUT;
            *pState = (uint16)(0x0);
            continue;
        }

        /* Get the PHY local state of the input portId */
        regLocal = phyIdx2opLocalStateAddr[cva_g_userPort2T1phy[portId]];
        /* Get the port's local OPFSM state by reading the value of the specific register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (regLocal << 1u), &regValue));
        *pState = (uint16)(regValue & 0xFFu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the local OPFSM state of the designated port.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portId Port ID (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_6TH).\n
 *                   The user port index of the selected port.
 * @param[in] state The local OPFSM state (OP_LOCAL_SLEEP, OP_LOCAL_STANDBY, OP_LOCAL_NORMAL).\n
 *                  Valid values for the local OPFSM state:
 *                  - 0x10: OP_LOCAL_SLEEP
 *                  - 0x20: OP_LOCAL_STANDBY
 *                  - 0x30: OP_LOCAL_NORMAL
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input port ID or the input local OPFSM state is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert() and cva_drv_hw_indirect_set().
 */
RtkApiRet cva_op_set_port_local_state(uint32 unit, RtkPort portId, uint16 state)
{
    uint32    regLocal = 0u;
    uint32    regValue = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input port index is valid or not */
        if((CVA_OP_T1_USER_PORT_MIN > portId) || (CVA_OP_T1_USER_PORT_MAX < portId))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Check input state value is valid or not */
        switch(state)
        {
            case OP_LOCAL_SLEEP:
                regValue = 0x9;
                break;
            case OP_LOCAL_STANDBY:
                regValue = 0xa;
                break;
            case OP_LOCAL_NORMAL:
                regValue = 0xb;
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        /* Set the PHY local state of the input portId */
        regLocal = cva_g_sysPortToLocalCmd[cva_g_userPort2sysPort[portId]];
        /* Set the port's local OPFSM state by writing the value to the specific register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (regLocal << 1u), regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the port map of ports with the designated local OPFSM state.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] state Local OPFSM state (OP_LOCAL_SLEEP, OP_LOCAL_STANDBY, OP_LOCAL_NORMAL).\n
 *                  Valid values for the local OPFSM state:
 *                  - 0x10: OP_LOCAL_SLEEP
 *                  - 0x20: OP_LOCAL_STANDBY
 *                  - 0x30: OP_LOCAL_NORMAL
 * @param[out] pPortMap Pointer to the port map indicating ports with the designated local OP state (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Each bit in the port map indicates a port:
 *                      - bit1: The port map includes port1
 *                      - bit2: The port map includes port2
 *                      - bit3: The port map includes port3
 *                      - bit4: The port map includes port4
 *                      - bit5: The port map includes port5
 *                      - bit6: The port map includes port6
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input local OPFSM state is invalid.
 * @retval -RT_ERR_NULL_POINTER if the input pPortMap is a null pointer.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get().
 */
RtkApiRet cva_op_get_portmap_local_state(uint32 unit, uint16 state, RtkPmap *pPortMap)
{
    uint32    regLocal   = 0u;
    uint32    regValue   = 0u;
    RtkPort   userPortId = 1u;
    RtkApiRet retVal     = RT_ERR_OK;
    /* The mapping from the phy idx to local state OCP addr:         phy0    phy1    phy2    phy3    phy4    phy5    phy6    phy7    phy8    phy9    phy10   phy11 */
    const uint32 phyIdx2opLocalStateAddr[CVA_RTK_SYS_PORT_ID_MAX] = {0xB12u, 0xB14u, 0xB16u, 0xB18u, 0xB1Au, 0xB4Eu, 0xB50u, 0xB52u, 0xB54u, 0xB56u, 0xB58u, 0xB5Au};

    do
    {
        /* Check input state value is valid or not */
        if((OP_LOCAL_SLEEP != state) && (OP_LOCAL_STANDBY != state) && (OP_LOCAL_NORMAL != state))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Check input port map pointer */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Search the user port to be configured according to the input portMap */
        *pPortMap = 0u;
        for(userPortId = CVA_OP_T1_USER_PORT_MIN; userPortId <= CVA_RTK_USER_PORT_WITHT1PHY_ID_MAX; userPortId++)
        {
            /* Get the PHY local state of the user port index */
            regLocal = phyIdx2opLocalStateAddr[cva_g_userPort2T1phy[userPortId]];
            /* Get the port's local OPFSM state by reading the value of the specific register */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (regLocal << 1u), &regValue));
            /* If the current local OP state is the input query state */
            if(state == (uint16)(regValue & 0xFFu))
            {
                *pPortMap |= ((uint32)0x1u << userPortId);
            }
        }
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set the local OPFSM state of the selected ports.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map indicating selected ports (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Each bit in the port map indicates a selected port to configure the local OPFSM state:
 *                    - bit1: The port map includes port1
 *                    - bit2: The port map includes port2
 *                    - bit3: The port map includes port3
 *                    - bit4: The port map includes port4
 *                    - bit5: The port map includes port5
 *                    - bit6: The port map includes port6
 * @param[in] state Local OPFSM state (OP_LOCAL_SLEEP, OP_LOCAL_STANDBY, OP_LOCAL_NORMAL).\n
 *                  Valid values for the local OPFSM state:
 *                  - 0x10: OP_LOCAL_SLEEP
 *                  - 0x20: OP_LOCAL_STANDBY
 *                  - 0x30: OP_LOCAL_NORMAL
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input port map or the input local OPFSM state is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert() and cva_drv_hw_indirect_set().
 */
RtkApiRet cva_op_set_portmap_local_state(uint32 unit, RtkPmap portMap, uint16 state)
{
    uint32    regLocal   = 0u;
    uint32    regValue   = 0u;
    RtkPmap   dstPmap    = 0u;
    RtkPort   userPortId = 1u;
    RtkApiRet retVal     = RT_ERR_OK;

    do
    {
        /* Check input state value is valid or not */
        switch(state)
        {
            case OP_LOCAL_SLEEP:
                regValue = 0x9;
                break;
            case OP_LOCAL_STANDBY:
                regValue = 0xa;
                break;
            case OP_LOCAL_NORMAL:
                regValue = 0xb;
                break;
            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }

        /* Check the ports in the input user port map are all with T1-PHY interface */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Search the user port to be configured according to the input portMap */
        for(userPortId = CVA_OP_T1_USER_PORT_MIN; userPortId <= CVA_RTK_USER_PORT_WITHT1PHY_ID_MAX; userPortId++)
        {
            if(0u == (((uint32)0x1u << userPortId) & portMap))
            {
                continue;
            }
            /* Set the PHY local state of the input portId */
            regLocal = cva_g_sysPortToLocalCmd[cva_g_userPort2sysPort[userPortId]];
            /* Set the port's local OPFSM state by writing the value to the specific register */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (regLocal << 1u), regValue));
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_OP_STATE_LOCAL_ASIC */
/**@}*/ /* OPFSM_OP_STATE_LOCAL */

/**
 * @addtogroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_REQ Sleep request sending function
 * @{
 */
/**
 * @defgroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_REQ_ASIC Low Level Driver
 * @brief  The Low Level Driver function of sleep request sending provides sleep.request
 * @{
 */

/**
 * @brief Trigger the selected ports to start a remote sleep handshake with the link partner.\n
 *        The link status should be active, and the sleep capability of the selected ports must have been enabled.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map indicating selected ports (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Each bit in the port map indicates a specific port to initialize a sleep request:
 *                    - bit1: Select port1 to send sleep request
 *                    - bit2: Select port2 to send sleep request
 *                    - bit3: Select port3 to send sleep request
 *                    - bit4: Select port4 to send sleep request
 *                    - bit5: Select port5 to send sleep request
 *                    - bit6: Select port6 to send sleep request
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input port map is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_hw_indirect_set(), cva_reg_read(), and cva_reg_write().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot initialize a remote sleep handshake.
 *
 * @note Reference:
 *       - cva_op_set_phy_sleep_cap(uint32 unit, RtkPmap portMap, RtkEnable sleepCapOn).
 */
RtkApiRet cva_op_port_send_sleep_request(uint32 unit, RtkPmap portMap)
{
    uint32    regOffset  = 0u;
    uint32    regValue   = 0u;
    uint32    readClear  = 0u;
    RtkPmap   dstPmap    = 0u;
    RtkPort   userPortId = 1u;
    RtkPort   sysPort    = 0u;
    RtkApiRet retVal     = RT_ERR_OK;

    do
    {
        /* Check the ports in the input user port map are all with T1-PHY interface */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Search the user port to be configured according to the input user port map */
        for(userPortId = CVA_RTK_USER_PORT_1ST; userPortId <= CVA_RTK_USER_PORT_WITHT1PHY_ID_MAX; userPortId++)
        {
            if(0u == (((uint32)1u << userPortId) & portMap))
            {
                continue;
            }

            /* Read the register control the specific port to send sleep request */
            sysPort   = cva_g_userPort2sysPort[userPortId];
            regOffset = ((uint32)(cva_g_sysPortToOcpAddr[sysPort] | CVA_OCP_SLPCR)) << 1u;
            CHK_FUN_CONTINUE(retVal, cva_reg_read(unit, regOffset, &regValue));

            /* Need to clear the WUR and WUP flag in advance. Otherwise, the sleep request might not be sent successfully */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_WUR_FLAG << 1u), &readClear));
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_WUP_FLAG << 1u), &readClear));
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, CVA_OP_WUR_WUP_FLAG, 0x0u));
            /* Set bit[9] to 1 */
            CHK_FUN_CONTINUE(retVal, cva_reg_bit_set(unit, regOffset, OP_LOC_SLP_REQ_BIT, 1u));
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_REQ_ASIC */
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_REQ */

/**
 * @addtogroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_EVENT_STATUS Sleep event status function
 * @{
 */
/**
 * @defgroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_EVENT_STATUS_ASIC Low Level Driver
 * @brief  The Low Level Driver function return the receive sleep event, sleep.indication, sleepFail.indicate
 * @{
 */

/**
 * @brief Get the receiving sleep request indication flag.\n
 *        Note: The receiving sleep request indication flag is read and clear.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pRxSlpFlag Pointer to the receiving sleep flag (0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                        The value of the receiving sleep flag is a port map indicating whether each port has received a sleep request (LPS):
 *                        - bit1: port1 has received a sleep request (LPS)
 *                        - bit2: port2 has received a sleep request (LPS)
 *                        - bit3: port3 has received a sleep request (LPS)
 *                        - bit4: port4 has received a sleep request (LPS)
 *                        - bit5: port5 has received a sleep request (LPS)
 *                        - bit6: port6 has received a sleep request (LPS)
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer *pRxSlpFlag is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get(), cva_drv_hw_indirect_set(), and cva_drv_t1phy_pmap_convert().
 */
RtkApiRet cva_op_get_rx_sleep_flag(uint32 unit, uint16 *pRxSlpFlag)
{
    uint32    value32 = 0u;
    uint32    rxSleep = 0u;
    RtkPmap   dstPmap = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pRxSlpFlag)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Read the register indicating sleep event flags */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, CVA_OP_SLEEP_EVENT_FLAG, &value32));
        /* Get the receiving sleep request flag */
        rxSleep = (value32 & (CVA_OP_RX_SLP_EVENT_MASK)) >> CVA_OP_RX_SLP_EVENT_OFFSET;
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)rxSleep, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pRxSlpFlag = (uint16)(dstPmap);
        /* Clear the bits of receiving sleep request flag */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, CVA_OP_SLEEP_EVENT_FLAG, (value32 & (~CVA_OP_RX_SLP_EVENT_MASK))));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the sleep handshake fail indication flag.\n
 *        Note: The sleep handshake fail indication flag is read and clear.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pSlpFailFlag Pointer to the sleep handshake fail flag (0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                          The value of the sleep handshake fail flag is a port map indicating whether each port has a sleep fail event:
 *                          - bit1: port1 has a sleep fail event
 *                          - bit2: port2 has a sleep fail event
 *                          - bit3: port3 has a sleep fail event
 *                          - bit4: port4 has a sleep fail event
 *                          - bit5: port5 has a sleep fail event
 *                          - bit6: port6 has a sleep fail event
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer *pSlpFailFlag is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get(), cva_drv_hw_indirect_set(), and cva_drv_t1phy_pmap_convert().
 */
RtkApiRet cva_op_get_sleep_fail_flag(uint32 unit, uint16 *pSlpFailFlag)
{
    uint32    value32   = 0u;
    uint32    sleepFail = 0u;
    RtkPmap   dstPmap   = 0u;
    RtkApiRet retVal    = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pSlpFailFlag)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Read the register indicating sleep event flags */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, CVA_OP_SLEEP_EVENT_FLAG, &value32));
        /* Get the sleep handshake fail indication flag */
        sleepFail = (value32 & (CVA_OP_SLP_FAIL_EVENT_MASK)) >> CVA_OP_SLP_FAIL_EVENT_OFFSET;
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)sleepFail, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pSlpFailFlag = (uint16)(dstPmap);
        /* Clear the bits of sleep handshake fail indication flag */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, CVA_OP_SLEEP_EVENT_FLAG, (value32 & (~CVA_OP_SLP_FAIL_EVENT_MASK))));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the sleep handshake success indication flag.\n
 *        Note: The sleep handshake success indication flag is read and clear.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pSlpSuccessFlag Pointer to the sleep handshake success flag (0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                             The value of the sleep handshake success flag is a port map indicating whether each port has entered sleep mode:
 *                             - bit1: port1 has entered sleep mode
 *                             - bit2: port2 has entered sleep mode
 *                             - bit3: port3 has entered sleep mode
 *                             - bit4: port4 has entered sleep mode
 *                             - bit5: port5 has entered sleep mode
 *                             - bit6: port6 has entered sleep mode
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer *pSlpSuccessFlag is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get(), cva_drv_hw_indirect_set(), and cva_drv_t1phy_pmap_convert().
 */
RtkApiRet cva_op_get_sleep_success_flag(uint32 unit, uint16 *pSlpSuccessFlag)
{
    uint32    value32      = 0u;
    uint32    sleepSuccess = 0u;
    RtkPmap   dstPmap      = 0u;
    RtkApiRet retVal       = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pSlpSuccessFlag)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Read the register indicating sleep event flags */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, CVA_OP_SLEEP_EVENT_FLAG, &value32));
        /* Get the sleep handshake success indication flag */
        sleepSuccess = (value32 & (CVA_OP_SLP_SUCCESS_EVENT_MASK)) >> CVA_OP_SLP_SUCCESS_EVENT_OFFSET;
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)sleepSuccess, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pSlpSuccessFlag = (uint16)(dstPmap);
        /* Clear the bits of sleep handshake success indication flag */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, CVA_OP_SLEEP_EVENT_FLAG, (value32 & (~CVA_OP_SLP_SUCCESS_EVENT_MASK))));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_EVENT_STATUS_ASIC */
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_SLP_EVENT_STATUS */

/**
 * @addtogroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_REQ Wake-up request sending function
 * @{
 */
/**
 * @defgroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_REQ_ASIC Low Level Driver
 * @brief  The Low Level Driver function of wake request sending provides wakeup.request
 * @{
 */

/**
 * @brief Trigger the selected ports to send a remote wakeup to the link partner.\n
 *        If the link status of a port is in active mode, it will send WUR.\n
 *        If the link status of a port is in passive mode, it will send WUP.\n
 *        The selected ports must have the sleep capability enabled in advance.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] portMap The port map indicating selected ports (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Each bit in the port map indicates a specific port to initialize a remote wakeup:
 *                    - bit1: Select port1 to initialize a remote wakeup
 *                    - bit2: Select port2 to initialize a remote wakeup
 *                    - bit3: Select port3 to initialize a remote wakeup
 *                    - bit4: Select port4 to initialize a remote wakeup
 *                    - bit5: Select port5 to initialize a remote wakeup
 *                    - bit6: Select port6 to initialize a remote wakeup
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input port map is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_hw_indirect_get(), and cva_drv_hw_indirect_set().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot initialize a remote wakeup.
 *
 * @note Reference:
 *       - cva_op_set_en_wupr_tx(uint32 unit, RtkPmap portMap, uint8 cfgValue).
 */
RtkApiRet cva_op_port_send_rwake(uint32 unit, RtkPmap portMap)
{
    uint32    regLocal   = 0u;
    uint32    regValue   = 0u;
    RtkPmap   dstPmap    = 0u;
    RtkPort   userPortId = 1u;
    RtkApiRet retVal     = RT_ERR_OK;

    do
    {
        /* Check the ports in the input user port map are all with T1-PHY interface */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Search the user port to be configured according to the input user port map */
        for(userPortId = CVA_RTK_USER_PORT_1ST; userPortId <= CVA_RTK_USER_PORT_WITHT1PHY_ID_MAX; userPortId++)
        {
            if((((uint32)0x1u << userPortId) & portMap) == 0u)
            {
                continue;
            }

            /* Get the register for send remote wake of the current user port */
            regLocal = cva_g_sysPortToLocalCmd[cva_g_userPort2sysPort[userPortId]];
            /* Enable the bit4 to send remote wake on the current user port */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, ((regLocal) << 1u), &regValue));
            regValue |= 0x10u;
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, ((regLocal) << 1u), regValue));
        }
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_REQ_ASIC */
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_REQ */

/**
 * @addtogroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_EVENT_STATUS Wake-up event status function
 * @{
 */
/**
 * @defgroup OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_EVENT_STATUS_ASIC Low Level Driver
 * @brief  The Low Level Driver function provides the wakeup.indication
 * @{
 */

/**
 * @brief Get the WUP flag.\n
 *        If a port has received a WUP, the corresponding bit in the WUP flag will be 1.\n
 *        Note: The WUP flag is read-clear.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[out] pPortMap The WUP flag (0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      The WUP flag value is a port map indicating whether each port has received WUP:
 *                      - bit1: port1 has received WUP
 *                      - bit2: port2 has received WUP
 *                      - bit3: port3 has received WUP
 *                      - bit4: port4 has received WUP
 *                      - bit5: port5 has received WUP
 *                      - bit6: port6 has received WUP
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer pPortMap is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get(), cva_drv_hw_indirect_set(), and cva_drv_t1phy_pmap_convert().
 */
RtkApiRet cva_op_get_wup_flag(uint32 unit, RtkPmap *pPortMap)
{
    uint32    value32     = 0u;
    uint32    wakeoCfg    = 0u;
    uint32    pulseCfg    = 0u;
    uint32    virIntrMode = 0u;
    RtkPmap   dstPmap     = 0u;
    RtkApiRet retVal      = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Read 32 bits data from CVA_OP_WAKEO_GPIOB1_PORTS, including CVA_OP_WAKEO_GPIOB1_PORTS(bit[15:0]) and CVA_OP_WAKEO_GPIOB2_PORTS(bit[31:16]) */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_get(unit, CVA_OP_WAKEO_GPIOB1_PORTS, &wakeoCfg));
        /* Get Wake-out forwarding pulse config */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_get(unit, CVA_OP_WAKEO_PULSE_CFG, &pulseCfg));
        /* Get the g_Opfsm_VirInterrupt_Mode */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_get(unit, CVA_OP_VIR_INTR_MODE, &virIntrMode));
        /* The valid bit is 8 */
        virIntrMode &= 0xFFu;

        if(((0u != wakeoCfg) && (OP_WAKEO_PULSETYPE_UNKNOWN != pulseCfg)) || (CVA_OP_VIRINTERUPT_POLLING_ENABLE == virIntrMode))
        {
            /* Wake-out forwarding supported by ROM code is enabled */
            /* bit[15:0] of CVA_OP_WUR_WUP_FLAG is CVA_OP_WUP_FLAG */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, CVA_OP_WUR_WUP_FLAG, &value32));
            /* Clear WUP-flag bit[15:0] in CVA_OP_WUR_WUP_FLAG, set to 0 */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, CVA_OP_WUR_WUP_FLAG, (value32 & 0xFFFF0000u)));
            /* Read WUP-flag */
            value32 &= 0x0000FFFFu;
        }
        else
        {
            /* Wake-out forwarding supported by ROM code is disabled. Read the WUP flag. */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_WUP_FLAG << 1u), &value32));
        }
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)value32, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (dstPmap & 0xFFFFu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get the WUR flag.\n
 *        If a port has received a WUR, the corresponding bit in the WUR flag will be 1.\n
 *        Note: The WUR flag is read-clear.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[out] pPortMap The WUR flag (0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      The WUR flag value is a port map indicating whether each port has received WUR:
 *                      - bit1: port1 has received WUR
 *                      - bit2: port2 has received WUR
 *                      - bit3: port3 has received WUR
 *                      - bit4: port4 has received WUR
 *                      - bit5: port5 has received WUR
 *                      - bit6: port6 has received WUR
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer pPortMap is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get(), cva_drv_hw_indirect_set(), and cva_drv_t1phy_pmap_convert().
 */
RtkApiRet cva_op_get_wur_flag(uint32 unit, RtkPmap *pPortMap)
{
    uint32    value32     = 0u;
    uint32    wakeoCfg    = 0u;
    uint32    pulseCfg    = 0u;
    uint32    virIntrMode = 0u;
    RtkPmap   dstPmap     = 0u;
    RtkApiRet retVal      = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Read 32 bits data from CVA_OP_WAKEO_GPIOB1_PORTS, including CVA_OP_WAKEO_GPIOB1_PORTS(bit[15:0]) and CVA_OP_WAKEO_GPIOB2_PORTS(bit[31:16]) */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_get(unit, CVA_OP_WAKEO_GPIOB1_PORTS, &wakeoCfg));
        /* Get Wake-out forwarding pulse config */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_get(unit, CVA_OP_WAKEO_PULSE_CFG, &pulseCfg));
        /* Get the g_Opfsm_VirInterrupt_Mode */
        CHK_FUN_CONTINUE(retVal, cva_drv_indirect_sram_get(unit, CVA_OP_VIR_INTR_MODE, &virIntrMode));
        /* The valid bit is 8 */
        virIntrMode &= 0xFFu;

        if(((0u != wakeoCfg) && (OP_WAKEO_PULSETYPE_UNKNOWN != pulseCfg)) || (CVA_OP_VIRINTERUPT_POLLING_ENABLE == virIntrMode))
        {
            /* Wake-out forwarding supported by ROM code is enabled */
            /* bit[31:16] of CVA_OP_WUR_WUP_FLAG is CVA_OP_WUR_FLAG */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, CVA_OP_WUR_WUP_FLAG, &value32));
            /* Clear WUR-flag bit[31:16] in CVA_OP_WUR_WUP_FLAG, set to 0 */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, CVA_OP_WUR_WUP_FLAG, (value32 & (0x0000FFFFu))));
            /* Read WUR-flag */
            value32 = ((value32 & 0xFFFF0000u) >> 16u);
        }
        else
        {
            /* Wake-out forwarding supported by ROM code is disabled. Read the WUR flag. */
            CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_WUR_FLAG << 1u), &value32));
        }
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)value32, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (dstPmap & 0xFFFFu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Clear wake flags including WUP flag, WUR flag, POWN (PowerOn), and LWAKE (LocalWakeup).
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval Other error codes as defined in cva_drv_hw_indirect_get() and cva_drv_hw_indirect_set().
 */
RtkApiRet cva_op_wakeflag_clearall(uint32 unit)
{
    uint32    readClear = 0u;
    RtkApiRet retVal    = RT_ERR_OK;

    do
    {
        /* Clear CVA_OP_WUR_WUP_FLAG */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, CVA_OP_WUR_WUP_FLAG, 0x0u));
        /* Read-clear WUR Reg */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_WUR_FLAG << 1u), &readClear));
        /* Read-clear WUP Reg */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_WUP_FLAG << 1u), &readClear));
        /* Read-clear Recover Reg */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_REC_FLAG << 1), &readClear));
        /* Read-clear Power-on Reg */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_PWON_FLAG << 1), &readClear));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_EVENT_STATUS_ASIC */
/**@}*/ /* OPFSM_SLP_WAKE_SERVICE_PRIMITIVES_WAKE_EVENT_STATUS */

/**
 * @addtogroup OPFSM_INTR_OPFSM_EVENT OPFSM event interrupt function
 * @{
 */
/**
 * @defgroup OPFSM_INTR_OPFSM_EVENT_ASIC Low Level Driver
 * @brief  The Low Level Driver to configure the OPFSM event interrupt to CPU or externnal interrupt
 * @{
 */

/**
 * @brief Set CVA_OP_OTHER_INTR_MSK port map.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] cfgValue Set value (0 to 0xF).\n
 *                     Each bit configuration of CVA_OP_OTHER_INTR_MSK:
 *                     - bit3: rec_rg_rst_intr_msk (set to 0x1 to stop reset by reg to trigger interrupt)
 *                     - bit2: rec_pin_rst_intr_msk (set to 0x1 to stop reset by pin to trigger interrupt)
 *                     - bit1: pwon_intr_msk (set to 0x1 to stop power on event to trigger interrupt)
 *                     - bit0: lwake_intr_msk (set to 0x1 to stop local wake to trigger interrupt)
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get() and cva_drv_hw_indirect_set().
 */
RtkApiRet cva_op_set_event_ext_intr_msk(uint32 unit, uint16 cfgValue)
{
    uint32    regValue = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((OP_REC_RG_RST_INTR_MSK | OP_REC_PIN_RST_INTR_MSK | OP_PWON_INTR_MSK | OP_LWAKE_INTR_MSK) < cfgValue)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /* Get the current setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_OTHER_INTR_MSK << 1u), &regValue));
        /* Get valid bits */
        regValue &= 0xFFFFu;
        regValue = (regValue & 0x0FFFu) | ((cfgValue << 12u) & 0xF000u);
        /* Write the updated current setting back to the register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (uint32)CVA_OP_OTHER_INTR_MSK << 1u, regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get CVA_OP_OTHER_INTR_MSK port map.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[out] pCfgValue Get CVA_OP_OTHER_INTR_MSK (0 to 0xF).\n
 *                       Each bit of CVA_OP_OTHER_INTR_MSK:
 *                       - bit3: rec_rg_rst_intr_msk (bit value 0x1: stop reset by reg to trigger interrupt)
 *                       - bit2: rec_pin_rst_intr_msk (bit value 0x1: stop reset by pin to trigger interrupt)
 *                       - bit1: pwon_intr_msk (bit value 0x1: stop power on event to trigger interrupt)
 *                       - bit0: lwake_intr_msk (bit value 0x1: stop local wake to trigger interrupt)
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer pCfgValue is NULL.
 * @retval Other error codes as defined in cva_drv_hw_indirect_get().
 */
RtkApiRet cva_op_get_event_ext_intr_msk(uint32 unit, uint16 *pCfgValue)
{
    uint32    regValue = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pCfgValue)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* get external interrupt mask of OPFSM events */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_OP_OTHER_INTR_MSK << 1), &regValue));
        /* Get valid bits */
        regValue &= 0xFFFFu;
        regValue   = (regValue >> 12u) & 0xFu;
        *pCfgValue = (uint16)regValue;
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* OPFSM_INTR_OPFSM_EVENT_ASIC */
/**@}*/ /* OPFSM_INTR_OPFSM_EVENT */

/**
 * @addtogroup OPFSM_INTR_SLEEP_WAKE_EVENT_INTR Sleep and Wake-up received interrupt function
 * @{
 */
/**
 * @defgroup OPFSM_INTR_SLEEP_WAKE_EVENT_INTR_ASIC Low Level Driver
 * @brief  The Low Level Driver to configure the sleep and wake-up received interrupt function
 * @{
 */

/**
 * @brief Set specific ports to send a signal on the interrupt pin (INTB) upon receiving a WUR.\n
 *        Users can get the WUR flag indicating which port received WUR by using the function cva_op_get_wur_flag(uint16 *getValue).\n
 *        The default WUR interrupt mask is all 1.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] portMap The WUR interrupt mask (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Each bit indicates a port to configure the WUR interrupt mask setting:
 *                    - bit1: set port1's WUR interrupt mask setting
 *                    - bit2: set port2's WUR interrupt mask setting
 *                    - bit3: set port3's WUR interrupt mask setting
 *                    - bit4: set port4's WUR interrupt mask setting
 *                    - bit5: set port5's WUR interrupt mask setting
 *                    - bit6: set port6's WUR interrupt mask setting
 * @param[in] cfgValue Set value (0 or 1).\n
 *                     Enable or disable the WUR interrupt mask setting:
 *                     - 0: Enable sending a signal on the interrupt pin (INTB).
 *                     - 1: Stop sending a signal on the interrupt pin (INTB).
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input port map or cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_hw_indirect_get(), and cva_drv_hw_indirect_set().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot be in sleep mode.
 *
 * @note Reference:
 *       - cva_op_get_wur_ext_intr_msk(uint32 unit, uint16 *getValue)
 */
RtkApiRet cva_op_set_wur_ext_intr_msk(uint32 unit, RtkPmap portMap, uint8 cfgValue)
{
    uint32    regValue = 0u;
    RtkPmap   dstPmap  = 0u;
    RtkPort   phyId    = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((1u != cfgValue) && (0u != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        /* Get the current setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_WUR_INTR_MSK << 1), &regValue));
        /* Get valid bits */
        regValue &= CVA_RTK_T1PHY_PMAP_ALL;
        /* Update the current setting according to the input */
        for(phyId = 0u; phyId <= CVA_RTK_PHY_ID_MAX; phyId++)
        {
            if(0u == (((uint32)0x1u << phyId) & dstPmap))
            {
                continue;
            }

            if(1u == cfgValue)
            {
                regValue = regValue | ((uint32)(0x1u) << phyId);
            }
            else
            {
                regValue = regValue & (((uint32)(0x1u) << phyId) ^ CVA_REG_DATA_MASK);
            }
        }
        /* Write the updated current setting back to the register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (CVA_WUR_INTR_MSK << 1u), regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get WUR interrupt mask.\n
 *        Once the ports mask the WUR interrupt received WUR, the WUR interrupt will not pass through the external interrupt PIN.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] cfgValue Set value (0 or 1).\n
 *                     Mask or unmask the WUR external interrupt:
 *                     - Mask: 1
 *                     - Unmask: 0
 * @param[out] pPortMap The WUR interrupt mask (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Returns the WUR interrupt mask:
 *                      - bit1: get port1's WUR interrupt mask setting
 *                      - bit2: get port2's WUR interrupt mask setting
 *                      - bit3: get port3's WUR interrupt mask setting
 *                      - bit4: get port4's WUR interrupt mask setting
 *                      - bit5: get port5's WUR interrupt mask setting
 *                      - bit6: get port6's WUR interrupt mask setting
 *                      If the bit value is 0: Enable sending a signal on the interrupt pin (INTB).\n
 *                      If the bit value is 1: Disable sending a signal on the interrupt pin (INTB).
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer pPortMap is NULL.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert() and cva_drv_hw_indirect_get().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot be in sleep mode.
 *
 * @note Reference:
 *       - cva_op_set_wur_ext_intr_msk(uint32 unit, RtkPmap portMap, uint8 cfgValue).
 */
RtkApiRet cva_op_get_wur_ext_intr_msk(uint32 unit, uint8 cfgValue, RtkPmap *pPortMap)
{
    uint32    value32 = 0u;
    RtkPmap   dstPmap = 0u;
    RtkApiRet retVal  = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* get external interrupt mask of WUR */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_WUR_INTR_MSK << 1), &value32));
        if(1u == cfgValue)
        {
            value32 &= (CVA_RTK_T1PHY_PMAP_ALL);
        }
        else
        {
            value32 = ((~value32) & CVA_RTK_T1PHY_PMAP_ALL);
        }
        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)value32, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (dstPmap & 0xFFFFu);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Set specific ports to enable sending a signal on the interrupt pin (INTB) after the specific port's PHY receives a WUP.\n
 *        Users can get the WUP flag indicating which port received WUP by using the function cva_op_get_wup_flag().\n
 *        The default WUP interrupt mask is all 1.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] portMap The WUP interrupt mask (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                    Each bit indicates a port to configure the WUP interrupt mask setting:
 *                    - bit1: set port1's WUP interrupt mask setting
 *                    - bit2: set port2's WUP interrupt mask setting
 *                    - bit3: set port3's WUP interrupt mask setting
 *                    - bit4: set port4's WUP interrupt mask setting
 *                    - bit5: set port5's WUP interrupt mask setting
 *                    - bit6: set port6's WUP interrupt mask setting
 * @param[in] cfgValue Set value (0 or 1).\n
 *                     Enable or disable the WUP interrupt mask setting:
 *                     - 0: Enable sending a signal on the interrupt pin (INTB).
 *                     - 1: Stop sending a signal on the interrupt pin (INTB).
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_INPUT if the input port map or cfgValue is invalid.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert(), cva_drv_hw_indirect_get(), and cva_drv_hw_indirect_set().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot be in sleep mode.
 *
 * @note Reference:
 *       - cva_op_get_wup_ext_intr_msk(uint32 unit, uint16 *getValue)
 */
RtkApiRet cva_op_set_wup_ext_intr_msk(uint32 unit, RtkPmap portMap, uint8 cfgValue)
{
    uint32    regValue = 0u;
    RtkPmap   dstPmap  = 0u;
    RtkPort   phyId    = 0u;
    RtkApiRet retVal   = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if((1u != cfgValue) && (0u != cfgValue))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Translate the user port map to PHY index mask */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert(portMap, &dstPmap, CVA_EM_USER_TO_T1PHY));
        dstPmap = (dstPmap << 2u);
        /* Get the current setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_WUP_INTR_MSK << 1u), &regValue));
        /* Get valid bits */
        regValue &= ((CVA_RTK_T1PHY_PMAP_ALL << 2u) | ((1u << 2u) - 1u));
        for(phyId = 0u; phyId <= (CVA_RTK_PHY_ID_MAX + 2u); phyId++)
        {
            if(0u == (((uint32)0x1u << phyId) & dstPmap))
            {
                continue;
            }

            if(1u == cfgValue)
            {
                regValue = regValue | ((uint32)(0x1u) << phyId);
            }
            else
            {
                regValue = regValue & (((uint32)(0x1u) << phyId) ^ CVA_REG_DATA_MASK);
            }
        }
        /* Write the updated current setting back to the register */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_set(unit, (CVA_WUP_INTR_MSK << 1u), regValue));
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Get WUP interrupt mask.\n
 *        Once the ports mask the WUP interrupt received WUP, the WUP interrupt will not pass through the external interrupt PIN.
 *
 * @param[in] unit Stacking unit (1 to 7).\n
 *                 The stacking unit ID is used for indicating which switch is to be accessed.
 * @param[out] pPortMap The WUP interrupt mask (0x0 to CVA_RTK_USER_PMAP_WITHT1PHY).\n
 *                      Returns the WUP interrupt mask:
 *                      - bit1: get port1's WUP interrupt mask setting
 *                      - bit2: get port2's WUP interrupt mask setting
 *                      - bit3: get port3's WUP interrupt mask setting
 *                      - bit4: get port4's WUP interrupt mask setting
 *                      - bit5: get port5's WUP interrupt mask setting
 *                      - bit6: get port6's WUP interrupt mask setting
 *                      If the bit value is 0: Enable sending a signal on the interrupt pin.\n
 *                      If the bit value is 1: Disable sending a signal on the interrupt pin.
 *
 * @retval RT_ERR_OK The function was executed successfully
 * @retval -RT_ERR_NULL_POINTER if the input pointer pPortMap is NULL.
 * @retval Other error codes as defined in cva_drv_t1phy_pmap_convert() and cva_drv_hw_indirect_get().
 *
 * @note The PHY of ports 7, 8, 9, 10, 11, and 12 cannot be in sleep mode.
 *
 * @note Reference:
 *       - cva_op_set_wup_ext_intr_msk(uint32 unit, RtkPmap portMap, uint8 cfgValue).
 */
RtkApiRet cva_op_get_wup_ext_intr_msk(uint32 unit, uint8 cfgValue, RtkPmap *pPortMap)
{
    uint32    value32   = 0u;
    uint32    regValTmp = 0u;
    RtkPmap   dstPmap   = 0u;
    RtkApiRet retVal    = RT_ERR_OK;

    do
    {
        /* Check input parameters */
        if(NULL == pPortMap)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get the current setting */
        CHK_FUN_CONTINUE(retVal, cva_drv_hw_indirect_get(unit, (CVA_WUP_INTR_MSK << 1), &regValTmp));
        /* Valid bits is bit2 to bit7 of value32 */
        value32 = regValTmp & (CVA_RTK_T1PHY_PMAP_ALL << 2u);
        value32 = value32 >> 2u; /* align reg "rwake_p0_intr_msk" to bit0 */
        if(0u == cfgValue)
        {
            value32 = ((~value32) & CVA_RTK_T1PHY_PMAP_ALL);
        }

        /* Translate the PHY index mask to user port map */
        CHK_FUN_CONTINUE(retVal, cva_drv_t1phy_pmap_convert((RtkPmap)value32, &dstPmap, CVA_EM_T1PHY_TO_USER));
        *pPortMap = (dstPmap & 0xFFFFu);
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* OPFSM_INTR_SLEEP_WAKE_EVENT_INTR_ASIC */
/**@}*/ /* OPFSM_INTR_SLEEP_WAKE_EVENT_INTR */

/**@}*/ /* RTK_MODULE_OPFSM */
