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
#include "rtkas_error.h"
#include "rtl907xc_vb_api.h"
#include "rtkas_types.h"
#include "rtl907xc_vb_reg_list.h"
#include "rtl907xc_vb_regField_list.h"
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_table_struct.h"
#include "rtkas_debug.h"
#include "rtl907xc_vb_asicdrv_vlan.h"
#include "rtkas_timer.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"

uint32 cvb_g_vlanTime = VLAN_FLUSH_TIMEOUT;

/**
 * @brief Cast vlan struct to vlan table entry
 * @param[in] pEntry vlan table entry pointer
 * @param[in] pVlanParam vlan struct member pointer
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_FAILED
 */
static RtkApiRet cvb_drv_vlanTbl_param2entry(cvb_VlanTblAsicEntry_t *const pEntry, cvb_VlanTblParam_t *const pVlanParam)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == pEntry) || (NULL == pVlanParam))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    if(RT_ERR_OK == retVal)
    {
        pVlanParam->interfaceId &= 0x3Fu;
        pVlanParam->interfaceValid &= 0x1u;
        pVlanParam->statefulChkMask &= 0xFFFFu;
        pVlanParam->mbr &= 0xFFFFu;
        pVlanParam->utag &= 0xFFFFu;
        pVlanParam->fid &= 0xFu;
        pVlanParam->ulfid &= 0x1u;
        pVlanParam->vbfwd &= 0x1u;
        pVlanParam->valid &= 0x1u;

        retVal = cvb_drv_drv_memset(pEntry, 0, sizeof(*pEntry));
        if(RT_ERR_OK == retVal)
        {
            pEntry->val[0] = ((pVlanParam->utag << 7u) | ((uint32)pVlanParam->fid << 3u) | ((uint32)pVlanParam->ulfid << 2u) | ((uint32)pVlanParam->vbfwd << 1u) | ((uint32)pVlanParam->valid));
            pEntry->val[1] = pVlanParam->mbr;
            pEntry->val[2] = pVlanParam->statefulChkMask | ((uint32)pVlanParam->interfaceValid << 17u) | ((uint32)pVlanParam->interfaceId << 18u);
        }
    }

    return retVal;
}

/**
 * @brief
 *
 * @param[in] pEntry
 * @param[out] pVlanParam
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_MEM_ACCESS
 */
static RtkApiRet cvb_drv_vlanTbl_entry2Param(const cvb_VlanTblAsicEntry_t *const pEntry, cvb_VlanTblParam_t *const pVlanParam)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == pEntry)
    {
        retVal = -RT_ERR_FAILED;
    }

    if(RT_ERR_OK == retVal)
    {
        retVal = cvb_drv_drv_memset(pVlanParam, 0, sizeof(*pVlanParam));
        if(RT_ERR_OK == retVal)
        {
            pVlanParam->interfaceId     = (uint8)((pEntry->val[2] >> 18u) & 0x3Fu);
            pVlanParam->interfaceValid  = (uint8)((pEntry->val[2] >> 17u) & 0x1u);
            pVlanParam->statefulChkMask = pEntry->val[2] & 0xFFFFu;
            pVlanParam->mbr             = pEntry->val[1] & 0xFFFFu;
            pVlanParam->utag            = (pEntry->val[0] >> 7u) & 0xFFFFu;
            pVlanParam->fid             = (uint8)((pEntry->val[0] >> 3u) & 0xFu);
            pVlanParam->ulfid           = (uint8)((pEntry->val[0] >> 2u) & 0x1u);
            pVlanParam->vbfwd           = (uint8)((pEntry->val[0] >> 1u) & 0x1u);
            pVlanParam->valid           = (uint8)(pEntry->val[0] & 0x1u);
        }
    }

    return retVal;
}

