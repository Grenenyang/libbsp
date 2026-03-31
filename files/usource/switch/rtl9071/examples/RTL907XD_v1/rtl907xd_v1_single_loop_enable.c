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
#include "rtkas_api_single_loop.h"
#include "rtkas_api_spi_flash.h"
#include "rtkas_api_reset.h"

/** @addtogroup EXAMPLE
 *  @{
 */

/** @defgroup SINGLE_LOOP_EXAMPLE Single Loop Example
 *  @{
 */

/**
 * @defgroup RTL907XDV1_SINGLE_LOOP_EXAMPLE RTL907XD_V1 Single Loop Example
 * @{
 */

/**
 * @defgroup RTL907XDV1_SINGLE_LOOP_BACKUP_EXAMPLE Single Loop Backup Switch Example
 *
 * @brief <b>Single Loop Backup Switch</b> \n
 *        When deploying Single Loop, it should contain 1 backup switch and others are normal switches. In this case, port 2 of\n
 *        the backup switch is set to forwarding and port 1 of the backup switch is set to blocking to prevent a loop.\n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of a backup switch in a Single Loop deployment.
 *
 * @section Description
 * This function configures a backup switch by updating the image, resetting the chip, and verifying the expected behavior.
 *
 * @image html sl/single_loop_backup_switch.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_single_loop_backup_switch(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_single_loop_backup_switch(void)
{
    RtkApiRet                  ret                         = RT_ERR_OK;
    uint32                     unit                        = DV1_UNIT;
    const int8                *fileName                    = "image/RTL907XD_v1/sl/sl_example_backup_switch.bin";
    UnitChip_t                 unitChip                    = {0u};
    SpiUpdate_t                spiUpdate                   = {0u};
    SlStateConfig_t            getSlStateGet               = {0u};
    SlDefaultPortStateConfig_t getSlDefaultPortStateConfig = {0u};
    SlPortStateConfig_t        slPortStateGet;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_V1;
    do
    {
        CHK_FUN_CONTINUE(ret, drv_memset(&slPortStateGet, 0, sizeof(slPortStateGet)));
        /** Step 1. Update image which contains the configurations as below.
         *         - Enable Single Loop.
         *         - Enable Single Loop auto recovery.
         *         - Set port 1 as control port A and set default state to blocking.
         *         - Set port 2 as control port B and set default state to forwarding.
         */
        spiUpdate.fileName = fileName;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_update(unitChip, &spiUpdate));
        /** Expected Behaviour: The image is updated successfully. */

        /** Step 2. Reset chip. */
        CHK_FUN_CONTINUE(ret, rtk_reset_chip_rst(unitChip));
        /* Expected Behaviour: The chip boots up with the configuration as below.
         *                     - Single Loop is enabled.
         *                     - Single Loop auto recovery is enabled.
         *                     - Port 1 is set as control port A and its default state is blocking.
         *                     - Port 2 is set as control port A and its default state is forwarding.
         */

        /** Step 3. Get Single Loop state and auto recovery state. */
        CHK_FUN_CONTINUE(ret, rtk_sl_state_get(unitChip, &getSlStateGet));
        /** Expected Behaviour: Get Single Loop state and auto recovery state are enabled. */

        /* Step 4. Get Single Loop control port number of A and B, and the control port default state of port A and B. */
        CHK_FUN_CONTINUE(ret, rtk_sl_default_port_state_get(unitChip, &getSlDefaultPortStateConfig));
        /** Expected Behaviour: Get port 1 is set as control port A and port 2 is set as control port B. Get default control
         *                     port state of port A is blocking and port B is forwarding.
         */

        /* Step 5. Get Single Loop operation port state of control port A and B. */
        CHK_FUN_CONTINUE(ret, rtk_sl_port_state_get(unitChip, &slPortStateGet));
        /** Expected Behaviour: Get Single Loop operation port state of control port A is blocking and port B is forwarding. */
    } while(0u == 1u);

    return ret;
}

