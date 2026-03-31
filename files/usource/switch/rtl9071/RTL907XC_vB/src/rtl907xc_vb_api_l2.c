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

#include "rtl907xc_vb_asicdrv_lut.h"
#include "rtl907xc_vb_api_ext.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"

/**
 * @addtogroup RTK_MODULE_L2 L2
 * @{
 */
/**
 * @addtogroup HASH_ALGORITHMS Hash Algorithms
 * @{
 */
/**
 * @defgroup HASH_ALGORITHMS_ASIC Low Level Driver
 * @brief L2 Low Level Driver
 * @{
 */
/**
 * @brief Get the hash algorithm from the address table.
 * @param[in]  unit  Stacking unit (1 to 7) \n
 *                   Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  pAlgo The read pointer of the hash algorithm (N/A) \n
 *                   Algorithm to get for hash.
 * @param[out] None
 * @retval RT_ERR_OK            The hash algorithm was retrieved successfully.
 * @retval -RT_ERR_NULL_POINTER The "pAlgo" was NULL.
 * @retval Others               Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_lut_hash_algo_get(uint32 unit, uint32 *pAlgo)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if(NULL == pAlgo)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    /* Get L2 hash algorithm */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_ADDRESS_TABLE_LOOKUP_CONTROL1, CVB_ADDRESS_TABLE_LOOKUP_CONTROL1_HASHALG_LSP, CVB_ADDRESS_TABLE_LOOKUP_CONTROL1_HASHALG_LEN, pAlgo));

    return retVal;
}
/**@}*/ /*HASH_ALGORITHMS_ASIC*/
/**@}*/ /*HASH_ALGORITHMS*/

/**
 * @addtogroup ADDRESS_LOOKUP Address Lookup
 * @{
 */
/**
 * @defgroup ADDRESS_LOOKUP_ASIC Low Level Driver
 * @brief L2 Low Level Driver
 * @{
 */
/**
 * @brief Get an L2 entry by table index.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  index     L2 entry index (0 to 4095) \n
 *                       Index of the L2 entry to get.
 * @param[out] pLutEntry L2 entry data (N/A) \n
 *                       Pointer to the searched L2 entry.
 * @retval RT_ERR_OK            The L2 entry was retrieved successfully.
 * @retval -RT_ERR_NULL_POINTER The "pLutEntry" was NULL.
 * @retval -RT_ERR_INPUT        The "index" was out of range.
 * @retval Others               Please refer to cvb_drv_drv_memset(), cvb_drv_lut_hash_algo_get(), cvb_drv_lut_entry_get(), cvb_drv_lut_sys_param2user_param().
 */
RtkApiRet cvb_lut_entry_get(uint32 unit, const uint16 index, LutParam_t *const pLutEntry)
{
    RtkApiRet          retVal  = RT_ERR_OK;
    uint32             hashAlg = 0u;
    cvb_LutAsicParam_t lutparam;

    /* Check if the input parameters are valid */
    if(NULL == pLutEntry)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(index > CVB_LUT_TABLE_INDEX_MAX)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&lutparam, 0, sizeof(lutparam)));
    /* Get L2 hash algorithm */
    CHK_FUN_RET(retVal, cvb_drv_lut_hash_algo_get(unit, &hashAlg));

    if(RT_ERR_OK == retVal)
    {
        lutparam.index = index;
    }
    /* Get entry from L2 table */
    CHK_FUN_RET(retVal, cvb_drv_lut_entry_get(unit, hashAlg, index, &lutparam));
    /* Convert system parameter to user parameter */
    CHK_FUN_RET(retVal, cvb_drv_lut_sys_param2user_param(&lutparam, pLutEntry));

    return retVal;
}

/**
 * @brief Search an L2 entry by MAC address.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  pMacAddr  MAC address (N/A) \n
 *                       Pointer to the MAC address to search.
 * @param[out] pLutEntry L2 entry data (N/A) \n
 *                       Pointer to the searched L2 entry.
 * @retval RT_ERR_OK            The L2 entry was retrieved successfully.
 * @retval -RT_ERR_NULL_POINTER The "pMacAddr" or "pLutEntry" was NULL.
 * @retval -RT_ERR_FAILED       The L2 entry retrieval failed.
 * @retval Others               Please refer to cvb_drv_drv_memset(), cvb_drv_lut_search(), cvb_lut_entry_get().
 */
RtkApiRet cvb_lut_entry_search(uint32 unit, const uint8 *const pMacAddr, LutParam_t *const pLutEntry)
{
    RtkApiRet            retVal  = RT_ERR_OK;
    uint32               found   = 0u;
    uint16               hitaddr = 0u;
    cvb_LutSearchParam_t searchParam;

    /* Check if the input parameters are valid */
    if((NULL == pMacAddr) || (NULL == pLutEntry))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&searchParam, 0, sizeof(searchParam)));
    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&searchParam, 0, sizeof(cvb_LutSearchParam_t)));
    CHK_FUN_RET(retVal, cvb_drv_drv_memcpy(searchParam.mac, pMacAddr, 6u));

    if((int32)RT_ERR_OK == retVal)
    {
        searchParam.defMac   = 1u;
        searchParam.startIdx = 0u;
    }

    /* Search entry index by MAC address */
    CHK_FUN_RET(retVal, cvb_drv_lut_search(unit, &searchParam, &found, &hitaddr));
    if((RT_ERR_OK == retVal) && (0u == found))
    {
        retVal = -RT_ERR_FAILED;
    }
    /* Get entry from L2 table */
    CHK_FUN_RET(retVal, cvb_lut_entry_get(unit, hitaddr, pLutEntry));

    return retVal;
}

/**
 * @brief Search a multicast entry by MAC and FID.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  pMacAddr  MAC address (N/A) \n
 *                       Pointer to the MAC address to search.
 * @param[in]  fid       Filter ID (0 to 14) \n
 *                       Filtering database identifier used for the SVL function.
 * @param[out] pLutEntry L2 entry data (N/A) \n
 *                       Pointer to the searched L2 entry.
 * @retval RT_ERR_OK                 The L2 entry was retrieved successfully.
 * @retval -RT_ERR_L2_ENTRY_NOTFOUND L2 entry not found.
 * @retval Others                    Please refer to cvb_drv_lut_multi_entry_input_check(), cvb_drv_lut_multi_entry_param_init(), cvb_lut_hash_algo_get(), \n
 *                                   cvb_drv_lut_multicast_index_get(), cvb_drv_drv_memset(), cvb_lut_entry_get().
 */
