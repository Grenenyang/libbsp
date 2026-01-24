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

#ifndef DVA_RTKAS_REG_LIST_FILE_ALE_MISC_H_
#define DVA_RTKAS_REG_LIST_FILE_ALE_MISC_H_

#ifdef DVA_FILE_ALE_MISC
/* File: ALE Misc */
#define DVA_IGMP_MLD_CONTROL                      0x4A510000u
#define DVA_IGMP_MLD_CONTROL_FIELDNUM             6u
#define DVA_IGMP_MLD_CONTROL_DEFAULTVAL           0x800000u
#define DVA_OPTION_HEADER_TRAP_CONTROL            0x4A510004u
#define DVA_OPTION_HEADER_TRAP_CONTROL_FIELDNUM   6u
#define DVA_OPTION_HEADER_TRAP_CONTROL_DEFAULTVAL 0x200000u
#define DVA_CFI_CONTROL                           0x4A510008u
#define DVA_CFI_CONTROL_FIELDNUM                  5u
#define DVA_CFI_CONTROL_DEFAULTVAL                0x40000u
#define DVA_ARP_CONTROL                           0x4A51000Cu
#define DVA_ARP_CONTROL_FIELDNUM                  5u
#define DVA_ARP_CONTROL_DEFAULTVAL                0x200000u
#define DVA_TCAM_CONTROL                          0x4A510010u
#define DVA_TCAM_CONTROL_FIELDNUM                 4u
#define DVA_TCAM_CONTROL_DEFAULTVAL               0x34u
#define DVA_ALE_CONTROL0                          0x4A510014u
#define DVA_ALE_CONTROL0_FIELDNUM                 7u
#define DVA_ALE_CONTROL0_DEFAULTVAL               0xC52316Au
#define DVA_ALE_CONTROL1                          0x4A510018u
#define DVA_ALE_CONTROL1_FIELDNUM                 7u
#define DVA_ALE_CONTROL1_DEFAULTVAL               0x1E102507u
#define DVA_ALE_CONTROL2                          0x4A51001Cu
#define DVA_ALE_CONTROL2_FIELDNUM                 7u
#define DVA_ALE_CONTROL2_DEFAULTVAL               0x3139B0u
#define DVA_SNMP_CONTROL                          0x4A510020u
#define DVA_SNMP_CONTROL_FIELDNUM                 6u
#define DVA_SNMP_CONTROL_DEFAULTVAL               0x0u
#define DVA_SNMP_UDP_PORT                         0x4A510024u
#define DVA_SNMP_UDP_PORT_FIELDNUM                2u
#define DVA_SNMP_UDP_PORT_DEFAULTVAL              0x0u
#define DVA_SNMP_IPV4                             0x4A510028u
#define DVA_SNMP_IPV4_FIELDNUM                    1u
#define DVA_SNMP_IPV4_DEFAULTVAL                  0x0u
#define DVA_SNMP_IPV60                            0x4A51002Cu
#define DVA_SNMP_IPV60_FIELDNUM                   1u
#define DVA_SNMP_IPV60_DEFAULTVAL                 0x0u
#define DVA_SNMP_IPV61                            0x4A510030u
#define DVA_SNMP_IPV61_FIELDNUM                   1u
#define DVA_SNMP_IPV61_DEFAULTVAL                 0x0u
#define DVA_SNMP_IPV62                            0x4A510034u
#define DVA_SNMP_IPV62_FIELDNUM                   1u
#define DVA_SNMP_IPV62_DEFAULTVAL                 0x0u
#define DVA_SNMP_IPV63                            0x4A510038u
#define DVA_SNMP_IPV63_FIELDNUM                   1u
#define DVA_SNMP_IPV63_DEFAULTVAL                 0x0u
#define DVA_CHECKSUM_ERROR_CONTROL                0x4A51003Cu
#define DVA_CHECKSUM_ERROR_CONTROL_FIELDNUM       3u
#define DVA_CHECKSUM_ERROR_CONTROL_DEFAULTVAL     0x2u
#define DVA_CRCERRCPUMSK                          0x4A510040u
#define DVA_CRCERRCPUMSK_FIELDNUM                 2u
#define DVA_CRCERRCPUMSK_DEFAULTVAL               0x8000u
#define DVA_ALE_SRAM_MODE0                        0x4A510044u
#define DVA_ALE_SRAM_MODE0_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE0_DEFAULTVAL             0x642u
#define DVA_ALE_SRAM_MODE1                        0x4A510048u
#define DVA_ALE_SRAM_MODE1_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE1_DEFAULTVAL             0x18Bu
#define DVA_ALE_SRAM_MODE2                        0x4A51004Cu
#define DVA_ALE_SRAM_MODE2_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE2_DEFAULTVAL             0x18Bu
#define DVA_ALE_SRAM_MODE3                        0x4A510050u
#define DVA_ALE_SRAM_MODE3_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE3_DEFAULTVAL             0x18Bu
#define DVA_ALE_SRAM_MODE4                        0x4A510054u
#define DVA_ALE_SRAM_MODE4_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE4_DEFAULTVAL             0x18Bu
#define DVA_ALE_SRAM_MODE5                        0x4A510058u
#define DVA_ALE_SRAM_MODE5_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE5_DEFAULTVAL             0x18Bu
#define DVA_ALE_TOP_GATED_CLOCK                   0x4A51005Cu
#define DVA_ALE_TOP_GATED_CLOCK_FIELDNUM          7u
#define DVA_ALE_TOP_GATED_CLOCK_DEFAULTVAL        0x7u
#define DVA_ASIC_TCAM_128X96_0_CTRL               0x4A510060u
#define DVA_ASIC_TCAM_128X96_0_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_0_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_128X96_1_CTRL               0x4A510064u
#define DVA_ASIC_TCAM_128X96_1_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_1_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_128X96_2_CTRL               0x4A510068u
#define DVA_ASIC_TCAM_128X96_2_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_2_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_128X96_3_CTRL               0x4A51006Cu
#define DVA_ASIC_TCAM_128X96_3_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_3_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_128X96_4_CTRL               0x4A510070u
#define DVA_ASIC_TCAM_128X96_4_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_4_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_128X96_5_CTRL               0x4A510074u
#define DVA_ASIC_TCAM_128X96_5_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_5_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_128X96_6_CTRL               0x4A510078u
#define DVA_ASIC_TCAM_128X96_6_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_6_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_128X96_7_CTRL               0x4A51007Cu
#define DVA_ASIC_TCAM_128X96_7_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_7_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_128X96_8_CTRL               0x4A510080u
#define DVA_ASIC_TCAM_128X96_8_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_8_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_128X96_9_CTRL               0x4A510084u
#define DVA_ASIC_TCAM_128X96_9_CTRL_FIELDNUM      13u
#define DVA_ASIC_TCAM_128X96_9_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_96X120_0_CTRL               0x4A510088u
#define DVA_ASIC_TCAM_96X120_0_CTRL_FIELDNUM      11u
#define DVA_ASIC_TCAM_96X120_0_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_96X120_1_CTRL               0x4A51008Cu
#define DVA_ASIC_TCAM_96X120_1_CTRL_FIELDNUM      11u
#define DVA_ASIC_TCAM_96X120_1_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_96X120_2_CTRL               0x4A510090u
#define DVA_ASIC_TCAM_96X120_2_CTRL_FIELDNUM      11u
#define DVA_ASIC_TCAM_96X120_2_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_96X120_3_CTRL               0x4A510094u
#define DVA_ASIC_TCAM_96X120_3_CTRL_FIELDNUM      11u
#define DVA_ASIC_TCAM_96X120_3_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_64X48_0_CTRL                0x4A510098u
#define DVA_ASIC_TCAM_64X48_0_CTRL_FIELDNUM       11u
#define DVA_ASIC_TCAM_64X48_0_CTRL_DEFAULTVAL     0x0u
#define DVA_ASIC_TCAM_64X48_1_CTRL                0x4A51009Cu
#define DVA_ASIC_TCAM_64X48_1_CTRL_FIELDNUM       11u
#define DVA_ASIC_TCAM_64X48_1_CTRL_DEFAULTVAL     0x0u
#define DVA_ASIC_TCAM_256X128_0_CTRL              0x4A5100A0u
#define DVA_ASIC_TCAM_256X128_0_CTRL_FIELDNUM     11u
#define DVA_ASIC_TCAM_256X128_0_CTRL_DEFAULTVAL   0x0u
#define DVA_ASIC_TCAM_256X128_1_CTRL              0x4A5100A4u
#define DVA_ASIC_TCAM_256X128_1_CTRL_FIELDNUM     11u
#define DVA_ASIC_TCAM_256X128_1_CTRL_DEFAULTVAL   0x0u
#define DVA_ASIC_TCAM_256X16_0_CTRL               0x4A5100A8u
#define DVA_ASIC_TCAM_256X16_0_CTRL_FIELDNUM      11u
#define DVA_ASIC_TCAM_256X16_0_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_256X16_1_CTRL               0x4A5100ACu
#define DVA_ASIC_TCAM_256X16_1_CTRL_FIELDNUM      11u
#define DVA_ASIC_TCAM_256X16_1_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_256X16_2_CTRL               0x4A5100B0u
#define DVA_ASIC_TCAM_256X16_2_CTRL_FIELDNUM      11u
#define DVA_ASIC_TCAM_256X16_2_CTRL_DEFAULTVAL    0x0u
#define DVA_ASIC_TCAM_256X8_0_CTRL                0x4A5100B4u
#define DVA_ASIC_TCAM_256X8_0_CTRL_FIELDNUM       11u
#define DVA_ASIC_TCAM_256X8_0_CTRL_DEFAULTVAL     0x0u
#define DVA_ALE_SRAM_MODE6                        0x4A5100B8u
#define DVA_ALE_SRAM_MODE6_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE6_DEFAULTVAL             0x18Bu
#define DVA_ALE_SRAM_MODE7                        0x4A5100BCu
#define DVA_ALE_SRAM_MODE7_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE7_DEFAULTVAL             0x18Bu
#define DVA_ALE_SRAM_MODE8                        0x4A5100C0u
#define DVA_ALE_SRAM_MODE8_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE8_DEFAULTVAL             0x18Bu
#define DVA_ALE_SRAM_MODE9                        0x4A5100C4u
#define DVA_ALE_SRAM_MODE9_FIELDNUM               2u
#define DVA_ALE_SRAM_MODE9_DEFAULTVAL             0x18Bu
#define DVA_ALE_SRAM_MODE10                       0x4A5100C8u
#define DVA_ALE_SRAM_MODE10_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE10_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE11                       0x4A5100CCu
#define DVA_ALE_SRAM_MODE11_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE11_DEFAULTVAL            0x642u
#define DVA_ALE_SRAM_MODE12                       0x4A5100D0u
#define DVA_ALE_SRAM_MODE12_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE12_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE13                       0x4A5100D4u
#define DVA_ALE_SRAM_MODE13_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE13_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE14                       0x4A5100D8u
#define DVA_ALE_SRAM_MODE14_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE14_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE15                       0x4A5100DCu
#define DVA_ALE_SRAM_MODE15_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE15_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE16                       0x4A5100E0u
#define DVA_ALE_SRAM_MODE16_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE16_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE17                       0x4A5100E4u
#define DVA_ALE_SRAM_MODE17_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE17_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE18                       0x4A5100E8u
#define DVA_ALE_SRAM_MODE18_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE18_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE19                       0x4A5100ECu
#define DVA_ALE_SRAM_MODE19_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE19_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE20                       0x4A5100F0u
#define DVA_ALE_SRAM_MODE20_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE20_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE21                       0x4A5100F4u
#define DVA_ALE_SRAM_MODE21_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE21_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE22                       0x4A5100F8u
#define DVA_ALE_SRAM_MODE22_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE22_DEFAULTVAL            0x18Bu
#define DVA_ALE_SRAM_MODE23                       0x4A5100FCu
#define DVA_ALE_SRAM_MODE23_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE23_DEFAULTVAL            0x642u
#define DVA_ALE_SRAM_MODE24                       0x4A510100u
#define DVA_ALE_SRAM_MODE24_FIELDNUM              2u
#define DVA_ALE_SRAM_MODE24_DEFAULTVAL            0x0u
#define DVA_FUSA_PG_LOCK_DMY_ALE_MISC             0x4A510104u
#define DVA_FUSA_PG_LOCK_DMY_ALE_MISC_FIELDNUM    2u
#define DVA_FUSA_PG_LOCK_DMY_ALE_MISC_DEFAULTVAL  0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_ALE_MISC_H_ */