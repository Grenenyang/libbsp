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

#ifndef DV1_RTKAS_REG_DEFINE_H_
#define DV1_RTKAS_REG_DEFINE_H_

/* Modules */
#define DV1_MOD_CPU_SERIES
#define DV1_MOD_ACL
#define DV1_MOD_BUFFER_MANAGEMENT
#define DV1_MOD_QOS
#define DV1_MOD_NETWORK_MONITOR
#define DV1_MOD_GENERAL
#define DV1_MOD_VLAN
#define DV1_MOD_MAC_CONTROL
#define DV1_MOD_EAV
#define DV1_MOD_RRP
#define DV1_MOD_COUNTER
#define DV1_MOD_INTERRUPT
#define DV1_MOD_LED
#define DV1_MOD_DIAG
#define DV1_MOD_INTERRUPT_TOP
#define DV1_MOD_L2
#define DV1_MOD_CPU

#ifdef DV1_MOD_CPU_SERIES
/* Files */
#define DV1_FILE_CPU_EFUSE
#define DV1_FILE_CPU_SMI
#define DV1_FILE_CPU_ENGINE
#define DV1_FILE_CPU_VARIABLE
#define DV1_FILE_NIC
#define DV1_FILE_KEY
#define DV1_FILE_REG_LOCK
#define DV1_FILE_CPU_SPECIAL
#endif

#ifdef DV1_MOD_ACL
/* Files */
#define DV1_FILE_RANGE_CHECK
#define DV1_FILE_PIE_TEMPLATEGENARATOR
#define DV1_FILE_ACL
#endif

#ifdef DV1_MOD_BUFFER_MANAGEMENT
/* Files */
#define DV1_FILE_FLOW_CONTROL
#endif

#ifdef DV1_MOD_QOS
/* Files */
#define DV1_FILE_REMARKING
#define DV1_FILE_IGR_EGR_BANDWIDTH_CONTROL
#define DV1_FILE_TSN
#define DV1_FILE_QUEUE_MANAGEMENT
#endif

#ifdef DV1_MOD_NETWORK_MONITOR
/* Files */
#define DV1_FILE_MIRRORING
#endif

#ifdef DV1_MOD_GENERAL
/* Files */
#define DV1_FILE_ALE_TABLE_ACCESS
#define DV1_FILE_MAC_TABLE_ACCESS
#define DV1_FILE_TABLE_DETAIL_EDIT
#define DV1_FILE_INTERFACE
#define DV1_FILE_RESET
#define DV1_FILE_BIST
#define DV1_FILE_AFE_MISC
#define DV1_FILE_HW_MISC
#define DV1_FILE_I2C
#define DV1_FILE_CHIP_VERSION
#endif

#ifdef DV1_MOD_VLAN
/* Files */
#define DV1_FILE_PORT_BASED_VLAN
#define DV1_FILE_VLAN_DECISION
#define DV1_FILE_PRIORITY_EXTRACTION
#define DV1_FILE_VLAN_TABLE
#endif

#ifdef DV1_MOD_MAC_CONTROL
/* Files */
#define DV1_FILE_PACKET_PARSER
#define DV1_FILE_ALE_IRPT
#define DV1_FILE_RMA
#define DV1_FILE_TX_MODIFICATION
#define DV1_FILE_SERDES_WRAP_SDS
#define DV1_FILE_ALE_MISC
#define DV1_FILE_SERDES_WTG_TOP_T1
#endif

#ifdef DV1_MOD_EAV
/* Files */
#define DV1_FILE_EAV
#endif

#ifdef DV1_MOD_RRP
/* Files */
#define DV1_FILE_RLDP
#endif

#ifdef DV1_MOD_COUNTER
/* Files */
#define DV1_FILE_MIB_COUNTER
#endif

#ifdef DV1_MOD_INTERRUPT
/* Files */
#define DV1_FILE_GPIO
#define DV1_FILE_INTERRUPT
#define DV1_FILE_WOL
#endif

#ifdef DV1_MOD_LED
/* Files */
#define DV1_FILE_LED
#define DV1_FILE_UART
#endif

#ifdef DV1_MOD_DIAG
/* Files */
#define DV1_FILE_ALE_RELATED_DIAG
#define DV1_FILE_MACSEC
#define DV1_FILE_FLOW_CONTROL_DIAG
#define DV1_FILE_SAFETY
#define DV1_FILE_MIITEST_DIAG
#endif

#ifdef DV1_MOD_INTERRUPT_TOP
/* Files */
#define DV1_FILE_INTERRUPT_TOP
#endif

#ifdef DV1_MOD_L2
/* Files */
#define DV1_FILE_L2_MISC
#define DV1_FILE_ADDR_TABLE_LOOKUP
#define DV1_FILE_SPANNING_TREE
#define DV1_FILE_SOURCE_MAC_LEARNING
#define DV1_FILE_802_1X
#define DV1_FILE_ATTACK_PREVENTION
#define DV1_FILE_L2_ENTRY_NUM_CONSTRAIN
#define DV1_FILE_STORM_CONTROL
#define DV1_FILE_PORT_ISOLATION
#define DV1_FILE_ROUTING
#define DV1_FILE_EEE
#define DV1_FILE_PORT_RELAY_DIS
#define DV1_FILE_LINK_AGGREGATE
#define DV1_FILE_STATEFUL
#endif

#ifdef DV1_MOD_CPU
/* Files */
#define DV1_FILE_CPU_DOMAIN
#define DV1_FILE_CLKRST
#define DV1_FILE_CPU_INFO
#define DV1_FILE_CPU_AUX_SYS
#define DV1_FILE_CPU_AUX_VSFC
#endif

#endif /* DV1_RTKAS_REG_DEFINE_H_ */
