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
#include "rtkas_api_port_isolation.h"
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
 * @defgroup RTK_MODULE_PORT_ISOLATION Port Isolation
 * @brief Port isolation APIs. This component is for port isolation.
 * @{
 */
/**
 * @defgroup PIA Port Isolation Assignment
 * @brief Port isolation APIs
 * @{
 */

/**
 * @brief Set isolated port mask.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in] portIsoInfo Port isolation set information (N/A). \n
 *                        Parameters required to configure port isolation.
 * @param[out] None
 * @return RT_ERR_OK           The isolated port was configured.
 * @return -RT_ERR_NOT_SUPPORT Not supported.
 * @return Others              Please refer to dv1_port_isolation_set(), dva_port_isolation_set(), \n
 *                             cvb_port_isolation_set(), or cva_port_isolation_set().
 * @note This API sets the blocked forwarding port mask of the specific port. \n
 *       A port cannot forward packets to those ports included in its isolated port mask.
 */
RtkApiRet rtk_port_isolation_set(UnitChip_t unitChip, PortIsoInfo_t *portIsoInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == portIsoInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_port_isolation_set(unitChip.unit, portIsoInfo->port, portIsoInfo->portmask);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_isolation_set(unitChip.unit, portIsoInfo->port, portIsoInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_port_isolation_set(unitChip.unit, portIsoInfo->port, portIsoInfo->portmask);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_port_isolation_set(unitChip.unit, portIsoInfo->port, portIsoInfo->portmask);
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
 * @brief Get isolated port mask.
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 * @param[in] unitChip Stacking unit and chip ID (N/A) \n
 *                     The stacking unit ID indicates which switch to access. \n
 *                     The chip ID indicates the chip model.
 * @param[in,out] portIsoInfo Port isolation get information (N/A). \n
 *                            Parameters required to obtain port isolation mask.
 * @param[out] None
 * @return RT_ERR_OK           The isolated port mask was obtained.
 * @return -RT_ERR_NOT_SUPPORT Not supported.
 * @return Others              Please refer to dv1_port_isolation_get(), dva_port_isolation_get(), \n
 *                             cvb_port_isolation_get(), or cva_port_isolation_get().
 * @note This API gets the blocked forwarding port mask of the specific port. \n
 *       A port cannot forward packets to those ports included in its isolated port mask.
 */
RtkApiRet rtk_port_isolation_get(UnitChip_t unitChip, PortIsoInfo_t *portIsoInfo)
{
    RtkApiRet retVal = (RtkApiRet)RT_ERR_OK;

    if(NULL == portIsoInfo)
    {
        retVal = (RtkApiRet)(-RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                retVal = dv1_port_isolation_get(unitChip.unit, portIsoInfo->port, &(portIsoInfo->portmask));
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                retVal = dva_port_isolation_get(unitChip.unit, portIsoInfo->port, &(portIsoInfo->portmask));
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                retVal = cvb_port_isolation_get(unitChip.unit, portIsoInfo->port, &(portIsoInfo->portmask));
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                retVal = cva_port_isolation_get(unitChip.unit, portIsoInfo->port, &(portIsoInfo->portmask));
                break;
#endif
            default:
                retVal = (RtkApiRet)(-RT_ERR_NOT_SUPPORT);
                break;
        }
    }

    return retVal;
}
/**@}*/ /* PIA */
/**@}*/ /* RTK_MODULE_PORT_ISOLATION */