/**
 * @brief Set VLAN table entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index the index of vlan table (0 ~ 4095)
 * @param[in] pParam vlan struct member pointer
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_MEM_ACCESS
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_vlan_tbl_entry_set(uint32 unit, const uint32 index, cvb_VlanTblParam_t *const pParam)
{
    RtkApiRet              retVal = RT_ERR_OK;
    cvb_VlanTblAsicEntry_t entry;

    retVal = cvb_drv_drv_memset(&entry, 0, sizeof(entry));

    if(NULL == pParam)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, cvb_drv_vlanTbl_param2entry(&entry, pParam));
    CHK_FUN_RET(retVal, cvb_table_write(unit, CVB_VLAN, index, entry.val));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index
 * @param[out] pParam
 * @retval RT_ERR_OK
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_MEM_ACCESS
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_vlan_tbl_entry_get(uint32 unit, const uint32 index, cvb_VlanTblParam_t *const pParam)
{
    RtkApiRet              retVal = RT_ERR_OK;
    cvb_VlanTblAsicEntry_t entry;

    if(NULL == pParam)
    {
        retVal = -RT_ERR_FAILED;
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(&entry, 0, sizeof(entry)));
    CHK_FUN_RET(retVal, cvb_table_read(unit, CVB_VLAN, index, entry.val));
    CHK_FUN_RET(retVal, cvb_drv_vlanTbl_entry2Param(&entry, pParam));

    return retVal;
}

RtkApiRet cvb_drv_vlan_tblparam2entry(uint32 unit, const cvb_VlanTblParam_t *const pVlanTblParam, VlanEntry_t *const pVlanEntry)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    if(NULL == pVlanEntry)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(pVlanEntry, 0, sizeof(*pVlanEntry)));

    srcPmap = (RtkPmap)(pVlanTblParam->mbr & (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP));
    CHK_FUN_RET(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER));
    if(RT_ERR_OK == retVal)
    {
        pVlanEntry->mbrMsk = (uint32)dstPmap;
    }

    srcPmap = (RtkPmap)(pVlanTblParam->utag & (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP));
    CHK_FUN_RET(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER));
    if(RT_ERR_OK == retVal)
    {
        pVlanEntry->utagMsk = (uint32)dstPmap;
    }

    srcPmap = (RtkPmap)(pVlanTblParam->statefulChkMask & (CVB_RTK_SYS_PMAP_ALL | CVB_RTK_CPU_PMAP));
    CHK_FUN_RET(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER));
    if(RT_ERR_OK == retVal)
    {
        pVlanEntry->statefulMsk = (uint32)dstPmap;
    }

    if(RT_ERR_OK == retVal)
    {
        pVlanEntry->fid            = (cvb_VlanFid)pVlanTblParam->fid;
        pVlanEntry->interfaceId    = pVlanTblParam->interfaceId;
        pVlanEntry->interfaceValid = pVlanTblParam->interfaceValid;

        switch(pVlanTblParam->ulfid)
        {
            case 0:
                pVlanEntry->ulfid = EM_VLAN_IVL;
                break;
            case 1u:
                pVlanEntry->ulfid = EM_VLAN_SVL;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }

        switch(pVlanTblParam->vbfwd)
        {
            case 0:
                pVlanEntry->vbfwd = EM_ALE_BASED_FWD;
                break;
            case 1u:
                pVlanEntry->vbfwd = EM_VLAN_BASED_FWD;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }

    return retVal;
}

RtkApiRet cvb_drv_vlan_entry2tblparam(uint32 unit, const VlanEntry_t *const pVlanEntry, cvb_VlanTblParam_t *const pVlanTblParam)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    if(NULL == pVlanTblParam)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(pVlanTblParam, 0, sizeof(*pVlanTblParam)));

    if(RT_ERR_OK == retVal)
    {
        /* stateful mask */
        srcPmap = (RtkPmap)(pVlanEntry->statefulMsk);
    }
    CHK_FUN_RET(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS));

    if(RT_ERR_OK == retVal)
    {
        pVlanTblParam->statefulChkMask = (uint32)dstPmap;
        /* member mask */
        srcPmap = (RtkPmap)(pVlanEntry->mbrMsk);
    }
    CHK_FUN_RET(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS));

    if(RT_ERR_OK == retVal)
    {
        pVlanTblParam->mbr = (uint32)dstPmap;
        /* untag member mask */
        srcPmap = (RtkPmap)(pVlanEntry->utagMsk);
    }
    CHK_FUN_RET(retVal, cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS));

    if(RT_ERR_OK == retVal)
    {
        pVlanTblParam->utag           = (uint32)dstPmap;
        pVlanTblParam->interfaceId    = (uint8)pVlanEntry->interfaceId;
        pVlanTblParam->interfaceValid = (uint8)pVlanEntry->interfaceValid;
        pVlanTblParam->fid            = (uint8)(pVlanEntry->fid & 0xffu);
        pVlanTblParam->ulfid          = (uint8)pVlanEntry->ulfid;
        pVlanTblParam->vbfwd          = (uint8)pVlanEntry->vbfwd;
    }

    return retVal;
}

/**
 * @brief Set VLAN port-based VID and priority
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port number of the switch
 * @param[in] vid Port-based vid (1 ~ 4095)
 * @param[in] pri Port-based vlan priority (0 ~ 7)
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_VLAN_PRIORITY
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_vlan_port_pvid_set(uint32 unit, const RtkPort port, const uint32 vid, const uint32 pri)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if(vid > CVB_VLAN_VID_MAX)
    {
        retVal = -RT_ERR_VLAN_VID;
    }
    else if(pri > CVB_VLAN_PRI_MAX)
    {
        retVal = -RT_ERR_VLAN_PRIORITY;
    }
    else
    {
        /* do nothing */
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_set(CVB_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_CONTROL_PIPRI_LSP, CVB_PORT0_VLAN_CONTROL_PIPRI_LEN, pri, &regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_set(CVB_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_CONTROL_PIVID_LSP, CVB_PORT0_VLAN_CONTROL_PIVID_LEN, vid, &regVal));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, regVal));

    return retVal;
}

/**
 * @brief Get VLAN port-based VID and priority
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[out] pVid the pointer of port-based vid
 * @param[out] pPri the pointer of port-base vlan priority
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_SMI
 */
