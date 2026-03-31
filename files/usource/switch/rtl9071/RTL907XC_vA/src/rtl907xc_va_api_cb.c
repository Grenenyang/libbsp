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
#include "rtl907xc_va_asicdrv_cb.h"
#include "rtl907xc_va_reg_list.h"
#include "rtl907xc_va_regField_list.h"
#include "rtl907xc_va_table_struct.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_api_cb.h"
#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_asicdrv_routing.h"
#include "rtl907xc_va_asicdrv_tcam.h"
#include "rtl907xc_va_api_tsn.h"
/**
 * @addtogroup RTK_MODULE_CB CB
 * @{
 */

/**
 * @addtogroup CB_DECODE Decode function
 * @{
 */
/**
 * @defgroup CB_DECODE_ASIC Low Level Driver
 * @brief CB Low Level Driver
 * @{
 */
/**
 * @brief Set R-tag parser enable state
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port          Port (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *                          Port that parses R-tag
 * @param[in] on            Enable state (0 to 1) \n
 *                          To turn ON/OFF r-tag parser
 * @param[out] None
 * @retval RT_ERR_OK        The R-tag parser status was set
 * @retval -RT_ERR_INPUT    Input "on" is out of range
 * @retval -RT_ERR_CB_PORT  Input "port" is out of range
 * @retval Others           Please refer to cva_reg_field_write()

 */
RtkApiRet cva_cb_parse_rtag_enabled_set(uint32 unit, RtkPort port, RtkEnable on)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0;

    do
    {
        /* Check input parameter */
        if((CVA_ENABLED != on) && (CVA_DISABLED != on))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((CVA_RTK_USER_PORT_1ST > port) || (CVA_RTK_USER_PORT_11TH < port))
        {
            retVal = -RT_ERR_CB_PORT;
            continue;
        }
        /* userport2sysPort */
        regAddr = CVA_PORT0_PACKET_PARSER_CONTROL + ((uint32)cva_g_userPort2sysPort[port] * 4u);
        /* Set R-tag parser enable register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, regAddr, CVA_PORT1_PACKET_PARSER_CONTROL_RTAG_PARSING_EN_LSP, CVA_PORT1_PACKET_PARSER_CONTROL_RTAG_PARSING_EN_LEN, on));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get R-tag parser enable state
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port          Port (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *                          Port that parses R-tag
 * @param[out] on           Enable state (0 to 1) \n
 *                          To turn ON/OFF r-tag parser
 * @param[out] None
 * @retval RT_ERR_OK        The R-tag parser status was gotten
 * @retval -RT_ERR_CB_PORT  Input "port" is out of range
 * @retval Others           Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_parse_rtag_enabled_get(uint32 unit, RtkPort port, RtkEnable *on)
{
    RtkApiRet retVal  = RT_ERR_OK;
    uint32    regAddr = 0;
    uint32    regData = 0;
    do
    {
        /* Check input parameter */
        if((CVA_RTK_USER_PORT_1ST > port) || (CVA_RTK_USER_PORT_11TH < port))
        {
            retVal = -RT_ERR_CB_PORT;
            continue;
        }
        /* userport2sysPort */
        regAddr = CVA_PORT0_PACKET_PARSER_CONTROL + ((uint32)cva_g_userPort2sysPort[port] * 4u);
        /* Get R-tag parser enable register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, regAddr, CVA_PORT1_PACKET_PARSER_CONTROL_RTAG_PARSING_EN_LSP, CVA_PORT1_PACKET_PARSER_CONTROL_RTAG_PARSING_EN_LEN, &regData));
        *on = regData;
    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* CB_DECODE_ASIC */
/**@}*/ /* CB_DECODE */
/**
 * @defgroup CB_GLB CB global configuration
 * @{
 */
/**
 * @defgroup CB_GLB_ASIC Low Level Driver
 * @brief CB Low Level Driver
 * @{
 */
/**
 * @brief Set CB enable
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] status        CB function enable status (0 to 1) \n
 *                          To turn ON/OFF CB function
 * @param[out] None
 * @retval RT_ERR_OK        The CB status was set
 * @retval -RT_ERR_INPUT    Input "status" is out of range
 * @retval Others           Please refer to cva_reg_field_write()
 */
RtkApiRet cva_cb_enabled_set(uint32 unit, RtkEnable status)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input parameter */
        if((CVA_ENABLED != status) && (CVA_DISABLED != status))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Set CB enable register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_BASIC_CONTROL, CVA_FRER_BASIC_CONTROL_CB_EN_LSP, CVA_FRER_BASIC_CONTROL_CB_EN_LEN, status));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the enable status of CB function
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] status               CB function enable status (0 to 1) \n
 *                                  To turn ON/OFF CB function
 * @retval RT_ERR_OK                The CB status was gotten
 * @retval -RT_ERR_NULL_POINTER     The "status" is NULL
 * @retval Others                   Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_enabled_get(uint32 unit, RtkEnable *status)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint32    registerTemp = 0;
    do
    {
        /* Check NULL pointer */
        if(NULL == status)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get CB enable register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_BASIC_CONTROL, CVA_FRER_BASIC_CONTROL_CB_EN_LSP, CVA_FRER_BASIC_CONTROL_CB_EN_LEN, &registerTemp));
        if(1u == (registerTemp & 0x1u))
        {
            *status = CVA_ENABLED;
        }
        else
        {
            *status = CVA_DISABLED;
        }

    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* CB_GLB_ASIC */
/**@}*/ /* CB_GLB */

/**
 * @addtogroup CB_RESET CB function reset
 * @{
 */
/**
 * @defgroup CB_RESET_ASIC Low Level Driver
 * @brief CB Low Level Driver
 * @{
 */
/**
 * @brief Reset CB Passive Stream identification entry to default value
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                     stream identification entry index (0 to 95) \n
 *                                      Indicate stream identification entry index number
 * @param[out] None
 * @retval RT_ERR_OK                    The passive stream identification entry was reset
 * @retval -RT_ERR_CB_ENTRY_INDEX       The input "index" is out of range
 * @retval Others                       Please refer to drv_memset()
 *                                      Please refer to cva_table_write()
 *
 */
