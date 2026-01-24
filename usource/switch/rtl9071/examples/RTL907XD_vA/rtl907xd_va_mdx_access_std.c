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
#include "rtkas_api_smi.h"

#if defined(RTL_907XD_VA)
#include "rtl907xd_va_reg_struct.h"
#endif

RtkApiRet dva_mdx_access_std(void);

/**
 * @addtogroup MDX_EXAMPLE
 * @{
 */
/**
 * @defgroup RTL907DVA_MDX_EXAMPLE RTL907XD_VA MDX Example
 * @{
 */
/**
 * @defgroup RTL907DVA_MDX_RW_EXAMPLE MDX Access Example
 * @brief Initialize the MDX interface and access the register of the slave PHY. \n
 *
 * @section Purpose
 * This demonstrates how to use the MDX APIs to access the slave PHY.
 *
 * @section Description
 * 1. Using rtk_std_mdx_init() to initialize the output clock and the preamble:
 *  - Select GPIOD pins to be the MDX interface;
 *  - Set the output clock to 2.5 MHz;
 *  - Set the length of the preamble to 32 bits;
 * 2. Using rtk_std_mdx_register_set() to set the register:
 *  - Set the ID of the slave PHY to 2;
 *  - Write to register 27 with the value of 0x1000u;
 * 3. Using rtk_std_mdx_register_get() to get the register:
 *  - Set the ID of the slave PHY to 2;
 *  - Read register 27;
 * 4. Check if the result of the comparison of these two values is equal.
 *
 * @{
 */
RtkApiRet dva_mdx_access_std(void)
{
    RtkApiRet        ret = RT_ERR_OK;
    UnitChip_t       uintChip;
    MdxInitSetInfo_t mdxInit;
    MdxRegInfo_t     mdxSet, mdxGet;
    uintChip.unit = DVA_UNIT;
    uintChip.chip = CHIP_907XD_VA;

    /* The target PHY */
    MdxSlvPhyID mdxID = (MdxSlvPhyID)0x2u;
    /* The target register */
    MdxSlvReg mdxReg = (MdxSlvReg)27u;

    do
    {
        /* Step 1. Select GPIOD pins as the MDX interface and set the clock to 2.5 MHz and the length of the preamble to 32 bits */
        mdxInit.gpioPin  = EM_MDX_MASTER_GPIOD_1_0;
        mdxInit.baudRate = (MdxBaudRate)5u;
        mdxInit.preamble = EM_MDX_PREAMBLE_32;
        CHK_FUN_CONTINUE(ret, rtk_std_mdx_init(uintChip, &mdxInit));
        /* Step 2. Write the value of 0x1000 to register 27 in PHY ID 2 */
        mdxSet.phyID   = mdxID;
        mdxSet.regNum  = mdxReg;
        mdxSet.mdxData = (MdxSlvData)0x1000u;
        CHK_FUN_CONTINUE(ret, rtk_std_mdx_register_set(uintChip, &mdxSet));
        /* Step 3. Read data from register 27 in PHY ID 2 */
        mdxGet.phyID  = mdxID;
        mdxGet.regNum = mdxReg;
        CHK_FUN_CONTINUE(ret, rtk_std_mdx_register_get(uintChip, &mdxGet));
        /* Expected Behavior:
         *   The external PHY register could be written and read.
         */
        if(mdxGet.mdxData != mdxSet.mdxData)
        {
            ret = -RT_ERR_PORT_MODE;
        }
    } while(0u == 1u);
    return ret;
}
/**@}*/ /* RTL907DVA_MDX_RW_EXAMPLE */
/**@}*/ /* RTL907DVA_MDX_EXAMPLE */
/**@}*/ /* MDX_EXAMPLE */