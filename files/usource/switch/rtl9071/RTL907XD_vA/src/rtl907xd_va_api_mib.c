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

#include "rtl907xd_va_asicdrv_mib.h"
#include "rtl907xd_va_api_ext.h"
#include "rtkas_api_ext.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"

/**
 * @addtogroup RTK_MODULE_MIB MIB
 * @{
 */

/**
 * @addtogroup MIB_ALL_RESET Reset All MIB Counters
 * @{
 */

/**
 * @addtogroup MIB_ALL_RESET_ASIC Low Level Driver
 * @brief Reset All MIB Counters Low Level Driver
 * @{
 */

/**
 * @brief Reset all ports and global MIB counters.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Reset all ports and global MIB counters successfully.
 * @retval Others Refer to dva_drv_mib_counter_reset() for the error.
 * @note This API resets all ports and global MIB counters.
 */
RtkApiRet dva_stat_reset_all(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Reset all port and global counters. */
        CHK_FUN_CONTINUE(retVal, dva_drv_mib_counter_reset(unit, 1u, (DVA_RTK_SYS_PMAP_ALL | DVA_RTK_CPU_PMAP)));

    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_ALL_RESET_ASIC */

/**@}*/ /* MIB_ALL_RESET */

/**
 * @addtogroup MIB_ALL_ENABLE Enable All MIB Counters
 * @{
 */

/**
 * @addtogroup MIB_ALL_ENABLE_ASIC Low Level Driver
 * @brief Enable All MIB Counters Low Level Driver
 * @{
 */

/**
 * @brief Start all MIB counters.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Start all MIB counters successfully.
 * @retval Others Refer to dva_drv_indirect_reg_field_set() or dva_drv_indirect_reg_field_get() for the error.
 * @note This API starts all MIB counters.
 */
RtkApiRet dva_stat_start_all(uint32 unit)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    RtkPort   sysPort = 0u;

    do
    {
        /* Set MIB counter control registers. */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL0, DVA_MIB_COUNTER_CONTROL0_ENMIBCOUNTER_LSP,
                                                                DVA_MIB_COUNTER_CONTROL0_ENMIBCOUNTER_LEN, 1u));
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL0, DVA_MIB_COUNTER_CONTROL0_ENDBMIBCOUNTER_LSP,
                                                                DVA_MIB_COUNTER_CONTROL0_ENDBMIBCOUNTER_LEN, 1u));
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_MIB_COUNTER_CONTROL2, 0u, 32u, &regVal));
        regVal |= 0xFFFFFFFFu;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL2, 0u, 32u, regVal));

        /* Set MIB counter control registers to ports. */
        for(sysPort = DVA_RTK_SYS_PORT_1ST; sysPort <= DVA_RTK_SYS_PORT_END; sysPort++)
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_MIB_COUNTER_CONTROL + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                                    DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 1u));
        }
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT15_MIB_COUNTER_CONTROL, DVA_PORT15_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                                DVA_PORT15_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 1u));
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_ALL_ENABLE_ASIC */

/**@}*/ /* MIB_ALL_ENABLE */

/**
 * @addtogroup MIB_ALL_DISABLE Disable All MIB Counters
 * @{
 */

/**
 * @addtogroup MIB_ALL_DISABLE_ASIC Low Level Driver
 * @brief Disable All MIB Counters Low Level Driver
 * @{
 */

/**
 * @brief Stop all MIB counters.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Stop all MIB counters successfully.
 * @retval Others Refer to dva_drv_indirect_reg_field_set() or dva_drv_indirect_reg_field_getfor the error.
 * @note This API stops all MIB counters.
 */
RtkApiRet dva_stat_stop_all(uint32 unit)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    RtkPort   sysPort = 0u;

    do
    {
        /* Set MIB counter control registers. */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_MIB_COUNTER_CONTROL0, 0u, 32u, &regVal));
        regVal &= 0xFFFFFFFCu;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL0, 0u, 32u, regVal));

        regVal = 0x0u;
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_MIB_COUNTER_CONTROL2, 0u, 32u, regVal));

        /* Set MIB counter control registers for ports. */
        for(sysPort = DVA_RTK_SYS_PORT_1ST; sysPort <= DVA_RTK_SYS_PORT_END; sysPort++)
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_MIB_COUNTER_CONTROL + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                                    DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 0u));
        }
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT15_MIB_COUNTER_CONTROL, DVA_PORT15_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                                DVA_PORT15_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 0u));
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_ALL_DISABLE_ASIC */

