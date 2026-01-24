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

#include "rtkas_api_ext.h"
#include "rtkas_api_mib.h"

/** @addtogroup EXAMPLE
 *  @{
 */

/** @addtogroup MIB_EXAMPLE MIB Example
 *  @{
 */

/**
 * @addtogroup RTL907XDV1_MIB_EXAMPLE RTL907XD_V1 MIB Example
 * @{
 */

/**
 * @addtogroup RTL907XDV1_MIB_ALL_ENABLE_EXAMPLE Enable All Ports MIB Counters Example
 * @{
 *
 * @brief <b>Enable All Ports MIB Counters</b> \n
 *        Start all port MIB counters on all ports.\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the enabling of MIB counters on all ports.
 *
 * @section Description
 * This function starts all port MIB counters on all ports and the CPU port, and verifies the expected behavior.
 *
 * @image html mib/mib_all_ports_start.jpg
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_mib_all_port_start(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_mib_all_port_start(void)
{
    RtkApiRet  retVal   = RT_ERR_OK;
    uint32     unit     = DV1_UNIT;
    UnitChip_t unitChip = {0u};

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_V1;

    /* Step 1. Start all port MIB counters on all ports and CPU port */
    CHK_FUN_RET(retVal, rtk_stat_start_all(unitChip));
    /* Expected Behaviour: All port MIB counters on all ports and CPU port will start counting */

    return retVal;
}
/**@}*/ /* RTL907XDV1_MIB_ALL_ENABLE_EXAMPLE */
/**@}*/ /* RTL907XDV1_MIB_EXAMPLE */
/**@}*/ /* MIB_EXAMPLE */
/**@}*/ /* EXAMPLE */
