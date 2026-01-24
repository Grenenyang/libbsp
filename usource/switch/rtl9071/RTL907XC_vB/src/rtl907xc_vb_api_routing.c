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

#include "rtl907xc_vb_asicdrv_routing.h"
#include "rtl907xc_vb_asicdrv_config.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
#include "rtl907xc_vb_asicdrv_lut.h"
#include "rtkas_api_routing.h"
#include "rtkas_timer.h"

/**
 * @addtogroup RTK_MODULE_ROUTING
 * @{
 */

/**
 * @addtogroup RTK_MODULE_ROUTING_GLB_CTRL
 * @{
 */

/**
 * @defgroup RTK_MODULE_ROUTING_GLB_CTRL_DRV Low Level Driver
 * @brief Routing Low Level Driver.
 * @{
 */

/**
 * @brief After enabling or disabling routing, initialize the routing process.
 *
 * @param[in] unit          Stacking unit (1 to 7) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] enable        Routing initial process trigger (DISABLED to ENABLED) \n
 *                          To enable/disable initial process start trigger.
 * @param[out] None
 * @retval RT_ERR_OK                Set the initializing process successfully.
 * @retval -RT_ERR_INPUT            The value of "enable" is invalid.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT Routing init setting timeout.
 * @retval Others                   Please refer to cvb_reg_bit_set(), cvb_drv_rt_route_mode_set(), cvb_drv_indirect_sram_1byte_set(), cvb_drv_rt_glb_reg_cfg_on(), cvb_drv_rt_glb_reg_cfg_off(), cvb_drv_indirect_reg_field_set(), rtk_usecond_delay().
 */
RtkApiRet cvb_route_init_process_trigger(uint32 unit, uint8 enable)
{
    RtkApiRet     ret      = RT_ERR_OK;
    uint32        regVal   = 0u;
    uint32        tryTimes = 0u;
    RtRouteMode_e rtMode   = EM_ROUTE_STATIC_MODE;

    do
    {
        if((ENABLED == enable) || (DISABLED == enable))
        {
            if(ENABLED == enable)
            {
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_route_mode_set(unit, &rtMode));
                CHK_FUN_CONTINUE(ret, cvb_drv_indirect_sram_1byte_set(unit, CVB_GLBCFG_RT_FIR_MODE_ADDR, (uint8)rtMode));
                /* Set global register */
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_glb_reg_cfg_on(unit));
            }
            else
            {
                CHK_FUN_CONTINUE(ret, cvb_drv_rt_glb_reg_cfg_off(unit));
            }

            /* Set the initializing process trigger bit */
            CHK_FUN_CONTINUE(ret, cvb_drv_indirect_reg_field_set(unit, CVB_HW_MISC_CTRL00, CVB_HWCTRL_ROUTING_EN_OFFSET, CVB_HWCTRL_ROUTING_EN_LEN, enable));

            do
            {
                /* Trigger routing init action. It will self-clear when the operation is finished */
                CHK_FUN_RET(ret, cvb_reg_bit_get(unit, CVB_HW_MISC_CTRL00, CVB_HWCTRL_ROUTING_EN_OFFSET, &regVal));
                CHK_FUN_RET(ret, rtk_usecond_delay(1000u));

                /* Check return value */
                if(RT_ERR_OK == ret)
                {
                    tryTimes++;
                    if(tryTimes > RT_INIT_TIMEOUT)
                    {
                        ret = -RT_ERR_BUSYWAIT_TIMEOUT;
                    }
                }

                if(RT_ERR_OK != ret)
                {
                    break;
                }
            } while(1u == regVal);
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }
    } while(1 == 0);

    return ret;
}

