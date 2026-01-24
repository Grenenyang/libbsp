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

#ifndef DVA_RTKAS_REG_LIST_FILE_I2C_H_
#define DVA_RTKAS_REG_LIST_FILE_I2C_H_

#ifdef DVA_FILE_I2C
/* File: I2C */
#define DVA_EXTERNAL_ACCESS_CONTROL                         0x4A450000u
#define DVA_EXTERNAL_ACCESS_CONTROL_FIELDNUM                4u
#define DVA_EXTERNAL_ACCESS_CONTROL_DEFAULTVAL              0x30u
#define DVA_I2C_EXTERNAL_ACCESS_STATUS                      0x4A450004u
#define DVA_I2C_EXTERNAL_ACCESS_STATUS_FIELDNUM             2u
#define DVA_I2C_EXTERNAL_ACCESS_STATUS_DEFAULTVAL           0x0u
#define DVA_I2C_EXTERNAL_ACCESS_ADDRESS                     0x4A450008u
#define DVA_I2C_EXTERNAL_ACCESS_ADDRESS_FIELDNUM            2u
#define DVA_I2C_EXTERNAL_ACCESS_ADDRESS_DEFAULTVAL          0x0u
#define DVA_EEPROM_AUTO_DOWNLOAD_CONTROL                    0x4A45000Cu
#define DVA_EEPROM_AUTO_DOWNLOAD_CONTROL_FIELDNUM           9u
#define DVA_EEPROM_AUTO_DOWNLOAD_CONTROL_DEFAULTVAL         0x20000u
#define DVA_EEPROM_CODE_SIZE_ADDR                           0x4A450010u
#define DVA_EEPROM_CODE_SIZE_ADDR_FIELDNUM                  2u
#define DVA_EEPROM_CODE_SIZE_ADDR_DEFAULTVAL                0x0u
#define DVA_EEPROM_AUTODOWNLOAD_MAC_POSTION                 0x4A450014u
#define DVA_EEPROM_AUTODOWNLOAD_MAC_POSTION_FIELDNUM        2u
#define DVA_EEPROM_AUTODOWNLOAD_MAC_POSTION_DEFAULTVAL      0x4000400u
#define DVA_EEPROM_AUTODOWNLOAD_PHY_POSTION                 0x4A450018u
#define DVA_EEPROM_AUTODOWNLOAD_PHY_POSTION_FIELDNUM        2u
#define DVA_EEPROM_AUTODOWNLOAD_PHY_POSTION_DEFAULTVAL      0x4000400u
#define DVA_EEPROM_AUTODOWNLOAD_LUT_POSTION                 0x4A45001Cu
#define DVA_EEPROM_AUTODOWNLOAD_LUT_POSTION_FIELDNUM        2u
#define DVA_EEPROM_AUTODOWNLOAD_LUT_POSTION_DEFAULTVAL      0x4000400u
#define DVA_EEPROM_STATUS                                   0x4A450020u
#define DVA_EEPROM_STATUS_FIELDNUM                          6u
#define DVA_EEPROM_STATUS_DEFAULTVAL                        0xAu
#define DVA_CPU_INDIRECT_ACCESS_EEPROM_CONTROL              0x4A450024u
#define DVA_CPU_INDIRECT_ACCESS_EEPROM_CONTROL_FIELDNUM     10u
#define DVA_CPU_INDIRECT_ACCESS_EEPROM_CONTROL_DEFAULTVAL   0x0u
#define DVA_CPU_INDIRECT_ACCESS_EEPROM_DATA0                0x4A450028u
#define DVA_CPU_INDIRECT_ACCESS_EEPROM_DATA0_FIELDNUM       4u
#define DVA_CPU_INDIRECT_ACCESS_EEPROM_DATA0_DEFAULTVAL     0x0u
#define DVA_CPU_INDIRECT_ACCESS_EEPROM_DATA1                0x4A45002Cu
#define DVA_CPU_INDIRECT_ACCESS_EEPROM_DATA1_FIELDNUM       4u
#define DVA_CPU_INDIRECT_ACCESS_EEPROM_DATA1_DEFAULTVAL     0x0u
#define DVA_I2C_8051_CPU_OCCUPY_STATUS                      0x4A450030u
#define DVA_I2C_8051_CPU_OCCUPY_STATUS_FIELDNUM             3u
#define DVA_I2C_8051_CPU_OCCUPY_STATUS_DEFAULTVAL           0x0u
#define DVA_I2C_DMY0                                        0x4A450034u
#define DVA_I2C_DMY0_FIELDNUM                               1u
#define DVA_I2C_DMY0_DEFAULTVAL                             0x0u
#define DVA_REGACC_CHANNEL_CONTROL                          0x4A450038u
#define DVA_REGACC_CHANNEL_CONTROL_FIELDNUM                 3u
#define DVA_REGACC_CHANNEL_CONTROL_DEFAULTVAL               0x3u
#define DVA_I2C_DMY1                                        0x4A45003Cu
#define DVA_I2C_DMY1_FIELDNUM                               1u
#define DVA_I2C_DMY1_DEFAULTVAL                             0x0u
#define DVA_MDC_MDIO_MASTER_CONTROL0                        0x4A450040u
#define DVA_MDC_MDIO_MASTER_CONTROL0_FIELDNUM               6u
#define DVA_MDC_MDIO_MASTER_CONTROL0_DEFAULTVAL             0x20Cu
#define DVA_MDC_MDIO_MASTER_CONTROL1                        0x4A450044u
#define DVA_MDC_MDIO_MASTER_CONTROL1_FIELDNUM               2u
#define DVA_MDC_MDIO_MASTER_CONTROL1_DEFAULTVAL             0x3u
#define DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0            0x4A450048u
#define DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_FIELDNUM   7u
#define DVA_MDC_MDIO_MASTER_REG_ACCESS_CONTROL_0_DEFAULTVAL 0x0u
#define DVA_SPI_SLAVE_CONTROL                               0x4A45004Cu
#define DVA_SPI_SLAVE_CONTROL_FIELDNUM                      3u
#define DVA_SPI_SLAVE_CONTROL_DEFAULTVAL                    0x8u
#define DVA_SPI_SLAVE_CHECKSUM_CONTROL                      0x4A450050u
#define DVA_SPI_SLAVE_CHECKSUM_CONTROL_FIELDNUM             4u
#define DVA_SPI_SLAVE_CHECKSUM_CONTROL_DEFAULTVAL           0x1213u
#define DVA_SPI_SLAVE_CHECKSUM_INTERRUPT_STATUS             0x4A450054u
#define DVA_SPI_SLAVE_CHECKSUM_INTERRUPT_STATUS_FIELDNUM    2u
#define DVA_SPI_SLAVE_CHECKSUM_INTERRUPT_STATUS_DEFAULTVAL  0x0u
#define DVA_SPI_SLAVE_CHECKSUM_INTERRUPT_CONTROL            0x4A450058u
#define DVA_SPI_SLAVE_CHECKSUM_INTERRUPT_CONTROL_FIELDNUM   2u
#define DVA_SPI_SLAVE_CHECKSUM_INTERRUPT_CONTROL_DEFAULTVAL 0x1u
#define DVA_SLAVE_INTERFACE_CHECKSUM_CONTROL                0x4A45005Cu
#define DVA_SLAVE_INTERFACE_CHECKSUM_CONTROL_FIELDNUM       7u
#define DVA_SLAVE_INTERFACE_CHECKSUM_CONTROL_DEFAULTVAL     0x0u
#define DVA_REGACC_CHANNEL_FSM                              0x4A450060u
#define DVA_REGACC_CHANNEL_FSM_FIELDNUM                     3u
#define DVA_REGACC_CHANNEL_FSM_DEFAULTVAL                   0x0u
#define DVA_PSEUDO_REG_PAGE_NUMBER                          0x4A450064u
#define DVA_PSEUDO_REG_PAGE_NUMBER_FIELDNUM                 2u
#define DVA_PSEUDO_REG_PAGE_NUMBER_DEFAULTVAL               0x0u
#define DVA_PSEUDO_REG_CMD                                  0x4A450068u
#define DVA_PSEUDO_REG_CMD_FIELDNUM                         1u
#define DVA_PSEUDO_REG_CMD_DEFAULTVAL                       0x0u
#define DVA_PSEUDO_REG_DAT                                  0x4A45006Cu
#define DVA_PSEUDO_REG_DAT_FIELDNUM                         1u
#define DVA_PSEUDO_REG_DAT_DEFAULTVAL                       0x0u
#define DVA_PSEUDO_REG_AUTH_CTRL                            0x4A450070u
#define DVA_PSEUDO_REG_AUTH_CTRL_FIELDNUM                   1u
#define DVA_PSEUDO_REG_AUTH_CTRL_DEFAULTVAL                 0x0u
#define DVA_FUSA_PG_LOCK_DMY_I2C                            0x4A450074u
#define DVA_FUSA_PG_LOCK_DMY_I2C_FIELDNUM                   2u
#define DVA_FUSA_PG_LOCK_DMY_I2C_DEFAULTVAL                 0x0u
#define DVA_I2C_FSM_CONTROL                                 0x4A450078u
#define DVA_I2C_FSM_CONTROL_FIELDNUM                        5u
#define DVA_I2C_FSM_CONTROL_DEFAULTVAL                      0xFFFFFFFu
#define DVA_PSEUDO_REG_ERROR_STATUS                         0x4A45007Cu
#define DVA_PSEUDO_REG_ERROR_STATUS_FIELDNUM                2u
#define DVA_PSEUDO_REG_ERROR_STATUS_DEFAULTVAL              0x0u
#endif
#endif /* DVA_RTKAS_REG_LIST_FILE_I2C_H_ */