RtkApiRet cvb_lut_multi_entry_get(uint32 unit, const uint8 *const pMacAddr, const cvb_VlanFid fid, LutParam_t *const pLutEntry)
{
    RtkApiRet               retVal  = RT_ERR_OK;
    uint32                  hashAlg = 0u;
    uint8                   idx     = 0u;
    uint16                  index   = 0u;
    LutParam_t              lutEntry;
    cvb_LutAsicMultiParam_t multiParam;

    /* Check if the input parameters are valid */
    retVal = cvb_drv_lut_multi_entry_input_check(pMacAddr, fid, pLutEntry);
    CHK_FUN_RET(retVal, cvb_drv_lut_multi_entry_param_init(pMacAddr, fid, &multiParam));
    CHK_FUN_RET(retVal, cvb_lut_hash_algo_get(unit, &hashAlg));

    if(RT_ERR_OK == retVal)
    {
        for(idx = CVB_LUT_8WAY_FIRST; idx < CVB_LUT_8WAY_END; idx++)
        {
            retVal = cvb_drv_lut_multicast_index_get(unit, hashAlg, &multiParam, idx, &index);
            CHK_FUN_RET(retVal, cvb_drv_drv_memset(&lutEntry, 0, sizeof(lutEntry)));
            CHK_FUN_RET(retVal, cvb_lut_entry_get(unit, index, &lutEntry));

            if((RT_ERR_OK == retVal) && (EM_LUT_MULTI == lutEntry.type) && (0 == rtl_memcmp(pMacAddr, lutEntry.multi.mac, sizeof(lutEntry.multi.mac))))
            {
                retVal = cvb_drv_drv_memcpy(pLutEntry, &lutEntry, sizeof(lutEntry));
                break;
            }
            else if((CVB_LUT_8WAY_END - 1u) == idx)
            {
                retVal = -RT_ERR_L2_ENTRY_NOTFOUND;
            }
            else
            {
                /* Do nothing */
            }
        }
    }

    return retVal;
}

/**
 * @brief Search an IP multicast entry by SIP and DIP.
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  sip      Source IP address (N/A) \n
 *                      Pointer to the SIP address.
 * @param[in]  dip      Destination IP address (N/A) \n
 *                      Pointer to the DIP address.
 * @param[out] portmask The port mask (N/A) \n
 *                      Port bitmap, which ports in the bitmap will flood the packet with the IP address.
 * @retval RT_ERR_OK                 The L2 entry was retrieved successfully.
 * @retval -RT_ERR_NULL_POINTER      The "portmask" was NULL.
 * @retval -RT_ERR_L2_ENTRY_NOTFOUND L2 entry not found.
 * @retval Others                    Please refer to cvb_drv_lut_ip_multi_entry_input_check(), cvb_drv_lut_ip_multi_entry_param_init(), cvb_lut_hash_algo_get(), \n
 *                                   cvb_drv_lut_ip_multi_index_get(), cvb_drv_lut_entry_get(), cvb_drv_lut_ip_multi_param_cmp(), cvb_drv_pmap_convert().
 */
RtkApiRet cvb_lut_ip_multi_entry_get(uint32 unit, const uint8 *const sip, const uint8 *const dip, RtkPmap *const portmask)
{
    RtkApiRet                 retVal  = RT_ERR_OK;
    uint8                     idx     = 0u;
    uint32                    hashAlg = 0u;
    uint32                    found   = 0u;
    uint16                    index   = 0u;
    RtkPmap                   srcPmap = 0u;
    RtkPmap                   dstPmap = 0u;
    cvb_LutAsicParam_t        retparam;
    cvb_LutAsicIpMultiParam_t ipMultiParam;

    /* Check if the input parameters are valid */
    if((NULL == portmask))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, cvb_drv_lut_ip_multi_entry_input_check(sip, dip));
    CHK_FUN_RET(retVal, cvb_drv_lut_ip_multi_entry_param_init(sip, dip, &ipMultiParam, &retparam));

    if(RT_ERR_OK == retVal)
    {
        for(idx = CVB_LUT_8WAY_FIRST; idx < CVB_LUT_8WAY_END; idx++)
        {
            retVal = cvb_lut_hash_algo_get(unit, &hashAlg);
            CHK_FUN_RET(retVal, cvb_drv_lut_ip_multi_index_get(unit, hashAlg, &ipMultiParam, idx, &index));
            CHK_FUN_RET(retVal, cvb_drv_lut_entry_get(unit, hashAlg, index, &retparam));

            if(RT_ERR_OK == retVal)
            {
                found = cvb_drv_lut_ip_multi_param_cmp(&ipMultiParam, &retparam);
                if(1u == found)
                {
                    srcPmap = (RtkPmap)retparam.ip.mbr;
                    retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER);
                    if(RT_ERR_OK == retVal)
                    {
                        *portmask = (uint32)dstPmap;
                        /* Found entry */
                        break;
                    }
                }
            }
        }
    }

    if((RT_ERR_OK == retVal) && (0u == found))
    {
        retVal = -RT_ERR_L2_ENTRY_NOTFOUND;
    }

    return retVal;
}

/**
 * @brief Get an L2 entry from the BCAM table by BCAM index.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  index     L2 BCAM entry index (0 to 4095) \n
 *                       Index of the L2 BCAM entry to get.
 * @param[out] pLutEntry L2 entry data (N/A) \n
 *                       Pointer to the searched L2 entry.
 * @retval RT_ERR_OK            The L2 entry was retrieved successfully.
 * @retval -RT_ERR_NULL_POINTER The "pLutEntry" was NULL.
 * @retval -RT_ERR_INPUT        The "index" was out of range.
 * @retval Others               Please refer to cvb_drv_drv_memset(), cvb_drv_lut_cam_entry_get(), cvb_drv_drv_memcpy().
 */
