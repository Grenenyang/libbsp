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

#ifndef CVA_REG_DEFINE_H__
#define CVA_REG_DEFINE_H__

/* Modules */
#define CVA_MOD_CPU_SERIES
#define CVA_MOD_ACL
#define CVA_MOD_BUFFER_MANAGEMENT
#define CVA_MOD_QOS
#define CVA_MOD_NETWORK_MONITOR
#define CVA_MOD_GENERAL
#define CVA_MOD_VLAN
#define CVA_MOD_MAC_CONTROL
#define CVA_MOD_EAV
#define CVA_MOD_RRP
#define CVA_MOD_COUNTER
#define CVA_MOD_INTERRUPT
#define CVA_MOD_LED
#define CVA_MOD_DIAG
#define CVA_MOD_L2
#define CVA_MOD_CPU

#ifdef CVA_MOD_CPU_SERIES
/* Files */
#define CVA_FILE_CPU_EFUSE
#define CVA_FILE_CPU_SMI
#define CVA_FILE_CPU_ENGINE
#define CVA_FILE_CPU_VARIABLE
#define CVA_FILE_NIC
#define CVA_FILE_KEY
#define CVA_FILE_REG_LOCK
#define CVA_FILE_CPU_SPECIAL
#endif

#ifdef CVA_MOD_ACL
/* Files */
#define CVA_FILE_RANGE_CHECK
#define CVA_FILE_PIE_TEMPLATEGENARATOR
#define CVA_FILE_ACL
#endif

#ifdef CVA_MOD_BUFFER_MANAGEMENT
/* Files */
#define CVA_FILE_FLOW_CONTROL
#endif

#ifdef CVA_MOD_QOS
/* Files */
#define CVA_FILE_REMARKING
#define CVA_FILE_IGR_EGR_BANDWIDTH_CONTROL
#define CVA_FILE_TSN
#define CVA_FILE_QUEUE_MANAGEMENT
#endif

#ifdef CVA_MOD_NETWORK_MONITOR
/* Files */
#define CVA_FILE_MIRRORING
#endif

#ifdef CVA_MOD_GENERAL
/* Files */
#define CVA_FILE_TABLE_ACCESS
#define CVA_FILE_INTERFACE
#define CVA_FILE_RESET
#define CVA_FILE_BIST
#define CVA_FILE_SAFETY
#define CVA_FILE_AFE_MISC
#define CVA_FILE_I2C
#define CVA_FILE_HW_MISC
#define CVA_FILE_CHIP_VERSION
#endif

#ifdef CVA_MOD_VLAN
/* Files */
#define CVA_FILE_PORT_BASED_VLAN
#define CVA_FILE_VLAN_DECISION
#define CVA_FILE_PRIORITY_EXTRACTION
#define CVA_FILE_VLAN_TABLE
#endif

#ifdef CVA_MOD_MAC_CONTROL
/* Files */
#define CVA_FILE_PACKET_PARSER
#define CVA_FILE_ALE_MISC
#define CVA_FILE_ALE_IRPT
#define CVA_FILE_RMA
#define CVA_FILE_TX_MODIFICATION
#endif

#ifdef CVA_MOD_EAV
/* Files */
#define CVA_FILE_EAV
#endif

#ifdef CVA_MOD_RRP
/* Files */
#define CVA_FILE_RLDP
#endif

#ifdef CVA_MOD_COUNTER
/* Files */
#define CVA_FILE_MIB_COUNTER
#endif

#ifdef CVA_MOD_INTERRUPT
/* Files */
#define CVA_FILE_GPIO
#define CVA_FILE_INTERRUPT
#define CVA_FILE_WOL
#endif

#ifdef CVA_MOD_LED
/* Files */
#define CVA_FILE_LED
#define CVA_FILE_UART
#endif

#ifdef CVA_MOD_DIAG
/* Files */
#define CVA_FILE_ALE_RELATED_DIAG
#define CVA_FILE_MACSEC
#define CVA_FILE_FLOW_CONTROL_DIAG
#define CVA_FILE_MIITEST_DIAG
#endif

#ifdef CVA_MOD_L2
/* Files */
#define CVA_FILE_L2_MISC
#define CVA_FILE_ADDR_TABLE_LOOKUP
#define CVA_FILE_SPANNING_TREE
#define CVA_FILE_SOURCE_MAC_LEARNING
#define CVA_FILE_802_1X
#define CVA_FILE_L2_ENTRY_NUM_CONSTRAIN
#define CVA_FILE_STORM_CONTROL
#define CVA_FILE_ATTACK_PREVENTION
#define CVA_FILE_PORT_ISOLATION
#define CVA_FILE_ROUTING
#define CVA_FILE_PORT_RELAY_DIS
#define CVA_FILE_EEE
#define CVA_FILE_LINK_AGGREGATE
#define CVA_FILE_STATEFUL
#endif

#ifdef CVA_MOD_CPU
/* Files */
#define CVA_FILE_CPU_DOMAIN
#define CVA_FILE_CLKRST
#define CVA_FILE_CPU_INFO
#endif

#endif /* CVA_REG_DEFINE_H__ */