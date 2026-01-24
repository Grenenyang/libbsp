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
 * provided "as is" and any express or implied warranties, including, but not
 * limited to, the implied warranties of merchantability and fitness for a
 * particular purpose are disclaimed. In no event shall Realtek
 * Semiconductor Corp. be liable for any direct, indirect, incidental, special,
 * exemplary, or consequential damages (including, but not limited to,
 * procurement of substitute goods or services; loss of use, data, or profits;
 * or business interruption) however caused and on any theory of liability,
 * whether in contract, strict liability, or tort (including negligence or
 * otherwise) arising in any way out of the use of this software, even if
 * advised of the possibility of such damage.
 */

#include "rtl907xd_v1_asicdrv_mib.h"
#include "rtl907xd_v1_api_ext.h"
#include "rtkas_api_ext.h"

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
 * @retval Others Refer to dv1_drv_mib_counter_reset() for the error.
 * @note This API resets all ports and global MIB counters.
 */
RtkApiRet dv1_stat_reset_all(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Reset all port and global counters. */
        CHK_FUN_CONTINUE(retVal, dv1_drv_mib_counter_reset(unit, 1u, (DV1_RTK_SYS_PMAP_ALL | DV1_RTK_CPU_PMAP)));

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
 * @retval Others Refer to dv1_reg_read(), dv1_reg_write() or dv1_reg_field_write() for the error.
 * @note This API starts all MIB counters.
 */
RtkApiRet dv1_stat_start_all(uint32 unit)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    RtkPort   sysPort = 0u;

    do
    {
        /* Set MIB counter control registers. */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_MIB_COUNTER_CONTROL0, DV1_MIB_COUNTER_CONTROL0_ENMIBCOUNTER_LSP,
                                                     DV1_MIB_COUNTER_CONTROL0_ENMIBCOUNTER_LEN, 1u));
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_MIB_COUNTER_CONTROL0, DV1_MIB_COUNTER_CONTROL0_ENDBMIBCOUNTER_LSP,
                                                     DV1_MIB_COUNTER_CONTROL0_ENDBMIBCOUNTER_LEN, 1u));
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(unit, DV1_MIB_COUNTER_CONTROL2, &regVal));
        regVal |= 0xFFFFFFFFu;
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(unit, DV1_MIB_COUNTER_CONTROL2, regVal));

        /* Set MIB counter control registers to ports. */
        for(sysPort = DV1_RTK_SYS_PORT_1ST; sysPort <= DV1_RTK_SYS_PORT_END; sysPort++)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT0_MIB_COUNTER_CONTROL + (sysPort * MIB_PORT_OFFSET), DV1_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                         DV1_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 1u));
        }
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT15_MIB_COUNTER_CONTROL, DV1_PORT15_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                     DV1_PORT15_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 1u));
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
 * @retval Others Refer to dv1_reg_read(), dv1_reg_write() or dv1_reg_field_write() for the error.
 * @note This API stops all MIB counters.
 */
RtkApiRet dv1_stat_stop_all(uint32 unit)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    RtkPort   sysPort = 0u;

    do
    {
        /* Set MIB counter control registers. */
        CHK_FUN_CONTINUE(retVal, dv1_reg_read(unit, DV1_MIB_COUNTER_CONTROL0, &regVal));
        regVal &= 0xFFFFFFFCu;
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(unit, DV1_MIB_COUNTER_CONTROL0, regVal));

        regVal = 0x0u;
        CHK_FUN_CONTINUE(retVal, dv1_reg_write(unit, DV1_MIB_COUNTER_CONTROL2, regVal));

        /* Set MIB counter control registers for ports. */
        for(sysPort = DV1_RTK_SYS_PORT_1ST; sysPort <= DV1_RTK_SYS_PORT_END; sysPort++)
        {
            CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT0_MIB_COUNTER_CONTROL + (sysPort * MIB_PORT_OFFSET), DV1_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                         DV1_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 0u));
        }
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT15_MIB_COUNTER_CONTROL, DV1_PORT15_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                     DV1_PORT15_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 0u));
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
 * @retval Others Refer to dv1_reg_read() for the error.
 * @note Get the reason of dot1dTpPortInDiscards. The dot1dTpPortInDiscards divides into Address Lookup Engine \n
 *       (ALE) drop which means system drop and RX drop. This API gets ALE drop reason and can only get the reason\n
 *       of the last ingress packet.
 */