RtkApiRet cvb_lut_cam_entry_get(uint32 unit, const uint16 index, LutParam_t *const pLutEntry)
{
    RtkApiRet          retVal = RT_ERR_OK;
    cvb_LutAsicParam_t lutparam;

    /* Check if the input parameters are valid */
    if(NULL == pLutEntry)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(index > CVB_LUT_TABLE_INDEX_MAX)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    if(RT_ERR_OK == retVal)
    {
        lutparam.index = index;
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&lutparam, 0, sizeof(lutparam)));
    /* Get entry from L2 BCAM */
    CHK_FUN_RET(retVal, cvb_drv_lut_cam_entry_get(unit, index, &lutparam));
    /* Convert system parameter to user parameter */
    CHK_FUN_RET(retVal, cvb_drv_lut_sys_param2user_param(&lutparam, pLutEntry));

    return retVal;
}

/**
 * @brief Control the action when address table lookup misses.
 * @param[in]  unit   Stacking unit (1 to 7) \n
 *                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port   Port ID (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                    The port to be set.
 * @param[in]  type   Lookup miss type (CVB_LM_L2_UNICAST to CVB_LM_V6_MULTICAST) \n
 *                    The type to be set.
 * @param[in]  action Lookup miss action (CVB_LM_ACTION_DROP to CVB_LM_ACTION_FLOOD) \n
 *                    The action set to the port.
 * @param[out] None
 * @retval RT_ERR_OK      The lookup miss control was set successfully.
 * @retval -RT_ERR_FAILED The lookup miss control setting failed.
 * @retval Others         Please refer to cvb_drv_lut_lookup_miss_ctrl_input_check(), cvb_drv_indirect_reg_field_get(), cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_lut_lookup_miss_ctrl(uint32 unit, RtkPort port, uint8 type, uint8 action)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    /* Check if the input parameters are valid */
    retVal = cvb_drv_lut_lookup_miss_ctrl_input_check(port, type, action);
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_ADDRESS_TABLE_LOOKUP_MISS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, &regVal));

    if(RT_ERR_OK == retVal)
    {
        switch(type)
        {
            case(uint8)CVB_LM_L2_UNICAST:
                /* Disable FPM[12] */
                regVal &= 0xFFFFEFFFu;
                regVal &= 0xFFFFFFFCu;
                regVal |= ((uint32)action & 0x3u);
                break;
            case(uint8)CVB_LM_L2_BROADCAST:
                /* Disable FPM[14] */
                regVal &= 0xFFFFBFFFu;
                regVal &= 0xFFFFFCFFu;
                regVal |= ((uint32)action & 0x3u) << 8u;
                break;
            case(uint8)CVB_LM_L2_MULTICAST:
                /* Disable FPM[13] */
                regVal &= 0xFFFFDFFFu;
                regVal &= 0xFFFFFF3Fu;
                regVal |= ((uint32)action & 0x3u) << 6u;
                break;
            case(uint8)CVB_LM_L3_MULTICAST:
                regVal &= 0xFFFFF3FFu;
                regVal |= ((uint32)action & 0x3u) << 10u;
                break;
            case(uint8)CVB_LM_V4_MULTICAST:
                /* Disable FPM[13] */
                regVal &= 0xFFFFDFFFu;
                regVal &= 0xFFFFFFCFu;
                regVal |= ((uint32)action & 0x3u) << 4u;
                break;
            case(uint8)CVB_LM_V6_MULTICAST:
                /* Disable FPM[13] */
                regVal &= 0xFFFFDFFFu;
                regVal &= 0xFFFFFFF3u;
                regVal |= ((uint32)action & 0x3u) << 2u;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_ADDRESS_TABLE_LOOKUP_MISS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, regVal));

    return retVal;
}
/**@}*/ /*ADDRESS_LOOKUP_ASIC*/
/**@}*/ /*ADDRESS_LOOKUP*/

/**
 * @addtogroup ADDRESS_LEARNING Address Learning
 * @{
 */
/**
 * @defgroup ADDRESS_LEARNING_ASIC Low Level Driver
 * @brief L2 Low Level Driver
 * @{
 */
/**
 * @brief Enable MAC learning for address table lookup.
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  portmask Port mask (N/A) \n
 *                      Port bitmap indicating which ports will be set.
 * @param[in]  enable   Enable or disable (0 to 1) \n
 *                      Set source learning enable/disable.
 * @param[out] None
 * @retval RT_ERR_OK MAC learning was set successfully.
 * @retval Others    Please refer to cvb_drv_pmap_convert(), cvb_reg_field_write().
 */
RtkApiRet cvb_lut_mac_lrn_en(uint32 unit, RtkPmap portmask, uint8 enable)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    srcPmap = (RtkPmap)portmask;
    retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS);
    if(RT_ERR_OK == retVal)
    {
        if(0u == enable)
        {
            regVal = (uint32)~dstPmap;
        }
        else
        {
            regVal = (uint32)dstPmap;
        }
        regVal &= (uint32)(CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP);
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_ADDRESS_TABLE_LOOKUP_CONTROL1, CVB_ADDRESS_TABLE_LOOKUP_CONTROL1_SRCLEARNEN_LSP, CVB_ADDRESS_TABLE_LOOKUP_CONTROL1_SRCLEARNEN_LEN, regVal));

    return retVal;
}

/**
 * @brief Add a static unicast entry to the lookup address table.
 * @param[in]  unit  Stacking unit (1 to 7) \n
 *                   Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port  Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                   The port that forwards packets with the MAC address.
 * @param[in]  mac   MAC address (N/A) \n
 *                   Pointer to the MAC address.
 * @param[in]  vid   VLAN ID (0 to 4095) \n
 *                   VLAN identifier.
 * @param[in]  fid   Filter ID (0 to 14) \n
 *                   Filtering database identifier used for the SVL function.
 * @param[in]  ulfid Use FID for hash (0 or 1) \n
 *                   Indicator to determine if FID is an input for hash or not.
 * @param[out] None
 * @retval RT_ERR_OK            The entry was set successfully.
 * @retval -RT_ERR_PORT_ID      The "port" was out of range.
 * @retval -RT_ERR_NULL_POINTER The "mac" was NULL.
 * @retval -RT_ERR_INPUT        The "mac", "vid", "fid", or "ulfid" was out of range.
 * @retval Others               Please refer to cvb_drv_drv_memset(), cvb_drv_drv_memcpy(), cvb_drv_lut_uni_entry_add().
 */