/**@}*/ /* MIB_ALL_DISABLE */

/**
 * @addtogroup MIB_PORT_REASON_ALE Get ALE Drop Reason Example
 * @{
 */

/**
 * @addtogroup MIB_PORT_REASON_ALE_ASIC Low Level Driver
 * @brief Get ALE Drop Reason Low Level Driver
 * @{
 */

/**
 * @brief Get the ALE drop reason.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] dropReason Drop Reason (0 to 255) \n
 *             Indicates the drop reason in the ALE stage. \n
 *             7: Rx Disable \n
 *             8: Packet parser IDEI(CFI) \n
 *             9: LA LACP packet \n
 *             10: LA marker packet \n
 *             11: RMA \n
 *             12: ACL redirect drop/trap \n
 *             13: ACL drop to CPU action \n
 *             14: ACL policer drop \n
 *             15: ACL lookup miss \n
 *             16: Ingress bandwidth control \n
 *             17: SNMP trap/drop/copy \n
 *             26: Accept frame tag type \n
 *             27: VLAN table lookup miss \n
 *             28: VLAN ingress filter \n
 *             29: Dot1X ingress check \n
 *             30: Dot1X egress filter (MAC/Port) \n
 *             31: Spanning tree ingress filter \n
 *             32: DSTSWMACCHK(switch DA check) \n
 *             34: IGMP/MLD/ARP \n
 *             35: TTL drop \n
 *             36: L2UCIPMC \n
 *             37: L2MCIPMC \n
 *             38: DA block \n
 *             39: LKMISS(L2\L3, unicast\multicast\broadcast) \n
 *             40: invalid SA (including DA == SA) \n
 *             41: Forwarding table PM empty \n
 *             42: SA block \n
 *             43: SA moving (legal/illegal) \n
 *             44: MAC constrain \n
 *             46: Multicast egress filter (router port/multidisport) \n
 *             47: Source port self filter \n
 *             48: storm suppression \n
 *             49: Egress VLAN filter \n
 *             50: Egress SPT filter \n
 *             52: Port isolation \n
 *             53: CPU port self filter \n
 *             54: Link aggregation member port change \n
 *             56: Mirror egress filter \n
 *             57: Tx disable \n
 *             58: Flow control \n
 *             59: Port link down
 *             71: IPV4 UC ZERO SIP ACTION DROP \n
 *             74: IPV6 UC ZERO SIP ACTION DROP \n
 *             92: IPV4 MC HDR ERR ACTION DROP \n
 *             94: IPV6 MC HDR ERR ACTION DROP \n
 *             95: IPV4 MC BAD SIP ACTION DROP \n
 *             97: IPV4 MC ZERO SIP ACTION DROP \n
 *             99: IPV6 MC BAD SIP ACTION DROP \n
 *             101: IPV6 MCZERO SIP ACTION DROP \n
 *             104: IPV4 MC DMAC MISMATH ACTION DROP \n
 *             106: IPV6 MC DMAC MISMATH ACTION DROP \n
 *             107: IPV4 MC HDR OPT ACTION DROP \n
 *             110: IPV6 MC HBH ERR ACTION DROP \n
 *             113: IPV6 MC HBH ACTION DROP \n
 *             116: IPV6 MC ROUTE ACTION DROP \n
 *             120: IPV4 MC ROUTE LU MISS ACTION DROP \n
 *             122: IPV6 MC ROUTE LU MISS ACTION DROP \n
 *             123: UNICAST ROUTE DISABLE \n
 *             129: L3 Unicast lookup miss drop \n
 *             130: IPv4 TTL check drop \n
 *             131: IPv6 HL check drop \n
 *             133: Non IP drop \n
 *             134: IVID != Ingress table INTFID \n
 *             135: max sdu drop: the frame exceeds the stream max sdu. \n
 *             136: gate close drop: the frame is received when stream gate state is close \n
 *             137: exceed interval octet drop: the frame bandwidth exceeds the interval octect max. \n
 *             138: yellow frame drop: the flow meter judge this is yellow frame and drop on yellow is enabled. \n
 *             139: red frame drop: the flow meter judges this is red frame. \n
 *             140: Inactive flow meter drop: the frame is discarded because assigned to a inactive flow meter \n
 *             141: ACL TCAM error handling \n
 *             142: IP Routing TCAM error handling \n
 *             143: Qci TCAM error handling \n
 *             144: EACL redirect drop \n
 *             145: EACL drop \n
 *             146: EACL policer drop \n
 *             147: EACL lookup miss drop \n
 *             149: Invalid SA detection \n
 *             150: ACL_TIMER_DISCARD \n
 *             151: Dot1X egress filter \n
 *             152: L3_NH_AGEOUT_ACT \n
 *             153: CB drop \n
 *             154: ACL abnormal drop \n
 *             155: EACL abnormal drop \n
 *             156: EACL_TIMER_DISCARD \n
 *             Others: Reserved
 * @retval RT_ERR_OK Get the ALE drop reason successfully.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_drv_indirect_reg_field_get() for the error.
 * @note Get the reason of dot1dTpPortInDiscards. The dot1dTpPortInDiscards divides into Address Lookup Engine \n
 *       (ALE) drop which means system drop and RX drop. This API gets ALE drop reason and can only get the reason\n
 *       of the last ingress packet.
 */
