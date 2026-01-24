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

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_macphy.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_macphy.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_macphy.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_macphy.h"
#endif
#include "rtkas_api_macphy.h"
#include "rtl907xc_va_reg_struct.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup MACPHY_EXAMPLE Macphy Example
 * @{
 */

/**
 * @defgroup RTL907CVA_MACPHY_EXAMPLE RTL907XC_VA Macphy Example
 * @{
 */

/**
 * @defgroup RTL907CVA_MACPHY_GET_SPI_PARAM_EXAMPLE Macphy Get SPI Parameter Example
 * @{
 */

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
RtkApiRet cva_exam_macphy_get(void);

/******************************************************************************/
/* Functions Definition                                                       */
/******************************************************************************/
/**
 * @brief <b>Get macphy SPI parameters</b>
 *
 * @section Purpose
 *   The switch provides a way to get the SPI macphy interface, including clock source, clock divider, RX sample delay and port ID.
 *
 * @section Description
 *   This example shows how to get macphy SPI parameters. \n
 *   Including SPI clock source, SPI clock divider, SPI RX sample delay, and the port ID.
 */
RtkApiRet cva_exam_macphy_get(void)
{
    RtkApiRet    ret         = RT_ERR_OK;
    macphyInfo_t macphyParam = {0};
    UnitChip_t   unitChip    = {0};

    /*Configure the unit and chip ID*/
    unitChip.chip = CHIP_907XC_VA;
    unitChip.unit = CVA_UNIT;

    /* Step 1.  Get macphy SPI parameters*/
    /* Get SPI parameters*/
    ret = rtk_macphy_get_spi_param(unitChip, &macphyParam);

    /* Expected Behavior:
     *   Users can get the SPI settings.
     */

    return ret;
}

/**@}*/ /* RTL907CVA_MACPHY_GET_SPI_PARAM_EXAMPLE */
/**@}*/ /* RTL907CVA_MACPHY_EXAMPLE */
/**@}*/ /* MACPHY_EXAMPLE */
/**@}*/ /* EXAMPLE */
