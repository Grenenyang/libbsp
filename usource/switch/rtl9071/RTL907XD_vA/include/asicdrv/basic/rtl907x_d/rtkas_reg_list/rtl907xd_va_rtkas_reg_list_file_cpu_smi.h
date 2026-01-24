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

#ifndef DVA_RTKAS_REG_LIST_FILE_CPU_SMI_H_
#define DVA_RTKAS_REG_LIST_FILE_CPU_SMI_H_

#ifdef DVA_FILE_CPU_SMI
/* File: SMI */
#define DVA_SMI_CONTROL                                0x4A180000u
#define DVA_SMI_CONTROL_FIELDNUM                       10u
#define DVA_SMI_CONTROL_DEFAULTVAL                     0xC000008Cu
#define DVA_PHY_REG_ACCESS_CONTROL0                    0x4A180004u
#define DVA_PHY_REG_ACCESS_CONTROL0_FIELDNUM           7u
#define DVA_PHY_REG_ACCESS_CONTROL0_DEFAULTVAL         0x0u
#define DVA_PHY_REG_ACCESS_CONTROL1                    0x4A180008u
#define DVA_PHY_REG_ACCESS_CONTROL1_FIELDNUM           5u
#define DVA_PHY_REG_ACCESS_CONTROL1_DEFAULTVAL         0x0u
#define DVA_PHY_REG_ACCESS_CONTROL2                    0x4A18000Cu
#define DVA_PHY_REG_ACCESS_CONTROL2_FIELDNUM           5u
#define DVA_PHY_REG_ACCESS_CONTROL2_DEFAULTVAL         0x1001u
#define DVA_PHY_AUTO_ACCESS_MASK                       0x4A180010u
#define DVA_PHY_AUTO_ACCESS_MASK_FIELDNUM              6u
#define DVA_PHY_AUTO_ACCESS_MASK_DEFAULTVAL            0x0u
#define DVA_SMI_DMY0                                   0x4A180014u
#define DVA_SMI_DMY0_FIELDNUM                          1u
#define DVA_SMI_DMY0_DEFAULTVAL                        0x0u
#define DVA_PORT0_PROPERTY_CONFIGURE                   0x4A980000u
#define DVA_PORT0_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT0_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT1_PROPERTY_CONFIGURE                   0x4A980800u
#define DVA_PORT1_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT1_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT2_PROPERTY_CONFIGURE                   0x4A981000u
#define DVA_PORT2_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT2_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT3_PROPERTY_CONFIGURE                   0x4A981800u
#define DVA_PORT3_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT3_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT4_PROPERTY_CONFIGURE                   0x4A982000u
#define DVA_PORT4_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT4_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT5_PROPERTY_CONFIGURE                   0x4A982800u
#define DVA_PORT5_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT5_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT6_PROPERTY_CONFIGURE                   0x4A983000u
#define DVA_PORT6_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT6_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT7_PROPERTY_CONFIGURE                   0x4A983800u
#define DVA_PORT7_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT7_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT8_PROPERTY_CONFIGURE                   0x4A984000u
#define DVA_PORT8_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT8_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT9_PROPERTY_CONFIGURE                   0x4A984800u
#define DVA_PORT9_PROPERTY_CONFIGURE_FIELDNUM          16u
#define DVA_PORT9_PROPERTY_CONFIGURE_DEFAULTVAL        0x841u
#define DVA_PORT10_PROPERTY_CONFIGURE                  0x4A985000u
#define DVA_PORT10_PROPERTY_CONFIGURE_FIELDNUM         16u
#define DVA_PORT10_PROPERTY_CONFIGURE_DEFAULTVAL       0x841u
#define DVA_PORT11_PROPERTY_CONFIGURE                  0x4A985800u
#define DVA_PORT11_PROPERTY_CONFIGURE_FIELDNUM         16u
#define DVA_PORT11_PROPERTY_CONFIGURE_DEFAULTVAL       0x841u
#define DVA_PORT12_PROPERTY_CONFIGURE                  0x4A986000u
#define DVA_PORT12_PROPERTY_CONFIGURE_FIELDNUM         16u
#define DVA_PORT12_PROPERTY_CONFIGURE_DEFAULTVAL       0x841u
#define DVA_PORT13_PROPERTY_CONFIGURE                  0x4A986800u
#define DVA_PORT13_PROPERTY_CONFIGURE_FIELDNUM         16u
#define DVA_PORT13_PROPERTY_CONFIGURE_DEFAULTVAL       0x841u
#define DVA_PORT14_PROPERTY_CONFIGURE                  0x4A987000u
#define DVA_PORT14_PROPERTY_CONFIGURE_FIELDNUM         16u
#define DVA_PORT14_PROPERTY_CONFIGURE_DEFAULTVAL       0x841u
#define DVA_PORT15_PROPERTY_CONFIGURE                  0x4A987800u
#define DVA_PORT15_PROPERTY_CONFIGURE_FIELDNUM         16u
#define DVA_PORT15_PROPERTY_CONFIGURE_DEFAULTVAL       0x841u
#define DVA_PORT0_LINK_STATUS                          0x4A980004u
#define DVA_PORT0_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT0_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT1_LINK_STATUS                          0x4A980804u
#define DVA_PORT1_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT1_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT2_LINK_STATUS                          0x4A981004u
#define DVA_PORT2_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT2_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT3_LINK_STATUS                          0x4A981804u
#define DVA_PORT3_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT3_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT4_LINK_STATUS                          0x4A982004u
#define DVA_PORT4_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT4_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT5_LINK_STATUS                          0x4A982804u
#define DVA_PORT5_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT5_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT6_LINK_STATUS                          0x4A983004u
#define DVA_PORT6_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT6_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT7_LINK_STATUS                          0x4A983804u
#define DVA_PORT7_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT7_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT8_LINK_STATUS                          0x4A984004u
#define DVA_PORT8_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT8_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT9_LINK_STATUS                          0x4A984804u
#define DVA_PORT9_LINK_STATUS_FIELDNUM                 14u
#define DVA_PORT9_LINK_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT10_LINK_STATUS                         0x4A985004u
#define DVA_PORT10_LINK_STATUS_FIELDNUM                14u
#define DVA_PORT10_LINK_STATUS_DEFAULTVAL              0x0u
#define DVA_PORT11_LINK_STATUS                         0x4A985804u
#define DVA_PORT11_LINK_STATUS_FIELDNUM                14u
#define DVA_PORT11_LINK_STATUS_DEFAULTVAL              0x0u
#define DVA_PORT12_LINK_STATUS                         0x4A986004u
#define DVA_PORT12_LINK_STATUS_FIELDNUM                14u
#define DVA_PORT12_LINK_STATUS_DEFAULTVAL              0x0u
#define DVA_PORT13_LINK_STATUS                         0x4A986804u
#define DVA_PORT13_LINK_STATUS_FIELDNUM                14u
#define DVA_PORT13_LINK_STATUS_DEFAULTVAL              0x0u
#define DVA_PORT14_LINK_STATUS                         0x4A987004u
#define DVA_PORT14_LINK_STATUS_FIELDNUM                14u
#define DVA_PORT14_LINK_STATUS_DEFAULTVAL              0x0u
#define DVA_PORT15_LINK_STATUS                         0x4A987804u
#define DVA_PORT15_LINK_STATUS_FIELDNUM                14u
#define DVA_PORT15_LINK_STATUS_DEFAULTVAL              0x0u
#define DVA_PORT0_SMI_STATUS                           0x4A980008u
#define DVA_PORT0_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT0_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT1_SMI_STATUS                           0x4A980808u
#define DVA_PORT1_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT1_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT2_SMI_STATUS                           0x4A981008u
#define DVA_PORT2_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT2_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT3_SMI_STATUS                           0x4A981808u
#define DVA_PORT3_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT3_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT4_SMI_STATUS                           0x4A982008u
#define DVA_PORT4_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT4_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT5_SMI_STATUS                           0x4A982808u
#define DVA_PORT5_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT5_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT6_SMI_STATUS                           0x4A983008u
#define DVA_PORT6_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT6_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT7_SMI_STATUS                           0x4A983808u
#define DVA_PORT7_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT7_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT8_SMI_STATUS                           0x4A984008u
#define DVA_PORT8_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT8_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT9_SMI_STATUS                           0x4A984808u
#define DVA_PORT9_SMI_STATUS_FIELDNUM                  5u
#define DVA_PORT9_SMI_STATUS_DEFAULTVAL                0x0u
#define DVA_PORT10_SMI_STATUS                          0x4A985008u
#define DVA_PORT10_SMI_STATUS_FIELDNUM                 5u
#define DVA_PORT10_SMI_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT11_SMI_STATUS                          0x4A985808u
#define DVA_PORT11_SMI_STATUS_FIELDNUM                 5u
#define DVA_PORT11_SMI_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT12_SMI_STATUS                          0x4A986008u
#define DVA_PORT12_SMI_STATUS_FIELDNUM                 5u
#define DVA_PORT12_SMI_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT13_SMI_STATUS                          0x4A986808u
#define DVA_PORT13_SMI_STATUS_FIELDNUM                 5u
#define DVA_PORT13_SMI_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT14_SMI_STATUS                          0x4A987008u
#define DVA_PORT14_SMI_STATUS_FIELDNUM                 5u
#define DVA_PORT14_SMI_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT15_SMI_STATUS                          0x4A987808u
#define DVA_PORT15_SMI_STATUS_FIELDNUM                 5u
#define DVA_PORT15_SMI_STATUS_DEFAULTVAL               0x0u
#define DVA_PORT0_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98000Cu
#define DVA_PORT0_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT0_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT1_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98080Cu
#define DVA_PORT1_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT1_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT2_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98100Cu
#define DVA_PORT2_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT2_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT3_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98180Cu
#define DVA_PORT3_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT3_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT4_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98200Cu
#define DVA_PORT4_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT4_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT5_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98280Cu
#define DVA_PORT5_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT5_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT6_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98300Cu
#define DVA_PORT6_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT6_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT7_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98380Cu
#define DVA_PORT7_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT7_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT8_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98400Cu
#define DVA_PORT8_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT8_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT9_FUSA_PG_LOCK_DMY_CPU_SMI             0x4A98480Cu
#define DVA_PORT9_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM    2u
#define DVA_PORT9_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL  0x0u
#define DVA_PORT10_FUSA_PG_LOCK_DMY_CPU_SMI            0x4A98500Cu
#define DVA_PORT10_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM   2u
#define DVA_PORT10_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL 0x0u
#define DVA_PORT11_FUSA_PG_LOCK_DMY_CPU_SMI            0x4A98580Cu
#define DVA_PORT11_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM   2u
#define DVA_PORT11_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL 0x0u
#define DVA_PORT12_FUSA_PG_LOCK_DMY_CPU_SMI            0x4A98600Cu
#define DVA_PORT12_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM   2u
#define DVA_PORT12_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL 0x0u
#define DVA_PORT13_FUSA_PG_LOCK_DMY_CPU_SMI            0x4A98680Cu
#define DVA_PORT13_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM   2u
#define DVA_PORT13_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL 0x0u
#define DVA_PORT14_FUSA_PG_LOCK_DMY_CPU_SMI            0x4A98700Cu
#define DVA_PORT14_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM   2u
#define DVA_PORT14_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL 0x0u
#define DVA_PORT15_FUSA_PG_LOCK_DMY_CPU_SMI            0x4A98780Cu
#define DVA_PORT15_FUSA_PG_LOCK_DMY_CPU_SMI_FIELDNUM   2u
#define DVA_PORT15_FUSA_PG_LOCK_DMY_CPU_SMI_DEFAULTVAL 0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_CPU_SMI_H_ */