RtkApiRet cva_cb_ingress_passive_sid_reset(uint32 unit, uint32 index)
{
    uint8               i      = 0;
    RtkApiRet           retVal = RT_ERR_OK;
    cva_CbSidTabEntry_t sidTabentry;
    CbSidparam_t        sidTabParam;
    do
    {
        /* Check input */
        if(CVA_TSN_SID_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&sidTabentry, 0, sizeof(sidTabentry)));
        CHK_FUN_CONTINUE(retVal, drv_memset(&sidTabParam, 0, sizeof(sidTabParam)));
        /* Clear stream identification entry to 0 */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_sidentry_set(unit, index, &sidTabParam));

        /* Write table to init value */
        /* Gen data entry */
        for(i = 0; i < CVA_FERE_SID_DATAFIELD_WORDS_CNT; i++)
        {
            if(CVA_FERE_SID_VALIDFIELD_WORD_NUM == i)
            {
                /* Valid bit */
                sidTabentry.val[i] = 0;
            }
            else
            {
                sidTabentry.val[i] = CVA_SID_TABLE_INIT_DATA;
            }
        }
        /* Generate carebit entry */
        for(i = CVA_FERE_SID_CAREFIELD_1ST_WORD_NUM; i <= CVA_FERE_SID_CAREFIELD_LAST_WORD_NUM; i++)
        {
            sidTabentry.val[i] = CVA_SID_TABLE_INIT_DATA;
        }
        /* Reserved field */
        sidTabentry.val[CVA_FERE_SID_RESERVEDFIELD0_WORD_NUM] = 0;
        sidTabentry.val[CVA_FERE_SID_RESERVEDFIELD1_WORD_NUM] = 0;

        /* Set stream identification table */
        retVal = cva_drv_tcam_write(unit, CVA_STREAM_IDENTIFICATION_TABLE, index, (uint32 *)sidTabentry.val);

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Reset CB Individual recovery entry to default value
 * @param[in] unit                      Stacking unit
 * @param[in] index                     IndRcv entry index (0 to 95) \n
 *                                      Indicate individual recovery entry index
 * @param[out] None
 * @retval RT_ERR_OK                    The individual entry was reset
 * @retval -RT_ERR_CB_ENTRY_INDEX       The input "index" is out of range
 * @retval Others                       Please refer to drv_memset()
 *                                      Please refer to cva_reg_field_write()
 *                                      Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_ingress_indrcv_reset(uint32 unit, uint32 index)
{
    RtkApiRet      retVal = RT_ERR_OK;
    CbIndivParam_t indivParam;
    do
    {
        /* Check input */
        if(CVA_FRER_INDIVRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&indivParam, 0, sizeof(indivParam)));
        /* Set indRcv entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_indivrcventry_set(unit, index, &indivParam));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Reset CB Sequence recovery entry to default value
 * @param[in] unit                   Stacking unit
 * @param[in] index                  SeqRcv entry index (0 to 47) \n
 *                                   Indicates sequence recovery index
 * @param[out] None
 * @retval RT_ERR_OK                 The Sequence entry was reset
 * @retval -RT_ERR_CB_ENTRY_INDEX    The input "index" is out of range
 * @retval Others                    Please refer to drv_memset()
 *                                   Please refer to cva_table_write()
 *                                   Please refer to cva_reg_field_write()
 *                                   Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_egress_seqrcv_reset(uint32 unit, uint32 index)
{
    RtkApiRet              retVal = RT_ERR_OK;
    cva_CbSeqRcvTabEntry_t seqRcvTabentryInit;
    CbSeqRcvTabParam_t     seqRcvTabparamZero;
    do
    {
        /* Check input */
        if(CVA_FRER_SEQRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&seqRcvTabentryInit, 0, sizeof(seqRcvTabentryInit)));
        CHK_FUN_CONTINUE(retVal, drv_memset(&seqRcvTabparamZero, 0, sizeof(seqRcvTabparamZero)));
        /* Set sequence recovery entry to 0 */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_seqrcvtab_entry_set(unit, index, &seqRcvTabparamZero));

        /* Write table to init value */
        /* Valid bit is 0 */
        seqRcvTabentryInit.val[0] = CVA_SEQRCV_TABLE_INIT_DATA0;
        seqRcvTabentryInit.val[1] = CVA_SEQRCV_TABLE_INIT_DATA1;
        /* Reserved field */
        seqRcvTabentryInit.val[2] = 0;
        /* Write seqRcv table */
        CHK_FUN_CONTINUE(retVal, cva_table_write(unit, CVA_CB_SEQUENCY_RECOVERY_TABLE, index, (uint32 *)seqRcvTabentryInit.val));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Reset CB Look up table entry
 * @param[in] unit                  Stacking unit
 * @param[in] index                 LUT entry index (0 to 95) \n
 *                                  Indicates CB LUT entry index
 * @param[out] None
 * @retval RT_ERR_OK                The CB Look Up entry was reset
 * @retval -RT_ERR_CB_ENTRY_INDEX   The input "index" is out of range
 * @retval Others                   Please refer to drv_memset()
 *                                  Please refer to cva_table_write()
 */
RtkApiRet cva_cb_misc_reset(uint32 unit, uint32 index)
{
    uint8            i      = 0;
    RtkApiRet        retVal = RT_ERR_OK;
    CbLutParam_t     lutParam;
    cva_CbLutEntry_t lutEntry;
    do
    {
        /* Check input parameter */
        if(CVA_FRER_MISC_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&lutParam, 0, sizeof(lutParam)));
        CHK_FUN_CONTINUE(retVal, drv_memset(&lutEntry, 0, sizeof(lutEntry)));
        /* Assign entry value to default value */
        for(i = 0; i < (CVA_FERE_MISC_ENTRY_WORDS_CNT - 1u); i++)
        {
            lutEntry.val[i] = CVA_MISC_TABLE_INIT_DATA0;
        }
        lutEntry.val[2] = CVA_MISC_TABLE_INIT_DATA1;
        /* Assign invalid bit */
        lutEntry.val[0] &= ~(((uint32)1 << CVA_MISC_ENCODE_EN_BIT) | ((uint32)1 << CVA_MISC_ACTSID_EN_BIT) | ((uint32)1 << CVA_MISC_GENDEQNUM_EN_BIT));
        lutEntry.val[1] &= ~(((uint32)1 << CVA_MISC_INDRCV_EN_BIT) | ((uint32)1 << CVA_MISC_SEQRCV_EN_BIT) | ((uint32)1 << CVA_MISC_DPM_EN_BIT) | ((uint32)0x3Fu << CVA_MISC_SEQRCV_IDX_BIT));
        /* Set bit 19~24, seqIdx to 48u */
        lutEntry.val[1] |= ((uint32)CVA_MISC_TABLE_SEQRCV_IDX_DATA << CVA_MISC_SEQRCV_IDX_BIT);
        /* Write LUT table */
        CHK_FUN_CONTINUE(retVal, cva_table_write(unit, CVA_CB_LOOK_UP_TABLE, index, (uint32 *)lutEntry.val));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Reset CB global control register
 * @param[in] unit              Stacking unit
 * @param[out] None
 * @retval RT_ERR_OK            CB global control register was reset to default value
 * @retval -RT_ERR_INPUT        Error parameter was passed into sub function
 * @retval Others               Please refer to cva_cb_laten_timer_set()
 * @retval Others               Please refer to cva_cb_laten_reset_timer_set()
 * @retval Others               Please refer to cva_reg_field_write()
 * @retval Others               Please refer to cva_cb_interrupt_enable_set()
 * @retval Others               Please refer to cva_cb_enabled_set()
 * @retval Others               Please refer to cva_cb_sid_enabled_set()
 */
static RtkApiRet cva_cb_control_reset(uint32 unit)
{
    RtkApiRet retVal         = RT_ERR_OK;
    uint32    lattimvalue    = 0x2000;
    uint32    latrsttimvalue = 0x30000;
    uint16    rtag           = 0xF1C1;
    do
    {
        /* Initialize latent error detect timer */
        CHK_FUN_CONTINUE(retVal, cva_cb_laten_timer_set(unit, lattimvalue));
        /* Initialize latent error detect reset timer */
        CHK_FUN_CONTINUE(retVal, cva_cb_laten_reset_timer_set(unit, latrsttimvalue));
        /* Clear 48-bit interrupt states */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0_CB_INT_STS_31_0_LSP, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0_CB_INT_STS_31_0_LEN, 0xFFFFFFFFu));
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1_CB_INT_STS_47_32_LSP, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1_CB_INT_STS_47_32_LEN, 0xFFFFu));
        /* Reset Rtag to 0xf1c1 */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_BASIC_CONTROL, CVA_FRER_BASIC_CONTROL_RG_RTAG_LSP, CVA_FRER_BASIC_CONTROL_RG_RTAG_LEN, rtag));
        /* Disable interrupt */
        CHK_FUN_CONTINUE(retVal, cva_cb_interrupt_enable_set(unit, DISABLE));
        /* Disable FRER */
        CHK_FUN_CONTINUE(retVal, cva_cb_enabled_set(unit, DISABLE));

        /* Disable stream identification */
        CHK_FUN_CONTINUE(retVal, cva_cb_sid_enabled_set(unit, DISABLE));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Reset CB function
 * @param[in] unit                  Stacking unit
 * @param[out] None
 * @retval RT_ERR_OK                All CB functions were reset to default values
 * @retval Others                   Please refer to cva_cb_control_reset()
 *                                  Please refer to cva_cb_ingress_passive_sid_reset()
 *                                  Please refer to cva_cb_misc_reset()
 *                                  Please refer to cva_cb_ingress_indrcv_reset()
 *                                  Please refer to cva_cb_egress_seqrcv_reset()
 */
