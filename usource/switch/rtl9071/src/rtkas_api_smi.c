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
#include "rtkas_api_smi.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ext.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ext.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ext.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ext.h"
#endif

/**
 * @defgroup RTK_MODULE_SMI_MASTER MDX (SMI Master)
 * @brief These APIs are used for the initialization of the MDX interface and for accessing slave devices.
 * @{
 */
/**
 * @defgroup MDX_INIT MDX Initialization
 * @{
 */

/**
 * @brief This API is used for the MDX initialization, \n
 *        which includes GPIO pins assignment (RTL_907XD_V1/RTL_907XD_VA), and \n
 *        baud rate/preamble settings (RTL_907XD_V1/RTL_907XD_VA/RTL_907XC_VB/RTL_907XC_VA).
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in] mdxInitSetInfo The parameter information for the initialization of MDX (N/A). \n
 *                           Parameters required to initialize the MDX interface.
 * @param[out] None
 * @return RT_ERR_OK            The MDX interface was initialized.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  The chip is not supported.
 * @return Others               Please refer to dv1_std_mdx_init(), dva_std_mdx_init(), \n
 *                              cvb_std_mdx_init(), or cva_std_mdx_init().
 */
RtkApiRet rtk_std_mdx_init(UnitChip_t unitChip, MdxInitSetInfo_t *mdxInitSetInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == mdxInitSetInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_std_mdx_init(unitChip.unit, mdxInitSetInfo->gpioPin, mdxInitSetInfo->baudRate, mdxInitSetInfo->preamble);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_std_mdx_init(unitChip.unit, mdxInitSetInfo->gpioPin, mdxInitSetInfo->baudRate, mdxInitSetInfo->preamble);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_std_mdx_init(unitChip.unit, mdxInitSetInfo->baudRate, mdxInitSetInfo->preamble);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_std_mdx_init(unitChip.unit, mdxInitSetInfo->baudRate, mdxInitSetInfo->preamble);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* MDX_INIT */

/**
 * @defgroup MDX_ACCESS MDX Access
 * @brief APIs used by MDX to access slave devices.
 * @{
 */

/**
 * @brief This API is used to read registers of slave devices.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in] mdxRegInfo The parameter information for MDX read access (N/A). \n
 *                       Parameters required to obtain the target register.
 * @return RT_ERR_OK            The value of the register was obtained.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  The chip is not supported.
 * @return Others               Please refer to dv1_std_mdx_register_get(), dva_std_mdx_register_get(), \n
 *                              cvb_std_mdx_register_get(), or cva_std_mdx_register_get().
 */
RtkApiRet rtk_std_mdx_register_get(UnitChip_t unitChip, MdxRegInfo_t *mdxRegInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == mdxRegInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_std_mdx_register_get(unitChip.unit, mdxRegInfo->phyID, mdxRegInfo->regNum, &(mdxRegInfo->mdxData));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_std_mdx_register_get(unitChip.unit, mdxRegInfo->phyID, mdxRegInfo->regNum, &(mdxRegInfo->mdxData));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_std_mdx_register_get(unitChip.unit, mdxRegInfo->phyID, mdxRegInfo->regNum, &(mdxRegInfo->mdxData));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_std_mdx_register_get(unitChip.unit, mdxRegInfo->phyID, mdxRegInfo->regNum, &(mdxRegInfo->mdxData));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}

/**
 * @brief This API is used to write registers of slave devices.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in] mdxRegInfo The parameter information for MDX write access (N/A). \n
 *                       Parameters required to set the target register.
 * @return RT_ERR_OK            The target register was set.
 * @return -RT_ERR_NULL_POINTER The pointer is null.
 * @return -RT_ERR_NOT_SUPPORT  The chip is not supported.
 * @return Others               Please refer to dv1_std_mdx_register_set(), dva_std_mdx_register_set(), \n
 *                              cvb_std_mdx_register_set(), or cva_std_mdx_register_set().
 */
RtkApiRet rtk_std_mdx_register_set(UnitChip_t unitChip, MdxRegInfo_t *mdxRegInfo)
{
    RtkApiRet retVal = (int32)RT_ERR_OK;

    if(NULL == mdxRegInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_std_mdx_register_set(unitChip.unit, mdxRegInfo->phyID, mdxRegInfo->regNum, mdxRegInfo->mdxData);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_std_mdx_register_set(unitChip.unit, mdxRegInfo->phyID, mdxRegInfo->regNum, mdxRegInfo->mdxData);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_std_mdx_register_set(unitChip.unit, mdxRegInfo->phyID, mdxRegInfo->regNum, mdxRegInfo->mdxData);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_std_mdx_register_set(unitChip.unit, mdxRegInfo->phyID, mdxRegInfo->regNum, mdxRegInfo->mdxData);
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* MDX_ACCESS */
/**@}*/ /* RTK_MODULE_SMI_MASTER */
