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

#ifndef RTKAS_API_STACKING_H__
#define RTKAS_API_STACKING_H__

#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_stacking.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_stacking.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_stacking.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_stacking.h"
#endif

#define STACK_MAX_UNIT (4u)

/**
 * @brief This struct is used for stacking dynamic search.
 */
typedef struct
{
    uint32 numOnS0; /**< [out] *numOnS0 Number on S0 (0 to 3) */
    uint32 numOnS1; /**< [out] *numOnS1 Number on S1 (0 to 3) */
} StackingDevsearch_t;

/**
 * @brief This struct is used for stacking automatically configure.
 */
typedef struct
{
    uint32 selOnS0; /**< [in] selOnS0 Select number on S0 (0 to 3) */
    uint32 selOnS1; /**< [in] selOnS1 Select number on S1 (0 to 3) */
} StackingAutoConfig_t;

/**
 * @brief This struct is used for getting stacking status.
 */
typedef struct
{
    uint8 status; /**< [out] status Status (0 to 0x4) */
} StackingGetStatus_t;

/**
 * @brief This struct is used for getting stacking master id.
 */
typedef struct
{
    uint8 masterId; /**< [out] masterId Master ID (1 to 7) */
} StackingGetMasterId_t;

/**
 * @brief This struct is used for getting stacking group id.
 */
typedef struct
{
    uint16 groupId; /**< [out] groupId Group ID (1 to 0x1FFF) */
} StackingGetGroupId_t;

/**
 * @brief This struct is used for getting stacking unit map information.
 */
typedef struct
{
    uint8 unitID[STACK_MAX_UNIT]; /**< [out] unitID[4] unit ID (1 to 7 for 4 elements) */
    uint8 atPort[STACK_MAX_UNIT]; /**< [out] atPort[4] at local or at port (0 to 2 and 0xF for 4 elements) */
} StackingGetUnitmap_t;

/**
 * @brief This struct is used for checking stacking connection.
 */
typedef struct
{
    uint8 unitID[STACK_MAX_UNIT];     /**< [out] unitID[4] unit ID (1 to 7 for 4 elements) */
    uint8 linkStatus[STACK_MAX_UNIT]; /**< [out] linkStatus[4] Link status (0 to 3 for 4 elements) */
} StackingCheckConnection_t;

/**
 * @brief This struct is used for setting stacking otp.
 */
typedef struct
{
    uint32 role;         /**< [in] role Role (0 to 1) */
    uint32 portS0Enable; /**< [in] portS0Enable Port S0 enable (0 to 1) */
    uint32 portS1Enable; /**< [in] portS1Enable Port S1 enable (0 to 1) */
} StackingBootOtpSet_t;

/**
 * @brief This struct is used for getting stacking otp.
 */
typedef struct
{
    uint32 role;         /**< [out] role Role (0 to 1) */
    uint32 portS0Enable; /**< [out] portS0Enable Port S0 enable (0 to 1) */
    uint32 portS1Enable; /**< [out] portS1Enable Port S1 enable (0 to 1) */
} StackingBootOtpGet_t;

/**
 * @brief This struct is used for setting stacking flashless boot.
 */
typedef struct
{
#if defined(RTL_907XD_V1)
    uint32 slaveUnitDV1; /**< [in] slaveUnit Slave unit (1 to 7) */
#endif
#if defined(RTL_907XD_VA)
    uint32 slaveUnitDVa; /**< [in] slaveUnit Slave unit (1 to 7) */
#endif
#if defined(RTL_907XC_VB)
    uint32 slaveUnitCVb; /**< [in] slaveUnit Slave unit (1 to 7) */
#endif
#if defined(RTL_907XC_VA)
    uint32 slaveUnitCVa; /**< [in] slaveUnit Slave unit (1 to 7) */
#endif
    const int8 *fwFileName;  /**< fwFileName Firmware file name (1 to 255 characters) */
    const int8 *cfgFileName; /**< cfgFileName Configuration file name (1 to 255 characters) */
} StackingFlashlessUpdateSlave_t;

EXTERN_API RtkApiRet rtk_stacking_enable(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_stacking_disable(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_stacking_devsearch(UnitChip_t unitChip, StackingDevsearch_t *stackingDevsearch);
EXTERN_API RtkApiRet rtk_stacking_auto_config(UnitChip_t unitChip, StackingAutoConfig_t *stackingAutoConfig);
EXTERN_API RtkApiRet rtk_stacking_get_status(UnitChip_t unitChip, StackingGetStatus_t *stackingGetStatus);
EXTERN_API RtkApiRet rtk_stacking_get_master_id(UnitChip_t unitChip, StackingGetMasterId_t *stackingGetMasterId);
EXTERN_API RtkApiRet rtk_stacking_get_group_id(UnitChip_t unitChip, StackingGetGroupId_t *stackingGetGroupId);
EXTERN_API RtkApiRet rtk_stacking_get_unitmap(UnitChip_t unitChip, StackingGetUnitmap_t *stackingGetUnitmap);
EXTERN_API RtkApiRet rtk_stacking_check_connection(UnitChip_t unitChip, StackingCheckConnection_t *stackingCheckConnection);
EXTERN_API RtkApiRet rtk_stacking_boot_otp_set(UnitChip_t unitChip, StackingBootOtpSet_t *stackingBootOtpSet);
EXTERN_API RtkApiRet rtk_stacking_boot_otp_get(UnitChip_t unitChip, StackingBootOtpGet_t *stackingBootOtpGet);
EXTERN_API RtkApiRet rtk_stacking_flashless_update_slave(UnitChip_t unitChip, StackingFlashlessUpdateSlave_t *stackingFlashlessUpdateSlave);
#endif /* RTKAS_API_STACKING_H__ */