RtkApiRet cvb_drv_vlan_port_pvid_get(uint32 unit, const RtkPort port, uint32 *const pVid, uint32 *const pPri)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if((NULL == pVid) || (NULL == pPri))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(CVB_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_CONTROL_PIVID_LSP, CVB_PORT0_VLAN_CONTROL_PIVID_LEN, pVid, regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(CVB_PORT0_VLAN_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_CONTROL_PIPRI_LSP, CVB_PORT0_VLAN_CONTROL_PIPRI_LEN, pPri, regVal));

    return retVal;
}

/**
 * @brief Set VLAN support frame type
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port number of the switch
 * @param[in] acceptFrameType accept frame type
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_VLAN_ACCEPT_FRAME_TYPE
 * @retval -RT_ERR_FAILED
 * @note The API can set the port accept frame type.
 *     The accept frame type as following:
 *         FRAME_TYPE_BOTH
 *         FRAME_TYPE_TAGGED_ONLY
 *         FRAME_TYPE_UNTAGGED_ONLY
 *         FRAME_TYPE_NONE
 */
RtkApiRet cvb_drv_vlan_port_acpt_type_set(uint32 unit, const RtkPort port, const VlanAcceptFrameType_e acceptFrameType)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    uint32    acctag  = 0u;
    uint32    accutag = 0u;

    retVal = cvb_drv_vlan_check_valid_user_port(unit, port);

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    if(RT_ERR_OK == retVal)
    {
        switch(acceptFrameType)
        {
            case EM_ACCEPT_FRAME_TYPE_ALL:
                acctag  = 1u;
                accutag = 1u;
                break;
            case EM_ACCEPT_FRAME_TYPE_TAG_ONLY:
                acctag  = 1u;
                accutag = 0u;
                break;
            case EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY:
                acctag  = 0u;
                accutag = 1u;
                break;
            case EM_ACCEPT_FRAME_TYPE_NONE:
                acctag  = 0u;
                accutag = 0u;
                break;
            default:
                retVal = -RT_ERR_VLAN_ACCEPT_FRAME_TYPE;
                break;
        }
    }
    CHK_FUN_RET(retVal, cvb_reg_field_set(CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTITAG_LSP, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTITAG_LEN, acctag, &regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_set(CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTIUTAG_LSP, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTIUTAG_LEN, accutag, &regVal));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, regVal));

    return retVal;
}

/**
 * @brief Get VLAN support frame type
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[out] pAcceptFrameType accept frame type
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_SMI
 * @note The API can get the port accept frame type.
 *     The accept frame type as following:
 *         FRAME_TYPE_BOTH
 *         FRAME_TYPE_TAGGED_ONLY
 *         FRAME_TYPE_UNTAGGED_ONLY
 *         FRAME_TYPE_NONE
 */
RtkApiRet cvb_drv_vlan_port_acpt_type_get(uint32 unit, const RtkPort port, VlanAcceptFrameType_e *const pAcceptFrameType)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regVal  = 0u;
    uint32    acctag  = 0u;
    uint32    accutag = 0u;

    retVal = cvb_drv_vlan_check_valid_user_port(unit, port);

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTITAG_LSP, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTITAG_LEN, &acctag, regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTIUTAG_LSP, CVB_PORT0_ACCEPT_FRAME_TYPE_CONTROL_ACPTIUTAG_LEN, &accutag, regVal));

    if(RT_ERR_OK == retVal)
    {
        if((1u == acctag) && (1u == accutag))
        {
            *pAcceptFrameType = EM_ACCEPT_FRAME_TYPE_ALL;
        }
        else if((1u == acctag) && (0u == accutag))
        {
            *pAcceptFrameType = EM_ACCEPT_FRAME_TYPE_TAG_ONLY;
        }
        else if((0u == acctag) && (1u == accutag))
        {
            *pAcceptFrameType = EM_ACCEPT_FRAME_TYPE_UNTAG_ONLY;
        }
        else if((0u == acctag) && (0u == accutag))
        {
            *pAcceptFrameType = EM_ACCEPT_FRAME_TYPE_NONE;
        }
        else
        {
            /* do nothing */
        }
    }

    return retVal;
}

/**
 * @brief Get CVLAN egress tag mode
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[in] pTagRxu the pkts whose souce port is UNI port tx tag status
 * @param[in] pTagRxn the pkts whose souce port is NNI port tx tag status
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_NULL_POINTER
 * @retval -RT_ERR_SMI
 * @note The API is used for setting CVLAN egress tag mode
 *    The accept frame type as following:
 *    EM_VLAN_TAG_MODE_ORIGINAL
 *    EM_VLAN_TAG_MODE_KEEP_FORMAT
 *    EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT
 */
