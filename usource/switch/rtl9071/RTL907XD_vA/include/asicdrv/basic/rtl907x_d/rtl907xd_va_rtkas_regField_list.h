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

#ifndef DVA_RTKAS_REGFIELD_LIST_H_
#define DVA_RTKAS_REGFIELD_LIST_H_
#include "rtl907xd_va_rtkas_reg_define.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_cpu_smi.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_cpu_efuse.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_cpu_engine.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_cpu_variable.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_nic.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_key.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_reg_lock.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_cpu_special.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_range_check.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_pie_templategenarator.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_acl.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_flow_control.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_remarking.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_igr_egr_bandwidth_control.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_tsn.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_queue_management.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_mirroring.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_ale_table_access.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_mac_table_access.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_interface.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_reset.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_afe_misc.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_i2c.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_hw_misc.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_chip_version.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_mailbox.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_port_based_vlan.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_vlan_decision.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_priority_extraction.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_vlan_table.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_packet_parser.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_ale_misc.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_ale_irpt.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_rma.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_rx_filter.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_tx_modification.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_serdes_wrap_sds.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_serdes_wtg_top_t1.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_eav.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_rldp.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_mib_counter.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_gpio.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_interrupt.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_wol.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_led.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_uart.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_ale_related_diag.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_macsec.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_flow_control_diag.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_safety.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_miitest_diag.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_interrupt_top.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_l2_misc.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_addr_table_lookup.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_spanning_tree.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_source_mac_learning.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_802_1x.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_l2_entry_num_constrain.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_attack_prevention.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_storm_control.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_port_isolation.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_routing.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_port_relay_dis.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_eee.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_link_aggregate.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_stateful.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_cpu_domain.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_clkrst.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_cpu_info.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_cpu_aux_sys.h"
#include "rtkas_reg_field/rtl907xd_va_rtkas_reg_field_file_cpu_aux_vsfc.h"

#endif /* DVA_RTKAS_REGFIELD_LIST_H_ */
