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

#ifndef DVA_RTK_API_STACKING_H__
#define DVA_RTK_API_STACKING_H__

#define DVA_STACK_DEFAULT_UNIT (1u)

#define DVA_STACK_REMOTE_LOAD_LENGTH_UNIT (1276u)
/* DVA_STACK_REMOTE_LOAD_LENGTH_UNIT + random number length */
#define DVA_STACK_RAND_LOAD_LENGTH_UNIT (1280u)

#define DVA_STACK_COMMAND_REGISTER       DVA_INTERNAL_CPU_VARIABLE41
#define DVA_STACK_COMMAND_REG_ADDR       DVA_INTERNAL_CPU_VARIABLE42
#define DVA_STACK_COMMAND_REG_VALUE      DVA_INTERNAL_CPU_VARIABLE43
#define DVA_STACK_COMMAND_REG_STATUS     DVA_INTERNAL_CPU_VARIABLE44
#define DVA_STACK_STATUS_REG_1           DVA_INTERNAL_CPU_VARIABLE42
#define DVA_STACK_STATUS_REG_2           DVA_INTERNAL_CPU_VARIABLE43
#define DVA_STACK_STATUS_REG_3           DVA_INTERNAL_CPU_VARIABLE44
#define DVA_STACK_STATUS_REG_4           DVA_INTERNAL_CPU_VARIABLE45
#define DVA_STACK_STATUS_REG_5           DVA_INTERNAL_CPU_VARIABLE46
#define DVA_STACK_COMMAND_INITIAL        (0x01u)
#define DVA_STACK_COMMAND_DISABLE        (0x02u)
#define DVA_STACK_COMMAND_DEVSEARCH      (0x10u)
#define DVA_STACK_COMMAND_AUTOCFG        (0x20u)
#define DVA_STACK_COMMAND_REG_GET        (0x30u)
#define DVA_STACK_COMMAND_REG_GET_WAIT   (0x31u)
#define DVA_STACK_COMMAND_REG_GET_FAIL   (0x32u)
#define DVA_STACK_COMMAND_PAGE_GET       (0x38u)
#define DVA_STACK_COMMAND_PAGE_GET_WAIT  (0x39u)
#define DVA_STACK_COMMAND_PAGE_GET_FAIL  (0x3Au)
#define DVA_STACK_COMMAND_REG_SET        (0x40u)
#define DVA_STACK_COMMAND_REG_SET_WAIT   (0x41u)
#define DVA_STACK_COMMAND_REG_SET_FAIL   (0x42u)
#define DVA_STACK_COMMAND_PAGE_SET       (0x48u)
#define DVA_STACK_COMMAND_PAGE_SET_WAIT  (0x49u)
#define DVA_STACK_COMMAND_PAGE_SET_FAIL  (0x4Au)
#define DVA_STACK_COMMAND_DUMP_REG       (0x91u)
#define DVA_STACK_COMMAND_EXIT_BOOT_FLOW (0xE417B000u)

#define DVA_STACK_STS_DISABLE                 (0x00u)
#define DVA_STACK_STS_INIT                    (0x01u)
#define DVA_STACK_STS_DYNAMIC_COMPLETE        (0x02u)
#define DVA_STACK_STS_DYNAMIC_COMPLETE_REMOTE (0x03u)
#define DVA_STACK_STS_PRESET_COMPLETE         (0x04u)
#define DVA_STACK_STS_ERROR                   (0x05u)

#define DVA_STACK_PORT_NA    (0x00u)
#define DVA_STACK_PORT_S0    (0x01u)
#define DVA_STACK_PORT_S1    (0x02u)
#define DVA_STACK_PORT_LOCAL (0x03u)
#define DVA_STACK_DEV_NA     (0x00u)

#define DVA_STACK_MAX_HOP     (3u)
#define DVA_STACK_MAX_UNIT    (4u)
#define DVA_STACK_MAX_UNIT_ID (7u)

#define DVA_STACK_BOOTUP_END       (0xB6u)
#define DVA_STACK_BOOTUP_FLASHLESS (0xB5u)

#define DVA_FWSTS_MAIN_STACKBOOT_MASTER               (0x0050u)
#define DVA_FWSTS_MAIN_STACKBOOT_MASTER_END           (0x0051u)
#define DVA_FWSTS_MAIN_STACKBOOT_SLAVE                (0x0052u)
#define DVA_FWSTS_MAIN_STACKBOOT_SLAVE_END            (0x0053u)
#define DVA_FWSTS_MAIN_STACKBOOT_SLAVE_END_PURE_FLASH (0x0054u)
#define DVA_FWSTS_MAIN_STACKBOOT_FLASHLESS_MASTER     (0x0055u)

#define DVA_TIMEOUT_VALUE (0x10000u)

/****************************************************************
 *  Global Variables
 ****************************************************************/
extern uint32 dva_g_stackingMaster;

EXTERN_API RtkApiRet dva_stacking_enable(uint32 unit);
EXTERN_API RtkApiRet dva_stacking_disable(uint32 unit);
EXTERN_API RtkApiRet dva_stacking_devsearch(uint32 unit, uint32 *numOnS0, uint32 *numOnS1);
EXTERN_API RtkApiRet dva_stacking_auto_config(uint32 unit, uint32 selOnS0, uint32 selOnS1);
EXTERN_API RtkApiRet dva_stacking_get_status(uint32 unit, uint8 *status);
EXTERN_API RtkApiRet dva_stacking_get_master_id(uint32 unit, uint8 *masterId);
EXTERN_API RtkApiRet dva_stacking_get_group_id(uint32 unit, uint16 *groupId);
EXTERN_API RtkApiRet dva_stacking_get_unitmap(uint32 unit, uint8 *unitID, uint8 *atPort);
EXTERN_API RtkApiRet dva_stacking_check_connection(uint32 unit, uint8 *unitID, uint8 *linkStatus);
EXTERN_API RtkApiRet dva_stacking_boot_otp_set(uint32 unit, uint32 role, uint32 portS0Enable, uint32 portS1Enable);
EXTERN_API RtkApiRet dva_stacking_boot_otp_get(uint32 unit, uint32 *role, uint32 *portS0Enable, uint32 *portS1Enable);
EXTERN_API RtkApiRet dva_stacking_flashless_update_slave(uint32 slaveUnit, const int8 *fwFileName, const int8 *cfgFileName);
#endif /* DVA_RTK_API_STACKING_H__ */