RtkApiRet cvb_drv_vlan_port_egr_tag_mode_get(uint32 unit, const RtkPort port, VlanTagMode_e *const pTagRxu, VlanTagMode_e *const pTagRxn)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    if((NULL == pTagRxu) || (NULL == pTagRxn))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_TX_TAG_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_TX_TAG_CONTROL0_RXUTX_ITAG_KEEP_LSP, CVB_PORT0_TX_TAG_CONTROL0_RXUTX_ITAG_KEEP_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        switch(fieldVal)
        {
            case 0:
                *pTagRxu = EM_VLAN_TAG_MODE_ORIGINAL;
                break;
            case 1u:
                *pTagRxu = EM_VLAN_TAG_MODE_KEEP_FORMAT;
                break;
            case 2u:
                *pTagRxu = EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_TX_TAG_CONTROL0 + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_TX_TAG_CONTROL0_RXNTX_ITAG_KEEP_LSP, CVB_PORT0_TX_TAG_CONTROL0_RXNTX_ITAG_KEEP_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        switch(fieldVal)
        {
            case 0:
                *pTagRxn = EM_VLAN_TAG_MODE_ORIGINAL;
                break;
            case 1u:
                *pTagRxn = EM_VLAN_TAG_MODE_KEEP_FORMAT;
                break;
            case 2u:
                *pTagRxn = EM_VLAN_TAG_MODE_REAL_KEEP_FORMAT;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] enabled
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_tag_aware_set(uint32 unit, const RtkPort port, const uint32 enabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    retVal = cvb_drv_vlan_check_valid_user_port(unit, port);

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_IGNORE_VLAN_TAG_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_IGNORE_VLAN_TAG_CONTROL_IGNOREITAGVID_LSP, CVB_PORT0_IGNORE_VLAN_TAG_CONTROL_IGNOREITAGVID_LEN, (uint32)((0u != enabled) ? 0 : 1)));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pEnabled
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_tag_aware_get(uint32 unit, const RtkPort port, uint32 *const pEnabled)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    if(NULL == pEnabled)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_IGNORE_VLAN_TAG_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_IGNORE_VLAN_TAG_CONTROL_IGNOREITAGVID_LSP, CVB_PORT0_IGNORE_VLAN_TAG_CONTROL_IGNOREITAGVID_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        if(0u != fieldVal)
        {
            *pEnabled = 0u;
        }
        else
        {
            *pEnabled = 1u;
        }
    }

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] enabled
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_parser_tag_set(uint32 unit, const RtkPort port, const uint32 enabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_PACKET_PARSER_CONTROL + (4u * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_PACKET_PARSER_CONTROL_ITAGEN_LSP, CVB_PORT0_PACKET_PARSER_CONTROL_ITAGEN_LEN, (uint32)((enabled != 0u) ? 1 : 0)));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pEnabled
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_parser_tag_get(uint32 unit, const RtkPort port, uint32 *const pEnabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == pEnabled)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_PACKET_PARSER_CONTROL + (4u * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_PACKET_PARSER_CONTROL_ITAGEN_LSP, CVB_PORT0_PACKET_PARSER_CONTROL_ITAGEN_LEN, pEnabled));

    return retVal;
}

/**
 * @brief Set egress port CVLAN tag status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port number of the switch
 * @param[in] tagRxu tx CVLAN tag status for the pkts received from UNI port
 * @param[in] tagRxn tx CVLAN tag status for the pkts received from NNI port
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 * @retval -RT_ERR_FAILED
 * @note The API set egree port transimiting pakcet with or without
 *      CVLAN tag. Its priority is lower than ACL, and equal to VLAN
 *      untag set. It means the setting will be ignored if tag status
 *      has been determined by ACL. One of VLAN untag set and
 *      egress port tag status is untag, the packet will be untag.
 */
RtkApiRet cvb_drv_vlan_egr_tag_status_set(uint32 unit, const RtkPort port, const VlanTxTagStatus_e tagRxu, const VlanTxTagStatus_e tagRxn)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regVal    = 0u;
    uint32    rxuFileld = 0u;
    uint32    rxnFileld = 0u;

    retVal = cvb_drv_vlan_check_valid_user_port(unit, port);

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, &regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(CVB_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LSP, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LEN, &rxuFileld, regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_get(CVB_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LSP, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LEN, &rxnFileld, regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_set(CVB_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LSP, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LEN, ((int32)EM_TAG_STATUS_WITH_TAG == (int32)tagRxu) ? (rxuFileld & (~0x1u)) : (rxuFileld | 0x1u), &regVal));
    CHK_FUN_RET(retVal, cvb_reg_field_set(CVB_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LSP, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LEN, ((int32)EM_TAG_STATUS_WITH_TAG == (int32)tagRxn) ? (rxnFileld & (~0x1u)) : (rxnFileld | 0x1u), &regVal));
    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), 0u, 32u, regVal));

    return retVal;
}

/**
 * @brief Get egress port CVLAN tag status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port id
 * @param[out] pTagRxu the pointer of tx CVLAN tag status for the pkts received from UNI port
 * @param[out] pTagRxn the pointer of tx CVLAN tag status for the pkts received from NNI port
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_ENABLE
 * @note The API gets egree port transimiting pakcet with or without
 *      CVLAN tags. Its priority is lower than ACL, and equial to VLAN
 *      untag set. It means the setting will be ignored if tag status
 *      has been determined by ACL. One of VLAN untag set and
 *      egress port tag status is untag, the packet will be untag.
 */