RtkApiRet dva_stat_get_ale_drop_reason(uint32 unit, uint32 *dropReason)
{
    uint32    val    = 0u;
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid. */
        if(NULL == dropReason)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get packet process information register. */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PACKET_PROCESS_INFORMATION5_CR2, DVA_PACKET_PROCESS_INFORMATION5_CR2_REASON_LSP,
                                                                DVA_PACKET_PROCESS_INFORMATION5_CR2_REASON_LEN, &val));

        *dropReason = val;
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_REASON_ALE_ASIC */

/**@}*/ /* MIB_PORT_REASON_ALE */

/**
 * @addtogroup MIB_PORT_QUEUE Get Specific Port RX Max Queue Level
 * @{
 */

/**
 * @addtogroup MIB_PORT_QUEUE_ASIC Low Level Driver
 * @brief Get Specific Port RX Max Queue Level Low Level Driver
 * @{
 */

/**
 * @brief Get the specified port max queue level.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identification (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END, DVA_RTK_CPU_PORT) \n
 *            Indicates the port ID for getting max queue level.
 * @param[out] maxQueueLevel Max Queue Level (0 to 2047) \n
 *             Indicates the max queue level of the port.
 * @retval RT_ERR_OK Get the specified port max queue level successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_drv_indirect_reg_field_get() for the error.
 * @note This API gets the value of per port max queue level(unit = page; 1 page = 128 Bytes).
 */
RtkApiRet dva_stat_port_max_queue_level_get(uint32 unit, RtkPort port, uint32 *maxQueueLevel)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    pagePeakValue = 0u;
    RtkPort   sysPort       = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || (((uint32)DVA_RTK_USER_PORT_END < port) && ((uint32)DVA_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == maxQueueLevel)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_OCCUPIED_RX_PAGE_PEAK_COUNTER + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_OCCUPIED_RX_PAGE_PEAK_COUNTER_RXPAGEPEAKCNT_LSP,
                                                                DVA_PORT0_OCCUPIED_RX_PAGE_PEAK_COUNTER_RXPAGEPEAKCNT_LEN, &pagePeakValue));

        *maxQueueLevel = pagePeakValue;

    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_QUEUE_ASIC */

/**@}*/ /* MIB_PORT_QUEUE */

/**
 * @addtogroup MIB_PORT_RESET Reset Specific Port MIB Counters
 * @{
 */

/**
 * @addtogroup MIB_PORT_RESET_ASIC Low Level Driver
 * @brief Reset Specific Port MIB Counters Low Level Driver
 * @{
 */

/**
 * @brief Reset the specified port MIB counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identification (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END, DVA_RTK_CPU_PORT) \n
 *            Indicates the port ID for resetting the port counter.
 * @param[out] None
 * @retval RT_ERR_OK Reset the specified port MIB counter successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval Others Refer to dva_drv_mib_counter_reset() for the error.
 * @note This API resets the specified port MIB counter.
 */