RtkApiRet cvb_lut_uni_entry_add(uint32 unit, const uint8 *const mac, uint16 vid, uint8 port, uint16 fid, uint8 ulfid)
{
    RtkApiRet             retVal = RT_ERR_OK;
    cvb_LutAsicUniParam_t param;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == mac)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((1u == (mac[0] & 0x01u)) || (vid > CVB_LUT_VID_MAX) || (fid > CVB_LUT_FID_MAX) || (ulfid > CVB_LUT_ULFID_MAX))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&param, 0, sizeof(param)));
    CHK_FUN_RET(retVal, cvb_drv_drv_memcpy(&(param.mac), mac, sizeof(param.mac)));

    if(RT_ERR_OK == retVal)
    {
        param.vid     = (vid & (uint16)CVB_LUT_VID_MAX);
        param.spa     = (uint8)cvb_g_userPort2sysPort[port];
        param.bstatic = 1;
        param.ulfid   = ulfid;
        if((uint8)CVB_HASH_WITH_CVID == ulfid)
        {
            param.fid = (uint8)CVB_LUT_INVALID_FID;
        }
        else
        {
            param.fid = (uint8)fid;
        }
    }

    CHK_FUN_RET(retVal, cvb_drv_lut_uni_entry_add(unit, &param));

    return retVal;
}

/**
 * @brief Add a multicast entry to the lookup address table.
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  mac      MAC address (N/A) \n
 *                      Pointer to the MAC address.
 * @param[in]  vid      VLAN ID (0 to 4095) \n
 *                      The VID must be the same as the FID.
 * @param[in]  portmask Port mask (N/A) \n
 *                      Port bitmap indicating which ports in the bitmap will flood the packet with the MAC address.
 * @param[in]  fid      Filter ID (0 to 14) \n
 *                      Filtering database identifier used for the SVL function.
 * @param[out] None
 * @retval RT_ERR_OK            The entry was set successfully.
 * @retval -RT_ERR_NULL_POINTER The "mac" was NULL.
 * @retval -RT_ERR_INPUT        The "mac", "vid", or "fid" was out of range.
 * @retval Others               Please refer to cvb_drv_drv_memset(), cvb_drv_drv_memcpy(), cvb_drv_pmap_convert(), cvb_drv_lut_multi_entry_add().
 */
RtkApiRet cvb_lut_multi_entry_add(uint32 unit, const uint8 *const mac, cvb_RtkVlan vid, RtkPmap portmask, cvb_VlanFid fid)
{
    RtkApiRet               retVal  = RT_ERR_OK;
    RtkPmap                 srcPmap = 0u;
    RtkPmap                 dstPmap = 0u;
    cvb_LutAsicMultiParam_t multi_param;

    /* Check if the input parameters are valid */
    if(NULL == mac)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if((1u != (mac[0] & 0x01u)) || (vid > CVB_LUT_VID_MAX) || (fid > CVB_LUT_FID_MAX))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&multi_param, 0, sizeof(multi_param)));
    CHK_FUN_RET(retVal, cvb_drv_drv_memcpy(&(multi_param.mac), mac, sizeof(multi_param.mac)));

    if(RT_ERR_OK == retVal)
    {
        srcPmap = (RtkPmap)portmask;
        retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS);
        if(RT_ERR_OK == retVal)
        {
            multi_param.mbr = (uint32)dstPmap;
            multi_param.fid = (uint8)fid;
            multi_param.vid = (uint16)vid;
        }
        CHK_FUN_RET(retVal, cvb_drv_lut_multi_entry_add(unit, &multi_param));
    }

    return retVal;
}

/**
 * @brief Add an IP multicast entry to the lookup address table.
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  sip      Source IP address (N/A) \n
 *                      Pointer to the SIP address.
 * @param[in]  dip      Destination IP address (N/A) \n
 *                      Pointer to the DIP address.
 * @param[in]  portmask Port mask (N/A) \n
 *                      Port bitmap indicating which ports in the bitmap will flood the packet with the MAC address.
 * @param[out] None
 * @retval RT_ERR_OK            The entry was set successfully.
 * @retval -RT_ERR_NULL_POINTER The "sip" or "dip" was NULL.
 * @retval -RT_ERR_INPUT        The "dip" was out of range.
 * @retval Others               Please refer to cvb_drv_drv_memset(), cvb_drv_drv_memcpy(), cvb_drv_pmap_convert(), cvb_drv_lut_ip_multi_entry_add().
 */
RtkApiRet cvb_lut_ip_multi_entry_add(uint32 unit, const uint8 *const sip, const uint8 *const dip, const RtkPmap portmask)
{
    RtkApiRet                 retVal  = RT_ERR_OK;
    RtkPmap                   srcPmap = 0u;
    RtkPmap                   dstPmap = 0u;
    cvb_LutAsicIpMultiParam_t param;

    /* Check if the input parameters are valid */
    if((NULL == sip) || (NULL == dip))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(0xE0u != (dip[0] & 0xF0u))
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&param, 0, sizeof(param)));
    CHK_FUN_RET(retVal, cvb_drv_drv_memcpy(&(param.sip), sip, sizeof(param.sip)));
    CHK_FUN_RET(retVal, cvb_drv_drv_memcpy(&(param.dip), dip, sizeof(param.dip)));

    if(RT_ERR_OK == retVal)
    {
        srcPmap = (RtkPmap)portmask;
    }
    CHK_FUN_RET(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS));
    if(RT_ERR_OK == retVal)
    {
        param.mbr = (uint32)dstPmap;
    }

    CHK_FUN_RET(retVal, cvb_drv_lut_ip_multi_entry_add(unit, &param));

    return retVal;
}