/**
 * @brief Enable/disable routing
 *
 * @param[in] unit          Stacking unit (1 to 7) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] disable       Software routing global control (DISABLED to ENABLED) \n
 *                          Turn ON/OFF the software routing.
 * @param[out] None
 * @retval RT_ERR_OK        Set the routing successfully.
 * @retval -RT_ERR_INPUT    The value of "disable" is invalid.
 * @retval Others           Please refer to cvb_drv_indirect_sram_1byte_set().
 */
RtkApiRet cvb_route_disable(uint32 unit, uint8 disable)
{
    RtkApiRet ret = RT_ERR_OK;

    if((ENABLED == disable) || (DISABLED == disable))
    {
        /* Set the global routing disable status */
        ret = cvb_drv_indirect_sram_1byte_set(unit, CVB_GLBCFG_RT_DISABLE_ADDR, disable);
    }
    else
    {
        ret = -RT_ERR_INPUT;
    }

    return ret;
}

/**
 * @brief Enable/Disable range check
 *
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] enable     Enable/Disable range check (DISABLED to ENABLED) \n
 *                       To enable/disable range check function.
 * @param[out] None
 * @retval RT_ERR_OK     Enable/Disable range check successfully.
 * @retval -RT_ERR_INPUT The value of "enable" is invalid.
 * @retval Others        Please refer to cvb_reg_bit_set().
 */
RtkApiRet cvb_route_rng_chk_set(uint32 unit, uint8 enable)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if((ENABLED != enable) && (DISABLED != enable))
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        /* Set range check */
        ret = cvb_reg_bit_set(unit, CVB_L3_GLOBAL_CONTROL0, CVB_L3_GLOBAL_CONTROL0_RNAGE_CHECK_EN_LSP, enable);
    } while(1 == 0);

    return ret;
}

/**
 * @brief Flush routing tables
 *
 * @param[in] unit   Stacking unit (1 to 7) \n
 *                   Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK Flush routing tables successfully.
 * @retval -RT_ERR_BUSYWAIT_TIMEOUT table flush timeout.
 * @retval Others    Please refer to cvb_reg_write(), cvb_drv_rt_hosttbl_clear(), cvb_drv_rt_sw_intf_clear(), cvb_drv_rt_rttbl_clear(), cvb_drv_rt_nhpool_clear().
 */
RtkApiRet cvb_route_tbl_flush(uint32 unit)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    val     = 0u;
    uint32    timeout = 0u;

    /* Clean original HW tables */
    /* OIL table flush will consume at least 65535ns and we already indicate the index, so we don't flush OIL */
    val = (ENABLED << CVB_L3_TABLE_CLEAR_CTRL_CLEAR_IGRIF_TBL_LSP) | (ENABLED << CVB_L3_TABLE_CLEAR_CTRL_CLEAR_NET_TBL_LSP) | (ENABLED << CVB_L3_TABLE_CLEAR_CTRL_CLEAR_RH_TBL_LSP) | (ENABLED << CVB_L3_TABLE_CLEAR_CTRL_CLEAR_MCCOL_TBL_LSP);
    ret = cvb_reg_write(unit, CVB_L3_TABLE_CLEAR_CTRL, val);
    /* Clear host table by software */
    CHK_FUN_RET(ret, cvb_drv_rt_hosttbl_clear(unit));
    /* Clear software tables */
    CHK_FUN_RET(ret, cvb_drv_rt_sw_intf_clear(unit));
    CHK_FUN_RET(ret, cvb_drv_rt_rttbl_clear(unit));
    CHK_FUN_RET(ret, cvb_drv_rt_nhpool_clear(unit));

    /* Check if HW table flush is done */
    do
    {
        CHK_FUN_CONTINUE(ret, cvb_reg_read(unit, CVB_L3_TABLE_CLEAR_CTRL, &val));
        CHK_FUN_CONTINUE(ret, rtk_usecond_delay(10u));
        timeout++;
    } while((0u != val) && (timeout < CVB_TBL_CLEAR_POLLING_TIME));

    if(CVB_TBL_CLEAR_POLLING_TIME == timeout)
    {
        ret = -RT_ERR_BUSYWAIT_TIMEOUT;
    }

    return ret;
}

