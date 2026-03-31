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

#ifndef CVB_REG_DEFINE_H__
#define CVB_REG_DEFINE_H__

/* Modules */
#define CVB_MOD_CPU_SERIES
#define CVB_MOD_ACL
#define CVB_MOD_BUFFER_MANAGEMENT
#define CVB_MOD_QOS
#define CVB_MOD_NETWORK_MONITOR
#define CVB_MOD_GENERAL
#define CVB_MOD_VLAN
#define CVB_MOD_MAC_CONTROL
#define CVB_MOD_EAV
#define CVB_MOD_RRP
#define CVB_MOD_COUNTER
#define CVB_MOD_INTERRUPT
#define CVB_MOD_LED
#define CVB_MOD_DIAG
#define CVB_MOD_L2
#define CVB_MOD_CPU

#ifdef CVB_MOD_CPU_SERIES
/* Files */
#define CVB_FILE_CPU_EFUSE
#define CVB_FILE_CPU_SMI
#define CVB_FILE_CPU_ENGINE
#define CVB_FILE_CPU_VARIABLE
#define CVB_FILE_NIC
#define CVB_FILE_KEY
#define CVB_FILE_REG_LOCK
#define CVB_FILE_CPU_SPECIAL
#endif

#ifdef CVB_MOD_ACL
/* Files */
#define CVB_FILE_RANGE_CHECK
#define CVB_FILE_PIE_TEMPLATEGENARATOR
#define CVB_FILE_ACL
#endif

#ifdef CVB_MOD_BUFFER_MANAGEMENT
/* Files */
#define CVB_FILE_FLOW_CONTROL
#endif

#ifdef CVB_MOD_QOS
/* Files */
#define CVB_FILE_REMARKING
#define CVB_FILE_IGR_EGR_BANDWIDTH_CONTROL
#define CVB_FILE_TSN
#define CVB_FILE_QUEUE_MANAGEMENT
#endif

#ifdef CVB_MOD_NETWORK_MONITOR
/* Files */
#define CVB_FILE_MIRRORING
#endif

#ifdef CVB_MOD_GENERAL
/* Files */
#define CVB_FILE_TABLE_ACCESS
#define CVB_FILE_INTERFACE
#define CVB_FILE_RESET
#define CVB_FILE_BIST
#define CVB_FILE_SAFETY
#define CVB_FILE_AFE_MISC
#define CVB_FILE_I2C
#define CVB_FILE_HW_MISC
#define CVB_FILE_CHIP_VERSION
#endif

#ifdef CVB_MOD_VLAN
/* Files */
#define CVB_FILE_PORT_BASED_VLAN
#define CVB_FILE_VLAN_DECISION
#define CVB_FILE_PRIORITY_EXTRACTION
#define CVB_FILE_VLAN_TABLE
#endif

#ifdef CVB_MOD_MAC_CONTROL
/* Files */
#define CVB_FILE_PACKET_PARSER
#define CVB_FILE_ALE_MISC
#define CVB_FILE_ALE_IRPT
#define CVB_FILE_RMA
#define CVB_FILE_TX_MODIFICATION
#endif

#ifdef CVB_MOD_EAV
/* Files */
#define CVB_FILE_EAV
#endif

#ifdef CVB_MOD_RRP
/* Files */
#define CVB_FILE_RLDP
#endif

#ifdef CVB_MOD_COUNTER
/* Files */
#define CVB_FILE_MIB_COUNTER
#endif

#ifdef CVB_MOD_INTERRUPT
/* Files */
#define CVB_FILE_GPIO
#define CVB_FILE_INTERRUPT
#define CVB_FILE_WOL
#endif

#ifdef CVB_MOD_LED
/* Files */
#define CVB_FILE_LED
#define CVB_FILE_UART
#endif

#ifdef CVB_MOD_DIAG
/* Files */
#define CVB_FILE_ALE_RELATED_DIAG
#define CVB_FILE_MACSEC
#define CVB_FILE_FLOW_CONTROL_DIAG
#define CVB_FILE_MIITEST_DIAG
#endif

#ifdef CVB_MOD_L2
/* Files */
#define CVB_FILE_L2_MISC
#define CVB_FILE_ADDR_TABLE_LOOKUP
#define CVB_FILE_SPANNING_TREE
#define CVB_FILE_SOURCE_MAC_LEARNING
#define CVB_FILE_802_1X
#define CVB_FILE_L2_ENTRY_NUM_CONSTRAIN
#define CVB_FILE_STORM_CONTROL
#define CVB_FILE_ATTACK_PREVENTION
#define CVB_FILE_PORT_ISOLATION
#define CVB_FILE_ROUTING
#define CVB_FILE_PORT_RELAY_DIS
#define CVB_FILE_EEE
#define CVB_FILE_LINK_AGGREGATE
#define CVB_FILE_STATEFUL
#endif

#ifdef CVB_MOD_CPU
/* Files */
#define CVB_FILE_CPU_DOMAIN
#define CVB_FILE_CLKRST
#define CVB_FILE_CPU_INFO
#endif

#endif /* CVB_REG_DEFINE_H__ */