/**
 * @brief Delete an entry from the lookup address table.
 * @param[in]  unit  Stacking unit (1 to 7) \n
 *                   Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  mac   MAC address (N/A) \n
 *                   Pointer to the MAC address.
 * @param[in]  vid   VLAN ID (0 to 4095) \n
 *                   VLAN identifier.
 * @param[in]  fid   Filter ID (0 to 14) \n
 *                   Filtering database identifier used for the SVL function.
 * @param[in]  ulfid Use FID for hash (0 or 1) \n
 *                   Indicator to determine if FID is an input for hash or not.
 * @param[out] None
 * @retval RT_ERR_OK                 The entry was deleted successfully.
 * @retval -RT_ERR_L2_ENTRY_NOTFOUND The entry was not found for deletion.
 * @retval Others                    Please refer to cvb_drv_lut_entry_delete_input_check(), cvb_drv_lut_entry_delete_param_init(), cvb_drv_lut_search(), \n
 *                                   cvb_drv_lut_hash_algo_get(), cvb_drv_lut_multicast_index_get(), cvb_drv_lut_entry_get(), cvb_drv_lut_multi_entry_check(), \n
 *                                   cvb_drv_lut_entry_delete().
 */
RtkApiRet cvb_lut_entry_delete(uint32 unit, const uint8 *const mac, uint16 vid, uint16 fid, uint8 ulfid)
{
    RtkApiRet               retVal  = RT_ERR_OK;
    uint32                  hashAlg = 0u;
    uint32                  found   = 0u;
    uint16                  hitaddr = 0u;
    uint16                  index   = (uint16)CVB_LUT_8WAY_FIRST;
    uint16                  idx;
    cvb_LutSearchParam_t    param;
    cvb_LutAsicMultiParam_t multiParam;
    cvb_LutAsicParam_t      lutParam;

    /* Check if the input parameters are valid */
    retVal = cvb_drv_lut_entry_delete_input_check(mac, vid, fid, ulfid);
    CHK_FUN_RET(retVal, cvb_drv_lut_entry_delete_param_init(mac, vid, fid, ulfid, &param, &multiParam, &lutParam));

    if(RT_ERR_OK == retVal)
    {
        if(0u == (mac[0] & 0x01u))
        {
            retVal = cvb_drv_lut_search(unit, &param, &found, &hitaddr);
        }
        else
        {
            retVal = cvb_drv_lut_hash_algo_get(unit, &hashAlg);
            CHK_FUN_RET(retVal, cvb_drv_lut_multicast_index_get(unit, hashAlg, &multiParam, CVB_LUT_8WAY_FIRST, &index));
            for(idx = (uint16)CVB_LUT_8WAY_FIRST; idx < (uint16)CVB_LUT_8WAY_END; idx++)
            {
                CHK_FUN_RET(retVal, cvb_drv_lut_entry_get(unit, hashAlg, index + idx, &lutParam));
                if(RT_ERR_OK == retVal)
                {
                    found = cvb_drv_lut_multi_entry_check(mac, vid, fid, &lutParam);
                }
                if(1u == found)
                {
                    hitaddr = index + (uint16)idx;
                    break;
                }
            }
        }
    }

    if(RT_ERR_OK == retVal)
    {
        if(1u == found)
        {
            retVal = cvb_drv_lut_entry_delete(unit, hitaddr);
        }
        else
        {
            retVal = -RT_ERR_L2_ENTRY_NOTFOUND;
        }
    }

    return retVal;
}

/**
 * @brief Delete an IP multicast entry from the address table.
 * @param[in] unit Stacking unit (1 to 7) \n
 *                 Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] sip  Source IP address (N/A) \n
 *                 Pointer to the SIP address.
 * @param[in] dip  Destination IP address (N/A) \n
 *                 Pointer to the DIP address.
 * @retval RT_ERR_OK                 The IP multicast entry was deleted successfully.
 * @retval -RT_ERR_L2_ENTRY_NOTFOUND IP multicast entry not found.
 * @retval Others                    Please refer to cvb_drv_lut_ip_multi_entry_input_check(), cvb_drv_lut_ip_multi_entry_param_init(), cvb_drv_lut_hash_algo_get(), \n
 *                                   cvb_drv_lut_ip_multi_index_get(), cvb_drv_lut_entry_get(), cvb_drv_lut_ip_multi_param_cmp(), cvb_drv_lut_entry_delete().
 */
RtkApiRet cvb_lut_ip_multi_entry_delete(uint32 unit, const uint8 *const sip, const uint8 *const dip)
{
    RtkApiRet                 retVal  = RT_ERR_OK;
    uint32                    hashAlg = 0u;
    uint32                    found   = 0u;
    uint16                    hitaddr = 0u;
    uint16                    index   = (uint16)CVB_LUT_8WAY_FIRST;
    uint16                    idx;
    cvb_LutAsicIpMultiParam_t ipMultiParam;
    cvb_LutAsicParam_t        lutParam;

    /* Check if the input parameters are valid */
    retVal = cvb_drv_lut_ip_multi_entry_input_check(sip, dip);
    CHK_FUN_RET(retVal, cvb_drv_lut_ip_multi_entry_param_init(sip, dip, &ipMultiParam, &lutParam));

    if(RT_ERR_OK == retVal)
    {
        retVal = cvb_drv_lut_hash_algo_get(unit, &hashAlg);
        CHK_FUN_RET(retVal, cvb_drv_lut_ip_multi_index_get(unit, hashAlg, &ipMultiParam, CVB_LUT_8WAY_FIRST, &index));

        for(idx = (uint16)CVB_LUT_8WAY_FIRST; idx < (uint16)CVB_LUT_8WAY_END; idx++)
        {
            CHK_FUN_RET(retVal, cvb_drv_lut_entry_get(unit, hashAlg, index + idx, &lutParam));
            if(RT_ERR_OK == retVal)
            {
                found = cvb_drv_lut_ip_multi_param_cmp(&ipMultiParam, &lutParam);
                if(1u == found)
                {
                    hitaddr = index + (uint16)idx;
                    break;
                }
            }
        }
    }

    if(RT_ERR_OK == retVal)
    {
        if(1u == found)
        {
            retVal = cvb_drv_lut_entry_delete(unit, hitaddr);
        }
        else
        {
            retVal = -RT_ERR_L2_ENTRY_NOTFOUND;
        }
    }

    return retVal;
}