/**@}*/ /* RTK_MODULE_ROUTING_GLB_CTRL_DRV */
/**@}*/ /* RTK_MODULE_ROUTING_GLB_CTRL */

/**
 * @addtogroup RTK_MODULE_ROUTING_TBL_INTF
 * @{
 */

/**
 * @defgroup RTK_MODULE_ROUTING_TBL_INTF_DRV Low Level Driver
 * @brief Routing Low Level Driver.
 * @{
 */

/**
 * @brief Set routing interface table
 *
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] intfCfg Interface configuration (N/A) \n
 *                    To configure the interface entry.
 * @param[out] None
 * @retval RT_ERR_OK Set interface entry successfully.
 * @retval Others Please refer to cvb_drv_rt_intf_param_chk_runtime(), cvb_drv_rt_hw_igr_intf_set_runtime(), cvb_drv_rt_hw_egr_intf_set(), cvb_drv_rt_sw_intf_set().
 */
RtkApiRet cvb_route_intf_set(uint32 unit, RtIntfCfg_t *intfCfg)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check parameters */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_intf_param_chk_runtime(intfCfg));
        /* Change user port to system port */
        intfCfg->intfEntry.portId = cvb_g_userPort2sysPort[intfCfg->intfEntry.portId];
        /* Set hardware ingress interface */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_hw_igr_intf_set_runtime(unit, intfCfg));
        /* Set hardware egress interface */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_hw_egr_intf_set(unit, &intfCfg->intfEntry));
        /* Set software interface */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_sw_intf_set(unit, &intfCfg->intfEntry));
    } while(1 == 0);

    return ret;
}

/**
 * @brief Get routing interface table
 *
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] intfCfg Interface configuration (N/A) \n
 *                        To get the interface entry.
 * @retval RT_ERR_OK Get interface entry successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of intfCfg is NULL.
 * @retval -RT_ERR_ENTRY_INDEX The interface entry index is out of range.
 * @retval Others Please refer to cvb_drv_rt_sw_intf_get(), cvb_drv_rt_hw_igr_intf_get(), cvb_drv_rt_hw_egr_intf_get().
 */
RtkApiRet cvb_route_intf_get(uint32 unit, RtIntfCfg_t *intfCfg)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check parameters */
        if(NULL == intfCfg)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(CVB_RT_INTF_SIZE <= intfCfg->intfEntry.index)
        {
            ret = -RT_ERR_ENTRY_INDEX;
            continue;
        }

        /* Change user port to system port */
        intfCfg->intfEntry.portId = cvb_g_userPort2sysPort[intfCfg->intfEntry.portId];
        /* Get software interface */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_sw_intf_get(unit, &intfCfg->intfEntry));
        /* Get hardware ingress interface */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_hw_igr_intf_get(unit, intfCfg));
        /* Get hardware egress interface */
        if(ENABLED == intfCfg->intfEntry.valid)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_hw_egr_intf_get(unit, &intfCfg->intfEntry));
        }
    } while(1 == 0);

    return ret;
}

/**@}*/ /* RTK_MODULE_ROUTING_TBL_INTF_DRV */
/**@}*/ /* RTK_MODULE_ROUTING_TBL_INTF */

/**
 * @addtogroup RTK_MODULE_ROUTING_TBL_RT
 * @{
 */

/**
 * @defgroup RTK_MODULE_ROUTING_TBL_RT_DRV Low Level Driver
 * @brief Routing Low Level Driver.
 * @{
 */

/**
 * @brief Set routing unicast entry
 *
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] ucEntry Unicast entry configuration (N/A) \n
 *                        To configure the unicast entry.
 * @retval RT_ERR_OK Set unicast entry successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of ucEntry is NULL.
 * @retval -RT_ERR_ROUTE_TBL_TYPE Invalid table type.
 * @retval Others Please refer to cvb_drv_rt_nhtbl_avail_get_runtime(), cvb_drv_rt_uc_host_tbl_entry_runtime_set(), cvb_drv_rt_net_tbl_entry_set_runtime(), cvb_drv_rt_nhtbl_avail_set_runtime().
 */