RtkApiRet cvb_drv_vlan_egr_tag_status_get(uint32 unit, const RtkPort port, VlanTxTagStatus_e *const pTagRxu, VlanTxTagStatus_e *const pTagRxn)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    retVal = cvb_drv_vlan_check_valid_user_port(unit, port);

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LSP, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXU_TAGSTAT_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        if((fieldVal & 0x1u) != 0u)
        {
            *pTagRxu = EM_TAG_STATUS_WITHOUT_TAG;
        }
        else
        {
            *pTagRxu = EM_TAG_STATUS_WITH_TAG;
        }
    }

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LSP, CVB_PORT0_EGRESS_TAG_STATUS_CONTROL_RXN_TAGSTAT_LEN, &fieldVal));
    if(RT_ERR_OK == retVal)
    {
        if((fieldVal & 0x1u) != 0u)
        {
            *pTagRxn = EM_TAG_STATUS_WITHOUT_TAG;
        }
        else
        {
            *pTagRxn = EM_TAG_STATUS_WITH_TAG;
        }
    }

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] enabled
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_igr_filter_set(uint32 unit, const RtkPort port, const uint32 enabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    retVal = cvb_drv_vlan_check_valid_user_port(unit, port);

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL_IGFILTER_LSP, CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL_IGFILTER_LEN, (uint32)((0u != enabled) ? 1 : 0)));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pEnabled
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_igr_filter_get(uint32 unit, const RtkPort port, uint32 *const pEnabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == pEnabled)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL_IGFILTER_LSP, CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL_IGFILTER_LEN, pEnabled));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] act
 * @retval
 */
RtkApiRet cvb_drv_vlan_igr_filter_act_set(uint32 unit, const RtkPort port, const uint32 act)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(act >= (uint32)CVB_VLAN_IGRFILTER_END)
    {
        retVal = -RT_ERR_INPUT;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL_INVFLTACT_LSP, CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL_INVFLTACT_LEN, (uint32)((CVB_VLAN_IGRFILTER_DROP == act) ? 0 : 1)));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pAct
 * @retval
 */
RtkApiRet cvb_drv_vlan_igr_filter_act_get(uint32 unit, const RtkPort port, uint32 *const pAct)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == pAct)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL_INVFLTACT_LSP, CVB_PORT0_VLAN_INGRESS_FILTER_CONTROL_INVFLTACT_LEN, pAct));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] enabled
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_egr_filter_set(uint32 unit, const RtkPort port, const uint32 enabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    retVal = cvb_drv_vlan_check_valid_user_port(unit, port);

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_VLAN_EGRESS_FILTER_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_EGRESS_FILTER_CONTROL_EGFILTER_LSP, CVB_PORT0_VLAN_EGRESS_FILTER_CONTROL_EGFILTER_LEN, (uint32)((enabled != 0u) ? 1 : 0)));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pEnabled
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_egr_filter_get(uint32 unit, const RtkPort port, uint32 *const pEnabled)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == pEnabled)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_VLAN_EGRESS_FILTER_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_VLAN_EGRESS_FILTER_CONTROL_EGFILTER_LSP, CVB_PORT0_VLAN_EGRESS_FILTER_CONTROL_EGFILTER_LEN, pEnabled));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] portrole
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_role_set(uint32 unit, const RtkPort port, const cvb_VlanPortRole_e portrole)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldVal = 0u;

    if(portrole >= CVB_EM_PORTROLE_END)
    {
        retVal = -RT_ERR_INPUT;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_VLAN_PORTROLE_CONTROL, CVB_VLAN_PORTROLE_CONTROL_PORTROLE_LSP, CVB_VLAN_PORTROLE_CONTROL_PORTROLE_LEN, &fieldVal));

    if(0u != (uint32)portrole)
    {
        fieldVal = (fieldVal | ((uint32)1u << cvb_g_userPort2sysPort[port]));
    }
    else
    {
        fieldVal = (fieldVal & (~((uint32)1u << cvb_g_userPort2sysPort[port])));
    }

    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_VLAN_PORTROLE_CONTROL, CVB_VLAN_PORTROLE_CONTROL_PORTROLE_LSP, CVB_VLAN_PORTROLE_CONTROL_PORTROLE_LEN, fieldVal));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port
 * @param[in] pPortrole
 * @retval
 */
RtkApiRet cvb_drv_vlan_port_role_get(uint32 unit, const RtkPort port, cvb_VlanPortRole_e *const pPortrole)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldval = 0u;

    if(NULL == pPortrole)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_VLAN_PORTROLE_CONTROL, CVB_VLAN_PORTROLE_CONTROL_PORTROLE_LSP, CVB_VLAN_PORTROLE_CONTROL_PORTROLE_LEN, &fieldval));
    if(RT_ERR_OK == retVal)
    {
        if(0u != ((fieldval & ((uint32)1u << cvb_g_userPort2sysPort[port])) >> cvb_g_userPort2sysPort[port]))
        {
            *pPortrole = CVB_EM_PORTROLE_NNI;
        }
        else
        {
            *pPortrole = CVB_EM_PORTROLE_UNI;
        }
    }

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] tpid
 * @retval
 */
