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

#ifndef DVA_RTKAS_REG_DEFINE_H_
#define DVA_RTKAS_REG_DEFINE_H_

/* Modules */
#define DVA_MOD_CPU_SERIES
#define DVA_MOD_ACL
#define DVA_MOD_BUFFER_MANAGEMENT
#define DVA_MOD_QOS
#define DVA_MOD_NETWORK_MONITOR
#define DVA_MOD_GENERAL
#define DVA_MOD_VLAN
#define DVA_MOD_MAC_CONTROL
#define DVA_MOD_EAV
#define DVA_MOD_RRP
#define DVA_MOD_COUNTER
#define DVA_MOD_INTERRUPT
#define DVA_MOD_LED
#define DVA_MOD_DIAG
#define DVA_MOD_INTERRUPT_TOP
#define DVA_MOD_L2
#define DVA_MOD_CPU

#ifdef DVA_MOD_CPU_SERIES
/* Files */
#define DVA_FILE_CPU_SMI
#define DVA_FILE_CPU_EFUSE
#define DVA_FILE_CPU_ENGINE
#define DVA_FILE_CPU_VARIABLE
#define DVA_FILE_NIC
#define DVA_FILE_KEY
#define DVA_FILE_REG_LOCK
#define DVA_FILE_CPU_SPECIAL
#endif

#ifdef DVA_MOD_ACL
/* Files */
#define DVA_FILE_RANGE_CHECK
#define DVA_FILE_PIE_TEMPLATEGENARATOR
#define DVA_FILE_ACL
#endif

#ifdef DVA_MOD_BUFFER_MANAGEMENT
/* Files */
#define DVA_FILE_FLOW_CONTROL
#endif

#ifdef DVA_MOD_QOS
/* Files */
#define DVA_FILE_REMARKING
#define DVA_FILE_IGR_EGR_BANDWIDTH_CONTROL
#define DVA_FILE_TSN
#define DVA_FILE_QUEUE_MANAGEMENT
#endif

#ifdef DVA_MOD_NETWORK_MONITOR
/* Files */
#define DVA_FILE_MIRRORING
#endif

#ifdef DVA_MOD_GENERAL
/* Files */
#define DVA_FILE_ALE_TABLE_ACCESS
#define DVA_FILE_MAC_TABLE_ACCESS
#define DVA_FILE_TABLE_DETAIL_EDIT
#define DVA_FILE_INTERFACE
#define DVA_FILE_RESET
#define DVA_FILE_BIST
#define DVA_FILE_AFE_MISC
#define DVA_FILE_I2C
#define DVA_FILE_HW_MISC
#define DVA_FILE_CHIP_VERSION
#define DVA_FILE_MAILBOX
#endif

#ifdef DVA_MOD_VLAN
/* Files */
#define DVA_FILE_PORT_BASED_VLAN
#define DVA_FILE_VLAN_DECISION
#define DVA_FILE_PRIORITY_EXTRACTION
#define DVA_FILE_VLAN_TABLE
#endif

#ifdef DVA_MOD_MAC_CONTROL
/* Files */
#define DVA_FILE_PACKET_PARSER
#define DVA_FILE_ALE_MISC
#define DVA_FILE_ALE_IRPT
#define DVA_FILE_RMA
#define DVA_FILE_RX_FILTER
#define DVA_FILE_TX_MODIFICATION
#define DVA_FILE_SERDES_WRAP_SDS
#define DVA_FILE_SERDES_WTG_TOP_T1
#endif

#ifdef DVA_MOD_EAV
/* Files */
#define DVA_FILE_EAV
#endif

#ifdef DVA_MOD_RRP
/* Files */
#define DVA_FILE_RLDP
#endif

#ifdef DVA_MOD_COUNTER
/* Files */
#define DVA_FILE_MIB_COUNTER
#endif

#ifdef DVA_MOD_INTERRUPT
/* Files */
#define DVA_FILE_GPIO
#define DVA_FILE_INTERRUPT
#define DVA_FILE_WOL
#endif

#ifdef DVA_MOD_LED
/* Files */
#define DVA_FILE_LED
#define DVA_FILE_UART
#endif

#ifdef DVA_MOD_DIAG
/* Files */
#define DVA_FILE_ALE_RELATED_DIAG
#define DVA_FILE_MACSEC
#define DVA_FILE_FLOW_CONTROL_DIAG
#define DVA_FILE_SAFETY
#define DVA_FILE_MIITEST_DIAG
#endif

#ifdef DVA_MOD_INTERRUPT_TOP
/* Files */
#define DVA_FILE_INTERRUPT_TOP
#endif

#ifdef DVA_MOD_L2
/* Files */
#define DVA_FILE_L2_MISC
#define DVA_FILE_ADDR_TABLE_LOOKUP
#define DVA_FILE_SPANNING_TREE
#define DVA_FILE_SOURCE_MAC_LEARNING
#define DVA_FILE_802_1X
#define DVA_FILE_L2_ENTRY_NUM_CONSTRAIN
#define DVA_FILE_ATTACK_PREVENTION
#define DVA_FILE_STORM_CONTROL
#define DVA_FILE_PORT_ISOLATION
#define DVA_FILE_ROUTING
#define DVA_FILE_PORT_RELAY_DIS
#define DVA_FILE_EEE
#define DVA_FILE_LINK_AGGREGATE
#define DVA_FILE_STATEFUL
#endif

#ifdef DVA_MOD_CPU
/* Files */
#define DVA_FILE_CPU_DOMAIN
#define DVA_FILE_CLKRST
#define DVA_FILE_CPU_INFO
#define DVA_FILE_CPU_AUX_SYS
#define DVA_FILE_CPU_AUX_VSFC
#endif

#endif /* DVA_RTKAS_REG_DEFINE_H_ */