/**@}*/ /* RTL907XDV1_SINGLE_LOOP_BACKUP_EXAMPLE */

/**
 * @defgroup RTL907XDV1_SINGLE_LOOP_NORMAL_EXAMPLE Single Loop Normal Switch Example
 *
 * @brief <b>Single Loop Normal Switch</b> \n
 *        When deploying Single Loop, it should contain 1 backup switch and others are normal switches. In this case, the ports for\n
 *        normal switches are set to forwarding. \n
 *
 * @section Purpose
 * The purpose of this function is to demonstrate the configuration of a normal switch in a Single Loop deployment.
 *
 * @section Description
 * This function configures a normal switch by updating the image, resetting the chip, and verifying the expected behavior.
 *
 * @image html sl/single_loop_normal_switch.jpg
 * @{
 */
/******************************************************************************
 * Function Prototype
 ******************************************************************************/
RtkApiRet dv1_single_loop_normal_switch(void);
/******************************************************************************
 * Function
 ******************************************************************************/
RtkApiRet dv1_single_loop_normal_switch(void)
{
    RtkApiRet                  ret                         = RT_ERR_OK;
    uint32                     unit                        = DV1_UNIT;
    const int8                *fileName                    = "image/RTL907XD_v1/sl/sl_example_normal_switch.bin";
    UnitChip_t                 unitChip                    = {0u};
    SpiUpdate_t                spiUpdate                   = {0u};
    SlStateConfig_t            getSlStateGet               = {0u};
    SlDefaultPortStateConfig_t getSlDefaultPortStateConfig = {0u};
    SlPortStateConfig_t        slPortStateGet;

    unitChip.unit = unit;
    unitChip.chip = CHIP_907XD_V1;
    do
    {
        CHK_FUN_CONTINUE(ret, drv_memset(&slPortStateGet, 0, sizeof(slPortStateGet)));
        /** Step 1. Update image which contains the configuration as below.
         *         - Enable Single Loop.
         *         - Enable Single Loop auto recovery.
         *         - Set port 1 as control port A and set default state to forwarding.
         *         - Set port 2 as control port B and set default state to forwarding.
         */
        spiUpdate.fileName = fileName;
        CHK_FUN_CONTINUE(ret, rtk_spiflash_update(unitChip, &spiUpdate));
        /* Expected Behaviour: The image is updated successfully. */

        /** Step 2. Reset chip. */
        CHK_FUN_CONTINUE(ret, rtk_reset_chip_rst(unitChip));
        /* Expected Behaviour: The chip boots up with the configurations as below.
         *                     - Single Loop is enabled.
         *                     - Single Loop auto recovery is enabled.
         *                     - Port 1 is set as control port A and its default state is forwarding.
         *                     - Port 2 is set as control port A and its default state is forwarding.
         */

        /** Step 3. Get Single Loop state and auto recovery state. */

        CHK_FUN_CONTINUE(ret, rtk_sl_state_get(unitChip, &getSlStateGet));
        /** Expected Behaviour: Get Single Loop state and auto recovery state are enabled. */

        /* Step 4. Get Single Loop control port number of A and B, and the control port default state of port A and B. */
        CHK_FUN_CONTINUE(ret, rtk_sl_default_port_state_get(unitChip, &getSlDefaultPortStateConfig));
        /** Expected Behaviour: Get port 1 is set as control port A and port 2 is set as control port B. Get default control
         *                     port state of port A is forwarding and port B is forwarding.
         */

        /** Step 5. Get Single Loop operation port state of control port A and B. */
        CHK_FUN_CONTINUE(ret, rtk_sl_port_state_get(unitChip, &slPortStateGet));
        /** Expected Behaviour: Get Single Loop operation port state of control port A is forwarding and port B is forwarding. */
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* RTL907XDV1_SINGLE_LOOP_NORMAL_EXAMPLE */
/**@}*/ /* RTL907XDV1_SINGLE_LOOP_EXAMPLE */
/**@}*/ /* SINGLE_LOOP_EXAMPLE */
/**@}*/ /* EXAMPLE */