RtkApiRet cvb_drv_vlan_tpid_set(uint32 unit, const uint32 tpid)
{
    RtkApiRet retVal = RT_ERR_OK;

    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_PACKET_PARSER_PID_CONTROL_REGIST_0, CVB_PACKET_PARSER_PID_CONTROL_REGIST_0_TPID_LSP, CVB_PACKET_PARSER_PID_CONTROL_REGIST_0_TPID_LEN, tpid & 0xffffu));

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] pTpid
 * @retval
 */
RtkApiRet cvb_drv_vlan_tpid_get(uint32 unit, uint32 *const pTpid)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldval = 0u;

    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_PACKET_PARSER_PID_CONTROL_REGIST_0, CVB_PACKET_PARSER_PID_CONTROL_REGIST_0_TPID_LSP, CVB_PACKET_PARSER_PID_CONTROL_REGIST_0_TPID_LEN, &fieldval));

    if(RT_ERR_OK == retVal)
    {
        *pTpid = fieldval & 0xffffu;
    }

    return retVal;
}

/**
 * @brief
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @retval
 */
RtkApiRet cvb_drv_vlan_flush_set(uint32 unit)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    fieldval = 0u;
    uint32    pollcnt  = 0u;

    retVal = cvb_drv_indirect_reg_field_set(unit, CVB_VLAN_CONTROL0, CVB_VLAN_CONTROL0_FLUSHVLANTBL_LSP, CVB_VLAN_CONTROL0_FLUSHVLANTBL_LEN, 1);
    for(pollcnt = 0u; pollcnt < cvb_g_vlanTime; pollcnt++)
    {
        CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_VLAN_CONTROL0, CVB_VLAN_CONTROL0_FLUSHVLANTBL_LSP, CVB_VLAN_CONTROL0_FLUSHVLANTBL_LEN, &fieldval));
        if(((int32)RT_ERR_OK != retVal) || (0u == fieldval))
        {
            break;
        }
        retVal = rtk_usecond_delay(1);
    }
    if(cvb_g_vlanTime == pollcnt)
    {
        retVal = -RT_ERR_FAILED;
    }

    return retVal;
}

/**
 * @brief Get an ingress VLAN retagging entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx Ingress VLAN retagging table index
 * @param[out] pOriginalVid Original VLAN ID to be replaced
 * @param[out] pNewVid New VLAN ID
 * @param[out] pPbm Affected ingress port mask
 * @param[out] pValid Entry valid bit
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_NULL_POINTER
 */
RtkApiRet cvb_drv_vlan_igr_retagging_get(uint32             unit,
                                         const uint32       idx,
                                         cvb_RtkVlan *const pOriginalVid,
                                         cvb_RtkVlan *const pNewVid,
                                         RtkPmap *const     pPbm,
                                         uint8 *const       pValid)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    regVal         = 0u;
    uint32    newVidVal      = 0u;
    uint32    originalVidVal = 0u;
    uint32    validVal       = 0u;
    RtkPmap   srcPmap        = 0u;
    RtkPmap   dstPmap        = 0u;

    if(idx >= CVB_VLAN_INGRESS_RETAG_TBLSIZE)
    {
        retVal = -RT_ERR_VLAN_ING_RETAG_IDX;
    }
    else if((NULL == pOriginalVid) || (NULL == pNewVid) || (NULL == pPbm) || (NULL == pValid))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_INGRESS_VLAN_RETAGGING_CONTROL0 + (4u * 2u * idx), CVB_INGRESS_VLAN_RETAGGING_CONTROL0_ING_ENTRY0_PBM_LSP, CVB_INGRESS_VLAN_RETAGGING_CONTROL0_ING_ENTRY0_PBM_LEN, &regVal));

    CHK_FUN_RET(retVal, cvb_drv_drv_memset(pPbm, 0, sizeof(*pPbm)));
    if(RT_ERR_OK == retVal)
    {
        srcPmap = (RtkPmap)regVal;
        retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER);
        if(RT_ERR_OK == retVal)
        {
            *pPbm = (uint32)dstPmap;
        }
    }

    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_INGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_NEW_VID_LSP, CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_NEW_VID_LEN, &newVidVal));
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_INGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_ORI_VID_LSP, CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_ORI_VID_LEN, &originalVidVal));
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_INGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_VALID_LSP, CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_VALID_LEN, &validVal));
    if(RT_ERR_OK == retVal)
    {
        *pNewVid      = (cvb_RtkVlan)newVidVal;
        *pOriginalVid = (cvb_RtkVlan)originalVidVal;
        *pValid       = (uint8)(validVal & 0x1u);
    }

    return retVal;
}