/**
 * @brief Get the SA move legal ports of the specified port.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port      Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                       The physical port number of the switch.
 * @param[out] pPortmask Port mask (CVB_RTK_USER_PMAP_1ST to CVB_RTK_USER_PMAP_ALL, CVB_RTK_CPU_PMAP) \n
 *                       SA move legal port mask.
 * @retval RT_ERR_OK            The SA move legal ports were retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" was out of range.
 * @retval -RT_ERR_NULL_POINTER The "pPortmask" was NULL.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get(), cvb_drv_pmap_convert().
 */
RtkApiRet cvb_lut_sa_move_legal_port_get(uint32 unit, RtkPort port, RtkPmap *pPortmask)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;
    RtkPmap   srcPmap  = 0u;
    RtkPmap   dstPmap  = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pPortmask)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVPM_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVPM_LEN, &fieldVal));

    if(RT_ERR_OK == retVal)
    {
        srcPmap = (RtkPmap)(fieldVal & (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP));
        retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER);
        if(RT_ERR_OK == retVal)
        {
            *pPortmask = (uint32)dstPmap;
        }
    }

    return retVal;
}

/**
 * @brief Set the SA move legal ports of the specified port.
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port     Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                      The physical port number of the switch.
 * @param[in]  portmask Port mask (CVB_RTK_USER_PMAP_1ST to CVB_RTK_USER_PMAP_ALL, CVB_RTK_CPU_PMAP) \n
 *                      SA move legal port mask.
 * @param[out] None
 * @retval RT_ERR_OK       The SA move legal ports were set successfully.
 * @retval -RT_ERR_PORT_ID The "port" was out of range.
 * @retval Others          Please refer to cvb_drv_pmap_convert(), cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_lut_sa_move_legal_port_set(uint32 unit, RtkPort port, RtkPmap portmask)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }

    srcPmap = (RtkPmap)portmask;
    CHK_FUN_RET(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVPM_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVPM_LEN, (uint32)dstPmap));

    return retVal;
}

/**
 * @brief Get the SA move legal port action of the specified port.
 * @param[in]  unit          Stacking unit (1 to 7) \n
 *                           Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port          Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                           The physical port number of the switch.
 * @param[out] pLegalPortAct The action of the legal port (N/A) \n
 *                           The legal port action, includes forward (0u), forward & copy to CPU ports (1u), trap to CPU (2u).
 * @retval RT_ERR_OK            The SA move legal port action was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" was out of range.
 * @retval -RT_ERR_NULL_POINTER The "pLegalPortAct" was NULL.
 * @retval -RT_ERR_FAILED       Got unknown action.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get().
 */
RtkApiRet cvb_lut_sa_move_legal_port_get_act(uint32 unit, RtkPort port, LegalPortAct_e *pLegalPortAct)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pLegalPortAct)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVACT_TRAP_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVACT_TRAP_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        if(1u == fieldVal)
        {
            /* If LEGALMVACT_trap is 0x1, the switch will ignore the setting of the LEGALMVACT field */
            *pLegalPortAct = EM_LEGAL_TRAP_CPU;
        }
        else
        {
            retVal = cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVACT_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVACT_LEN, &fieldVal);
            if(RT_ERR_OK == retVal)
            {
                if(0u == fieldVal)
                {
                    *pLegalPortAct = EM_LEGAL_FWD;
                }
                else if(1u == fieldVal)
                {
                    *pLegalPortAct = EM_LEGAL_FWD_N_COPY_CPU;
                }
                else
                {
                    retVal = -RT_ERR_FAILED;
                }
            }
        }
    }

    return retVal;
}

/**
 * @brief Set the SA move legal port action of the specified port.
 * @param[in]  unit         Stacking unit (1 to 7) \n
 *                          Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port         Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                          The physical port number of the switch.
 * @param[in]  legalPortAct The action of the legal port (EM_LEGAL_FWD to EM_LEGAL_TRAP_CPU) \n
 *                          The legal port action includes forward (0u), forward & copy to CPU ports (1u), trap to CPU (2u).
 * @param[out] None
 * @retval RT_ERR_OK       The SA move legal port action was set successfully.
 * @retval -RT_ERR_PORT_ID The "port" was out of range.
 * @retval -RT_ERR_INPUT   Set wrong legal port action.
 * @retval Others          Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_lut_sa_move_legal_port_set_act(uint32 unit, RtkPort port, LegalPortAct_e legalPortAct)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    mvAct     = 0u;
    uint32    mvActTrap = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }

    if(RT_ERR_OK == retVal)
    {
        switch(legalPortAct)
        {
            case EM_LEGAL_FWD:
                mvAct     = 0u;
                mvActTrap = 0u;
                break;

            case EM_LEGAL_FWD_N_COPY_CPU:
                mvAct     = 1u;
                mvActTrap = 0u;
                break;

            /* If LEGALMVACT_trap is 0x1, the switch will ignore the setting of the LEGALMVACT field */
            case EM_LEGAL_TRAP_CPU:
                mvAct     = 0u;
                mvActTrap = 1u;
                break;

            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVACT_TRAP_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVACT_TRAP_LEN, mvActTrap));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVACT_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGALMVACT_LEN, mvAct));

    return retVal;
}

/**
 * @brief Get the SA move illegal port action of the specified port.
 * @param[in]  unit            Stacking unit (1 to 7) \n
 *                             Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port            Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                             The physical port number of the switch.
 * @param[out] pIllegalPortAct The action of the illegal port (N/A) \n
 *                             The illegal port action includes drop (0u), trap to CPU (1u).
 * @retval RT_ERR_OK            The SA move illegal port action was retrieved successfully.
 * @retval -RT_ERR_PORT_ID      The "port" was out of range.
 * @retval -RT_ERR_NULL_POINTER The "pIllegalPortAct" was NULL.
 * @retval -RT_ERR_FAILED       Got unknown action.
 * @retval Others               Please refer to cvb_drv_indirect_reg_field_get().
 */