RtkApiRet cva_cb_reset(uint32 unit)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     index  = 0;
    do
    {
        /* Reset control register */
        CHK_FUN_CONTINUE(retVal, cva_cb_control_reset(unit));
        /* Reset CB table entries */
        for(index = 0u; index < CVA_TSN_SID_INSTANCE_NUM_MAX; index++)
        {
            /* Reset stream identification table */
            CHK_FUN_CONTINUE(retVal, cva_cb_ingress_passive_sid_reset(unit, index));
        }
        for(index = 0u; index < CVA_FRER_MISC_INSTANCE_NUM_MAX; index++)
        {
            /* Reset MISC table */
            CHK_FUN_CONTINUE(retVal, cva_cb_misc_reset(unit, index));

            /* Reset IndRcv table */
            CHK_FUN_CONTINUE(retVal, cva_cb_ingress_indrcv_reset(unit, index));
        }
        /* Reset SeqRcv table */
        for(index = 0u; index < CVA_FRER_SEQRCV_INSTANCE_NUM_MAX; index++)
        {
            CHK_FUN_CONTINUE(retVal, cva_cb_egress_seqrcv_reset(unit, index));
        }

    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* CB_RESET_ASIC */
/**@}*/ /* CB_RESET */

/**
 * @addtogroup PASSIVE_SID Passive stream identification function
 * @{
 */
/**
 * @defgroup PASSIVE_SID_ASIC Low Level Driver
 * @brief CB Low Level Driver
 * @{
 */
/**
 * @brief Set the enable status of passive stream identification
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used for indicating which switch to be accessed.
 * @param[in] status        The enable status of stream identification (0 to 1) \n
 *                          To turn ON/OFF stream identification
 * @param[out] None
 * @retval RT_ERR_OK        The passive stream identification status was set
 * @retval -RT_ERR_INPUT    Input "status" is out of range
 * @retval others           Please refer to cva_reg_field_write()
 */
RtkApiRet cva_cb_sid_enabled_set(uint32 unit, RtkEnable status)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Check input */
        if((CVA_ENABLED != status) && (CVA_DISABLED != status))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Set stream identification enable register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_BASIC_CONTROL, CVA_FRER_BASIC_CONTROL_CB_SID_EN_LSP, CVA_FRER_BASIC_CONTROL_CB_SID_EN_LEN, status));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the enable status of passive stream identification
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit id is used for indicating which switch to be accessed.
 * @param[out] status           The enable status of stream identification (0 to 1) \n
 *                              To turn ON/OFF stream identification
 * @retval RT_ERR_OK            The passive stream identification status was gotten
 * @retval -RT_ERR_NULL_POINTER Input "status" is NULL
 * @retval others               Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_sid_enabled_get(uint32 unit, RtkEnable *status)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    registerTemp;
    do
    {
        /* Check NULL pointer */
        if(NULL == status)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get stream identification enable register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_BASIC_CONTROL, CVA_FRER_BASIC_CONTROL_CB_SID_EN_LSP, CVA_FRER_BASIC_CONTROL_CB_SID_EN_LEN, &registerTemp));
        if(1u == (registerTemp & 0x1u))
        {
            *status = (uint8)ENABLE;
        }
        else
        {
            *status = (uint8)DISABLE;
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Set the passive stream identification entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                 Entry index (0 to 95) \n
 *                                  Stream identification entry index
 * @param[in] sidentryParam         Stream identification entry parameters (N/A) \n
 *                                  Pointer that points to stream identification entry parameters
 * @param[out] None
 * @retval RT_ERR_OK                The passive stream identification entry was set
 * @retval -RT_ERR_CB_ENTRY_INDEX   Input "index" is out of range
 * @retval -RT_ERR_INPUT            There was a parameter error, including sidentryParam->streamHandle, sidentryParam->carebits,
 *                                  sidentryParam->dstmac, sidentryParam->srcmac, sidentryParam->vid, sidentryParam->valid,
 *                                  sidentryParam->pmsk, sidentryParam->ipFlag, sidentryParam->dstip, sidentryParam->srcip
 * @retval -RT_ERR_NULL_POINTER     The "sidentryParam" was NULL
 * @retval Others                   Please refer to the cva_drv_cb_sidentry_set()
 */
RtkApiRet cva_cb_ingress_passive_sid_set(uint32 unit, uint32 index, CbSidparam_t *sidentryParam)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input parameter */
        if(CVA_TSN_SID_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        if(NULL == sidentryParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(CVA_TSN_FRER_STREAM_HANDLE_MAX < sidentryParam->streamHandle)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if(CVA_TSN_CAREBITS_MAX < sidentryParam->carebits)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if(CVA_FRER_VID_MAX < sidentryParam->vlanId)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((CVA_ENABLED != sidentryParam->isValid) && (CVA_DISABLED != sidentryParam->isValid))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((0u != (sidentryParam->portMask & 0x1u)) || (sidentryParam->portMask > CVA_FRER_USER_PMSK_MAX))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((CVA_IPV4_FLAG != sidentryParam->ipFlag) && (CVA_IPV6_FLAG != sidentryParam->ipFlag))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Set CB stream identification entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_sidentry_set(unit, index, sidentryParam));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the passive stream identification entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                     Entry index (0 to 95) \n
 *                                      Stream identification entry index
 * @param[out] sidentryParam            stream identification entry parameters (N/A) \n
 *                                      Pointer that points to stream identification entry parameters
 * @retval RT_ERR_OK                    The passive stream identification entry was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX       Input "index" is out of range
 * @retval -RT_ERR_NULL_POINTER         The "sidentryParam" is NULL
 *                                      The NULL structure pointer was passed into the sub function
 * @retval Others                       Please refer to drv_memset()
 *                                      Please refer to cva_drv_cb_sidentry_get()
 */
RtkApiRet cva_cb_ingress_passive_sid_get(uint32 unit, uint32 index, CbSidparam_t *sidentryParam)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input */
        if(CVA_TSN_SID_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == sidentryParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(sidentryParam, 0, sizeof(CbSidparam_t)));
        /* Get stream identification entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_sidentry_get(unit, index, sidentryParam));

    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* PASSIVE_SID_ASIC */
/**@}*/ /* PASSIVE_SID */

/**
 * @addtogroup MISC CB MISC function
 * @{
 */
/**
 * @defgroup MISC_ASIC Low Level Driver
 * @brief CB Low Level Driver
 * @{
 */
/**
 * @brief Set the CB MISC table entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                 Entry index (0 to 95) \n
 *                                  CB MISC table entry index
 * @param[in] lutTabentry           LUT table entry parameters (N/A) \n
 *                                  Pointer that points to LUT table entry parameters
 * @param[out] None
 * @retval RT_ERR_OK                The CB MISC table was set
 * @retval -RT_ERR_CB_ENTRY_INDEX   Input "index" is out of range
 * @retval -RT_ERR_NULL_POINTER     The NULL structure pointer was passed into the sub function
 * @retval Others                   Please refer to cva_drv_cb_misc_check_enable_status()
 *                                  Please refer to cva_drv_cb_misc_check_value_range()
 *                                  Please refer to cva_drv_cb_lutentry_set()
 */
RtkApiRet cva_cb_misc_set(uint32 unit, uint32 index, CbLutParam_t *lutTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input parameter */
        if(CVA_FRER_MISC_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == lutTabentry)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_misc_check_enable_status(lutTabentry));
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_misc_check_value_range(lutTabentry));

        /* Set misc entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_lutentry_set(unit, index, lutTabentry));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the CB MISC table entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                 Entry index (0 to 95) \n
 *                                  CB MISC table entry index
 * @param[out] lutTabentry          LUT table entry parameters (N/A) \n
 *                                  Pointer that points to LUT table entry parameters
 * @retval RT_ERR_OK                CB MISC entry was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX   Input "index" is out of range
 * @retval -RT_ERR_NULL_POINTER     The "lutTabentry" is NULL
 *                                  The NULL structure was passed into the sub function
 * @retval Others                   Please refer to drv_memset()
 *                                  Please refer to cva_drv_cb_lutentry_get()
 */
RtkApiRet cva_cb_misc_get(uint32 unit, uint32 index, CbLutParam_t *lutTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input */
        if(CVA_FRER_MISC_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == lutTabentry)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(lutTabentry, 0, sizeof(CbLutParam_t)));
        /* Get misc entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_lutentry_get(unit, index, lutTabentry));

    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* MISC_ASIC */
/**@}*/ /* MISC */

/**
 * @addtogroup INDIVIDUAL_RECOVERY Individual recovery function
 * @{
 */
/**
 * @defgroup INDIVIDUAL_RECOVERY_ASIC Low Level Driver
 * @brief CB Low Level Driver
 * @{
 */
/**
 * @brief Set the CB individual recovery entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                 Entry index (0 to 95) \n
 *                                  CB individual recovery entry index
 * @param[in] rstload               Reset time (0 to 0xFFFFFFFF) \n
 *                                  Load value of CB individual recovery entry reset timer
 * @param[in] valid                 Valid bit (0 to 1) \n
 *                                  Indicates whether individual recovery entry is valid or not
 * @param[out] None
 * @retval RT_ERR_OK                CB individual recovery entry was set
 * @retval -RT_ERR_CB_ENTRY_INDEX   Input "index" is out of range
 * @retval -RT_ERR_NULL_POINTER     The NULL structure was passed into sub function
 * @retval -RT_ERR_INPUT            The "valid" is out of range
 * @retval Others                   Please refer to cva_drv_cb_indivrcventry_set()
 */
RtkApiRet cva_cb_ingress_indrcv_set(uint32 unit, uint32 index, CbIndivParam_t *indivTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input */
        if(CVA_FRER_INDIVRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == indivTabentry)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        if((DISABLED != indivTabentry->valid) && (ENABLED != indivTabentry->valid))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Set individual recovery entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_indivrcventry_set(unit, index, indivTabentry));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the CB individual recovery entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                  Entry index (0 to 95) \n
 *                                   CB individual recovery entry index
 * @param[out] rstload               Reset timer load value (0 to 0xFFFFFFFF) \n
 *                                   Individual recovery entry reset timer load value
 * @param[out] valid                 Valid bit (0 to 1) \n
 *                                   Indicates whether individual recovery entry is valid or not
 * @param[out] rcvnum                Sequence number (0 to 65535) \n
 *                                   Indicates sequence number that entry is processing
 * @param[out] rsttime               Reset timer value (0 to 0xFFFFFFFF) \n
 *                                   Individual recovery entry reset timer
 * @param[out] takeany               TAKEANY (0 to 1) \n
 *                                   Flag indicates whether this entry will receive the next frame,
 *                                   no matter what the value of its sequence number
 * @retval RT_ERR_OK                 CB individual recovery entry was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX    The input entry index was out of range
 * @retval -RT_ERR_NULL_POINTER      The pointer of "indivTabentry" is NULL
 * @retval Others                    Please refer to cva_drv_cb_indivrcventry_get()
 */
RtkApiRet cva_cb_ingress_indrcv_get(uint32 unit, uint32 index, CbIndivParam_t *indivTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check index */
        if(CVA_FRER_INDIVRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == indivTabentry)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get individual recovery entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_indivrcventry_get(unit, index, indivTabentry));
    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* INDIVIDUAL_RECOVERY_ASIC */
/**@}*/ /* INDIVIDUAL_RECOVERY */
/**
 * @addtogroup SEQUENCE_RECOVERY Sequence recovery function
 * @{
 */
/**
 * @defgroup SEQUENCE_RECOVERY_ASIC Low Level Driver
 * @brief CB Low Level Driver
 * @{
 */
/**
 * @brief Set the CB sequence recovery entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                     Entry index (0 to 47) \n
 *                                      CB sequence recovery entry index
 * @param[in] seqTabentry               Sequence recovery table entry parameters (N/A) \n
 *                                      Pointer that points to sequence recovery table entry
 * @param[out] None
 * @retval RT_ERR_OK                    CB sequence recovery entry was set
 * @retval Others                       Please refer to cva_drv_cb_egress_seqrcv_checkif() and cva_drv_cb_seqrcvtab_entry_set()
 */
RtkApiRet cva_cb_egress_seqrcv_set(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *seqTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input parameters */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_egress_seqrcv_checkif(index, seqTabentry));
        /* Set sequence recovery entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_seqrcvtab_entry_set(unit, index, seqTabentry));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the CB sequence recovery entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                     Entry index (0 to 47) \n
 *                                      CB sequence recovery entry index
 * @param[out] seqTabentry              Sequence recovery table entry parameters (N/A) \n
 *                                      Pointer that points to sequence recovery table entry
 * @retval RT_ERR_OK                    CB sequence recovery entry was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX       Input "index" is out of range
 * @retval -RT_ERR_NULL_POINTER         The NULL pointer was passed into sub function
 *                                      The pointer of out parameter is NULL
 * @retval Others                       Please refer to drv_memset()
 *                                      Please refer to cva_drv_cb_seqrcvtab_entry_get()
 */
RtkApiRet cva_cb_egress_seqrcv_get(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *seqTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check index */
        if(CVA_FRER_SEQRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == seqTabentry)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(seqTabentry, 0, sizeof(CbSeqRcvTabParam_t)));
        /* Get sequence recovery entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_seqrcvtab_entry_get(unit, index, seqTabentry));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set the value of latent error test timer
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] lattimvalue       Latent error test timer (0 to 0xFFFFFFFF) \n
 *                              The value of CB latent error test timer
 * @param[out] None
 * @retval RT_ERR_OK            The value of CB latent error test timer was set.
 * @retval Others               Please refer to cva_reg_field_write()
 */
RtkApiRet cva_cb_laten_timer_set(uint32 unit, uint32 lattimvalue)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Set CB latent error test timer register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_LATT_CONFIGURE1, CVA_FRER_LATT_CONFIGURE1_CB_LATERR_DET_TIME_LSP, CVA_FRER_LATT_CONFIGURE1_CB_LATERR_DET_TIME_LEN, lattimvalue));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the value of latent error test timer
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] lattimvalue          Latent error test timer (0 to 0xFFFFFFFF) \n
 *                                  The value of CB latent error test timer
 * @retval RT_ERR_OK                The value of CB latent error test timer was gotten.
 * @retval -RT_ERR_NULL_POINTER     Input the NULL pointer to the function
 * @retval Others                   Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_laten_timer_get(uint32 unit, uint32 *lattimvalue)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(NULL == lattimvalue)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get CB latent error test timer register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_LATT_CONFIGURE1, CVA_FRER_LATT_CONFIGURE1_CB_LATERR_DET_TIME_LSP, CVA_FRER_LATT_CONFIGURE1_CB_LATERR_DET_TIME_LEN, lattimvalue));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Set the value of latent error reset timer.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] latrsttimvalue        Latent error reset time (0 to 0xFFFFFFFF) \n
 *                                  The value of CB latent error reset timer
 * @param[out] None
 * @retval RT_ERR_OK                The value of CB latent error reset timer was set.
 * @retval Others                   Please refer to cva_reg_field_write()
 */