RtkApiRet cvb_route_uc_set(uint32 unit, RtUcEntry_t *ucEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == ucEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get nhIdx */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nhtbl_avail_get_runtime(unit, &ucEntry->nhIdx));
        ucEntry->nhAlloc = ENABLED;
        /* Set table */
        if(EM_HOST_TBL == ucEntry->tblInfo.tblType)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_uc_host_tbl_entry_runtime_set(unit, ucEntry));
        }
        else if(EM_NET_TBL == ucEntry->tblInfo.tblType)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_net_tbl_entry_set_runtime(unit, ucEntry));
        }
        else
        {
            ret = -RT_ERR_ROUTE_TBL_TYPE;
            continue;
        }
        /* Update nh pool */
        CHK_FUN_CONTINUE(ret, cvb_drv_rt_nhtbl_avail_set_runtime(unit, ucEntry->nhIdx, ENABLED));
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get routing unicast entry
 *
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] ucEntry Unicast entry configuration (N/A) \n
 *                        To get the unicast entry.
 * @retval RT_ERR_OK Get unicast entry successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of ucEntry is NULL.
 * @retval -RT_ERR_ROUTE_TBL_TYPE Invalid table type.
 * @retval Others Please refer to cvb_drv_rt_uc_host_tbl_entry_get(), cvb_drv_rt_net_tbl_entry_get().
 */
RtkApiRet cvb_route_uc_get(uint32 unit, RtUcEntry_t *ucEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == ucEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Set table */
        if(EM_HOST_TBL == ucEntry->tblInfo.tblType)
        {
            ret = cvb_drv_rt_uc_host_tbl_entry_get(unit, ucEntry);
        }
        else if(EM_NET_TBL == ucEntry->tblInfo.tblType)
        {
            ret = cvb_drv_rt_net_tbl_entry_get(unit, ucEntry);
        }
        else
        {
            ret = -RT_ERR_ROUTE_TBL_TYPE;
        }
    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set routing multicast entry
 *
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] mcEntry Multicast entry configuration (N/A) \n
 *                        To configure the multicast entry.
 * @retval RT_ERR_OK Set multicast entry successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of mcEntry is NULL.
 * @retval -RT_ERR_ROUTE_TBL_TYPE Invalid table type.
 * @retval Others Please refer to cvb_drv_rt_mc_host_tbl_entry_runtime_set(), cvb_drv_rt_mc_col_tbl_entry_set_runtime().
 */
RtkApiRet cvb_route_mc_set(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == mcEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Set table */
        if(EM_HOST_TBL == mcEntry->tblInfo.tblType)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_host_tbl_entry_runtime_set(unit, mcEntry));
        }
        else if(EM_MCCOL_TBL == mcEntry->tblInfo.tblType)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_col_tbl_entry_set_runtime(unit, mcEntry));
        }
        else
        {
            ret = -RT_ERR_ROUTE_TBL_TYPE;
            continue;
        }

    } while(1u == 0u);

    return ret;
}

/**
 * @brief Get routing multicast entry
 *
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] mcEntry Multicast entry configuration (N/A) \n
 *                        To get the multicast entry.
 * @retval RT_ERR_OK Get multicast entry successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of mcEntry is NULL.
 * @retval -RT_ERR_ROUTE_TBL_TYPE Invalid table type.
 * @retval Others Please refer to cvb_drv_rt_mc_host_tbl_entry_get(), cvb_drv_rt_mc_col_tbl_entry_get().
 */