/**
 * @brief Set an ingress VLAN retagging entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx Index
 * @param[in] originalVid Original VLAN ID to be replaced
 * @param[in] newVid New VLAN ID
 * @param[in] pbm Affected ingress port mask
 * @param[in] valid Entry valid bit
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_vlan_igr_retagging_set(uint32            unit,
                                         const uint32      idx,
                                         const cvb_RtkVlan originalVid,
                                         const cvb_RtkVlan newVid,
                                         const RtkPmap     pbm,
                                         const uint8       valid)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    if(idx >= CVB_VLAN_INGRESS_RETAG_TBLSIZE)
    {
        retVal = -RT_ERR_VLAN_ING_RETAG_IDX;
    }
    else if((originalVid > CVB_VLAN_VID_MAX) || (newVid > CVB_VLAN_VID_MAX))
    {
        retVal = -RT_ERR_VLAN_VID;
    }
    else
    {
        /* do nothing */
    }

    if(RT_ERR_OK == retVal)
    {
        srcPmap = (RtkPmap)pbm;
        retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS);
    }
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_INGRESS_VLAN_RETAGGING_CONTROL0 + (4u * 2u * idx), CVB_INGRESS_VLAN_RETAGGING_CONTROL0_ING_ENTRY0_PBM_LSP, CVB_INGRESS_VLAN_RETAGGING_CONTROL0_ING_ENTRY0_PBM_LEN, (uint32)dstPmap));
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_INGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_NEW_VID_LSP, CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_NEW_VID_LEN, (uint32)newVid));
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_INGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_ORI_VID_LSP, CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_ORI_VID_LEN, (uint32)originalVid));
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_INGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_VALID_LSP, CVB_INGRESS_VLAN_RETAGGING_CONTROL1_ING_ENTRY0_VALID_LEN, (uint32)valid & 0x1u));

    return retVal;
}

/**
 * @brief Get an egress VLAN retagging entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx Egress VLAN retagging table index
 * @param[out] pOriginalVid Original VLAN ID to be replaced
 * @param[out] pNewVid New VLAN ID
 * @param[out] pPbm Affected egress port mask
 * @param[out] pValid Entry valid bit
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_NULL_POINTER
 */
RtkApiRet cvb_drv_vlan_egress_retagging_get(uint32             unit,
                                            const uint32       idx,
                                            cvb_RtkVlan *const pOriginalVid,
                                            cvb_RtkVlan *const pNewVid,
                                            RtkPmap *const     pPbm,
                                            uint8 *const       pValid)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    regVal         = 0u;
    uint32    newVidVal      = 0u;
    uint32    originalVidVal = 0u;
    uint32    validVal       = 0u;
    RtkPmap   srcPmap        = 0u;
    RtkPmap   dstPmap        = 0u;

    if(idx >= CVB_VLAN_EGRESS_RETAG_TBLSIZE)
    {
        retVal = -RT_ERR_VLAN_EGR_RETAG_IDX;
    }
    else if((NULL == pOriginalVid) || (NULL == pNewVid) || (NULL == pPbm) || (NULL == pValid))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        /* do nothing */
    }

    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_EGRESS_VLAN_RETAGGING_CONTROL0 + (4u * 2u * idx), CVB_EGRESS_VLAN_RETAGGING_CONTROL0_ENTRY0_PBM_LSP, CVB_EGRESS_VLAN_RETAGGING_CONTROL0_ENTRY0_PBM_LEN, &regVal));
    CHK_FUN_RET(retVal, cvb_drv_drv_memset(pPbm, 0, sizeof(*pPbm)));
    if(RT_ERR_OK == retVal)
    {
        srcPmap = (RtkPmap)regVal;
        retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_SYS_TO_USER);
        if(RT_ERR_OK == retVal)
        {
            *pPbm = (uint32)dstPmap;
        }
    }

    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_EGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_NEW_VID_LSP, CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_NEW_VID_LEN, &newVidVal));
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_EGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_ORI_VID_LSP, CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_ORI_VID_LEN, &originalVidVal));
    CHK_FUN_RET(retVal, cvb_reg_field_read(unit, CVB_EGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_VALID_LSP, CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_VALID_LEN, &validVal));

    if(RT_ERR_OK == retVal)
    {
        *pNewVid      = (cvb_RtkVlan)newVidVal;
        *pOriginalVid = (cvb_RtkVlan)originalVidVal;
        *pValid       = (uint8)(validVal & 0x1u);
    }

    return retVal;
}

