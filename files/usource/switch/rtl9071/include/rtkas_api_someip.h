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

#ifndef RTKAS_API_SOMEIP_H__
#define RTKAS_API_SOMEIP_H__

#include "rtkas_common.h"

/**
 * @brief This structure represents the SOME/IP L4 destination port configuration information.
 *
 * This structure includes the index and L4 destination port.
 */
typedef struct
{
    uint8  idx;     /**< Index of the L4 destination port. (1 to SOMEIP_L4DPORT_NUM) */
    uint16 l4dport; /**< L4 destination port. (0 to 65535) */
} someipL4Dport_t;

/**
 * @brief This structure represents the SOME/IP firewall HW whitelist configuration information.
 *
 * This structure includes the index and HW whitelist.
 */
typedef struct
{
    uint8  idx;       /**< [in] Index of the HW whitelist. (1 to SOMEIP_HW_WHITELIST_NUM) */
    uint32 whitelist; /**< [in] HW whitelist (0x0 to 0xFFFFFFFF)*/
} someipHWWhitelist_t;

EXTERN_API RtkApiRet rtk_someip_set_l4dport(UnitChip_t unitChip, someipL4Dport_t *someipL4Dport);
EXTERN_API RtkApiRet rtk_someip_get_l4dport(UnitChip_t unitChip, uint16 *l4dport);
EXTERN_API RtkApiRet rtk_someip_set_hw_whitelist(UnitChip_t unitChip, someipHWWhitelist_t *someipHWWhitelist);
EXTERN_API RtkApiRet rtk_someip_get_hw_whitelist(UnitChip_t unitChip, uint32 *whitelist);

#endif /* RTKAS_API_SOMEIP_H__ */