RtkApiRet cva_cb_laten_reset_timer_set(uint32 unit, uint32 latrsttimvalue)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        /* Set CB latent error reset timer register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_LATT_CONFIGURE0, CVA_FRER_LATT_CONFIGURE0_CB_LATERR_RST_TIME_LSP, CVA_FRER_LATT_CONFIGURE0_CB_LATERR_RST_TIME_LEN, latrsttimvalue));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the value of latent error reset timer.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] latrsttimvalue   Latent error reset time (0 to 0xFFFFFFFF) \n
 *                              The value of CB latent error reset timer
 * @retval RT_ERR_OK            The value of CB latent error reset timer was gotten.
 * @retval -RT_ERR_NULL_POINTER Input NULL pointer to the function
 * @retval Others               Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_laten_reset_timer_get(uint32 unit, uint32 *latrsttimvalue)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check NULL pointer */
        if(NULL == latrsttimvalue)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get CB latent error reset timer register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_LATT_CONFIGURE0, CVA_FRER_LATT_CONFIGURE0_CB_LATERR_RST_TIME_LSP, CVA_FRER_LATT_CONFIGURE0_CB_LATERR_RST_TIME_LEN, latrsttimvalue));

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Set the CB global interrupt enable status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] status            Interrupt enable status (0 to 1) \n
 *                              To turn ON/OFF CB latent error global interrupt
 * @param[out] None
 * @retval RT_ERR_OK            CB latent error interrupt enable status was set
 * @retval -RT_ERR_INPUT        Input "status" is out of range
 * @retval Others               Please refer to cva_reg_field_write()
 */