/**
 * @brief Set an egress VLAN retagging entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] idx Egress VLAN retagging table index
 * @param[in] originalVid Original VLAN ID to be replaced
 * @param[in] newVid New VLAN ID
 * @param[in] pbm Affected egress port mask
 * @param[in] valid Entry valid bit
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_vlan_egress_retagging_set(uint32            unit,
                                            const uint32      idx,
                                            const cvb_RtkVlan originalVid,
                                            const cvb_RtkVlan newVid,
                                            const RtkPmap     pbm,
                                            const uint8       valid)
{
    RtkApiRet retVal  = RT_ERR_OK;
    RtkPmap   srcPmap = 0u;
    RtkPmap   dstPmap = 0u;

    if(idx >= CVB_VLAN_EGRESS_RETAG_TBLSIZE)
    {
        retVal = -RT_ERR_VLAN_EGR_RETAG_IDX;
    }
    else if((originalVid > CVB_VLAN_VID_MAX) || (newVid > CVB_VLAN_VID_MAX))
    {
        retVal = -RT_ERR_VLAN_VID;
    }
    else
    {
        /* do nothing */
    }

    if(RT_ERR_OK == retVal)
    {
        srcPmap = (RtkPmap)pbm;
        retVal  = cvb_drv_pmap_convert(srcPmap, &dstPmap, CVB_EM_USER_TO_SYS);
    }
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_EGRESS_VLAN_RETAGGING_CONTROL0 + (4u * 2u * idx), CVB_EGRESS_VLAN_RETAGGING_CONTROL0_ENTRY0_PBM_LSP, CVB_EGRESS_VLAN_RETAGGING_CONTROL0_ENTRY0_PBM_LEN, (uint32)dstPmap));
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_EGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_NEW_VID_LSP, CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_NEW_VID_LEN, (uint32)newVid));
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_EGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_ORI_VID_LSP, CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_ORI_VID_LEN, (uint32)originalVid));
    CHK_FUN_RET(retVal, cvb_reg_field_write(unit, CVB_EGRESS_VLAN_RETAGGING_CONTROL1 + (4u * 2u * idx), CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_VALID_LSP, CVB_EGRESS_VLAN_RETAGGING_CONTROL1_ENTRY0_VALID_LEN, (uint32)valid & 0x1u));

    return retVal;
}

/**
 * @brief Get ingress VLAN retagging status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port port number
 * @param[out] pEnable status
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_NULL_POINTER
 */
RtkApiRet cvb_drv_vlan_igr_retagging_en_get(uint32 unit, const RtkPort port, cvb_RtkEnable *const pEnable)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if(NULL == pEnable)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL_INGRESSVLANRETAGGINGENABLE_LSP, CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL_INGRESSVLANRETAGGINGENABLE_LEN, &regVal));
    if(RT_ERR_OK == retVal)
    {
        switch(regVal)
        {
            case 0:
                *pEnable = CVB_DISABLED;
                break;
            case 1u:
                *pEnable = CVB_ENABLED;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }

    return retVal;
}

/**
 * @brief Set ingress VLAN retagging status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port number
 * @param[in] enable Status
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_vlan_igr_retagging_en_set(uint32 unit, const RtkPort port, const cvb_RtkEnable enable)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(enable >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL_INGRESSVLANRETAGGINGENABLE_LSP, CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL_INGRESSVLANRETAGGINGENABLE_LEN, (uint32)enable));

    return retVal;
}

/**
 * @brief Get egress VLAN retagging status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port number
 * @param[out] pEnable Status
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @retval -RT_ERR_NULL_POINTER
 */
RtkApiRet cvb_drv_vlan_egr_retagging_en_get(uint32 unit, const RtkPort port, cvb_RtkEnable *const pEnable)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    regVal = 0u;

    if(NULL == pEnable)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_get(unit, CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL_EGRESSVLANRETAGGINGENABLE_LSP, CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL_EGRESSVLANRETAGGINGENABLE_LEN, &regVal));
    if(RT_ERR_OK == retVal)
    {
        switch(regVal)
        {
            case 0:
                *pEnable = CVB_DISABLED;
                break;
            case 1u:
                *pEnable = CVB_ENABLED;
                break;
            default:
                retVal = -RT_ERR_FAILED;
                break;
        }
    }

    return retVal;
}

/**
 * @brief Set egress VLAN retagging status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port number
 * @param[in] enable Status
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 */
RtkApiRet cvb_drv_vlan_egr_retagging_en_set(uint32 unit, const RtkPort port, const cvb_RtkEnable enable)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(enable >= CVB_RTK_ENABLE_END)
    {
        retVal = -RT_ERR_ENABLE;
    }
    CHK_FUN_RET(retVal, cvb_drv_vlan_check_valid_user_port(unit, port));

    CHK_FUN_RET(retVal, cvb_drv_indirect_reg_field_set(unit, CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL + (PERPORT_REG_OFFSET * (uint32)cvb_g_userPort2sysPort[port]), CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL_EGRESSVLANRETAGGINGENABLE_LSP, CVB_PORT0_PROPERTY_BASED_VLAN_RETAG_CONTROL_EGRESSVLANRETAGGINGENABLE_LEN, (uint32)enable));

    return retVal;
}

/**
 * @brief Check if inputed user port is vaild
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port Port number
 * @param[out] None
 * @retval RT_ERR_OK
 * @retval -RT_ERR_PORT_ID
 */
RtkApiRet cvb_drv_vlan_check_valid_user_port(uint32 unit, const RtkPort port)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((port != (RtkPort)CVB_RTK_CPU_PORT) && ((port > (RtkPort)CVB_RTK_USER_PORT_END) || (port < (RtkPort)CVB_RTK_USER_PORT_1ST)))
    {
        retVal = -RT_ERR_PORT_ID;
    }

    return retVal;
}