RtkApiRet cvb_route_mc_get(uint32 unit, RtMcEntry_t *mcEntry)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(NULL == mcEntry)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Set table */
        if(EM_HOST_TBL == mcEntry->tblInfo.tblType)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_host_tbl_entry_get(unit, mcEntry));
        }
        else if(EM_MCCOL_TBL == mcEntry->tblInfo.tblType)
        {
            CHK_FUN_CONTINUE(ret, cvb_drv_rt_mc_col_tbl_entry_get(unit, mcEntry));
        }
        else
        {
            ret = -RT_ERR_ROUTE_TBL_TYPE;
            continue;
        }

    } while(1u == 0u);

    return ret;
}

/**
 * @brief Set network routing entry
 *
 * @param[in] unit      Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] rtEntry Network routing entry (N/A) \n
 *                        To configure the network routing entry.
 * @retval RT_ERR_OK                The network routing configuration is set successfully.
 * @retval -RT_ERR_ENTRY_INDEX      The table index is out of range.
 * @retval -RT_ERR_NULL_POINTER     The "rtEntry" is NULL.
 * @retval -RT_ERR_INPUT            The IP version is invalid.
 * @retval -RT_ERR_TBL_FULL         The available network route table is full.
 * @retval Others Please refer to drv_memcpy(), cvb_drv_indirect_sram_set(), cvb_drv_rt_subnet_set().
 */
RtkApiRet cvb_route_rttbl_set(uint32 unit, RtTbl_t *rtEntry)
{
    uint32          base                          = 0u;
    uint32          i                             = 0u;
    cvb_RtFwRtTbl_t fwRtEntry                     = {0};
    uint32          data[sizeof(cvb_RtFwRtTbl_t)] = {0};
    uint32          rtTblSize                     = (uint32)sizeof(cvb_RtFwRtTbl_t);
    RtkApiRet       ret                           = RT_ERR_OK;

    CHK_FUN_RET(ret, cvb_drv_rt_rttbl_param_chk(rtEntry));

    /* Set SW routing table configuration to SRAM */
    CHK_FUN_RET(ret, cvb_drv_rt_rttbl_sdk_to_fw(rtEntry, &fwRtEntry));

    CHK_FUN_RET(ret, drv_memcpy((uint8 *)data, (uint8 *)&fwRtEntry, sizeof(fwRtEntry)));

    base = CVB_SW_RT_TBL_BASE + (rtTblSize * rtEntry->index);

    for(i = 0u; i < (rtTblSize / 4u); i++)
    {
        CHK_FUN_RET(ret, cvb_drv_indirect_sram_set(unit, base + (i * 4u), data[i]));
    }

    /* Set HW routing tables */
    CHK_FUN_RET(ret, cvb_drv_rt_subnet_set(unit, rtEntry, (CVB_RT_MAX_NETTBL_NUM - 1u) - (uint32)rtEntry->index));

    return ret;
}

/**
 * @brief Get network routing entry
 *
 * @param[in] unit      Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] rtEntry Network routing entry (N/A) \n
 *                        To get the network routing entry.
 * @retval RT_ERR_OK                The network routing entry is retrieved successfully.
 * @retval -RT_ERR_NULL_POINTER     The "rtEntry" is NULL.
 * @retval Others Please refer to cvb_drv_indirect_sram_get(), drv_memcpy(), cvb_drv_rt_rttbl_fw_to_sdk().
 */
