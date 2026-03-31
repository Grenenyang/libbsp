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
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_serdes.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_serdes.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_serdes.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xd_v1_api_ext.h"
#include "rtl907xd_v1_api_serdes.h"
#endif
#include "rtkas_api_serdes.h"

RtkApiRet dv1_sds_pcs_reg_rw(UnitChip_t unitChip);
RtkApiRet dv1_sds_wtg_interface_aphy_reg_rw(UnitChip_t unitChip);
RtkApiRet dv1_sds_wrap_interface_aphy_reg_rw(UnitChip_t unitChip);

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup SERDES_EXAMPLE Serdes Example
 * @{
 */

/**
 * @defgroup RTL907DV1_SERDES_EXAMPLE RTL907XD_V1 SerDes Example
 * @{
 */

/**
 * @addtogroup RTL907DV1_SERDES_ACCESS_PCS_EXAMPLE Access PCS register Example
 * @brief <b>Read/Write value to PCS register</b>
 *
 * @section Purpose
 *   Read/Write PCS register.
 *
 * @section Description
 *   This example shows how to use the APIs to set and get pcs register and the switch needs config Serdes on port 4 to port 12 by GUI tool.
 * @{
 */
RtkApiRet dv1_sds_pcs_reg_rw(UnitChip_t unitChip)
{
    RtkApiRet            retVal = RT_ERR_OK;
    RtkPort              port   = 0u;
    SysSdsPcsRegConfig_t setPcsParameter;
    SysSdsPcsRegConfig_t getPcsParameter;

    do
    {
        setPcsParameter.page     = 0u;
        setPcsParameter.reg      = 2u;
        setPcsParameter.startBit = 0u;
        setPcsParameter.endBit   = 15u;
        setPcsParameter.value    = 0x7100u;

        getPcsParameter.page     = 0u;
        getPcsParameter.reg      = 2u;
        getPcsParameter.startBit = 0u;
        getPcsParameter.endBit   = 15u;

        for(port = DV1_RTK_USER_PORT_4TH; port <= DV1_RTK_USER_PORT_12TH; port++)
        {
            setPcsParameter.port = port;
            getPcsParameter.port = port;

            /* Step1: Write the value 0x7100 to the each serdes PCS port's page 0, reg 2, bit 0 to bit 15 */
            CHK_FUN_CONTINUE(retVal, rtk_sys_sds_reg_setbits_pcs(unitChip, &setPcsParameter));
            /* Step2: Read the each serdes port PCS register page 0, reg 2, bit 0 to 15 */
            CHK_FUN_CONTINUE(retVal, rtk_sys_sds_reg_getbits_pcs(unitChip, &getPcsParameter));

            if(getPcsParameter.value != 0x7100u)
            {
                retVal = -RT_ERR_FAILED;
                break;
            }
        }
        /*  Expected Behaviour:
         *	Value of PCS register page 0, reg 0, bit 0 to 15 shall be = 0x1413.
         */
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RTL907DV1_SERDES_ACCESS_PCS_EXAMPLE */

/**
 * @addtogroup RTL907DV1_SERDES_ACCESS_WTG_PORT_APHY_EXAMPLE Access APHY register on WTG interface Example
 * @brief <b>Read/Write value to APHY register on WTG interface</b>
 *
 * @section Purpose
 *   Read/Write APHY register on WTG interface.
 *
 * @section Description
 *   This example shows how to use the APIs to set and get PHY register on WTG interface and the switch needs config Serdes on port 9 to port 12 by GUI tool.
 * @{
 */
RtkApiRet dv1_sds_wtg_interface_aphy_reg_rw(UnitChip_t unitChip)
{
    RtkApiRet             retVal = RT_ERR_OK;
    RtkPort               port   = 0u;
    SysSdsAphyRegConfig_t setAphyParameter;
    SysSdsAphyRegConfig_t getAphyParameter;

    do
    {
        setAphyParameter.page     = 0u;
        setAphyParameter.reg      = 0u;
        setAphyParameter.startBit = 0u;
        setAphyParameter.endBit   = 15u;
        setAphyParameter.value    = 0x8000u;

        getAphyParameter.page     = 0u;
        getAphyParameter.reg      = 0u;
        getAphyParameter.startBit = 0u;
        getAphyParameter.endBit   = 15u;

        for(port = DV1_RTK_USER_PORT_9TH; port <= DV1_RTK_USER_PORT_12TH; port++)
        {
            setAphyParameter.port = port;
            getAphyParameter.port = port;

            /* Step1: Write the value 0x8000 to the WTG serdes APHY port's register 0x0, bit 0 to bit 15 */
            CHK_FUN_CONTINUE(retVal, rtk_sys_sds_reg_setbits_aphy(unitChip, &setAphyParameter));
            /* Step2; Read the WTG serdes port APHY register reg 0x0, bit 0 to 15 */
            CHK_FUN_CONTINUE(retVal, rtk_sys_sds_reg_getbits_aphy(unitChip, &getAphyParameter));

            if(getAphyParameter.value != 0x8000u)
            {
                retVal = -RT_ERR_FAILED;
                break;
            }
        }
        /*  Expected Behaviour:
         *	Value of APHY register page 0, reg 0x0, bit 0 to 15 shall be = 0x8000.
         */
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RTL907DV1_SERDES_ACCESS_WTG_PORT_APHY_EXAMPLE */

/**
 * @addtogroup RTL907DV1_SERDES_ACCESS_WRAP_PORT_APHY_EXAMPLE Access APHY register on WRAP interface Example
 * @brief <b>Read/Write value to APHY register on WRAP interface</b>
 *
 * @section Purpose
 *   Read/Write APHY register on WRAP interface.
 *
 * @section Description
 *   This example shows how to use the APIs to set and get PHY register on WRAP interface and the switch needs config Serdes on port 4 to port 8 by GUI tool.
 * @{
 */
RtkApiRet dv1_sds_wrap_interface_aphy_reg_rw(UnitChip_t unitChip)
{
    RtkApiRet             retVal = RT_ERR_OK;
    RtkPort               port   = 0u;
    SysSdsAphyRegConfig_t setAphyParameter;
    SysSdsAphyRegConfig_t getAphyParameter;

    do
    {
        setAphyParameter.page     = 0u;
        setAphyParameter.reg      = 0xau;
        setAphyParameter.startBit = 0u;
        setAphyParameter.endBit   = 15u;
        setAphyParameter.value    = 0x8000u;

        getAphyParameter.page     = 0u;
        getAphyParameter.reg      = 0xau;
        getAphyParameter.startBit = 0u;
        getAphyParameter.endBit   = 15u;

        for(port = DV1_RTK_USER_PORT_4TH; port <= DV1_RTK_USER_PORT_8TH; port++)
        {
            setAphyParameter.port = port;
            getAphyParameter.port = port;

            /* Step1: Write the value 0x8000 to the WRAP serdes APHY port's register 0xA, bit 0 to bit 15 */
            CHK_FUN_CONTINUE(retVal, rtk_sys_sds_reg_setbits_aphy(unitChip, &setAphyParameter));
            /* Step2: Read the WRAP serdes port APHY register reg 0xA, bit 0 to 15 */
            CHK_FUN_CONTINUE(retVal, rtk_sys_sds_reg_getbits_aphy(unitChip, &getAphyParameter));

            if(getAphyParameter.value != 0x8000u)
            {
                retVal = -RT_ERR_FAILED;
                break;
            }
        }
        /*  Expected Behaviour:
         *	Value of APHY register page 0, reg 0xA, bit 0 to 15 shall be = 0x8000.
         */
    } while(0u == 1u);

    return retVal;
}
/**@}*/ /* RTL907DV1_SERDES_ACCESS_WRAP_PORT_APHY_EXAMPLE */
/**@}*/ /* RTL907DV1_SERDES_EXAMPLE */
/**@}*/ /* SERDES_EXAMPLE */
/**@}*/ /* EXAMPLE */