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

#ifndef RTKAS_API_MACPHY_H__
#define RTKAS_API_MACPHY_H__

#include "rtkas_common.h"

/* SPI clock source */
#define RTK_MACPHY_CLOCK_SOURCE_150MHZ (0u) /**< 0: 150 MHz */
#define RTK_MACPHY_CLOCK_SOURCE_200MHZ (1u) /**< 1: 200 MHz */

/**
 * @brief This structure represents the macphy SPI parameter configuration information.
 *
 * This structure includes the SPI clock source, SPI clock divider, and SPI RX sample delay.
 */
typedef struct
{
    uint32 clockSrc;      /**< SPI clock source (0: 150 MHz, 1: 200 MHz) */
    uint32 clockDivider;  /**< SPI clock divider (Even value between 0x4 ~ 0xFFFE) */
    uint32 rxSampleDelay; /**< SPI RX sample delay. When the SPI clock source is set to 150 MHz, the unit is 6.67ns; when the SPI clock source is set to 200 MHz, the unit is 5ns. (0x0 ~ 0xFF) */
} MacphySpiParam_t;

/**
 * @brief This structure represents the configuration information of macphy.
 *
 * This structure includes the port ID and various macphy configuration details.
 */
typedef struct
{
    RtkPort          portId; /**< [in] Port number (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_2ND) */
    MacphySpiParam_t param;  /**< [in,out] SPI parameter (MacphySpiParam_t)*/
} macphyInfo_t;

EXTERN_API RtkApiRet rtk_macphy_get_spi_param(UnitChip_t unitChip, macphyInfo_t *macphyInfo);

#endif /* RTKAS_API_MACPHY_H__ */