RtkApiRet cvb_lut_sa_move_illegal_port_get_act(uint32 unit, RtkPort port, IllegalPortAct_e *pIllegalPortAct)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(NULL == pIllegalPortAct)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_ILLEGALMVACT_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_ILLEGALMVACT_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        if(0u == fieldVal)
        {
            *pIllegalPortAct = EM_ILLEGAL_DROP;
        }
        else if(1u == fieldVal)
        {
            *pIllegalPortAct = EM_ILLEGAL_TRAP_CPU;
        }
        else
        {
            retVal = -RT_ERR_FAILED;
        }
    }

    return retVal;
}

/**
 * @brief Set the SA move illegal port action of the specified port.
 * @param[in]  unit           Stacking unit (1 to 7) \n
 *                            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port           Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                            The physical port number of the switch.
 * @param[in]  IllegalPortAct The action of the illegal port (EM_ILLEGAL_DROP to EM_ILLEGAL_TRAP_CPU) \n
 *                            The illegal port action includes drop (0u), trap to CPU (1u).
 * @param[out] None
 * @retval RT_ERR_OK       The SA move illegal port action was set successfully.
 * @retval -RT_ERR_PORT_ID The "port" was out of range.
 * @retval -RT_ERR_INPUT   Set wrong illegal port action.
 * @retval Others          Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_lut_sa_move_illegal_port_set_act(uint32 unit, RtkPort port, IllegalPortAct_e IllegalPortAct)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    mvAct  = 0u;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }

    if(RT_ERR_OK == retVal)
    {
        switch(IllegalPortAct)
        {
            case EM_ILLEGAL_DROP:
                mvAct = 0u;
                break;

            case EM_ILLEGAL_TRAP_CPU:
                mvAct = 1u;
                break;

            default:
                retVal = -RT_ERR_INPUT;
                break;
        }
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_ILLEGALMVACT_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_ILLEGALMVACT_LEN, mvAct));

    return retVal;
}

/**
 * @brief Get the SA move CPU ports.
 * @param[in]  unit      Stacking unit (1 to 7) \n
 *                       Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] pPortmask Port mask (CVB_RTK_USER_PMAP_1ST to CVB_RTK_USER_PMAP_ALL, CVB_RTK_CPU_PMAP) \n
 *                       SA move CPU port mask.
 * @retval RT_ERR_OK            The SA move CPU ports were retrieved successfully.
 * @retval -RT_ERR_NULL_POINTER The "pPortmask" was NULL.
 * @retval Others               Please refer to cvb_reg_field_read(), cvb_drv_pmap_convert().
 */
RtkApiRet cvb_lut_sa_move_cpu_port_get(uint32 unit, RtkPmap *pPortmask)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;
    RtkPmap   srcPmap  = 0u;
    RtkPmap   dstPmap  = 0u;

    /* Check if the input parameters are valid */
    if(NULL == pPortmask)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_SOURCE_MAC_LEARNING_CONTROL1, CVB_SOURCE_MAC_LEARNING_CONTROL1_PMCPUMSK_LSP, CVB_SOURCE_MAC_LEARNING_CONTROL1_PMCPUMSK_LEN, &fieldVal));

    if(RT_ERR_OK == retVal)
    {
        srcPmap = (RtkPmap)(fieldVal & (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP));
        retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER);
        if(RT_ERR_OK == retVal)
        {
            *pPortmask = (uint32)dstPmap;
        }
    }

    return retVal;
}

/**
 * @brief Set the SA move CPU ports.
 * @param[in]  unit     Stacking unit (1 to 7) \n
 *                      Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  portmask Port mask (CVB_RTK_USER_PMAP_1ST to CVB_RTK_USER_PMAP_ALL, CVB_RTK_CPU_PMAP) \n
 *                      SA move CPU port mask.
 * @param[out] None
 * @retval RT_ERR_OK The SA move CPU ports were set successfully.
 * @retval Others    Please refer to cvb_drv_pmap_convert(), cvb_reg_field_write().
 */
RtkApiRet cvb_lut_sa_move_cpu_port_set(uint32 unit, RtkPmap portmask)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    srcPmap = (RtkPmap)portmask;
    retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS);
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_SOURCE_MAC_LEARNING_CONTROL1, CVB_SOURCE_MAC_LEARNING_CONTROL1_PMCPUMSK_LSP, CVB_SOURCE_MAC_LEARNING_CONTROL1_PMCPUMSK_LEN, (uint32)dstPmap));

    return retVal;
}

/**
 * @brief Enable/Disable the legal SA move record for a specific port.
 * @param[in]  unit   Stacking unit (1 to 7) \n
 *                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port   Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                    The physical port number of the switch.
 * @param[in]  enable The legal SA move record enable (0 to 1) \n
 *                    Enable (1)/Disable (0) the legal SA move record ability for a specific port.
 * @param[out] None
 * @retval RT_ERR_OK       Enabled/Disabled the legal SA move record of the specific port successfully.
 * @retval -RT_ERR_PORT_ID The "port" was out of range.
 * @retval -RT_ERR_INPUT   The "enable" was out of range.
 * @retval Others          Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_lut_legal_sa_move_record_en(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(enable > CVB_LUT_ENABLE_MAX)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGMVRECEN_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_LEGMVRECEN_LEN, enable));

    return retVal;
}

/**
 * @brief Enable/Disable the illegal SA move record for a specific port.
 * @param[in]  unit   Stacking unit (1 to 7) \n
 *                    Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  port   Port number (CVB_RTK_USER_PORT_1ST to CVB_RTK_USER_PORT_END, CVB_RTK_CPU_PORT) \n
 *                    The physical port number of the switch.
 * @param[in]  enable The illegal SA move record enable (0 to 1) \n
 *                    Enable (1)/Disable (0) the illegal SA move record ability for a specific port.
 * @param[out] None
 * @retval RT_ERR_OK       Enabled/Disabled the illegal SA move record of the specific port successfully.
 * @retval -RT_ERR_PORT_ID The "port" was out of range.
 * @retval -RT_ERR_INPUT   The "enable" was out of range.
 * @retval Others          Please refer to cvb_drv_indirect_reg_field_set().
 */
RtkApiRet cvb_lut_illegal_sa_move_record_en(uint32 unit, RtkPort port, uint8 enable)
{
    RtkApiRet retVal = RT_ERR_OK;

    /* Check if the input parameters are valid */
    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }
    else if(enable > CVB_LUT_ENABLE_MAX)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_ILLEGMVRECEN_LSP, CVB_PORT0_SOURCE_MAC_LEARNING_CONTROL_ILLEGMVRECEN_LEN, enable));

    return retVal;
}