RtkApiRet dva_stat_port_reset(uint32 unit, RtkPort port)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || (((uint32)DVA_RTK_USER_PORT_END < port) && ((uint32)DVA_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Reset port counters. */
        CHK_FUN_CONTINUE(retVal, dva_drv_mib_counter_reset(unit, 0u, ((dva_RtkPmap)1u << sysPort)));
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_RESET_ASIC */

/**@}*/ /* MIB_PORT_RESET */

/**
 * @addtogroup MIB_PORT_ENABLE Enable Specific Port MIB Counters
 * @{
 */

/**
 * @addtogroup MIB_PORT_ENABLE_ASIC Low Level Driver
 * @brief Enable Specific Port MIB Counters Low Level Driver
 * @{
 */

/**
 * @brief Start the specified port MIB counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identification (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END, DVA_RTK_CPU_PORT) \n
 *            Indicates the port ID for starting the port counter.
 * @param[out] None
 * @retval RT_ERR_OK Start the specified port MIB counter successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval Others Refer to dva_drv_mib_counter_enable() or dva_drv_indirect_reg_field_set() for the error.
 * @note This API starts the specified port MIB counter.
 */
RtkApiRet dva_stat_port_start(uint32 unit, RtkPort port)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || (((uint32)DVA_RTK_USER_PORT_END < port) && ((uint32)DVA_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Enable port counters. */
        CHK_FUN_CONTINUE(retVal, dva_drv_mib_counter_enable(unit, TRUE));
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_MIB_COUNTER_CONTROL + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                                DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 1u));

    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_ENABLE_ASIC */

/**@}*/ /* MIB_PORT_ENABLE */

/**
 * @addtogroup MIB_PORT_DISABLE Disable Specific Port MIB Counters
 * @{
 */

/**
 * @addtogroup MIB_PORT_DISABLE_ASIC Low Level Driver
 * @brief Disable Specific Port MIB Counters Low Level Driver
 * @{
 */

/**
 * @brief Stop the specified port MIB counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identification (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END, DVA_RTK_CPU_PORT) \n
 *            Indicates the port ID for stopping the port counter.
 * @param[out] None
 * @retval RT_ERR_OK Stop the specified port MIB counter successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval Others Refer to dva_drv_indirect_reg_field_set() for the error.
 * @note This API stops the specified port MIB counter.
 */
RtkApiRet dva_stat_port_stop(uint32 unit, RtkPort port)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || (((uint32)DVA_RTK_USER_PORT_END < port) && ((uint32)DVA_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Disable port counters. */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_MIB_COUNTER_CONTROL + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                                DVA_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 0u));
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_DISABLE_ASIC */

/**@}*/ /* MIB_PORT_DISABLE */

/**
 * @addtogroup MIB_PORT_GET Get Specific Port/Global Counter
 * @{
 */

/**
 * @addtogroup MIB_PORT_GET_ASIC Low Level Driver
 * @brief Get Specific Port/Global Counters Low Level Driver
 * @{
 */

/**
 * @brief Get the specified port MIB counter.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identification (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END, DVA_RTK_CPU_PORT) \n
 *            Indicates the port ID for getting the port counter.
 * @param[in] counterType Counter Type (N/A) \n
 *            Indicates the counter type for getting the counter value.
 * @param[out] counterValue Counter Value (0 to 4294967295) \n
 *             Indicates the counter value.
 * @retval RT_ERR_OK Get the specified port MIB counter successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_STAT_INVALID_PORT_CNTR The counter type is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_drv_mib_counter_get() for the error.
 * @note This API gets the specified port MIB counter.
 */
RtkApiRet dva_stat_port_get(uint32 unit, RtkPort port, RtkMibCounterType_e counterType, uint32 *counterValue)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || (((uint32)DVA_RTK_USER_PORT_END < port) && ((uint32)DVA_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if((EM_SOMEIP_DOIP_WHITELIST_HIT31 < counterType) || ((EM_IPV4_UNDER_SIZE_DROP < counterType) && (EM_WIRE_FRAMELEN_ERROR_DROP > counterType))
           || ((EM_QMAXSDU_P15Q7_DROP < counterType) && (EM_IFOUTOCTETS_LSB > counterType)))
        {
            retVal = -RT_ERR_STAT_INVALID_PORT_CNTR;
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == counterValue)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get port counter value. */
        CHK_FUN_CONTINUE(retVal, dva_drv_mib_counter_get(unit, counterType, sysPort, counterValue));
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_GET_ASIC */

/**@}*/ /* MIB_PORT_GET */

/**
 * @addtogroup MIB_PORT_REASON_RX Get RX Drop Reason
 * @{
 */

/**
 * @addtogroup MIB_PORT_REASON_RX_ASIC Low Level Driver
 * @brief Get RX Drop Reason Low Level Driver
 * @{
 */

/**
 * @brief Get the RX drop reason.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identification (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END, DVA_RTK_CPU_PORT) \n
 *            Indicates the port ID for getting the drop reason.
 * @param[out] dropReason Drop reason (0 to 31) \n
 *             Indicates the port drop reason. \n
 *             1: enrx = 0, rx enable reg is off \n
 *             2: stop_ale = 1, ale test enable reg(en_aletest) is on \n
 *             3: sys_flood happened, pkt buffer over the system flow control threshold \n
 *             4: crc error \n
 *             5: symbol error \n
 *             6: alignment error \n
 *             7: pkt is less than 64 bytes \n
 *             8: pkt is over max length \n
 *             9: received the pause pkt \n
 *             10: iol drop, iol pause function, including unknown op code, unknown da, unknown ether type \n
 *             11: iol length error \n
 *             12: iol max length \n
 *             13: over 48 times conflicts, pass pkt is over 1536 bytes(12 pages) \n
 *             14: attack prevention drop \n
 *             15: wol drop \n
 *             16: packet pass, no drop \n
 *             17: vlan hopping \n
 *             19: packet drop by port relay disable \n
 *             20: ALE bandwidth full drop \n
 *             21: wire length error drop \n
 *             Others: Reserved
 * @retval RT_ERR_OK Get the RX drop reason successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dva_drv_indirect_reg_field_get() for the error.
 * @note Get the reason of dot1dTpPortInDiscards. The dot1dTpPortInDiscards divides into Address Lookup Engine \n
 *       (ALE) drop which means system drop and RX drop. This API gets RX drop reason and can only get the reason\n
 *       of the last ingress packet.
 */
RtkApiRet dva_stat_port_get_rx_drop_reason(uint32 unit, RtkPort port, uint32 *dropReason)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    val     = 0u;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || (((uint32)DVA_RTK_USER_PORT_END < port) && ((uint32)DVA_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(NULL == dropReason)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get port drop reason register. */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_DROP_REASON + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_DROP_REASON_DROPREASON_LSP, DVA_PORT0_DROP_REASON_DROPREASON_LEN, &val));

        *dropReason = val;

    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_REASON_RX_ASIC */

/**@}*/ /* MIB_PORT_REASON_RX */

/**
 * @addtogroup MIB_PORT_RATE_CONFIG_SET Set Port Rate Config
 * @{
 */

/**
 * @addtogroup MIB_PORT_RATE_CONFIG_SET_ASIC Low Level Driver
 * @brief Set Port Rate Config Low Level Driver
 * @{
 */

/**
 * @brief Set the specified port rate configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] state Port Rate Meter State (DISABLED, ENABLED) \n
 *            Indicates the port rate meter state.
 * @param[in] ratePeriod Rate Period (0 to 4294967295) \n
 *            Indicates the rate period for calculating the port rate.
 * @retval RT_ERR_OK Set the specified port rate meter configuration successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_INPUT The rate period is invalid.
 * @retval Others Refer to dva_drv_indirect_reg_field_set() for the error.
 * @note This API sets the specified port rate configuration.
 */
RtkApiRet dva_stat_port_rate_config_set(uint32 unit, RtkPort port, RtkEnable state, RtkMibRatePeriod_e ratePeriod)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || (((uint32)DVA_RTK_USER_PORT_END < port) && ((uint32)DVA_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if(RTK_ENABLE_END <= state)
        {
            retVal = -RT_ERR_ENABLE;
            continue;
        }

        /* Check if the parameter is valid. */
        if(EM_RATE_PERIOD_END <= ratePeriod)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Set port rate meter state and rate period. */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_RATE_METER + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_RATE_METER_RATE_METER_PERIOD_LSP,
                                                                DVA_PORT0_RATE_METER_RATE_METER_PERIOD_LEN, ((uint32)ratePeriod)));
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_RATE_METER + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_RATE_METER_RATE_METER_ENABLE_LSP,
                                                                DVA_PORT0_RATE_METER_RATE_METER_ENABLE_LEN, state));
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* MIB_PORT_RATE_CONFIG_SET_ASIC */
/**@}*/ /* MIB_PORT_RATE_CONFIG_SET */

/**
 * @addtogroup MIB_PORT_RATE_CONFIG_GET Get Port Rate Config
 * @{
 */

/**
 * @addtogroup MIB_PORT_RATE_CONFIG_GET_ASIC Low Level Driver
 * @brief Get Port Rate Config Low Level Driver
 * @{
 */

/**
 * @brief Get the specified port rate configuration.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pState Port Rate Meter State (DISABLED, ENABLED) \n
 *            Indicates the port rate meter state.
 * @param[out] pRatePeriod Rate Period (N/A) \n
 *            Indicates the rate period for calculating the port rate.
 * @retval RT_ERR_OK Get the specified port rate meter configuration successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval -RT_ERR_FAILED The port period is invalid.
 * @retval Others Refer to dva_drv_indirect_reg_field_get() for the error.
 * @note This API gets the specified port rate configuration.
 */
RtkApiRet dva_stat_port_rate_config_get(uint32 unit, RtkPort port, RtkEnable *pState, RtkMibRatePeriod_e *pRatePeriod)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;
    uint32    val     = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || (((uint32)DVA_RTK_USER_PORT_END < port) && ((uint32)DVA_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if((NULL == pState) || (NULL == pRatePeriod))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get port rate meter state and rate period. */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_RATE_METER + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_RATE_METER_RATE_METER_PERIOD_LSP,
                                                                DVA_PORT0_RATE_METER_RATE_METER_PERIOD_LEN, &val));
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_RATE_METER + (sysPort * MIB_PORT_OFFSET), DVA_PORT0_RATE_METER_RATE_METER_ENABLE_LSP,
                                                                DVA_PORT0_RATE_METER_RATE_METER_ENABLE_LEN, pState));
        /* Transform the value getting from register to rate period. */
        if((uint32)EM_RATE_PERIOD_HALF_SECOND == val)
        {
            *pRatePeriod = EM_RATE_PERIOD_HALF_SECOND;
        }
        else if((uint32)EM_RATE_PERIOD_1_SECOND == val)
        {
            *pRatePeriod = EM_RATE_PERIOD_1_SECOND;
        }
        else if((uint32)EM_RATE_PERIOD_2_SECOND == val)
        {
            *pRatePeriod = EM_RATE_PERIOD_2_SECOND;
        }
        else
        {
            *pRatePeriod = EM_RATE_PERIOD_END;
            retVal       = (-RT_ERR_FAILED);
        }
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_RATE_CONFIG_GET_ASIC */
/**@}*/ /* MIB_PORT_RATE_CONFIG_GET */