RtkApiRet cva_cb_interrupt_enable_set(uint32 unit, RtkEnable status)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input */
        if((CVA_ENABLED != status) && (CVA_DISABLED != status))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        /* Set CB interrupt enable register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_INTERRUPT_CONTROL, CVA_FRER_INTERRUPT_CONTROL_CB_INT_EN_LSP, CVA_FRER_INTERRUPT_CONTROL_CB_INT_EN_LEN, status));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the CB global interrupt enable status
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] status           Interrupt enable status (0 to 1) \n
 *                              To turn ON/OFF CB latent error global interrupt
 * @retval RT_ERR_OK            CB latent error interrupt enable status was gotten
 * @retval -RT_ERR_NULL_POINTER Input "status" is NULL
 * @retval Others               Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_interrupt_enable_get(uint32 unit, RtkEnable *status)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    registerTemp;
    do
    {
        /* Check NULL pointer */
        if(NULL == status)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get CB interrupt enable register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_INTERRUPT_CONTROL, CVA_FRER_INTERRUPT_CONTROL_CB_INT_EN_LSP, CVA_FRER_INTERRUPT_CONTROL_CB_INT_EN_LEN, &registerTemp));

        if(1u == (registerTemp & 0x1u))
        {
            *status = ENABLE;
        }
        else
        {
            *status = DISABLE;
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the CB global interrupt flag
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] flag                 Interrupt flag (0 to 1) \n
 *                                  Indicates whether there is a latent error
 * @retval RT_ERR_OK                Global interrupt flag of CB latent error was gotten
 * @retval -RT_ERR_NULL_POINTER     Input "flag" is NULL
 * @retval Others                   Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_interrupt_flag_get(uint32 unit, uint32 *flag)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint32    registerTemp = 0;
    do
    {
        /* Check NULL pointer */
        if(NULL == flag)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get CB interrupt flag register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_INTERRUPT_STATUS, CVA_FRER_INTERRUPT_STATUS_CB_INT_LSP, CVA_FRER_INTERRUPT_STATUS_CB_INT_LEN, &registerTemp));

        if(1u == (registerTemp & 0x1u))
        {
            *flag = CVA_LATE_ERR_OCCUR;
        }
        else
        {
            *flag = CVA_LATE_ERR_CLEAR;
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get 48-bit latent error mask for all entries
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] status               Latent error status (0 to 0xFFFFFFFFFFFF) \n
 *                                  Indicates which sequence recovery entries have errors
 * @retval RT_ERR_OK                CB 48-bit interrupt mask was gotten
 * @retval -RT_ERR_NULL_POINTER     The "status" is NULL
 * @retval Others                   Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_interrupt_mask_get(uint32 unit, uint64 *status)
{
    RtkApiRet retVal   = RT_ERR_OK;
    uint32    regdatal = 0;
    uint32    regdatah = 0;

    do
    {
        /* Check input */
        if(NULL == status)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get CB latent error status register */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0_CB_INT_STS_31_0_LSP, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0_CB_INT_STS_31_0_LEN, &regdatal));
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1_CB_INT_STS_47_32_LSP, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1_CB_INT_STS_47_32_LEN, &regdatah));
        *status = ((uint64)regdatah << 32) | (uint64)regdatal;
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Clear latent error state for an entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                 Sequence recovery entry index (0 to 47) \n
 *                                  Indicates sequence recovery entry index
 * @param[out] None
 * @retval RT_ERR_OK                The latent error state of this sequence recovery entry was cleared up
 * @retval -RT_ERR_CB_ENTRY_INDEX   The input "index" is out of range
 * @retval Others                   Please refer to cva_reg_field_write()
 */
RtkApiRet cva_cb_interrupt_state_clear(uint32 unit, uint32 index)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regdata   = 0;
    uint32    indexTemp = 0;
    do
    {
        /* Check input */
        if(CVA_FRER_SEQRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Set CB latent error state register */
        if(CVA_FRER_INT_STATUS_BIT_31TH >= index)
        {
            regdata |= ((uint32)1u << index);
            CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0_CB_INT_STS_31_0_LSP, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0_CB_INT_STS_31_0_LEN, regdata));
        }
        else
        {
            indexTemp = index - 32u;
            regdata |= ((uint32)1u << indexTemp);
            CHK_FUN_CONTINUE(retVal, cva_reg_field_write(unit, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1_CB_INT_STS_47_32_LSP, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1_CB_INT_STS_47_32_LEN, regdata));
        }

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get latent error status for an entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                 Sequence recovery entry index (0 to 47) \n
 *                                  Indicates sequence recovery entry index
 * @param[out] status               Latent error status (0 to 1) \n
 *                                  Latent error status of this sequence recovery entry
 * @retval RT_ERR_OK                The latent error state of this sequence recovery entry was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX   The input "index" is out of range
 * @retval -RT_ERR_NULL_POINTER     Input "status" is NULL
 * @retval Others                   Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_interrupt_state_get(uint32 unit, uint32 index, uint32 *status)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    regdata   = 0;
    uint32    indexTemp = 0;
    do
    {
        /* Check input */
        if(CVA_FRER_SEQRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == status)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get CB latent error state register */
        if(CVA_FRER_INT_STATUS_BIT_31TH >= index)
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0_CB_INT_STS_31_0_LSP, CVA_FRER_INTERRUPT_PERSTREAM_STATUS0_CB_INT_STS_31_0_LEN, &regdata));
            *status = (regdata >> index) & 0x1u;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1_CB_INT_STS_47_32_LSP, CVA_FRER_INTERRUPT_PERSTREAM_STATUS1_CB_INT_STS_47_32_LEN, &regdata));
            indexTemp = index - 32u;
            *status   = (regdata >> indexTemp) & 0x1u;
        }
    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Get the CB sequence recovery history entry
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index Entry index \n
 *                  CB sequence recovery history entry index
 * @param[out] seqHistoryEntry Sequence recovery history entry \n
 *                             Sequence recovery history entry data
 * @retval RT_ERR_OK                    CB sequence recovery history entry was gotten
 * @retval -RT_ERR_NULL_POINTER         The pointer of "seqHistoryEntry" is NULL
 *                                      The NULL pointer was passed into sub function
 * @retval -RT_ERR_CB_ENTRY_INDEX       Input index is out of range
 * @retval Others                       Please refer to drv_memset()
 *                                      Please refer to cva_cb_egress_seqrcv_passcnt_get()
 *                                      Please refer to cva_drv_cb_read_table()
 */