/**
 * @brief Get the legal SA move record.
 * @param[in]  unit        Stacking unit (1 to 7) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  idx         The index of the legal SA move record (0 to 2) \n
 *                         Legal SA move record index, there are three registers to record information.
 * @param[in]  pMoveRecord Legal SA move record (N/A) \n
 *                         Legal SA move record, includes new port number, old port number, and MAC address.
 * @param[out] None
 * @retval RT_ERR_OK            The legal SA move record was retrieved successfully.
 * @retval -RT_ERR_INPUT        The "idx" was out of range.
 * @retval -RT_ERR_NULL_POINTER The "pMoveRecord" was NULL.
 * @retval Others               Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_lut_legal_sa_move_get_record(uint32 unit, uint8 idx, SaMoveRecord_t *pMoveRecord)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    /* Check if the input parameters are valid */
    if(idx > CVB_LUT_RECORD_IDX_MAX)
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == pMoveRecord)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* Do nothing */
    }

    /* Get LEGAL_MV_SA_31_0 */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_0 + (4u * 2u * (uint32)idx), CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_0_LEGAL_MV_SA_31_0_LSP, CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_0_LEGAL_MV_SA_31_0_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        pMoveRecord->mac[5] = (uint8)(fieldVal & 0xffu);
        pMoveRecord->mac[4] = (uint8)((fieldVal & 0xff00u) >> 8u);
        pMoveRecord->mac[3] = (uint8)((fieldVal & 0xff0000u) >> 16u);
        pMoveRecord->mac[2] = (uint8)((fieldVal & 0xff000000u) >> 24u);
    }
    /* Get LEG_MV_SA_47_32 */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1 + (4u * 2u * (uint32)idx), CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_LEG_MV_SA_47_32_LSP, CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_LEG_MV_SA_47_32_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        pMoveRecord->mac[1] = (uint8)(fieldVal & 0xffu);
        pMoveRecord->mac[0] = (uint8)((fieldVal & 0xff00u) >> 8u);
    }

    /* Get OLD_PORT_NUM */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1 + (4u * 2u * (uint32)idx), CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_OLD_PORT_NUM_LSP, CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_OLD_PORT_NUM_LEN, &fieldVal));
    if(0u != fieldVal)
    {
        pMoveRecord->oldPortNum = (uint32)cvb_g_sysPort2userPort[fieldVal];
    }

    /* Get NEW_PORT_NUM */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1 + (4u * 2u * (uint32)idx), CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_NEW_PORT_NUM_LSP, CVB_LEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_NEW_PORT_NUM_LEN, &fieldVal));
    if(0u != fieldVal)
    {
        pMoveRecord->newPortNum = (uint32)cvb_g_sysPort2userPort[fieldVal];
    }

    return retVal;
}

/**
 * @brief Get the illegal SA move record.
 * @param[in]  unit        Stacking unit (1 to 7) \n
 *                         Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  idx         The index of the illegal SA move record (0 to 2) \n
 *                         Illegal SA move record index, there are three registers to record information.
 * @param[in]  pMoveRecord Illegal SA move record (N/A) \n
 *                         Illegal SA move record, includes new port number, old port number, and MAC address.
 * @param[out] None
 * @retval RT_ERR_OK            The illegal SA move record was retrieved successfully.
 * @retval -RT_ERR_INPUT        The "idx" was out of range.
 * @retval -RT_ERR_NULL_POINTER The "pMoveRecord" was NULL.
 * @retval Others               Please refer to cvb_reg_field_read().
 */
RtkApiRet cvb_lut_illegal_sa_move_get_record(uint32 unit, uint8 idx, SaMoveRecord_t *pMoveRecord)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    /* Check if the input parameters are valid */
    if(idx > CVB_LUT_RECORD_IDX_MAX)
    {
        retVal = -RT_ERR_INPUT;
    }
    else if(NULL == pMoveRecord)
    {
        retVal = -RT_ERR_INPUT;
    }
    else
    {
        /* Do nothing */
    }

    /* Get LEGAL_MV_SA_31_0 */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_0 + (4u * 2u * (uint32)idx), CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_0_ILLEGAL_MV_SA_31_0_LSP, CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_0_ILLEGAL_MV_SA_31_0_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        pMoveRecord->mac[5] = (uint8)(fieldVal & 0xffu);
        pMoveRecord->mac[4] = (uint8)((fieldVal & 0xff00u) >> 8u);
        pMoveRecord->mac[3] = (uint8)((fieldVal & 0xff0000u) >> 16u);
        pMoveRecord->mac[2] = (uint8)((fieldVal & 0xff000000u) >> 24u);
    }
    /* Get LEG_MV_SA_47_32 */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1 + (4u * 2u * (uint32)idx), CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_ILLEG_MV_SA_47_32_LSP, CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_ILLEG_MV_SA_47_32_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        pMoveRecord->mac[1] = (uint8)(fieldVal & 0xffu);
        pMoveRecord->mac[0] = (uint8)((fieldVal & 0xff00u) >> 8u);
    }

    /* Get OLD_PORT_NUM */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1 + (4u * 2u * (uint32)idx), CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_OLD_PORT_NUM_LSP, CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_OLD_PORT_NUM_LEN, &fieldVal));
    if(0u != fieldVal)
    {
        pMoveRecord->oldPortNum = (uint32)cvb_g_sysPort2userPort[fieldVal];
    }

    /* Get NEW_PORT_NUM */
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1 + (4u * 2u * (uint32)idx), CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_NEW_PORT_NUM_LSP, CVB_ILLEGAL_SOURCE_ADDRESS_MOVE_RECORD0_1_NEW_PORT_NUM_LEN, &fieldVal));
    if(0u != fieldVal)
    {
        pMoveRecord->newPortNum = (uint32)cvb_g_sysPort2userPort[fieldVal];
    }

    return retVal;
}
/**@}*/ /*ADDRESS_LEARNING_ASIC*/
/**@}*/ /*ADDRESS_LEARNING*/
/**@}*/ /*L2*/