RtkApiRet dv1_stat_get_ale_drop_reason(uint32 unit, uint32 *dropReason)
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
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_read(unit, DV1_PACKET_PROCESS_INFORMATION5_CR2, DV1_PACKET_PROCESS_INFORMATION5_CR2_REASON_LSP,
                                                    DV1_PACKET_PROCESS_INFORMATION5_CR2_REASON_LEN, &val));

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
 * @param[in] port Port Identification (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END, DV1_RTK_CPU_PORT) \n
 *            Indicates the port ID for getting max queue level.
 * @param[out] maxQueueLevel Max Queue Level (0 to 2047) \n
 *             Indicates the max queue level of the port.
 * @retval RT_ERR_OK Get the specified port max queue level successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dv1_reg_read() for the error.
 * @note This API gets the value of per port max queue level(unit = page; 1 page = 128 Bytes).
 */
RtkApiRet dv1_stat_port_max_queue_level_get(uint32 unit, RtkPort port, uint32 *maxQueueLevel)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    pagePeakValue = 0u;
    RtkPort   sysPort       = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DV1_RTK_USER_PORT_1ST > port) || (((uint32)DV1_RTK_USER_PORT_END < port) && ((uint32)DV1_RTK_CPU_PORT != port)))
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
        sysPort = (uint32)dv1_g_userPort2sysPort[port];
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_read(unit, DV1_PORT0_OCCUPIED_RX_PAGE_PEAK_COUNTER + (sysPort * MIB_PORT_OFFSET), DV1_PORT0_OCCUPIED_RX_PAGE_PEAK_COUNTER_RXPAGEPEAKCNT_LSP,
                                                    DV1_PORT0_OCCUPIED_RX_PAGE_PEAK_COUNTER_RXPAGEPEAKCNT_LEN, &pagePeakValue));

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
 * @param[in] port Port Identification (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END, DV1_RTK_CPU_PORT) \n
 *            Indicates the port ID for resetting the port counter.
 * @param[out] None
 * @retval RT_ERR_OK Reset the specified port MIB counter successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval Others Refer to dv1_drv_mib_counter_reset() for the error.
 * @note This API resets the specified port MIB counter.
 */
