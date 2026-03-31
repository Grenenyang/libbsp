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

#ifndef RTKAS_API_SMI_H__
#define RTKAS_API_SMI_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_common.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define RTK_STD_PHY_ADDR_MAX (31u)
#define RTK_STD_PHY_REG_MAX  (31u)

#define PAD_MUX_GPIOC_1_2_MDIO_MASTER_FUNC 0x1u
#define PAD_MUX_GPIOC_1_2_DEFAULT_FUNC     0xFu
#define PAD_MUX_GPIOD_0_1_MDIO_MASTER_FUNC 0x1u
#define PAD_MUX_GPIOD_0_1_DEFAULT_FUNC     0xFu

#define RTK_TIMER_MDX_USECOUND (10u)
#define RTK_TIMER_MDX_TRYTIMES (0x3E8u)

/******************************************************************************/
/* Typedef Data                                                              */
/******************************************************************************/
typedef uint8  MdxBaudRate; /**< the data type of MDX baud rate */
typedef uint8  MdxSlvPhyID; /**< the data type of MDX slave PHY ID */
typedef uint8  MdxSlvReg;   /**< the data type of MDX slave register address */
typedef uint16 MdxSlvData;  /**< the data type of MDX data */

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
typedef enum
{
    EM_MDX_MASTER_GPIOD_1_0 = 0, /**< GPIOD0/GPIOD1 as MDX master pins */
    EM_MDX_MASTER_GPIOC_2_1,     /**< GPIOC1/GPIOC2 as MDX master pins */
    EM_MDX_MASTER_GPIO_END
} MdxMasterPin_e;

typedef enum
{
    EM_MDX_PREAMBLE_0  = 0, /**< 0-bit preamble */
    EM_MDX_PREAMBLE_8  = 1, /**< 8-bit preamble */
    EM_MDX_PREAMBLE_16 = 2, /**< 16-bit preamble */
    EM_MDX_PREAMBLE_32 = 3, /**< 32-bit preamble */
    EM_MDX_PREAMBLE_END
} MdxPreamble_e;

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/**
 * @brief The information structure of MDX initialization.
 *
 * This structure is used to initialize the MDX interface.
 */
typedef struct
{
    MdxMasterPin_e gpioPin;  /**< [in] GPIO pins used as the MDX pins (EM_MDX_MASTER_GPIOD_1_0 to EM_MDX_MASTER_GPIOC_2_1). */
    MdxBaudRate    baudRate; /**< [in] MDX baud rate (1 to 31). \n
                              The period of the output clock of MDX is the product of baudRate and 80 nanoseconds; \n
                              For example, if baudRate is 1, the period of the output clock equals 1*80 nanoseconds (12.5 MHz).
                              */
    MdxPreamble_e preamble;  /**< [in] The length of the MDX preamble (EM_MDX_PREAMBLE_0 to EM_MDX_PREAMBLE_32). */
} MdxInitSetInfo_t;

/**
 * @brief The information structure for MDX access.
 *
 * This structure is used to access the slaves.
 */
typedef struct
{
    MdxSlvPhyID phyID;   /**< [in] Slave PHY address (0 to 31) */
    MdxSlvReg   regNum;  /**< [in] PHY register address (0 to 31) */
    MdxSlvData  mdxData; /**< [in] PHY register data (0 to 0xFFFF) */
} MdxRegInfo_t;

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_std_mdx_init(UnitChip_t unitChip, MdxInitSetInfo_t *mdxInitSetInfo);
EXTERN_API RtkApiRet rtk_std_mdx_register_get(UnitChip_t unitChip, MdxRegInfo_t *mdxRegInfo);
EXTERN_API RtkApiRet rtk_std_mdx_register_set(UnitChip_t unitChip, MdxRegInfo_t *mdxRegInfo);

#endif /* RTKAS_API_SMI_H__ */