/**
 * @addtogroup MIB_PORT_RATE_Get Get Port Rate
 * @{
 */

/**
 * @addtogroup MIB_PORT_RATE_GET_ASIC Low Level Driver
 * @brief Get Port Rate Low Level Driver
 * @{
 */

/**
 * @brief Get the port current RX and TX rate.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port Identification (DVA_RTK_USER_PORT_1ST to DVA_RTK_USER_PORT_END, DVA_RTK_CPU_PORT) \n
 *            Indicates the port ID for getting the port rate.
 * @param[out] pRxRate Port RX Rate (0 to 4294967295) \n
 *            Indicates the RX port rate in bytes.
 * @param[out] pTxRate Port TX Rate (0 to 4294967295) \n
 *            Indicates the TX port rate in bytes.
 * @retval RT_ERR_OK Get the port rate successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval -RT_ERR_FAILED The port period is invalid.
 * @retval Others Refer to dva_drv_indirect_reg_field_get() for the error.
 * @note This API gets the specified port rate.
 */
RtkApiRet dva_stat_port_rate_get(uint32 unit, RtkPort port, uint32 *pRxRate, uint32 *pTxRate)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DVA_RTK_USER_PORT_1ST > port) || (((uint32)DVA_RTK_USER_PORT_END < port) && ((uint32)DVA_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the parameter is valid. */
        if((NULL == pRxRate) || (NULL == pTxRate))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dva_g_userPort2sysPort[port];
        /* Get port RX and TX rate. */
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_RX_RATE + (sysPort * MIB_PORT_OFFSET), 0u, 32u, pRxRate));
        CHK_FUN_CONTINUE(retVal, dva_drv_indirect_reg_field_get(unit, DVA_PORT0_TX_RATE + (sysPort * MIB_PORT_OFFSET), 0u, 32u, pTxRate));
    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_RATE_GET_ASIC */

/**@}*/ /* MIB_PORT_RATE_Get */

/**@}*/
