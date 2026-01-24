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

#include "rtkas_types.h"
#include "rtkas_system_porting.h"
#include "rtl907xc_vb_asicdrv_stacking.h"
#include "rtkas_common_types.h"
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtl907xc_vb_api_stacking.h"
#include "rtkas_api_stacking.h"

/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @addtogroup STK_EXAMPLE Stacking Example
 * @{
 */

/**
 * @addtogroup RTL907CVB_STACKING_EXAMPLE RTL907XC_VB Stacking Example
 * @{
 */

/**
 * @addtogroup RTL907CVB_STACKING_DYNAMIC_EXAMPLE Stacking Dynamic Setup Example
 * @brief <b>Dynamic Stacking Construct</b>
 *
 * @section Purpose
 *   Construct stacking in dynamic mode.
 *
 * @section Description
 *   This example constructs stacking in dynamic mode. \n
 *   The example connects 2 DUTs with different I2C IDs to the stacking topology. \n
 *   The example checks the stacking construction result after finishing constructing stacking.
 *
 * @image html stacking/stacking_init.jpg
 *
 * @{
 */
RtkApiRet cvb_stacking_dynamic_setup(uint32 unit);
RtkApiRet cvb_stacking_dynamic_setup(uint32 unit)
{
    RtkApiRet                 retVal  = RT_ERR_OK;
    uint32                    timeout = 0u;
    UnitChip_t                unitChip;
    StackingDevsearch_t       stackingDevsearch;
    StackingAutoConfig_t      stackingAutoConfig;
    StackingGetStatus_t       stackingGetStatus;
    StackingGetMasterId_t     stackingGetMasterId;
    StackingGetGroupId_t      stackingGetGroupId;
    StackingGetUnitmap_t      stackingGetUnitmap;
    StackingCheckConnection_t stackingCheckConnection;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XC_VB;

    /* Step 1. Disable and Enable stacking */
    retVal = rtk_stacking_disable(unitChip);
    CHK_FUN_RET(retVal, rtk_stacking_enable(unitChip));

    /* Step 2. Search devices that are stackable, expect to find 1 0n S0 and 0 on S1 */
    stackingDevsearch.numOnS0 = 0;
    stackingDevsearch.numOnS1 = 0;
    CHK_FUN_RET(retVal, rtk_stacking_devsearch(unitChip, &stackingDevsearch));

    /* Step 3. Waiting for stacking neighbor response */
    while((timeout < cvb_g_stackingTimeout) && (retVal == RT_ERR_OK))
    {
        if((stackingDevsearch.numOnS0 + stackingDevsearch.numOnS1) == 0u)
        {
            CHK_FUN_CONTINUE(retVal, rtk_stacking_devsearch(unitChip, &stackingDevsearch));
            CHK_FUN_CONTINUE(retVal, rtk_usecond_delay(1));
            timeout++;
        }
        else
        {
            break;
        }
    }

    /* Step 4. Auto configuration: Select 1 switch from P10 to construct stacking */
    stackingAutoConfig.selOnS0 = stackingDevsearch.numOnS0;
    stackingAutoConfig.selOnS1 = 0u;
    CHK_FUN_RET(retVal, rtk_stacking_auto_config(unitChip, &stackingAutoConfig));
    /* Expected Behaviour: The stacking construct success */

    /* Step 5 Check stacking data */
    CHK_FUN_RET(retVal, rtk_stacking_get_status(unitChip, &stackingGetStatus));
    CHK_FUN_RET(retVal, rtk_stacking_get_master_id(unitChip, &stackingGetMasterId));
    CHK_FUN_RET(retVal, rtk_stacking_get_group_id(unitChip, &stackingGetGroupId));
    CHK_FUN_RET(retVal, rtk_stacking_get_unitmap(unitChip, &stackingGetUnitmap));
    CHK_FUN_RET(retVal, rtk_stacking_check_connection(unitChip, &stackingCheckConnection));
    /* Expected Behaviour: The data is the same with the topology */

    return retVal;
}

/**@}*/ /* RTL907CVB_STACKING_DYNAMIC_EXAMPLE */
/**@}*/ /* RTL907CVB_STACKING_EXAMPLE */
/**@}*/ /* STK_EXAMPLE */
/**@}*/ /* EXAMPLE */