RtkApiRet cva_cb_egress_seqhistory_get(uint32 unit, uint32 index, CbSeqHistoryEntry_t *seqHistoryEntry)
{
    RtkApiRet           retVal       = RT_ERR_OK;
    uint32              tableAddress = 0;
    cva_table_list_t    tableType;
    CbSeqHistoryEntry_t seqHistoryEntryTemp;
    CbSeqHistoryEntry_t seqHistoryEntryTemperror;
    uint64              passcnt = 0u;

    do
    {
        /* Check index */
        if(CVA_FRER_SEQRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == seqHistoryEntry)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&seqHistoryEntryTemp, 0, sizeof(CbSeqHistoryEntry_t)));
        CHK_FUN_CONTINUE(retVal, drv_memset(&seqHistoryEntryTemperror, 0, sizeof(CbSeqHistoryEntry_t)));

        tableType    = CVA_CB_HISTORY_TABLE;
        tableAddress = index;
        /* Read SeqRcv pass counter value */
        CHK_FUN_CONTINUE(retVal, cva_cb_egress_seqrcv_passcnt_get(unit, index, &passcnt));
        /* Check if history window has been used? */
        if(0u != passcnt)
        {
            /* Get correct table value */
            CHK_FUN_CONTINUE(retVal, cva_drv_cb_read_table(unit, tableType, tableAddress, seqHistoryEntryTemp.val, seqHistoryEntryTemperror.val, sizeof(CbSeqHistoryEntry_t)));
        }
        /* Output entry value */
        seqHistoryEntry->val[0]  = seqHistoryEntryTemp.val[0];
        seqHistoryEntry->val[1]  = seqHistoryEntryTemp.val[1];
        seqHistoryEntry->val[2]  = seqHistoryEntryTemp.val[2];
        seqHistoryEntry->val[3]  = seqHistoryEntryTemp.val[3];
        seqHistoryEntry->val[4]  = seqHistoryEntryTemp.val[4];
        seqHistoryEntry->val[5]  = seqHistoryEntryTemp.val[5];
        seqHistoryEntry->val[6]  = seqHistoryEntryTemp.val[7];
        seqHistoryEntry->val[7]  = seqHistoryEntryTemp.val[8];
        seqHistoryEntry->val[8]  = seqHistoryEntryTemp.val[9];
        seqHistoryEntry->val[9]  = seqHistoryEntryTemp.val[10];
        seqHistoryEntry->val[10] = seqHistoryEntryTemp.val[11];
        seqHistoryEntry->val[11] = seqHistoryEntryTemp.val[12];
        seqHistoryEntry->val[12] = seqHistoryEntryTemp.val[13];
        seqHistoryEntry->val[13] = seqHistoryEntryTemp.val[14];
        seqHistoryEntry->val[14] = seqHistoryEntryTemp.val[15];
        seqHistoryEntry->val[15] = seqHistoryEntryTemp.val[16];
        seqHistoryEntry->val[16] = 0;

    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* SEQUENCE_RECOVERY_ASIC */
/**@}*/ /* SEQUENCE_RECOVERY */

/**
 * @addtogroup ACTIVE_SID Active stream identification function
 * @{
 */
/**
 * @defgroup ACTIVE_SID_ASIC Low Level Driver
 * @brief CB Low Level Driver
 * @{
 */
/**
 * @brief Set the replication header entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] rhIndex           Entry index (0 to 95) \n
 *                              Replication header table entry index
 * @param[in] rhEntryParm       RH entry parameters (N/A) \n
 *                              Pointer that points to RH entry parameters
 * @param[out] None
 * @retval RT_ERR_OK                Replication header entry was set
 * @retval -RT_ERR_CB_ENTRY_INDEX   Input "rhIndex" is out of range
 * @retval -RT_ERR_NULL_POINTER     The "rhEntryParm" is NULL
 * @retval -RT_ERR_INPUT            There is an error
 * @retval Others                   Please refer to drv_memset()
 * @retval Others                   Please refer to cva_drv_rt_rhtbl_set()
 */
RtkApiRet cva_cb_actsid_rhtbl_set(uint32 unit, uint32 rhIndex, const CbActsidRhTblParm_t *rhEntryParm)
{
    RtkApiRet         retVal = RT_ERR_OK;
    cva_RtRhTblParm_t rhTableParm;
    uint8             i = 0u;
    do
    {
        /* Check input */
        if(CVA_FRER_RH_INSTANCE_MAX <= rhIndex)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == rhEntryParm)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        for(i = CVA_RTK_USER_PORT_1ST; i <= CVA_RTK_USER_PORT_11TH; i++)
        {
            if(CVA_ACTIVE_RHTBL_FIELD_MAX < rhEntryParm->portRhIdx[i])
            {
                retVal = -RT_ERR_INPUT;
                continue;
            }
        }
        if(RT_ERR_OK != retVal)
        {
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&rhTableParm, 0, sizeof(cva_RtRhTblParm_t)));
        /* userPort2SysPort */
        /* SysPort2userPort */
        for(i = CVA_RTK_USER_PORT_1ST; i <= CVA_RTK_USER_PORT_11TH; i++)
        {
            rhTableParm.portRhIdx[cva_g_userPort2sysPort[i]] = rhEntryParm->portRhIdx[i] + CVA_FRER_OIL_TABLE_OFFSET;
        }

        /* Set RH table entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_rt_rhtbl_set(unit, (rhIndex + CVA_FRER_RH_TABLE_OFFSET), &rhTableParm));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the replication header entry.
 * @param[in] unit Stacking unit \n
 *            Indicates stacking unit
 * @param[in] rhIndex Entry index (0 to 95) \n
 *                    Replication header table entry index
 * @param[out] rhEntryParm RH entry parameter struct \n
 *                         Pointer that points to RH entry parameters
 * @retval RT_ERR_OK                Replication header entry was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX   Input "rhIndex" is out of range
 * @retval -RT_ERR_NULL_POINTER     The "rhEntryParm" is NULL
 * @retval Others                   Please refer to drv_memset()
 *                                  Please refer to cva_drv_rt_rhtbl_get()
 */
RtkApiRet cva_cb_actsid_rhtbl_get(uint32 unit, uint32 rhIndex, CbActsidRhTblParm_t *rhEntryParm)
{
    RtkApiRet         retVal = RT_ERR_OK;
    cva_RtRhTblParm_t rhTableParm;
    uint32            i = 0;

    do
    {
        /* Check input */
        if(CVA_FRER_RH_INSTANCE_MAX <= rhIndex)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == rhEntryParm)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&rhTableParm, 0, sizeof(cva_RtRhTblParm_t)));
        /* Get RH table entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_rt_rhtbl_get(unit, rhIndex + CVA_FRER_RH_TABLE_OFFSET, &rhTableParm));
        /* SysPort2userPort */
        for(i = CVA_RTK_USER_PORT_1ST; i <= CVA_RTK_USER_PORT_11TH; i++)
        {
            if(CVA_FRER_OIL_TABLE_OFFSET <= (rhTableParm.portRhIdx[cva_g_userPort2sysPort[i]] & 0x1FFFu))
            {
                rhEntryParm->portRhIdx[i] = rhTableParm.portRhIdx[cva_g_userPort2sysPort[i]] - CVA_FRER_OIL_TABLE_OFFSET;
            }
            else
            {
                rhEntryParm->portRhIdx[i] = rhTableParm.portRhIdx[cva_g_userPort2sysPort[i]];
            }
        }

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Set the outgoing interface list entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] oilIndex Entry index (0 to 95) \n
 *                     Outgoing Interface List Table entry index
 * @param[in] oilEntryParm Entry information
 * @param[out] None
 * @retval RT_ERR_OK                Outgoing interface list entry was set
 * @retval -RT_ERR_CB_ENTRY_INDEX   Input "oilIndex" is out of range
 * @retval -RT_ERR_NULL_POINTER     The "oilEntryParm" is NULL
 * @retval -RT_ERR_INPUT            There was a parameter error, including egifIdx, oilnextValid and nextOilIdx
 * @retval Others                   Please refer to drv_memset()
 *                                  Please refer to cva_drv_rt_oiltbl_set()
 */