RtkApiRet cvb_route_rttbl_get(uint32 unit, RtTbl_t *rtEntry)
{
    uint32          base                          = 0u;
    uint32          i                             = 0u;
    cvb_RtFwRtTbl_t fwRtEntry                     = {0};
    uint32          data[sizeof(cvb_RtFwRtTbl_t)] = {0};
    uint32          rtTblSize                     = (uint32)sizeof(cvb_RtFwRtTbl_t);
    RtkApiRet       ret                           = RT_ERR_OK;

    if(NULL == rtEntry)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        base = CVB_SW_RT_TBL_BASE + (rtTblSize * rtEntry->index);

        for(i = 0u; i < (rtTblSize / 4u); i++)
        {
            CHK_FUN_RET(ret, cvb_drv_indirect_sram_get(unit, base + (i * 4u), &data[i]));
        }

        CHK_FUN_RET(ret, drv_memcpy((uint8 *)&fwRtEntry, (uint8 *)data, sizeof(fwRtEntry)));

        /* Get SW routing table configuration */
        CHK_FUN_RET(ret, cvb_drv_rt_rttbl_fw_to_sdk(&fwRtEntry, rtEntry));
    }

    return ret;
}

/**
 * @brief Lookup routing table
 *
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] searchInfo Set/Get lookup information (N/A) \n
 *                       To configure lookup keys and get lookup result.
 * @retval -RT_ENTRY_NOT_FOUND Not found available entry
 * @retval -RT_ENTRY_FOUND Found available entry
 * @retval -RT_ERR_INPUT Invalid VID
 * @retval Others Please refer to cvb_drv_rt_host_tbl_lookup(), cvb_drv_rt_net_tbl_lookup(), cvb_drv_rt_mccol_tbl_lookup()
 */
RtkApiRet cvb_route_tbl_lookup(uint32 unit, RtTableSearchInfo_t *searchInfo)
{
    RtkApiRet ret  = RT_ERR_OK;
    bool      isUc = false;

    do
    {
        if(NULL == searchInfo)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        if(CVB_VLAN_NUM_MAX <= searchInfo->luKey.vid)
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

        isUc = (0u == ((uint32)searchInfo->luKey.entryType % 2u));

        /* Host table lookup */
        ret = cvb_drv_rt_host_tbl_lookup(unit, searchInfo);

        if(-RT_ERR_TBL_FULL == ret)
        {
            if(isUc)
            {
                /* Net table lookup */
                ret = cvb_drv_rt_net_tbl_lookup(unit, searchInfo);
            }
            else
            {
                /* Mccol table lookup */
                ret = cvb_drv_rt_mccol_tbl_lookup(unit, searchInfo);
            }

            if(-RT_ERR_TBL_FULL == ret)
            {
                searchInfo->availTblType = EM_TBL_MAX;
            }
        }
    } while(1 == 0);

    return ret;
}

/**
 * @brief Get host entry type list \n
 *        Retrieve the types stored in the same host table index
 *
 * @param[in] unit       Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in,out] typeList Host entry type list (N/A) \n
 *                         To get the host entry type list.
 * @retval RT_ERR_OK Get host entry type list successfully.
 * @retval -RT_ERR_NULL_POINTER The pointer of typeList is NULL.
 * @retval Others Please refer to cvb_drv_host_raw_get(), cvb_drv_host_entry_type_valid_get(), cvb_drv_host_entry_typelist_get().
 */
RtkApiRet cvb_route_host_entry_typelist_get(uint32 unit, RtHostTypeList_t *typeList)
{
    RtkApiRet             ret            = RT_ERR_OK;
    cvb_RtHostTbl_t       hostTblEntry   = {0};
    cvb_RtHostValidType_t entryValidType = {0};

    if(NULL == typeList)
    {
        ret = -RT_ERR_NULL_POINTER;
    }
    else
    {
        CHK_FUN_RET(ret, cvb_drv_host_raw_get(unit, typeList->index, &hostTblEntry));
        CHK_FUN_RET(ret, cvb_drv_host_entry_type_valid_get(&hostTblEntry, &entryValidType));
        CHK_FUN_RET(ret, cvb_drv_host_entry_typelist_get(&entryValidType, typeList));
    }

    return ret;
}
/**@}*/ /* RTK_MODULE_ROUTING_TBL_RT_DRV */
/**@}*/ /* RTK_MODULE_ROUTING_TBL_RT */
/**@}*/ /* RTK_MODULE_ROUTING */