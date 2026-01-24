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

#ifndef CVB_ASICDRV_ETHERNET_ACCESS_H
#define CVB_ASICDRV_ETHERNET_ACCESS_H

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_vb_api_ethernet_access.h"

/* MAC_ADDR_LEN + MAC_ADDR_LEN + RTK_ETHERTYPE_LEN + 19*4 */
#define CVB_ETHERACCESS_CHECKID_OFFSET (90u)

EXTERN_API RtkApiRet cvb_drv_eth_acc_gen_page_select(uint8 sMac[MAC_ADDR_LEN], uint8 dMac[MAC_ADDR_LEN], uint32 page, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_drv_eth_acc_check_page_select(uint8 *packet);
EXTERN_API RtkApiRet cvb_drv_eth_acc_flow_page_select(uint32 page);
EXTERN_API uint16    cvb_drv_eth_acc_checksum(const EtherAccessIpHdr_t *header);
EXTERN_API uint32    cvb_drv_eth_acc_data_checksum(const uint8 *data, uint32 len, uint32 checksum);
EXTERN_API RtkApiRet cvb_drv_eth_acc_gen_burst_rw_check(uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue);
EXTERN_API RtkApiRet cvb_drv_eth_acc_gen_patch_start_check(uint32 addr, uint32 size, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_drv_eth_acc_gen_write_check(const uint8 *data, const uint32 seqId, EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_drv_eth_acc_flash_write(uint32 addr, uint32 len, const uint8 *data, uint32 checksum);
EXTERN_API RtkApiRet cvb_drv_eth_acc_flow_flash_erase_check(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], const uint32 delay);
EXTERN_API RtkApiRet cvb_drv_eth_acc_patch_write(uint32 addr, uint32 len, const uint8 *data, uint32 checksum);
EXTERN_API RtkApiRet cvb_drv_eth_acc_reset_system_check(const EtherAccessMac_t mac);
EXTERN_API RtkApiRet cvb_drv_eth_acc_table_check(cvb_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 regNum, const uint32 *data);
EXTERN_API RtkApiRet cvb_drv_eth_acc_table_entry_get(uint32 entryRegNum, uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue, uint32 *data);
EXTERN_API RtkApiRet cvb_drv_eth_acc_table_entry_set(uint32 regNum, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue);
EXTERN_API RtkApiRet cvb_drv_eth_acc_table_get(uint32 unit, cvb_table_list_t table, uint32 startIndex, uint32 entryNum, uint32 *data);
EXTERN_API RtkApiRet cvb_drv_eth_acc_table_set(uint32 unit, cvb_table_list_t table, uint32 startIndex, uint32 entryNum, const uint32 *data);
EXTERN_API RtkApiRet cvb_drv_eth_acc_check_l3_otp_rep_null(void *packet, const uint8 *uuid, uint32 *ackId, uint8 *data);
EXTERN_API RtkApiRet cvb_drv_eth_acc_txrx_checkid(EtherAccessL2Header_t *txPkt, const uint32 txPktLen, uint8 *rxPkt, uint32 *rxPktLen, uint32 option);
EXTERN_API RtkApiRet cvb_drv_eth_acc_flow_check_id(const uint32 option);
EXTERN_API RtkApiRet cvb_drv_eth_acc_gen_check_id(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], EtherAccessL2Header_t *packet);
EXTERN_API RtkApiRet cvb_drv_eth_acc_check_rep_check_id(const uint8 *packet, const uint32 option, uint32 *ackId);
#ifdef RTK_MODULE_STACKING
EXTERN_API RtkApiRet cvb_drv_eth_acc_stacking_gen_burst(const EtherAccessMac_t mac, cvb_StackBurstReg_t *stkHdr, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], const uint32 *regValue, cvb_StackEthBurstHeader_t *packet);
EXTERN_API RtkApiRet cvb_drv_eth_acc_stacking_check_burst(cvb_StackBurstReg_t *stkHdr, uint32 number, const uint8 *regCmd, uint8 regAddr[][RTK_REG_ADDR_LEN], uint32 *regValue, uint8 *packet);
EXTERN_API RtkApiRet cvb_drv_eth_acc_stk_gen_fw(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], cvb_StackPayloadRemoteLoad_t *stkHdr, const uint8 *data, cvb_StackEthFwHeader_t *packet);
EXTERN_API RtkApiRet cvb_drv_eth_acc_stk_gen_cfg(const uint8 sMac[MAC_ADDR_LEN], const uint8 dMac[MAC_ADDR_LEN], cvb_StackPayloadRemoteCfg_t *stkHdr, const uint8 *data, cvb_StackEthCfgHeader_t *packet);
EXTERN_API RtkApiRet cvb_drv_eth_acc_stk_check_rep(const uint8 *packet, const uint32 seqId);
#endif

#endif /*_RTL906X_ASICDRV_ETHERNET_ACCESS_H_*/