RtkApiRet cva_cb_actsid_oiltbl_set(uint32 unit, uint32 oilIndex, CbActsidOiltblParm_t *oilEntryParm)
{
    RtkApiRet          retVal = RT_ERR_OK;
    cva_RtOilTblParm_t oilTableParm;
    do
    {
        /* Check input */
        if(CVA_FRER_OIL_INSTANCE_NUM_MAX <= oilIndex)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == oilEntryParm)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        if(CVA_FRER_EGIF_INSTANCE <= oilEntryParm->egifIdx)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if(CVA_FRER_OIL_INSTANCE_NUM_MAX <= oilEntryParm->nextOilIdx)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((CVA_ENABLED != oilEntryParm->oilnextValid) && (CVA_DISABLED != oilEntryParm->oilnextValid))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&oilTableParm, 0, sizeof(oilTableParm)));
        /* Convert CBOilIndex to routingOilIndex */
        oilTableParm.egrIfIdx = oilEntryParm->egifIdx + (uint8)CVA_FRER_EGIF_TABLE_OFFSET;
        if(1u == oilEntryParm->oilnextValid)
        {
            oilTableParm.oilNext = ((uint16)1u << 13u) + (uint16)oilEntryParm->nextOilIdx + (uint16)CVA_FRER_OIL_TABLE_OFFSET;
        }
        else
        {
            oilTableParm.oilNext = (uint16)oilEntryParm->nextOilIdx + (uint16)CVA_FRER_OIL_TABLE_OFFSET;
        }
        /* Set OIL table entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_rt_oiltbl_set(unit, oilIndex + CVA_FRER_OIL_TABLE_OFFSET, &oilTableParm));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the outgoing interface list entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] oilIndex Entry index (0 to 95) \n
 *                     Outgoing Interface List Table entry index
 * @param[out] oilEntryParm Entry information
 * @retval RT_ERR_OK                Outgoing interface list entry was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX   Input "oilIndex" is out of range
 * @retval -RT_ERR_NULL_POINTER     The "oilEntryParm" is NULL
 * @retval Others                   Please refer to drv_memset()
 *                                  Please refer to cva_drv_rt_oiltbl_get()
 */
RtkApiRet cva_cb_actsid_oiltbl_get(uint32 unit, uint32 oilIndex, CbActsidOiltblParm_t *oilEntryParm)
{
    RtkApiRet          retVal = RT_ERR_OK;
    cva_RtOilTblParm_t oilTableParm;
    do
    {
        /* Check input */
        if(CVA_FRER_OIL_INSTANCE_NUM_MAX <= oilIndex)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == oilEntryParm)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&oilTableParm, 0, sizeof(oilTableParm)));
        /* Get OIL table entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_rt_oiltbl_get(unit, oilIndex + CVA_FRER_OIL_TABLE_OFFSET, &oilTableParm));
        /* Convert routingEgrIndex to cbEgrIndex */
        if(CVA_FRER_EGIF_TABLE_OFFSET <= oilTableParm.egrIfIdx)
        {
            oilEntryParm->egifIdx = (uint8)oilTableParm.egrIfIdx - (uint8)CVA_FRER_EGIF_TABLE_OFFSET;
        }
        else
        {
            oilEntryParm->egifIdx = (uint8)oilTableParm.egrIfIdx;
        }
        oilEntryParm->oilnextValid = (uint8)(oilTableParm.oilNext >> 13u);
        if(CVA_FRER_OIL_TABLE_OFFSET <= (oilTableParm.oilNext & 0x1FFFu))
        {
            oilEntryParm->nextOilIdx = (oilTableParm.oilNext & 0x1FFFu) - (uint16)CVA_FRER_OIL_TABLE_OFFSET;
        }
        else
        {
            oilEntryParm->nextOilIdx = (oilTableParm.oilNext & 0x1FFFu);
        }
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Set the egress interface entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] egifIndex  Egress interface entry index (0 to 95) \n
 *                       Egress interface entry index
 * @param[in] actegifParm Egress interface entry parameter struct \n
 *                        Pointer that points to egress interface entry parameters
 * @param[out] None
 * @retval RT_ERR_OK                Egress interface entry was set
 * @retval Others                   Please refer to cva_drv_cb_actsid_egiftbl_checkif(), drv_memset() \n
 *                                  cva_drv_cb_actsid_param2egifparam() and Please refer to cva_drv_rt_egriftbl_set()
 */
