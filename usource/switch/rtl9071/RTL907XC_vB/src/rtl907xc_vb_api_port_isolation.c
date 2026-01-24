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

#include "rtl907xc_vb_asicdrv_port.h"
#include "rtl907xc_vb_asicdrv.h"
#include "rtl907xc_vb_api_ext.h"

/**
 * @addtogroup RTK_MODULE_PORT_ISOLATION Port Isolation
 * @{
 */
/**
 * @addtogroup PIA Port Isolation Assignment
 * @{
 */
/**
 * @addtogroup PIA_ASIC Low Level Driver
 * @{
 */

/**
 * @brief Set isolated port mask
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port        Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                        The user port index.
 * @param[in] usrPmap     User port mask (0 to CVB_RTK_USER_PMAP_ALL) \n
 *                        The isolated port mask.
 * @param[out] None
 * @retval RT_ERR_OK          The isolated port was configured.
 * @retval -RT_ERR_PORT_ID    The port was out of range.
 * @retval Others             Please refer to cvb_drv_port_iso_mask_set() and cvb_drv_pmap_convert() in Interface AppNote.
 * @note This API sets the blocked forwarding port mask of the specific port. \n
 *       A port cannot forward packets to those ports included in its isolated port mask.
 */
RtkApiRet cvb_port_isolation_set(uint32 unit, RtkPort port, RtkPmap usrPmap)
{
    RtkApiRet ret     = RT_ERR_OK;
    RtkPort   sysPort = 0u;
    RtkPmap   sysPmap = 0u;

    do
    {
        if(CVB_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }
        sysPort = (RtkPort)cvb_g_userPort2sysPort[port];

        if(0xFFu == sysPort)
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(usrPmap, &sysPmap, CVB_EM_USER_TO_SYS));
        CHK_FUN_CONTINUE(ret, cvb_drv_port_iso_mask_set(unit, (uint32)sysPort, (sysPmap | ((uint32)1u << sysPort)) & CVB_RTK_MAX_PORT_MASK));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief Get isolated port mask
 * @param[in] unit          Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] port          Port index (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END) \n
 *                          The user port index.
 * @param[out] pPortmask    Port mask (0 to CVB_RTK_USER_PMAP_ALL) \n
 *                          The isolated port mask.
 * @return RT_ERR_OK               The isolated port was obtained.
 * @return -RT_ERR_PORT_ID         The port was out of range.
 * @return -RT_ERR_NULL_POINTER    One of the structure pointers was NULL.
 * @return Others                  Please refer to cvb_drv_port_iso_mask_get() and cvb_drv_pmap_convert() in Interface AppNote.
 * @note This API gets the blocked forwarding port mask of the specific port. \n
 *       A port cannot forward packets to those ports included in its isolated port mask.
 */
RtkApiRet cvb_port_isolation_get(uint32 unit, RtkPort port, RtkPmap *pUsrPmap)
{
    RtkApiRet ret;
    RtkPort   sysPort = 0u;
    RtkPmap   usrPmap = 0u;
    RtkPmap   sysPmap = 0u;

    do
    {

        if(CVB_IS_VALID_PORT_ID(port) == 0u)
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }
        sysPort = (RtkPort)cvb_g_userPort2sysPort[port];

        if(0xFFu == sysPort)
        {
            ret = -(RtkApiRet)RT_ERR_PORT_ID;
            continue;
        }

        if(NULL == pUsrPmap)
        {
            ret = -(RtkApiRet)RT_ERR_NULL_POINTER;
            continue;
        }

        CHK_FUN_CONTINUE(ret, cvb_drv_port_iso_mask_get(unit, sysPort, &sysPmap));
        CHK_FUN_CONTINUE(ret, cvb_drv_pmap_convert(sysPmap, &usrPmap, CVB_EM_SYS_TO_USER));

        usrPmap &= (~((uint32)1u << port)) & CVB_RTK_MAX_PORT_MASK;
        *pUsrPmap = usrPmap;
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* PIA_ASIC */
/**@}*/ /* PIA */
/**@}*/ /* RTK_MODULE_PORT_ISOLATION */
