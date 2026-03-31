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

/**
 * @defgroup RTK_MODULE_MACPHY MACPHY
 * @brief MACPHY APIs
 * @{
 */

/**
 * @defgroup MACPHY_GET Get MACPHY SPI parameter
 * @brief Get MACPHY SPI parameter
 * @{
 */

/**
 * @brief This API gets the MACPHY SPI configuration.
 *        Supported chips: RTL_907XD_VA/RTL_907XD_V1/RTL_907XC_VB/RTL_907XC_VA
 *
 * @param[in] unitChip              Stacking unit and chip ID (N/A) \n
 *                                  Stacking unit ID is used for indicating which switch is to be accessed.
 *                                  Chip ID is used for the chip model.
 * @param[out] macphyInfo           MACPHY information (N/A) \n
 *                                  The parameters required to obtain MACPHY SPI.
 * @retval RT_ERR_OK                The settings of MACPHY were obtained successfully. \n
 * @retval -RT_ERR_NOT_SUPPORT      Not supported. \n
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others                   Please refer to dv1_macphy_get_spi_param(), dva_macphy_get_spi_param(), cvb_macphy_get_spi_param(), and cva_macphy_get_spi_param()
 */
RtkApiRet rtk_macphy_get_spi_param(UnitChip_t unitChip, macphyInfo_t *macphyInfo)
{
    RtkApiRet ret = RT_ERR_OK;
    if(NULL == macphyInfo)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_macphy_get_spi_param(unitChip.unit, macphyInfo->portId, &macphyInfo->param);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_macphy_get_spi_param(unitChip.unit, macphyInfo->portId, &macphyInfo->param);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_macphy_get_spi_param(unitChip.unit, macphyInfo->portId, &macphyInfo->param);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_macphy_get_spi_param(unitChip.unit, macphyInfo->portId, &macphyInfo->param);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}
/**@}*/ /*MACPHY_GET*/

/**@}*/ /* MACPHY */