RtkApiRet cva_cb_actsid_egiftbl_set(uint32 unit, uint32 egifIndex, CbActiveEgifTblParm_t *actegifParm)
{
    RtkApiRet           retVal = RT_ERR_OK;
    cva_RtEgIfTblParm_t egifTableParm;
    do
    {
        /* Check input parameters */
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_actsid_egiftbl_checkif(egifIndex, actegifParm));
        CHK_FUN_CONTINUE(retVal, drv_memset(&egifTableParm, 0, sizeof(cva_RtEgIfTblParm_t)));
        CHK_FUN_CONTINUE(retVal, cva_drv_cb_actsid_param2egifparam(actegifParm, &egifTableParm));
        /* Set egif table entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_rt_egriftbl_set(unit, egifIndex + CVA_FRER_EGIF_TABLE_OFFSET, &egifTableParm));
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the egress interface entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] egifIndex  Egress interface entry index (0 to 95) \n
 *                       Egress interface entry index
 * @param[out] actegifParm Egress interface entry parameter struct \n
 *                         Pointer that points to egress interface entry parameters
 * @retval RT_ERR_OK                Egress interface entry was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX   Input "egifIndex" is out of range
 * @retval -RT_ERR_NULL_POINTER     The "actegifParm" is NULL
 * @retval Others                   Please refer to drv_memset()
 *                                  Please refer to cva_drv_rt_egriftbl_get()
 */
RtkApiRet cva_cb_actsid_egiftbl_get(uint32 unit, uint32 egifIndex, CbActiveEgifTblParm_t *actegifParm)
{
    RtkApiRet           retVal = RT_ERR_OK;
    cva_RtEgIfTblParm_t egifTableParm;
    uint8               replaceMac[MAC_ADDR_LEN];

    do
    {
        /* Check input */
        if(CVA_FRER_EGIF_INSTANCE <= egifIndex)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        /* Check NULL pointer */
        if(NULL == actegifParm)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(replaceMac, 0, MAC_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, drv_memset(&egifTableParm, 0, sizeof(egifTableParm)));
        /* Get egif table entry */
        CHK_FUN_CONTINUE(retVal, cva_drv_rt_egriftbl_get(unit, egifIndex + CVA_FRER_EGIF_TABLE_OFFSET, &egifTableParm));
        actegifParm->saReplaceEn  = egifTableParm.macFmtSel;
        actegifParm->daReplaceEn  = egifTableParm.macReplace;
        actegifParm->vidReplaceEn = egifTableParm.vidReplace;
        actegifParm->pcpReplaceEn = egifTableParm.pcpReplace;
        actegifParm->mac[0]       = (uint8)(egifTableParm.macId47to32 >> 8u);
        actegifParm->mac[1]       = (uint8)(egifTableParm.macId47to32 & 0xFFu);
        actegifParm->mac[2]       = (uint8)(egifTableParm.macId31to0 >> 24u);
        actegifParm->mac[3]       = (uint8)(egifTableParm.macId31to0 >> 16u);
        actegifParm->mac[4]       = (uint8)(egifTableParm.macId31to0 >> 8u);
        actegifParm->mac[5]       = (uint8)(egifTableParm.macId31to0 & 0xFFu);
        actegifParm->vid          = egifTableParm.vid;
        actegifParm->pcp          = egifTableParm.pcp;
    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* ACTIVE_SID_ASIC */
/**@}*/ /* ACTIVE_SID */
/**
 * @addtogroup CB_COUNTER CB counters
 * @{
 */
/**
 * @defgroup CB_COUNTER_ASIC Low Level Driver
 * @brief CB Low Level Driver
 * @{
 */
/**
 * @brief Get passive per-stream identification counter value
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                      Entry index (0 to 95) \n
 *                                       Passive stream identification entry index
 * @param[out] identityNum               Per-stream identification counter value (0 to 0xFFFFFFFFFFFFFFFF) \n
 *                                       Records the number of packets identified by stream identification
 * @retval RT_ERR_OK                     Per-stream identification counter value was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX        Input "index" is out of range
 * @retval -RT_ERR_NULL_POINTER          The "identityNum" is NULL
 * @retval -RT_ERR_CB_INTERNAL_CMD       The execution of the internal channel has not been completed
 * @retval Others                        Please refer to cva_reg_write()
 *                                       Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_ingress_perstream_idcnt_get(uint32 unit, uint32 index, uint64 *identityNum)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    addrcmd       = 0;
    uint32    intercmd      = 0;
    uint32    interdatal    = 0;
    uint32    interdatah    = 0;
    uint32    registerIndex = 0;
    do
    {
        /* Check input */
        if(CVA_TSN_SID_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        if(NULL == identityNum)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* ------------Step 1: write read command------------ */
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_0;
        /* Set internal access cmd */
        addrcmd  = (uint32)CVA_STREAMID_CNTPERSTERAM_INERNAL_ADDR + index;
        intercmd = ((uint32)CVA_EXECUTE_ACCESS << CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP)
                   | ((uint32)CVA_INTERNALREAD << CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_WR_LSP) | addrcmd;
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, registerIndex, intercmd));
        /* ------------Step 2: Get Execute register to check value is 0 ------------ */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LEN, &intercmd));
        if(CVA_COMPLETE_ACCESS != intercmd)
        {
            retVal = -RT_ERR_CB_INTERNAL_CMD;
            continue;
        }
        /* ------------Step 3: Read Internal Data------------ */
        /* Get internal data register */
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_1;
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LEN, &interdatal));
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_2;
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LEN, &interdatah));

        *identityNum = ((uint64)interdatah << 32) | (uint64)interdatal;
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get passive per-port identification counter value
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] port                       Port (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_11TH) \n
 *                                       Port number
 * @param[out] identityNum               Per-port identification counter value (0 to 0xFFFFFFFFFFFFFFFF) \n
 *                                       Records the number of packets identified by stream identification on this port
 * @retval RT_ERR_OK                     Per-port identification counter value was gotten
 * @retval -RT_ERR_CB_PORT               Input "port" is out of range
 * @retval -RT_ERR_NULL_POINTER          The "identityNum" is NULL
 * @retval -RT_ERR_CB_INTERNAL_CMD       The execution of the internal channel has not been completed
 * @retval Others                        Please refer to cva_reg_write()
 *                                       Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_ingress_perport_idcnt_get(uint32 unit, RtkPort port, uint64 *identityNum)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    addrcmd       = 0;
    uint32    intercmd      = 0;
    uint32    interdatal    = 0;
    uint32    interdatah    = 0;
    uint32    registerIndex = 0;
    do
    {
        /* Check input */
        if((CVA_RTK_USER_PORT_1ST > port) || (CVA_RTK_USER_PORT_11TH < port))
        {
            retVal = -RT_ERR_CB_PORT;
            continue;
        }
        if(NULL == identityNum)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* ------------Step 1: write read command------------ */
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_0;
        /* Set internal access cmd */
        addrcmd  = (uint32)CVA_STREAMID_CNTPERPORT_INERNAL_ADDR + (uint32)cva_g_userPort2sysPort[port];
        intercmd = ((uint32)CVA_EXECUTE_ACCESS << CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP)
                   | ((uint32)CVA_INTERNALREAD << CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_WR_LSP) | addrcmd;
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, registerIndex, intercmd));
        /* ------------Step 2: Get Execute register to check value is 0 ------------ */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LEN, &intercmd));
        if(CVA_COMPLETE_ACCESS != intercmd)
        {
            retVal = -RT_ERR_CB_INTERNAL_CMD;
            continue;
        }
        /* ------------Step 3: Get Internal Data register------------ */
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_1;
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LEN, &interdatal));
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_2;
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LEN, &interdatah));
        *identityNum = ((uint64)interdatah << 32) | (uint64)interdatal;

    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the counter value for packets that have dropped by the sequence recovery entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                      Entry index (0 to 47) \n
 *                                       Sequence recovery entry index
 * @param[out] dropcnt                   The counter value of dropped packets (0 to 0xFFFFFFFFFFFFFFFF) \n
 *                                       Records the number of packets dropped by Sequence recovery
 * @retval RT_ERR_OK                     The counter value of dropped packets was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX        Input "index" is out of range
 * @retval -RT_ERR_NULL_POINTER          The "dropcnt" is NULL
 * @retval -RT_ERR_CB_INTERNAL_CMD       The execution of the internal channel has not been completed
 * @retval Others                        Please refer to cva_reg_write()
 *                                       Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_egress_seqrcv_dropcnt_get(uint32 unit, uint32 index, uint64 *dropcnt)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    addrcmd       = 0;
    uint32    intercmd      = 0;
    uint32    interdatal    = 0;
    uint32    interdatah    = 0;
    uint32    registerIndex = 0;
    do
    {
        /* Check input */
        if(CVA_FRER_SEQRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        if(NULL == dropcnt)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* ------------Step 1: write read command------------ */
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_0;
        /* Set internal access cmd */
        addrcmd  = (uint32)CVA_SEQRCV_CNTDROP_INERNAL_ADDR + (uint32)index;
        intercmd = ((uint32)CVA_EXECUTE_ACCESS << CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP)
                   | ((uint32)CVA_INTERNALREAD << CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_WR_LSP) | addrcmd;
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, registerIndex, intercmd));
        /* ------------Step 2: Get Execute register to check value is 0 ------------ */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LEN, &intercmd));
        if(CVA_COMPLETE_ACCESS != intercmd)
        {
            retVal = -RT_ERR_CB_INTERNAL_CMD;
            continue;
        }
        /* ------------Step 3: Get Internal Data register------------ */
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_1;
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LEN, &interdatal));
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_2;
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LEN, &interdatah));
        *dropcnt = ((uint64)interdatah << 32) | (uint64)interdatal;
    } while(0u == 1u);
    return retVal;
}
/**
 * @brief Get the counter value for packets that have passed through the sequence recovery entry.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] index                     Entry index (0 to 47) \n
 *                                      Sequence recovery entry index
 * @param[out] passcnt                  The counter value of passed packets (0 to 0xFFFFFFFFFFFFFFFF) \n
 *                                      Records the number of packets passed by Sequence recovery
 * @retval RT_ERR_OK                    The counter value of passed packets was gotten
 * @retval -RT_ERR_CB_ENTRY_INDEX       Input "index" is out of range
 * @retval -RT_ERR_NULL_POINTER         The "passcnt" is NULL
 * @retval -RT_ERR_CB_INTERNAL_CMD      The execution of the internal channel has not been completed
 * @retval Others                       Please refer to cva_reg_write()
 *                                      Please refer to cva_reg_field_read()
 */
RtkApiRet cva_cb_egress_seqrcv_passcnt_get(uint32 unit, uint32 index, uint64 *passcnt)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    addrcmd       = 0;
    uint32    intercmd      = 0;
    uint32    interdatal    = 0;
    uint32    interdatah    = 0;
    uint32    registerIndex = 0;
    do
    {
        /* Check input */
        if(CVA_FRER_SEQRCV_INSTANCE_NUM_MAX <= index)
        {
            retVal = -RT_ERR_CB_ENTRY_INDEX;
            continue;
        }
        if(NULL == passcnt)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* ------------Step 1: write read command------------ */
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_0;
        /* Set internal access cmd */
        addrcmd  = (uint32)CVA_SEQRCV_CNTPASS_INERNAL_ADDR + (uint32)index;
        intercmd = ((uint32)CVA_EXECUTE_ACCESS << CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP)
                   | ((uint32)CVA_INTERNALREAD << CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_WR_LSP) | addrcmd;
        CHK_FUN_CONTINUE(retVal, cva_reg_write(unit, registerIndex, intercmd));
        /* ------------Step 2: Get Execute register to check value is 0 ------------ */
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LEN, &intercmd));
        if(CVA_COMPLETE_ACCESS != intercmd)
        {
            retVal = -RT_ERR_CB_INTERNAL_CMD;
            continue;
        }
        /* ------------Step 3: Get Internal Data register------------ */
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_1;
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LEN, &interdatal));
        registerIndex = CVA_FRER_INTERNAL_ACCESS_CHANNEL_2;
        CHK_FUN_CONTINUE(retVal, cva_reg_field_read(unit, registerIndex, CVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LSP, CVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LEN, &interdatah));
        *passcnt = ((uint64)interdatah << 32) | (uint64)interdatal;
    } while(0u == 1u);
    return retVal;
}
/**@}*/ /* CB_COUNTER_ASIC */
/**@}*/ /* CB_COUNTER */
/**@}*/ /* CB */