RtkApiRet dv1_stat_port_reset(uint32 unit, RtkPort port)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DV1_RTK_USER_PORT_1ST > port) || (((uint32)DV1_RTK_USER_PORT_END < port) && ((uint32)DV1_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dv1_g_userPort2sysPort[port];
        /* Reset port counters. */
        CHK_FUN_CONTINUE(retVal, dv1_drv_mib_counter_reset(unit, 0u, ((dv1_RtkPmap)1u << sysPort)));
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
 * @param[in] port Port Identification (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END, DV1_RTK_CPU_PORT) \n
 *            Indicates the port ID for starting the port counter.
 * @param[out] None
 * @retval RT_ERR_OK Start the specified port MIB counter successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval Others Refer to dv1_drv_mib_counter_reset() or dv1_reg_field_write() for the error.
 * @note This API starts the specified port MIB counter.
 */
RtkApiRet dv1_stat_port_start(uint32 unit, RtkPort port)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DV1_RTK_USER_PORT_1ST > port) || (((uint32)DV1_RTK_USER_PORT_END < port) && ((uint32)DV1_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dv1_g_userPort2sysPort[port];
        /* Enable port counters. */
        CHK_FUN_CONTINUE(retVal, dv1_drv_mib_counter_enable(unit, TRUE));
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT0_MIB_COUNTER_CONTROL + (sysPort * MIB_PORT_OFFSET), DV1_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                     DV1_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 1u));

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
 * @param[in] port Port Identification (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END, DV1_RTK_CPU_PORT) \n
 *            Indicates the port ID for stopping the port counter.
 * @param[out] None
 * @retval RT_ERR_OK Stop the specified port MIB counter successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval Others Refer to dv1_reg_field_write() for the error.
 * @note This API stops the specified port MIB counter.
 */
RtkApiRet dv1_stat_port_stop(uint32 unit, RtkPort port)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DV1_RTK_USER_PORT_1ST > port) || (((uint32)DV1_RTK_USER_PORT_END < port) && ((uint32)DV1_RTK_CPU_PORT != port)))
        {
            retVal = -RT_ERR_PORT_ID;
            continue;
        }

        /* Transform the mapping between user port and system port. */
        sysPort = (uint32)dv1_g_userPort2sysPort[port];
        /* Disable port counters. */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_write(unit, DV1_PORT0_MIB_COUNTER_CONTROL + (sysPort * MIB_PORT_OFFSET), DV1_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LSP,
                                                     DV1_PORT0_MIB_COUNTER_CONTROL_COUNTERSTART_LEN, 0u));
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
 * @param[in] port Port Identification (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END, DV1_RTK_CPU_PORT) \n
 *            Indicates the port ID for getting the port counter.
 * @param[in] counterType Counter Type (N/A) \n
 *            Indicates the counter type for getting the counter value.
 * @param[out] counterValue Counter Value (0 to 4294967295) \n
 *             Indicates the counter value.
 * @retval RT_ERR_OK Get the specified port MIB counter successfully.
 * @retval -RT_ERR_PORT_ID The port is invalid.
 * @retval -RT_ERR_STAT_INVALID_PORT_CNTR The counter type is invalid.
 * @retval -RT_ERR_NULL_POINTER Null pointer for output parameter.
 * @retval Others Refer to dv1_drv_mib_counter_get() for the error.
 * @note This API gets the specified port MIB counter.
 */
RtkApiRet dv1_stat_port_get(uint32 unit, RtkPort port, RtkMibCounterType_e counterType, uint32 *counterValue)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DV1_RTK_USER_PORT_1ST > port) || (((uint32)DV1_RTK_USER_PORT_END < port) && ((uint32)DV1_RTK_CPU_PORT != port)))
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
        sysPort = (uint32)dv1_g_userPort2sysPort[port];
        /* Get port counter value. */
        CHK_FUN_CONTINUE(retVal, dv1_drv_mib_counter_get(unit, counterType, sysPort, counterValue));
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
 * @param[in] port Port Identification (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END, DV1_RTK_CPU_PORT) \n
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
 * @retval Others Refer to dv1_reg_read() for the error.
 * @note Get the reason of dot1dTpPortInDiscards. The dot1dTpPortInDiscards divides into Address Lookup Engine \n
 *       (ALE) drop which means system drop and RX drop. This API gets RX drop reason and can only get the reason\n
 *       of the last ingress packet.
 */
RtkApiRet dv1_stat_port_get_rx_drop_reason(uint32 unit, RtkPort port, uint32 *dropReason)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    val     = 0u;
    RtkPort   sysPort = 0u;

    do
    {
        /* Check if the parameter is valid. */
        if(((uint32)DV1_RTK_USER_PORT_1ST > port) || (((uint32)DV1_RTK_USER_PORT_END < port) && ((uint32)DV1_RTK_CPU_PORT != port)))
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
        sysPort = (uint32)dv1_g_userPort2sysPort[port];
        /* Get port drop reason register. */
        CHK_FUN_CONTINUE(retVal, dv1_reg_field_read(unit, DV1_PORT0_DROP_REASON + (sysPort * MIB_PORT_OFFSET), DV1_PORT0_DROP_REASON_DROPREASON_LSP, DV1_PORT0_DROP_REASON_DROPREASON_LEN, &val));

        *dropReason = val;

    } while(0u == 1u);

    return retVal;
}

/**@}*/ /* MIB_PORT_REASON_RX_ASIC */

/**@}*/ /* MIB_PORT_REASON_RX */

/**@}*/
