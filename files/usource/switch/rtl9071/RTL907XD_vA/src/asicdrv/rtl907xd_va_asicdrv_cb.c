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

/* Function Name:
 *       asicdrv_cb_ipv4string2param
 * Description:
 *       convert input ipv4 string to ipv4 parameter
 * Input:
 *       ipv4strBuffer            -input ipv4addr string
 * Output:
 *       ipv4param            -output ipv4 parameter
 * Return:
 *       None
 *
 * Note:
 *
 */

#include "rtkas_common_types.h"
#include "rtl907xd_va_api_cb.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_table_struct.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv_cb.h"
#include "rtkas_system_porting.h"
#include "rtl907xd_va_api_tsn.h"
#include "rtl907xd_va_asicdrv_tcam.h"
/* Function Name:
 *       asicdrv_cb_ipv6string2param
 * Description:
 *       convert input ipv6 string to ipv6 parameter
 * Input:
 *       ipv6strBuffer           -input ipv6 string
 * Output:
 *       ipv6param           -output ipv6 parameter
 * Return:
 *       None
 *
 * Note:
 *
 */

void dva_drv_cb_gen_ipv6array(uint32 *ipaddrentry, uint8 *ipaddrarray)
{
    ipaddrarray[0]  = (uint8)(ipaddrentry[0] >> 24);
    ipaddrarray[1]  = (uint8)(ipaddrentry[0] >> 16);
    ipaddrarray[2]  = (uint8)(ipaddrentry[0] >> 8);
    ipaddrarray[3]  = (uint8)(ipaddrentry[0] & 0xFFU);
    ipaddrarray[4]  = (uint8)(ipaddrentry[1] >> 24);
    ipaddrarray[5]  = (uint8)(ipaddrentry[1] >> 16);
    ipaddrarray[6]  = (uint8)(ipaddrentry[1] >> 8);
    ipaddrarray[7]  = (uint8)(ipaddrentry[1] & 0xFFU);
    ipaddrarray[8]  = (uint8)(ipaddrentry[2] >> 24);
    ipaddrarray[9]  = (uint8)(ipaddrentry[2] >> 16);
    ipaddrarray[10] = (uint8)(ipaddrentry[2] >> 8);
    ipaddrarray[11] = (uint8)(ipaddrentry[2] & 0xFFU);
    ipaddrarray[12] = (uint8)(ipaddrentry[3] >> 24);
    ipaddrarray[13] = (uint8)(ipaddrentry[3] >> 16);
    ipaddrarray[14] = (uint8)(ipaddrentry[3] >> 8);
    ipaddrarray[15] = (uint8)(ipaddrentry[3] & 0xFFU);
}
static RtkApiRet dva_drv_cb_gen_ipv6entry(CbSidparam_t *sidentryParam, dva_CbSidTabParamIn_t *sidTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     srcipv6temp[IPV6_ADDR_LEN];
    uint8     dstipv6temp[IPV6_ADDR_LEN];
    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(srcipv6temp, 0, IPV6_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, drv_memset(dstipv6temp, 0, IPV6_ADDR_LEN));
        if(DVA_IPV4_FLAG == sidentryParam->ipFlag)
        {
            dstipv6temp[10] = 0xFFu;
            dstipv6temp[11] = 0xFFu;
            CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(&dstipv6temp[12], sidentryParam->dstIp, IPV4_ADDR_LEN));

            srcipv6temp[10] = 0xFFu;
            srcipv6temp[11] = 0xFFu;
            CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(&srcipv6temp[12], sidentryParam->srcIp, IPV4_ADDR_LEN));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(dstipv6temp, sidentryParam->dstIp, IPV6_ADDR_LEN));
            CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(srcipv6temp, sidentryParam->srcIp, IPV6_ADDR_LEN));
        }
        sidTabentry->dstIp[0] = ((uint32)dstipv6temp[0] << 24) | ((uint32)dstipv6temp[1] << 16)
                                | ((uint32)dstipv6temp[2] << 8) | ((uint32)dstipv6temp[3]);
        sidTabentry->dstIp[1] = ((uint32)dstipv6temp[4] << 24) | ((uint32)dstipv6temp[5] << 16)
                                | ((uint32)dstipv6temp[6] << 8) | ((uint32)dstipv6temp[7]);
        sidTabentry->dstIp[2] = ((uint32)dstipv6temp[8] << 24) | ((uint32)dstipv6temp[9] << 16)
                                | ((uint32)dstipv6temp[10] << 8) | ((uint32)dstipv6temp[11]);
        sidTabentry->dstIp[3] = ((uint32)dstipv6temp[12] << 24) | ((uint32)dstipv6temp[13] << 16)
                                | ((uint32)dstipv6temp[14] << 8) | ((uint32)dstipv6temp[15]);

        sidTabentry->srcIp[0] = ((uint32)srcipv6temp[0] << 24) | ((uint32)srcipv6temp[1] << 16)
                                | ((uint32)srcipv6temp[2] << 8) | ((uint32)srcipv6temp[3]);
        sidTabentry->srcIp[1] = ((uint32)srcipv6temp[4] << 24) | ((uint32)srcipv6temp[5] << 16)
                                | ((uint32)srcipv6temp[6] << 8) | ((uint32)srcipv6temp[7]);
        sidTabentry->srcIp[2] = ((uint32)srcipv6temp[8] << 24) | ((uint32)srcipv6temp[9] << 16)
                                | ((uint32)srcipv6temp[10] << 8) | ((uint32)srcipv6temp[11]);
        sidTabentry->srcIp[3] = ((uint32)srcipv6temp[12] << 24) | ((uint32)srcipv6temp[13] << 16)
                                | ((uint32)srcipv6temp[14] << 8) | ((uint32)srcipv6temp[15]);

    } while(0u == 1u);
    return retVal;
}
static RtkApiRet dva_drv_cb_set_sid_valentry(dva_CbSidTabEntry_t *const sidTabentry, dva_CbSidTabParamIn_t *const sidTabParam)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     i      = 0;
    do
    {
        for(i = 0u; i < DVA_MAC_FIG_BYTES_NUM; i++)
        {
            sidTabParam->dstMac[i] &= 0xFFu;
        }
        for(i = 0u; i < DVA_MAC_FIG_BYTES_NUM; i++)
        {
            sidTabParam->srcMac[i] &= 0xFFu;
        }
        sidTabParam->ethertype &= 0xFFFFu;

        sidTabParam->vlanId &= 0xFFFu;
        for(i = 0u; (i < (DVA_IPV6_FIG_BYTES_NUM / 4u)); i++)
        {
            sidTabParam->srcIp[i] &= 0xFFFFFFFFu;
        }
        for(i = 0u; (i < (DVA_IPV6_FIG_BYTES_NUM / 4u)); i++)
        {
            sidTabParam->dstIp[i] &= 0xFFFFFFFFu;
        }
        sidTabParam->dscp &= 0x3Fu;
        sidTabParam->nextProtocol &= 0xFFu;
        sidTabParam->srcPort &= 0xFFFFu;
        sidTabParam->dstPort &= 0xFFFFu;
        sidTabParam->vlanPri &= 0x7u;
        sidTabParam->l4PortChkTblMap &= 0xFu;

        CHK_FUN_CONTINUE(retVal, drv_memset(sidTabentry, 0, sizeof(*sidTabentry)));

        sidTabentry->val[0] = (((uint32)sidTabParam->dstMac[5]) | ((uint32)sidTabParam->dstMac[4] << 8u) | ((uint32)sidTabParam->dstMac[3] << 16u) | ((uint32)sidTabParam->dstMac[2] << 24u));
        sidTabentry->val[1] = (((uint32)sidTabParam->dstMac[1]) | ((uint32)sidTabParam->dstMac[0] << 8u) | ((uint32)sidTabParam->srcMac[5] << 16u) | ((uint32)sidTabParam->srcMac[4] << 24u));
        sidTabentry->val[2] = (((uint32)sidTabParam->srcMac[3]) | ((uint32)sidTabParam->srcMac[2] << 8) | ((uint32)sidTabParam->srcMac[1] << 16u) | ((uint32)sidTabParam->srcMac[0] << 24u));

        sidTabentry->val[3] = (((uint32)sidTabParam->ethertype & 0xFFFFu)) | (((uint32)sidTabParam->vlanId & 0xFFFu) << 17u) | ((sidTabParam->srcIp[3] & 0x3u) << 30u);
        sidTabentry->val[4] = (((uint32)sidTabParam->srcIp[3] >> 2u) & 0x3FFFFFFFu) | (((uint32)sidTabParam->srcIp[2] & 0x3u) << 30u);
        sidTabentry->val[5] = (((uint32)sidTabParam->srcIp[2] >> 2u) & 0x3FFFFFFFu) | (((uint32)sidTabParam->srcIp[1] & 0x3u) << 30u);

        sidTabentry->val[7] = (((uint32)sidTabParam->srcIp[1] >> 2u) & 0x3FFFFFFFu) | (((uint32)sidTabParam->srcIp[0] & 0x3u) << 30u);
        sidTabentry->val[8] = (((uint32)sidTabParam->srcIp[0] >> 2u) & 0x3FFFFFFFu);

        sidTabentry->val[8]  = sidTabentry->val[8] | ((sidTabParam->dstIp[3] & 0x1u) << 31u);
        sidTabentry->val[9]  = ((sidTabParam->dstIp[3] >> 1u) & 0x7FFFFFFFu) | ((sidTabParam->dstIp[2] & 0x1u) << 31u);
        sidTabentry->val[10] = ((sidTabParam->dstIp[2] >> 1u) & 0x7FFFFFFFu) | ((sidTabParam->dstIp[1] & 0x1u) << 31u);
        sidTabentry->val[11] = ((sidTabParam->dstIp[1] >> 1u) & 0x7FFFFFFFu) | ((sidTabParam->dstIp[0] & 0x1u) << 31u);
        sidTabentry->val[12] = ((sidTabParam->dstIp[0] >> 1u) & 0x7FFFFFFFu);

        sidTabentry->val[13] = (((uint32)sidTabParam->dscp & 0x3Fu) | ((uint32)sidTabParam->nextProtocol << 7u) | ((uint32)sidTabParam->srcPort << 16u));
        sidTabentry->val[14] = ((uint32)sidTabParam->dstPort << 1u) | ((uint32)sidTabParam->vlanPri << 18u) | ((uint32)sidTabParam->l4PortChkTblMap << 21u);
        sidTabentry->val[15] = 0u;
    } while(0u == 1u);
    return retVal;
}
static void dva_drv_cb_sid_gen_wildcare_control(dva_CbSidTabEntry_t *const sidTabentry, dva_CbSidTabParamIn_t *const sidTabParam, uint16 *careBitsandCtrl,
                                                uint32 selectField, uint8 entryWord, uint16 carabitmask, int32 ctrlBitOffset)
{
    careBitsandCtrl[0] = 0u;
    careBitsandCtrl[1] = 0u;
    if(0u != (sidTabParam->carebits & selectField))
    {
        careBitsandCtrl[0] = carabitmask;
        careBitsandCtrl[1] = 0x1u;
    }
    else
    {
        careBitsandCtrl[0] = 0x0000u;
        careBitsandCtrl[1] = 0x0u;
        if(-1 != ctrlBitOffset)
        {
            sidTabentry->val[entryWord] |= ((uint32)1 << (uint8)ctrlBitOffset);
        }
    }
}

static RtkApiRet dva_drv_cb_sid_gen_ip_wildcare_control(dva_CbSidTabEntry_t *const sidTabentry, dva_CbSidTabParamIn_t *const sidTabParam, dva_CbSidTabParamIn_t *generatedWildcareBits,
                                                        uint32 selectField, uint8 entryWord, uint32 carabitmask, uint8 ctrlBitOffset)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     i      = 0;
    uint32    ip[(DVA_IPV6_FIG_BYTES_NUM / 4u)];
    uint8     ip_control = 0u;
    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(ip, 0, DVA_IPV6_FIG_BYTES_NUM));
        if(0u != (sidTabParam->carebits & selectField))
        {
            for(i = 0u; i < (DVA_IPV6_FIG_BYTES_NUM / 4u); i++)
            {
                ip[i] = carabitmask;
            }
            ip_control = 0x1u;
        }
        else
        {
            for(i = 0u; i < (DVA_IPV6_FIG_BYTES_NUM / 4u); i++)
            {
                ip[i] = 0x00u;
            }
            ip_control = 0x0u;
            sidTabentry->val[entryWord] |= ((uint32)1 << ctrlBitOffset);
        }
        if(DVA_TSN_SELECTED_FILED_SRCIP == selectField)
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(generatedWildcareBits->srcIp, ip, 16));
            generatedWildcareBits->sourceIpCtrl = ip_control;
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(generatedWildcareBits->dstIp, ip, 16));
            generatedWildcareBits->dstIpCtrl = ip_control;
        }
    } while(0u == 1u);
    return retVal;
}
static RtkApiRet dva_drv_cb_sid_gen_mac_wildcare_control(dva_CbSidTabEntry_t *const sidTabentry, dva_CbSidTabParamIn_t *const sidTabParam, dva_CbSidTabParamIn_t *generatedWildcareBits,
                                                         uint32 selectField, uint8 carabitmask)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint8     i      = 0;
    uint8     mac[6];
    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(mac, 0, 6));
        if(0u != (sidTabParam->carebits & selectField))
        {
            for(i = 0u; i < DVA_MAC_FIG_BYTES_NUM; i++)
            {
                mac[i] = carabitmask;
            }
        }
        else
        {
            for(i = 0u; i < DVA_MAC_FIG_BYTES_NUM; i++)
            {
                mac[i] = 0x00u;
            }
        }
        if(DVA_TSN_SELECTED_FILED_SRCADR == selectField)
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(generatedWildcareBits->srcMac, mac, 6));
        }
        else
        {
            CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(generatedWildcareBits->dstMac, mac, 6));
        }
    } while(0u == 1u);
    return retVal;
}
static RtkApiRet dva_asic_cb_wordto2byte(uint32 index, uint32 *inputVal, uint8 *output)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     dataTemp[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    if(0u == (index % 4u))
    {
        dataTemp[0] = (uint8)(inputVal[0] & 0xFFu);
        dataTemp[1] = (uint8)((inputVal[0] >> 8) & 0xFFu);
    }
    else if(1u == (index % 4u))
    {
        dataTemp[0] = (uint8)((inputVal[0] >> 16) & 0xFFu);
        dataTemp[1] = (uint8)((inputVal[0] >> 24) & 0xFFu);
    }
    else if(2u == (index % 4u))
    {
        dataTemp[0] = (uint8)(inputVal[1] & 0xFFu);
        dataTemp[1] = (uint8)((inputVal[1] >> 8) & 0xFFu);
    }
    else if(3u == (index % 4u))
    {
        dataTemp[0] = (uint8)((inputVal[1] >> 16) & 0xFFu);
        dataTemp[1] = (uint8)((inputVal[1] >> 24) & 0xFFu);
    }
    else
    {
        /* */
    }
    retVal = dva_drv_memcpy(output, dataTemp, 2u);
    return retVal;
}
static RtkApiRet dva_asic_cb_wordto4byte(uint32 index, uint32 *inputVal, uint8 *output)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     dataTemp[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    if(0u == (index % 2u))
    {
        dataTemp[0] = (uint8)((inputVal[0]) & 0xFFu);
        dataTemp[1] = (uint8)((inputVal[0] >> 8) & 0xFFu);
        dataTemp[2] = (uint8)((inputVal[0] >> 16) & 0xFFu);
        dataTemp[3] = (uint8)((inputVal[0] >> 24) & 0xFFu);
    }
    else if(1u == (index % 2u))
    {
        dataTemp[0] = (uint8)((inputVal[1]) & 0xFFu);
        dataTemp[1] = (uint8)((inputVal[1] >> 8) & 0xFFu);
        dataTemp[2] = (uint8)((inputVal[1] >> 16) & 0xFFu);
        dataTemp[3] = (uint8)((inputVal[1] >> 24) & 0xFFu);
    }
    else
    {
        /*  */
    }
    retVal = dva_drv_memcpy(output, dataTemp, 4u);
    return retVal;
}
static RtkApiRet dva_asic_cb_wordto1byte(uint32 index, uint32 *inputVal, uint8 *output)
{
    RtkApiRet retVal      = RT_ERR_OK;
    uint8     dataTemp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    if(DVA_INTERNAL_DATAENTRY_BIT63 >= index)
    {
        dataTemp[0] = ((uint8)((((uint64)inputVal[1] << 32u) | (uint64)inputVal[0]) >> index) & 0x1u);
    }
    else if(DVA_INTERNAL_DATAENTRY_BIT95 >= index)
    {
        dataTemp[0] = ((uint8)(inputVal[0] >> (index - 64u)) & 0x1u);
    }
    else
    {
        /*  */
    }
    retVal = dva_drv_memcpy(output, dataTemp, 1u);
    return retVal;
}
static RtkApiRet dva_asic_cb_word2byte(uint32 index, uint8 byteNum, uint32 *inputVal, uint8 *output)
{
    RtkApiRet retVal                                       = RT_ERR_OK;
    uint8     dataTemp[DVA_INTERNAL_PERDATAENTRY_BYTE_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};
    do
    {
        if(2u == byteNum)
        {
            CHK_FUN_CONTINUE(retVal, dva_asic_cb_wordto2byte(index, inputVal, output));
        }

        else if(4u == byteNum)
        {
            CHK_FUN_CONTINUE(retVal, dva_asic_cb_wordto4byte(index, inputVal, output));
        }
        else if(1u == byteNum)
        {
            CHK_FUN_CONTINUE(retVal, dva_asic_cb_wordto1byte(index, inputVal, output));
        }
        else if(8u == byteNum)
        {
            dataTemp[0] = (uint8)((inputVal[0]) & 0xFFu);
            dataTemp[1] = (uint8)((inputVal[0] >> 8) & 0xFFu);
            dataTemp[2] = (uint8)((inputVal[0] >> 16) & 0xFFu);
            dataTemp[3] = (uint8)((inputVal[0] >> 24) & 0xFFu);
            dataTemp[4] = (uint8)((inputVal[1]) & 0xFFu);
            dataTemp[5] = (uint8)((inputVal[1] >> 8) & 0xFFu);
            dataTemp[6] = (uint8)((inputVal[1] >> 16) & 0xFFu);
            dataTemp[7] = (uint8)((inputVal[1] >> 24) & 0xFFu);
            CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(output, dataTemp, 8u));
        }
        else
        {
            /* */
        }
    } while(0u == 1u);
    return retVal;
}
static RtkApiRet dva_asic_cb_read_internal_channel(uint32 unit, uint32 index, uint32 addr, uint8 *data, uint8 byteNum)
{
    RtkApiRet retVal                                       = RT_ERR_OK;
    uint32    intercmd                                     = 0;
    uint32    registerIndex                                = 0;
    uint32    interdata[2]                                 = {0, 0};
    uint8     datatemp[DVA_INTERNAL_PERDATAENTRY_BYTE_NUM] = {0, 0, 0, 0, 0, 0, 0, 0};

    do
    {
        /* read data from internal channel*/

        /* ------------Step1 write read command------------ */
        registerIndex = DVA_FRER_INTERNAL_ACCESS_CHANNEL_0;
        /* Set internal access cmd */
        intercmd = ((uint32)DVA_EXECUTE_ACCESS << DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP)
                   | ((uint32)DVA_INTERNALREAD << DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_WR_LSP) | addr;
        CHK_FUN_CONTINUE(retVal, dva_reg_write(unit, registerIndex, intercmd));
        /* ------------Step2 Get Execute register to check value is 0 ------------ */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, registerIndex, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LEN, &intercmd));
        if(DVA_COMPLETE_ACCESS != intercmd)
        {
            retVal = -RT_ERR_CB_INTERNAL_CMD;
            continue;
        }
        /* ------------Step3 Read Internal Data------------ */
        registerIndex = DVA_FRER_INTERNAL_ACCESS_CHANNEL_1;
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, registerIndex, DVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LSP, DVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LEN, &interdata[0]));
        registerIndex = DVA_FRER_INTERNAL_ACCESS_CHANNEL_2;
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, registerIndex, DVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LSP, DVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LEN, &interdata[1]));
        CHK_FUN_CONTINUE(retVal, dva_asic_cb_word2byte(index, byteNum, interdata, datatemp));
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(data, datatemp, 8));

    } while(0u == 1u);
    return retVal;
}
static RtkApiRet dva_drv_cb_read_1bit(uint32 unit, uint32 index, uint32 *regAddr, uint32 regLsp, uint32 regLen, uint8 *data)
{
    RtkApiRet retVal       = RT_ERR_OK;
    uint32    indextemp    = 0;
    uint32    registerTemp = 0;
    do
    {
        if(31u >= index)
        {
            indextemp = index;
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regAddr[0], regLsp, regLen, &registerTemp));
            *data = (uint8)((registerTemp >> indextemp) & 0x1u);
        }
        else if(63u >= index)
        {
            indextemp = index - 32u;
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regAddr[1], regLsp, regLen, &registerTemp));
            *data = (uint8)((registerTemp >> indextemp) & 0x1u);
        }
        else if(95u >= index)
        {
            indextemp = index - 64u;
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regAddr[2], regLsp, regLen, &registerTemp));
            *data = (uint8)((registerTemp >> indextemp) & 0x1u);
        }
        else
        {
            /* Do nothing */
        }
    } while(0u == 1u);
    return retVal;
}
static RtkApiRet dva_drv_cb_write_1bit(uint32 unit, uint32 index, uint32 *registerIndex, uint32 regLsp, uint32 regLen, uint8 data)
{
    RtkApiRet retVal    = RT_ERR_OK;
    uint32    dataTemp  = 0;
    uint32    addr      = 0;
    uint32    indextemp = 0;
    do
    {
        if(31u >= index)
        {
            indextemp = index;
            addr      = registerIndex[0];
        }
        else if(63u >= index)
        {
            indextemp = index - 32u;
            addr      = registerIndex[1];
        }
        else if(95u >= index)
        {
            indextemp = index - 64u;
            addr      = registerIndex[2];
        }
        else
        {
            /*    */
        }
        /*write */
        if(1u == data)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, addr, regLsp, regLen, &dataTemp));
            dataTemp |= ((uint32)1 << indextemp);
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, addr, regLsp, regLen, dataTemp));
        }
        else if(0u == data)
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, addr, regLsp, regLen, &dataTemp));
            dataTemp &= ~((uint32)1 << indextemp);
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, addr, regLsp, regLen, dataTemp));
        }
        else
        {
            /*    */
        }
    } while(0u == 1u);
    return retVal;
}
static RtkApiRet dva_drv_cb_read_8bits(uint32 unit, uint32 index, uint32 regAddr, uint32 *regLsp, uint32 regLen, uint32 *data)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        if(0u == (index % 4u))
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regAddr, regLsp[0], regLen, data));
        }
        if(1u == (index % 4u))
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regAddr, regLsp[1], regLen, data));
        }
        if(2u == (index % 4u))
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regAddr, regLsp[2], regLen, data));
        }
        if(3u == (index % 4u))
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, regAddr, regLsp[3], regLen, data));
        }

    } while(0u == 1u);
    return retVal;
}
static RtkApiRet dva_drv_cb_write_8bits(uint32 unit, uint32 index, uint32 regAddr, uint32 *regLsp, uint32 regLen, uint32 data)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        if(0u == (index % 4u))
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regAddr, regLsp[0], regLen, data));
        }
        if(1u == (index % 4u))
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regAddr, regLsp[1], regLen, data));
        }
        if(2u == (index % 4u))
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regAddr, regLsp[2], regLen, data));
        }
        if(3u == (index % 4u))
        {
            CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, regAddr, regLsp[3], regLen, data));
        }
        else
        {
            /*  */
        }
    } while(0u == 1u);

    return retVal;
}
static void dva_drv_cb_read_carebits_data(dva_CbSidTabEntry_t *const sidTabentry, dva_CbSidTabParamIn_t *const sidTabParam, dva_CbSidParamGetInfo_t *sidParamGetInfo, uint16 *data)
{

    uint8 ctrlBit = 0u;
    *data         = 0u;
    ctrlBit       = (uint8)((sidTabentry->val[sidParamGetInfo->ctrlBitWord] >> sidParamGetInfo->ctrlBitoffset) & sidParamGetInfo->ctrlBitmask);
    if(sidParamGetInfo->ctrlBitmask == ctrlBit)
    {
        sidTabParam->carebits |= sidParamGetInfo->carebitmask;
        *data = (uint16)((sidTabentry->val[sidParamGetInfo->fieldWord] >> sidParamGetInfo->fieldoffset) & sidParamGetInfo->fieldmask);
    }
    else
    {
        *data = (uint16)((sidTabentry->val[sidParamGetInfo->fieldWord] >> sidParamGetInfo->fieldoffset) & sidParamGetInfo->fieldmask);
    }
}
RtkApiRet dva_drv_cb_actsid_param2egifparam(CbActiveEgifTblParm_t *actegifParm, dva_RtEgIfTblParm_t *egifTableParm)
{
    RtkApiRet retVal = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(egifTableParm, 0, sizeof(dva_RtEgIfTblParm_t)));
        egifTableParm->macFmtSel  = actegifParm->saReplaceEn;
        egifTableParm->macReplace = actegifParm->daReplaceEn;

        egifTableParm->vidReplace = actegifParm->vidReplaceEn;
        egifTableParm->pcpReplace = actegifParm->pcpReplaceEn;
        egifTableParm->macId31to0 = ((uint32)actegifParm->mac[2] << 24u)
                                    | ((uint32)actegifParm->mac[3] << 16u)
                                    | ((uint32)actegifParm->mac[4] << 8u)
                                    | ((uint32)actegifParm->mac[5]);
        egifTableParm->macId47to32 = ((uint16)actegifParm->mac[0] << 8u)
                                     | ((uint16)actegifParm->mac[1]);
        egifTableParm->vid = actegifParm->vid;
        egifTableParm->pcp = actegifParm->pcp;
    } while(0u == 1u);
    return retVal;
}
RtkApiRet dva_drv_cb_misc_check_enable_status(CbLutParam_t *lutTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        if((DVA_ENABLED != lutTabentry->encEn) && (DVA_DISABLED != lutTabentry->encEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_ENABLED != lutTabentry->indRcvEn) && (DVA_DISABLED != lutTabentry->indRcvEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_ENABLED != lutTabentry->seqRcvEn) && (DVA_DISABLED != lutTabentry->seqRcvEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_ENABLED != lutTabentry->splitEn) && (DVA_DISABLED != lutTabentry->splitEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_ENABLED != lutTabentry->genSeqNumEn) && (DVA_DISABLED != lutTabentry->genSeqNumEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_ENABLED != lutTabentry->activeEn) && (DVA_DISABLED != lutTabentry->activeEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);
    return retVal;
}
RtkApiRet dva_drv_cb_misc_check_value_range(CbLutParam_t *lutTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        if(DVA_FRER_INDIVRCV_INSTANCE_NUM_MAX <= lutTabentry->indRcvIdx)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        if(DVA_FRER_SEQRCV_INSTANCE_NUM_MAX <= lutTabentry->seqRcvIdx)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }

        if((0u != (lutTabentry->splitDPM & 0x1u)) || (DVA_FRER_USER_PMSK_MAX < lutTabentry->splitDPM))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if(DVA_FRER_RH_INSTANCE_MAX <= lutTabentry->activeIdx)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);
    return retVal;
}
/* Function Name:
 *      dva_drv_cb_sid_param2entry
 * Description:
 *      convert cb stream identification parameter to entry
 * Input:
 *      sidTabParam           -stream identification param
 * Output:
 *      sidTabentry         -stream identification entry
 * Return:
 *      RT_ERR_OK - Success.
 *      RT_ERR_NULL_POINTER - null pointer

 * Note:
 *
 *
 */
RtkApiRet dva_drv_cb_sid_param2entry(dva_CbSidTabEntry_t *const sidTabentry, dva_CbSidTabParamIn_t *const sidTabParam)
{
    RtkApiRet             retVal = RT_ERR_OK;
    dva_CbSidTabParamIn_t generatedWildcareBits;
    uint16                sidParamGetInfo[2] = {0u, 0u};

    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(&generatedWildcareBits, 0, sizeof(generatedWildcareBits)));
        /*check NULL pointer*/
        if((NULL == sidTabentry) || (NULL == sidTabParam))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /*initial sidTabParam*/
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_set_sid_valentry(sidTabentry, sidTabParam));
        /*generation DSTADR wildcard and control*/
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_sid_gen_mac_wildcare_control(sidTabentry, sidTabParam, &generatedWildcareBits, DVA_TSN_SELECTED_FILED_DSTADR, 0xFFu));
        /*generation SRCADR wildcard and control*/
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_sid_gen_mac_wildcare_control(sidTabentry, sidTabParam, &generatedWildcareBits, DVA_TSN_SELECTED_FILED_SRCADR, 0xFFu));

        /*generation ETHTERTYPE wildcard and control*/
        dva_drv_cb_sid_gen_wildcare_control(sidTabentry, sidTabParam, sidParamGetInfo, DVA_TSN_SELECTED_FILED_ETHTERTYPE, 3u, 0xFFFFu, 16);
        generatedWildcareBits.ethertype     = sidParamGetInfo[0];
        generatedWildcareBits.ethertypeCtrl = (uint8)sidParamGetInfo[1];
        /*generation VID wildcard and control*/
        dva_drv_cb_sid_gen_wildcare_control(sidTabentry, sidTabParam, sidParamGetInfo, DVA_TSN_SELECTED_FILED_VID, 3u, 0xFFFu, 29);
        generatedWildcareBits.vlanId     = sidParamGetInfo[0];
        generatedWildcareBits.vlanIdCtrl = (uint8)sidParamGetInfo[1];
        /*generation SRCIP wildcard and control*/
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_sid_gen_ip_wildcare_control(sidTabentry, sidTabParam, &generatedWildcareBits, DVA_TSN_SELECTED_FILED_SRCIP, 8u, 0xFFFFFFFFu, 30u));

        /*generation DSTIP wildcard and control*/
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_sid_gen_ip_wildcare_control(sidTabentry, sidTabParam, &generatedWildcareBits, DVA_TSN_SELECTED_FILED_DSTIP, 12u, 0xFFFFFFFFu, 31u));

        /*generation DSCP wildcard and control*/
        dva_drv_cb_sid_gen_wildcare_control(sidTabentry, sidTabParam, sidParamGetInfo, DVA_TSN_SELECTED_FILED_DSCP, 13u, 0x3Fu, 6);
        generatedWildcareBits.dscp     = (uint8)sidParamGetInfo[0];
        generatedWildcareBits.dscpCtrl = (uint8)sidParamGetInfo[1];
        /*generation NEXTPROC wildcard and control*/
        dva_drv_cb_sid_gen_wildcare_control(sidTabentry, sidTabParam, sidParamGetInfo, DVA_TSN_SELECTED_FILED_NEXTPROC, 13u, 0xFFu, 15);
        generatedWildcareBits.nextProtocol     = (uint8)sidParamGetInfo[0];
        generatedWildcareBits.nextProtocolCtrl = (uint8)sidParamGetInfo[1];
        /*generation SRCPORT wildcard and control*/
        dva_drv_cb_sid_gen_wildcare_control(sidTabentry, sidTabParam, sidParamGetInfo, DVA_TSN_SELECTED_FIELD_SRCPORT, 14u, 0xFFFFu, 0);
        generatedWildcareBits.srcPort     = sidParamGetInfo[0];
        generatedWildcareBits.srcPortCtrl = (uint8)sidParamGetInfo[1];
        /*generation DSTPORT wildcard and control*/
        dva_drv_cb_sid_gen_wildcare_control(sidTabentry, sidTabParam, sidParamGetInfo, DVA_TSN_SELECTED_FIELD_DSTPORT, 14u, 0xFFFFu, 17);
        generatedWildcareBits.dstPort     = sidParamGetInfo[0];
        generatedWildcareBits.dstPortCtrl = (uint8)sidParamGetInfo[1];

        /*generation VLANPRI wildcard and control*/
        dva_drv_cb_sid_gen_wildcare_control(sidTabentry, sidTabParam, sidParamGetInfo, DVA_TSN_SELECTED_FIELD_VIDPRI, 14u, 0x7u, -1);
        generatedWildcareBits.vlanPri = (uint8)sidParamGetInfo[0];

        /*generation L4PORTRANGE wildcard and control*/
        dva_drv_cb_sid_gen_wildcare_control(sidTabentry, sidTabParam, sidParamGetInfo, DVA_TSN_SELECTED_FIELD_L4PORTRANGE, 14u, 0xFu, -1);

        generatedWildcareBits.l4PortChkTblMap = (uint8)sidParamGetInfo[0];

        sidTabentry->val[16] = ((uint32)(generatedWildcareBits.dstMac[5]) | ((uint32)generatedWildcareBits.dstMac[4] << 8u)
                                | ((uint32)generatedWildcareBits.dstMac[3] << 16u) | ((uint32)generatedWildcareBits.dstMac[2] << 24u));
        sidTabentry->val[17] = (((uint32)generatedWildcareBits.dstMac[1]) | ((uint32)generatedWildcareBits.dstMac[0] << 8u)
                                | ((uint32)generatedWildcareBits.srcMac[5] << 16u) | ((uint32)generatedWildcareBits.srcMac[4] << 24u));
        sidTabentry->val[18] = (((uint32)generatedWildcareBits.srcMac[3]) | ((uint32)generatedWildcareBits.srcMac[2] << 8)
                                | ((uint32)generatedWildcareBits.srcMac[1] << 16) | ((uint32)generatedWildcareBits.srcMac[0] << 24u));

        sidTabentry->val[19] = (((uint32)generatedWildcareBits.ethertype & 0xFFFFu) | ((uint32)generatedWildcareBits.ethertypeCtrl << 16u)
                                | (((uint32)generatedWildcareBits.vlanId & 0xFFFu) << 17u) | ((uint32)generatedWildcareBits.vlanIdCtrl << 29u) | (((uint32)generatedWildcareBits.srcIp[3] & 0x3u) << 30u));
        sidTabentry->val[20] = ((generatedWildcareBits.srcIp[3] >> 2) & 0x3FFFFFFFu) | ((generatedWildcareBits.srcIp[2] & 0x3u) << 30u);
        sidTabentry->val[21] = ((generatedWildcareBits.srcIp[2] >> 2) & 0x3FFFFFFFu) | ((generatedWildcareBits.srcIp[1] & 0x3u) << 30u);
        sidTabentry->val[22] = ((generatedWildcareBits.srcIp[1] >> 2) & 0x3FFFFFFFu) | ((generatedWildcareBits.srcIp[0] & 0x3u) << 30u);
        sidTabentry->val[23] = ((generatedWildcareBits.srcIp[0] >> 2) & 0x3FFFFFFFu) | ((uint32)generatedWildcareBits.sourceIpCtrl << 30u) | ((generatedWildcareBits.dstIp[3] & 0x1u) << 31u);
        sidTabentry->val[24] = ((generatedWildcareBits.dstIp[3] >> 1) & 0x7FFFFFFFu) | ((generatedWildcareBits.dstIp[2] & 0x1u) << 31u);
        sidTabentry->val[25] = ((generatedWildcareBits.dstIp[2] >> 1) & 0x7FFFFFFFu) | ((generatedWildcareBits.dstIp[1] & 0x1u) << 31u);
        sidTabentry->val[26] = ((generatedWildcareBits.dstIp[1] >> 1) & 0x7FFFFFFFu) | ((generatedWildcareBits.dstIp[0] & 0x1u) << 31u);
        sidTabentry->val[27] = ((generatedWildcareBits.dstIp[0] >> 1) & 0x7FFFFFFFu) | ((uint32)generatedWildcareBits.dstIpCtrl << 31u);

        sidTabentry->val[28] = (((uint32)generatedWildcareBits.dscp & 0x3Fu) | ((uint32)generatedWildcareBits.dscpCtrl << 6u) | ((uint32)generatedWildcareBits.nextProtocol << 7u)
                                | ((uint32)generatedWildcareBits.nextProtocolCtrl << 15u) | ((uint32)generatedWildcareBits.srcPort << 16u));
        sidTabentry->val[29] = (((uint32)generatedWildcareBits.srcPortCtrl) | ((uint32)generatedWildcareBits.dstPort << 1u) | ((uint32)generatedWildcareBits.dstPortCtrl << 17u)
                                | ((uint32)generatedWildcareBits.vlanPri << 18u) | ((uint32)generatedWildcareBits.l4PortChkTblMap << 21u));
        sidTabentry->val[30] = 0u;
        sidTabentry->val[6]  = (uint32)sidTabParam->isValid;
    } while(0u == 1u);
    return retVal;
}
/* Function Name:
 *      dva_drv_cb_sid_entry2param
 * Description:
 *      convert cb stream identification entry to parameter
 * Input:
 *      sidTabentry     -stream identification entry
 * Output:
 *      sidTabParam     -stream identification parameter
 * Return:
 *      RT_ERR_OK - Success.
 * Note:
 *
 *
 */
RtkApiRet dva_drv_cb_sid_entry2param(dva_CbSidTabEntry_t *const sidTabentry, dva_CbSidTabParamIn_t *const sidTabParam)
{
    RtkApiRet               retVal = RT_ERR_OK;
    uint16                  data   = 0u;
    dva_CbSidParamGetInfo_t sidParamGetInfo;
    uint32                  ipTemp[4];
    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(ipTemp, 0, 16u));
        CHK_FUN_CONTINUE(retVal, drv_memset(sidTabParam, 0, sizeof(dva_CbSidTabParamIn_t)));
        CHK_FUN_CONTINUE(retVal, drv_memset(&sidParamGetInfo, 0, sizeof(dva_CbSidParamGetInfo_t)));
        /*check NULL pointer*/
        if((NULL == sidTabentry) || (NULL == sidTabParam))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        sidTabParam->isValid = (uint8)((sidTabentry->val[6]) & 0x01u);
        /* initial */
        sidTabParam->carebits = 0u;
        /* wildcare bits of DA */
        if(0xFFFFFFFFu == sidTabentry->val[16])
        {
            sidTabParam->carebits |= DVA_TSN_SELECTED_FILED_DSTADR;
            sidTabParam->dstMac[0] = (uint8)((sidTabentry->val[1] >> 8u) & 0xFFu);
            sidTabParam->dstMac[1] = (uint8)((sidTabentry->val[1]) & 0xFFu);
            sidTabParam->dstMac[2] = (uint8)((sidTabentry->val[0] >> 24u) & 0xFFu);
            sidTabParam->dstMac[3] = (uint8)((sidTabentry->val[0] >> 16u) & 0xFFu);
            sidTabParam->dstMac[4] = (uint8)((sidTabentry->val[0] >> 8u) & 0xFFu);
            sidTabParam->dstMac[5] = (uint8)((sidTabentry->val[0]) & 0xFFu);
        }
        else
        {
            sidTabParam->dstMac[0] = (uint8)((sidTabentry->val[1] >> 8u) & 0xFFu);
            sidTabParam->dstMac[1] = (uint8)((sidTabentry->val[1]) & 0xFFu);
            sidTabParam->dstMac[2] = (uint8)((sidTabentry->val[0] >> 24u) & 0xFFu);
            sidTabParam->dstMac[3] = (uint8)((sidTabentry->val[0] >> 16u) & 0xFFu);
            sidTabParam->dstMac[4] = (uint8)((sidTabentry->val[0] >> 8u) & 0xFFu);
            sidTabParam->dstMac[5] = (uint8)((sidTabentry->val[0]) & 0xFFu);
        }
        /* wildcare bits of SA */
        if(0xFFFFFFFFu == sidTabentry->val[18])
        {
            sidTabParam->carebits |= DVA_TSN_SELECTED_FILED_SRCADR;

            sidTabParam->srcMac[0] = (uint8)((sidTabentry->val[2] >> 24u) & 0xFFu);
            sidTabParam->srcMac[1] = (uint8)((sidTabentry->val[2] >> 16u) & 0xFFu);
            sidTabParam->srcMac[2] = (uint8)((sidTabentry->val[2] >> 8u) & 0xFFu);
            sidTabParam->srcMac[3] = (uint8)((sidTabentry->val[2]) & 0xFFu);
            sidTabParam->srcMac[4] = (uint8)((sidTabentry->val[1] >> 24u) & 0xFFu);
            sidTabParam->srcMac[5] = (uint8)((sidTabentry->val[1] >> 16u) & 0xFFu);
        }
        else
        {

            sidTabParam->srcMac[0] = (uint8)((sidTabentry->val[2] >> 24u) & 0xFFu);
            sidTabParam->srcMac[1] = (uint8)((sidTabentry->val[2] >> 16u) & 0xFFu);
            sidTabParam->srcMac[2] = (uint8)((sidTabentry->val[2] >> 8u) & 0xFFu);
            sidTabParam->srcMac[3] = (uint8)((sidTabentry->val[2]) & 0xFFu);
            sidTabParam->srcMac[4] = (uint8)((sidTabentry->val[1] >> 24u) & 0xFFu);
            sidTabParam->srcMac[5] = (uint8)((sidTabentry->val[1] >> 16u) & 0xFFu);
        }

        /* sidParamInfo of EtherType */
        sidParamGetInfo.ctrlBitWord   = 19u;
        sidParamGetInfo.ctrlBitoffset = 16u;
        sidParamGetInfo.ctrlBitmask   = 0x1u;
        sidParamGetInfo.fieldWord     = 3u;
        sidParamGetInfo.fieldmask     = 0xFFFFu;
        sidParamGetInfo.fieldoffset   = 0u;
        sidParamGetInfo.carebitmask   = DVA_TSN_SELECTED_FILED_ETHTERTYPE;
        dva_drv_cb_read_carebits_data(sidTabentry, sidTabParam, &sidParamGetInfo, &data);
        sidTabParam->ethertype = data;

        /* sidParamInfo of DVA_VLAN ID */
        sidParamGetInfo.ctrlBitWord   = 19u;
        sidParamGetInfo.ctrlBitoffset = 29u;
        sidParamGetInfo.ctrlBitmask   = 0x1u;
        sidParamGetInfo.fieldWord     = 3u;
        sidParamGetInfo.fieldmask     = 0xFFFu;
        sidParamGetInfo.fieldoffset   = 17u;
        sidParamGetInfo.carebitmask   = DVA_TSN_SELECTED_FILED_VID;
        dva_drv_cb_read_carebits_data(sidTabentry, sidTabParam, &sidParamGetInfo, &data);
        sidTabParam->vlanId = data;
        /*sidParamInfo of SIP */
        sidTabParam->sourceIpCtrl = (uint8)((sidTabentry->val[23] >> 30u) & 0x1u);
        if(0x1u == sidTabParam->sourceIpCtrl)
        {
            sidTabParam->carebits |= DVA_TSN_SELECTED_FILED_SRCIP;
        }

        sidTabParam->srcIp[0] = ((sidTabentry->val[7] >> 30) & 0x3u) | ((sidTabentry->val[8] & 0x3FFFFFFFu) << 2);
        sidTabParam->srcIp[1] = ((sidTabentry->val[5] >> 30) & 0x3u) | ((sidTabentry->val[7] & 0x3FFFFFFFu) << 2);
        sidTabParam->srcIp[2] = ((sidTabentry->val[4] >> 30) & 0x3u) | ((sidTabentry->val[5] & 0x3FFFFFFFu) << 2);
        sidTabParam->srcIp[3] = ((sidTabentry->val[3] >> 30) & 0x3u) | ((sidTabentry->val[4] & 0x3FFFFFFFu) << 2);

        /* sidParamInfo of DIP */
        sidTabParam->dstIpCtrl = (uint8)((sidTabentry->val[27] >> 31) & 0x1u);
        if(0x1u == sidTabParam->dstIpCtrl)
        {
            sidTabParam->carebits |= DVA_TSN_SELECTED_FILED_DSTIP;
        }

        sidTabParam->dstIp[0] = ((sidTabentry->val[11] >> 31) & 0x1u) | ((sidTabentry->val[12] & 0x7FFFFFFFu) << 1);
        sidTabParam->dstIp[1] = ((sidTabentry->val[10] >> 31) & 0x1u) | ((sidTabentry->val[11] & 0x7FFFFFFFu) << 1);
        sidTabParam->dstIp[2] = ((sidTabentry->val[9] >> 31) & 0x1u) | ((sidTabentry->val[10] & 0x7FFFFFFFu) << 1);
        sidTabParam->dstIp[3] = ((sidTabentry->val[8] >> 31) & 0x1u) | ((sidTabentry->val[9] & 0x7FFFFFFFu) << 1);

        /* sidParamInfo of DSCP */
        sidParamGetInfo.ctrlBitWord   = 28u;
        sidParamGetInfo.ctrlBitoffset = 6u;
        sidParamGetInfo.ctrlBitmask   = 0x1u;
        sidParamGetInfo.fieldWord     = 13u;
        sidParamGetInfo.fieldmask     = 0x3Fu;
        sidParamGetInfo.fieldoffset   = 0u;
        sidParamGetInfo.carebitmask   = DVA_TSN_SELECTED_FILED_DSCP;
        dva_drv_cb_read_carebits_data(sidTabentry, sidTabParam, &sidParamGetInfo, &data);
        sidTabParam->dscp = (uint8)data;
        /* sidParamInfo of next protocol */
        sidParamGetInfo.ctrlBitWord   = 28u;
        sidParamGetInfo.ctrlBitoffset = 15u;
        sidParamGetInfo.ctrlBitmask   = 0x1u;
        sidParamGetInfo.fieldWord     = 13u;
        sidParamGetInfo.fieldmask     = 0xFFu;
        sidParamGetInfo.fieldoffset   = 7u;
        sidParamGetInfo.carebitmask   = DVA_TSN_SELECTED_FILED_NEXTPROC;
        dva_drv_cb_read_carebits_data(sidTabentry, sidTabParam, &sidParamGetInfo, &data);
        sidTabParam->nextProtocol = (uint8)data;
        /* sidParamInfo of source port */
        sidParamGetInfo.ctrlBitWord   = 29u;
        sidParamGetInfo.ctrlBitoffset = 0u;
        sidParamGetInfo.ctrlBitmask   = 0x1u;
        sidParamGetInfo.fieldWord     = 13u;
        sidParamGetInfo.fieldmask     = 0xFFFFu;
        sidParamGetInfo.fieldoffset   = 16u;
        sidParamGetInfo.carebitmask   = DVA_TSN_SELECTED_FIELD_SRCPORT;
        dva_drv_cb_read_carebits_data(sidTabentry, sidTabParam, &sidParamGetInfo, &data);
        sidTabParam->srcPort = data;

        /* sidParamInfo of destination port */
        sidParamGetInfo.ctrlBitWord   = 29u;
        sidParamGetInfo.ctrlBitoffset = 17u;
        sidParamGetInfo.ctrlBitmask   = 0x1u;
        sidParamGetInfo.fieldWord     = 14u;
        sidParamGetInfo.fieldmask     = 0xFFFFu;
        sidParamGetInfo.fieldoffset   = 1u;
        sidParamGetInfo.carebitmask   = DVA_TSN_SELECTED_FIELD_DSTPORT;
        dva_drv_cb_read_carebits_data(sidTabentry, sidTabParam, &sidParamGetInfo, &data);
        sidTabParam->dstPort = data;

        /* sidParamInfo of vlanpri */
        sidParamGetInfo.ctrlBitWord   = 29u;
        sidParamGetInfo.ctrlBitoffset = 18u;
        sidParamGetInfo.ctrlBitmask   = 0x7u;
        sidParamGetInfo.fieldWord     = 14u;
        sidParamGetInfo.fieldmask     = 0x7u;
        sidParamGetInfo.fieldoffset   = 18u;
        sidParamGetInfo.carebitmask   = DVA_TSN_SELECTED_FIELD_VIDPRI;
        dva_drv_cb_read_carebits_data(sidTabentry, sidTabParam, &sidParamGetInfo, &data);
        sidTabParam->vlanPri = (uint8)data;
        /* sidParamInfo of l4portRange */
        sidParamGetInfo.ctrlBitWord   = 29u;
        sidParamGetInfo.ctrlBitoffset = 21u;
        sidParamGetInfo.ctrlBitmask   = 0xFu;
        sidParamGetInfo.fieldWord     = 14u;
        sidParamGetInfo.fieldmask     = 0xFu;
        sidParamGetInfo.fieldoffset   = 21u;
        sidParamGetInfo.carebitmask   = DVA_TSN_SELECTED_FIELD_L4PORTRANGE;
        dva_drv_cb_read_carebits_data(sidTabentry, sidTabParam, &sidParamGetInfo, &data);
        sidTabParam->l4PortChkTblMap = (uint8)data;

    } while(0u == 1u);
    return retVal;
}

/* Function Name:
 *      dva_drv_cb_sidentry_set
 * Description:
 *      set cb stream identification entry
 * Input:
 *      Index           -SID entry index
 *      sidParam        -SID entry Param
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK - Success.
 * Note:
 *
 *
 */
RtkApiRet dva_drv_cb_sidentry_set(uint32 unit, const uint32 index, CbSidparam_t *const sidParam)
{
    int32                 retVal       = RT_ERR_OK;
    uint32                tableAddress = 0;
    dva_CbSidTabEntry_t   sidTabentry;
    uint32                registerIndex = 0;
    uint32                regLen        = 0;
    uint32                data          = 0;
    dva_CbSidTabParamIn_t sidParamIn;
    uint32                shdleRegLsp[4] = {DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_0_LSP,
                                            DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_1_LSP,
                                            DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_2_LSP,
                                            DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_3_LSP};

    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(&sidTabentry, 0, sizeof(sidTabentry)));
        CHK_FUN_CONTINUE(retVal, drv_memset(&sidParamIn, 0, sizeof(dva_CbSidTabParamIn_t)));
        if(NULL == sidParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /*Check IP and covert*/
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_gen_ipv6entry(sidParam, &sidParamIn));

        /* Assign parameters */
        sidParamIn.ethertype       = sidParam->ethertype;
        sidParamIn.vlanId          = sidParam->vlanId;
        sidParamIn.dscp            = sidParam->dscp;
        sidParamIn.nextProtocol    = sidParam->nextProtocol;
        sidParamIn.srcPort         = sidParam->srcPort;
        sidParamIn.dstPort         = sidParam->dstPort;
        sidParamIn.vlanPri         = sidParam->vlanPri;
        sidParamIn.l4PortChkTblMap = sidParam->l4PortChkTblMap;
        sidParamIn.carebits        = sidParam->carebits;
        sidParamIn.carebits        = sidParam->carebits;
        sidParamIn.isValid         = sidParam->isValid;
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(sidParamIn.dstMac, sidParam->dstMac, MAC_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(sidParamIn.srcMac, sidParam->srcMac, MAC_ADDR_LEN));
        /* userPort2SysPort */
        sidParamIn.portMask = sidParam->portMask;
        /* convert param to Entry */
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_sid_param2entry(&sidTabentry, &sidParamIn));
        /* Write table */
        tableAddress = index;

        CHK_FUN_CONTINUE(retVal, dva_drv_tcam_set_sid_entry(unit, tableAddress, (uint32 *)sidTabentry.val));
        /* Write portmask */
        registerIndex = DVA_FRER_SID_CONFIGURE_PMASK0 + (((uint32)index) * 4u);
        data          = (uint32)sidParamIn.portMask;
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, registerIndex, DVA_FRER_SID_CONFIGURE_PMASK0_CB_CFG_PMASK_0_LSP, DVA_FRER_SID_CONFIGURE_PMASK0_CB_CFG_PMASK_0_LEN, data));
        /* Write stream handle */

        registerIndex = DVA_FRER_SID_CONFIGURE_SHDLE0 + (((uint32)index / 4u) * 4u);
        regLen        = DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_0_LEN;
        data          = (uint32)sidParam->streamHandle;
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_write_8bits(unit, index, registerIndex, shdleRegLsp, regLen, data));
    } while(0u == 1u);

    return retVal;
}

/* Function Name:
 *      dva_drv_cb_sidentry_get
 * Description:
 *      get cb stream identification entry
 * Input:
 *      Index           -SID entry indexs
 * Output:
 *      sidParam         -SID entry Param
 * Return:
 *      RT_ERR_OK - Success.
 *      -RT_ERR_NULL_POINTER
 *      -RT_ERR_FRER_INVALID_ENTRY_INDEX
 * Note:
 *
 *
 */
RtkApiRet dva_drv_cb_sidentry_get(uint32 unit, uint32 index, CbSidparam_t *sidParam)
{
    RtkApiRet             retVal       = RT_ERR_OK;
    uint32                tableAddress = 0;
    uint32                registerTemp = 0;
    dva_table_list_t      tableType;
    dva_CbSidTabEntry_t   sidTabentry;
    dva_CbSidTabParamIn_t sidTabParam;
    uint32                registerIndex  = 0;
    uint32                regLen         = 0;
    uint32                shdleRegLsp[4] = {
        DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_0_LSP,
        DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_1_LSP,
        DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_2_LSP,
        DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_3_LSP};

    do
    {
        /*check NULL pointer*/
        if(NULL == sidParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&sidTabentry, 0, sizeof(dva_CbSidTabEntry_t)));
        CHK_FUN_CONTINUE(retVal, drv_memset(&sidTabParam, 0, sizeof(dva_CbSidTabParamIn_t)));
        /* read table */
        tableType    = DVA_STREAM_IDENTIFICATION_TABLE;
        tableAddress = (uint32)index;
        CHK_FUN_CONTINUE(retVal, dva_table_read(unit, tableType, tableAddress, sidTabentry.val));
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_sid_entry2param(&sidTabentry, &sidTabParam));
        /* read port mask */
        registerIndex = DVA_FRER_SID_CONFIGURE_PMASK0 + ((uint32)index * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, registerIndex, DVA_FRER_SID_CONFIGURE_PMASK0_CB_CFG_PMASK_0_LSP, DVA_FRER_SID_CONFIGURE_PMASK0_CB_CFG_PMASK_0_LEN, &registerTemp));

        /* SysPort2userPort*/
        sidParam->portMask = ((uint16)registerTemp);
        /* read stream handle */
        registerIndex = DVA_FRER_SID_CONFIGURE_SHDLE0 + (((uint32)index / 4u) * 4u);
        regLen        = DVA_FRER_SID_CONFIGURE_SHDLE0_CB_CFG_SHLDE_0_LEN;
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_read_8bits(unit, index, registerIndex, shdleRegLsp, regLen, &registerTemp));
        sidParam->streamHandle = (uint8)registerTemp;

        /* gen ipv6array */
        dva_drv_cb_gen_ipv6array(sidTabParam.srcIp, sidParam->srcIp);
        dva_drv_cb_gen_ipv6array(sidTabParam.dstIp, sidParam->dstIp);
        /* Assign parameters */
        sidParam->ethertype       = sidTabParam.ethertype;
        sidParam->vlanId          = sidTabParam.vlanId;
        sidParam->carebits        = sidTabParam.carebits;
        sidParam->nextProtocol    = sidTabParam.nextProtocol;
        sidParam->srcPort         = sidTabParam.srcPort;
        sidParam->dstPort         = sidTabParam.dstPort;
        sidParam->dscp            = sidTabParam.dscp;
        sidParam->vlanPri         = sidTabParam.vlanPri;
        sidParam->l4PortChkTblMap = sidTabParam.l4PortChkTblMap;
        sidParam->isValid         = sidTabParam.isValid;
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(sidParam->srcMac, sidTabParam.srcMac, MAC_ADDR_LEN));
        CHK_FUN_CONTINUE(retVal, dva_drv_memcpy(sidParam->dstMac, sidTabParam.dstMac, MAC_ADDR_LEN));
    } while(0u == 1u);
    return retVal;
}
/* Function Name:
 *      dva_drv_cb_lut_param2entry
 * Description:
 *      set cb LUT entry
 * Input:
 *      Index           -LUT entry index
 *      entry_value     -LUT entry value
 * Output:
 * Return:
 *      RT_ERR_OK - Success.
 * Note:
 *
 *
 */
RtkApiRet dva_drv_cb_lut_param2entry(dva_CbLutEntry_t *lutEntry, CbLutParam_t *lutParam)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint16    activeIdxTemp = 0;
    do
    {
        if((NULL == lutEntry) || (NULL == lutParam))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Covert lutParam->activeIdx to routing Rh index */
        activeIdxTemp    = lutParam->activeIdx + DVA_FRER_RH_TABLE_OFFSET;
        lutEntry->val[0] = ((((uint32)lutParam->genSeqNum & 0x7fffu) << 17u) | ((uint32)lutParam->genSeqNumEn << 16u) | ((uint32)activeIdxTemp << 2u) | ((uint32)lutParam->activeEn << 1u) | (uint32)(lutParam->encEn));
        lutEntry->val[1] = (((uint32)lutParam->genSeqNum >> 15u) | ((uint32)lutParam->splitEn << 1u) | ((uint32)lutParam->splitDPM << 2u) | ((uint32)lutParam->seqRcvEn << 18u) | ((uint32)lutParam->seqRcvIdx << 19u) | ((uint32)lutParam->indRcvEn << 25u) | (((uint32)lutParam->indRcvIdx & 0x3fu) << 26u));
        lutEntry->val[2] = ((uint32)lutParam->indRcvIdx >> 6);

    } while(0u == 1u);
    return retVal;
}
/* Function Name:
 *      dva_drv_cb_lut_entry2param
 * Description:
 *      set cb LUT entry
 * Input:
 *      Index           -LUT entry index
 *      entry_value     -LUT entry value
 * Output:
 * Return:
 *      RT_ERR_OK - Success.
 * Note:
 *
 *
 */
RtkApiRet dva_drv_cb_lut_entry2param(dva_CbLutEntry_t *const lutEntry, CbLutParam_t *const lutParam)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /*check NULL pointer*/
        if((NULL == lutEntry) || (NULL == lutParam))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(lutParam, 0, sizeof(CbLutParam_t)));
        lutParam->encEn    = (uint8)(lutEntry->val[0] & 0x1u);
        lutParam->activeEn = (uint8)(lutEntry->val[0] >> 1) & 0x1u;
        /* Covert routing Rh index to lutParam->activeIdx*/
        lutParam->activeIdx   = ((uint16)(lutEntry->val[0] >> 2) & 0x3FFFu) - DVA_FRER_RH_TABLE_OFFSET;
        lutParam->genSeqNumEn = (uint8)(lutEntry->val[0] >> 16) & 0x1u;
        lutParam->genSeqNum   = ((uint16)(lutEntry->val[0] >> 17) & 0x7FFFu) | (((uint16)lutEntry->val[1] & 0x1u) << 15);
        lutParam->splitEn     = (uint8)(lutEntry->val[1] >> 1) & 0x01u;
        /*sysPort2userPort*/
        lutParam->splitDPM  = ((uint16)(lutEntry->val[1] >> 2) & 0xFFFFu);
        lutParam->seqRcvEn  = (uint8)(lutEntry->val[1] >> 18) & 0x1u;
        lutParam->seqRcvIdx = (uint8)(lutEntry->val[1] >> 19) & 0x3Fu;
        lutParam->indRcvEn  = (uint8)(lutEntry->val[1] >> 25) & 0x1u;
        lutParam->indRcvIdx = ((uint8)(lutEntry->val[1] >> 26) & 0x3Fu) | ((uint8)(lutEntry->val[2] & 0x1u) << 6);
    } while(0u == 1u);
    return retVal;
}

/* Function Name:
 *      dva_drv_cb_lutentry_set
 * Description:
 *      set cb LUT entry
 * Input:
 *      Index           -LUT entry index
 *      LUTparam     -LUT entry value
 * Output:
 * Return:
 *      RT_ERR_OK - Success.
 * Note:
 *
 *
 */
RtkApiRet dva_drv_cb_lutentry_set(uint32 unit, uint32 index, CbLutParam_t *lutParam)
{
    RtkApiRet        retVal = RT_ERR_OK;
    uint32           tableAddress;
    dva_table_list_t tableType;
    dva_CbLutEntry_t lutEntry;

    do
    {
        /*check NULL pointer */
        if(NULL == lutParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* convert param to Entry */
        CHK_FUN_CONTINUE(retVal, drv_memset(&lutEntry, 0, sizeof(lutEntry)));
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_lut_param2entry(&lutEntry, lutParam));
        /* write table */
        tableAddress = index;
        tableType    = DVA_CB_LOOK_UP_TABLE;
        CHK_FUN_CONTINUE(retVal, dva_table_write(unit, tableType, tableAddress, lutEntry.val));
    } while(0u == 1u);
    return retVal;
}

/* Function Name:
 *      dva_drv_cb_lutentry_get
 * Description:
 *      get cb LUT entry
 * Input:
 *      Index           -cb LUT entry index
 * Output:
 *      lutParam        -cb LUT entry value
 * Return:
 *      RT_ERR_OK - Success.
 * Note:
 *
 *
 */
RtkApiRet dva_drv_cb_lutentry_get(uint32 unit, uint32 index, CbLutParam_t *lutParam)
{
    RtkApiRet        retVal = RT_ERR_OK;
    uint32           tableAddress;
    dva_table_list_t tableType;
    dva_CbLutEntry_t lutEntry;
    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(&lutEntry, 0, sizeof(dva_CbLutEntry_t)));
        if(NULL == lutParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* read table */
        tableAddress = index;
        tableType    = DVA_CB_LOOK_UP_TABLE;
        CHK_FUN_CONTINUE(retVal, dva_table_read(unit, tableType, tableAddress, lutEntry.val));
        /* convert Entry to param*/
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_lut_entry2param(&lutEntry, lutParam));
    } while(0u == 1u);

    return retVal;
}

/* Function Name:
 *       dva_drv_cb_indivrcventry_set
 * Description:
 *      set a individual entry
 * Input:
 *      Index           -cb individual entry index
 *      indivParam      -cb individual entry Param
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK - Success.
 * Note:
 */

RtkApiRet dva_drv_cb_indivrcventry_set(uint32 unit, uint32 index, CbIndivParam_t *indivParam)
{
    RtkApiRet retVal           = RT_ERR_OK;
    uint32    registerIndex    = 0;
    uint32    regLsp           = 0;
    uint32    regLen           = 0;
    uint32    validRegIndex[3] = {DVA_FRER_INDRCV_BASIC_CONTROL_0, DVA_FRER_INDRCV_BASIC_CONTROL_1, DVA_FRER_INDRCV_BASIC_CONTROL_2};
    do
    {
        /*check NULL pointer */
        if(NULL == indivParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* write RSTLOAD */

        registerIndex = DVA_FRER_INDRCV_CONFIGURE_RSTLOAD_0 + ((uint32)index * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, registerIndex, DVA_FRER_INDRCV_CONFIGURE_RSTLOAD_0_CB_INDRCV_RSTLOAD_LSP, DVA_FRER_INDRCV_CONFIGURE_RSTLOAD_0_CB_INDRCV_RSTLOAD_LEN, indivParam->rstload));

        /*write valid */
        regLsp = DVA_FRER_INDRCV_BASIC_CONTROL_0_CB_INDRCV_VALID_31_0_LSP;
        regLen = DVA_FRER_INDRCV_BASIC_CONTROL_0_CB_INDRCV_VALID_31_0_LEN;
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_write_1bit(unit, index, validRegIndex, regLsp, regLen, indivParam->valid));

    } while(0u == 1u);
    return retVal;
}

/* Function Name:
 *       dva_drv_cb_indivrcventry_get
 * Description:
 *      set a individual entry
 * Input:
 *      Index           -cb individual entry index
 *      indivParam      -cb individual entry Param
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK - Success.
 * Note:
 */

RtkApiRet dva_drv_cb_indivrcventry_get(uint32 unit, uint32 index, CbIndivParam_t *indivParam)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    registerIndex = 0u;
    uint32    registerTemp  = 0u;
    uint32    addr          = 0;
    uint8     data[8]       = {0, 0, 0, 0, 0, 0, 0, 0};
    uint32    regaddr[3]    = {DVA_FRER_INDRCV_BASIC_CONTROL_0, DVA_FRER_INDRCV_BASIC_CONTROL_1, DVA_FRER_INDRCV_BASIC_CONTROL_2};

    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(indivParam, 0, sizeof(*indivParam)));
        /* check NULL pointer */
        if(NULL == indivParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* read rstload */
        registerIndex = DVA_FRER_INDRCV_CONFIGURE_RSTLOAD_0 + ((uint32)index * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, registerIndex, DVA_FRER_INDRCV_CONFIGURE_RSTLOAD_0_CB_INDRCV_RSTLOAD_LSP, DVA_FRER_INDRCV_CONFIGURE_RSTLOAD_0_CB_INDRCV_RSTLOAD_LEN, &registerTemp));
        indivParam->rstload = registerTemp;

        /* read valid */

        CHK_FUN_CONTINUE(retVal, dva_drv_cb_read_1bit(unit, index, regaddr, 0, 32u, &indivParam->valid));

        /* read rcvnum from internal channel*/
        /* input rcvnum internal addr */
        addr = (uint32)(DVA_INDIVRCV_RCVNUM_INERNAL_ADDR + ((uint32)index / 4u));
        CHK_FUN_CONTINUE(retVal, dva_asic_cb_read_internal_channel(unit, index, addr, data, 2u));
        indivParam->rcvnum = (((uint16)data[1] << 8) | (uint16)data[0]);

        /* read rsttime from internal channel*/
        /* input rsttime internal addr */
        addr = (uint32)DVA_INDIVRCV_RSTTIME_INERNAL_ADDR + ((uint32)index / 2u);
        CHK_FUN_CONTINUE(retVal, dva_asic_cb_read_internal_channel(unit, index, addr, data, 4u));
        indivParam->rsttime = ((uint32)data[3] << 24) | ((uint32)data[2] << 16) | ((uint32)data[1] << 8) | (uint32)data[0];

        /* read takeany from internal channel*/

        /* input takeany internal addr */
        if(63u >= index)
        {
            addr = (uint32)DVA_INDIVRCV_TAKEANY_INERNAL_ADDR;
        }
        else if(95u >= index)
        {
            addr = (uint32)DVA_INDIVRCV_TAKEANY_INERNAL_ADDR + 1u;
        }
        else
        {
            /*  */
        }
        CHK_FUN_CONTINUE(retVal, dva_asic_cb_read_internal_channel(unit, index, addr, data, 1u));
        indivParam->takeany = (uint8)data[0];

    } while(0u == 1u);
    return retVal;
}
/* Function Name:
 *      dva_drv_cb_seqrcvtab_param2entry
 * Description:
 *      convert Sequence recovery parameter to entry
 * Input:
 *      seqRcvparam     -Sequence recovery parameter
 * Output:
 *      seqrRcvEntry     -Sequence recovery entry
 * Return:
 *      RT_ERR_OK - Success.
 *      -RT_ERR_NULL_POINTER    -null pointer
 * Note:
 *
 *
 */
RtkApiRet dva_drv_cb_seqrcvtab_param2entry(dva_CbSeqRcvTabEntry_t *const seqrRcvEntry, CbSeqRcvTabParam_t *const seqRcvparam)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* check NULL pointer */
        if((NULL == seqrRcvEntry) || (NULL == seqRcvparam))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* initial seqRcvparam */
        seqRcvparam->hisLen &= 0xFFFFu;
        seqRcvparam->hisIdx &= 0xFFFFu;
        seqRcvparam->rcvNum &= 0xFFFFu;
        seqRcvparam->takeno &= 0x1u;
        seqRcvparam->alg &= 0x1u;
        seqRcvparam->valid &= 0x1u;

        CHK_FUN_CONTINUE(retVal, drv_memset(seqrRcvEntry, 0, sizeof(*seqrRcvEntry)));
        seqrRcvEntry->val[0] = (((uint32)seqRcvparam->takeno << 2u) | ((uint32)seqRcvparam->alg << 1u) | ((uint32)seqRcvparam->valid));
        seqrRcvEntry->val[1] = ((uint32)seqRcvparam->hisLen << 3u);

    } while(0u == 1u);
    return retVal;
}
/* Function Name:
 *      dva_drv_cb_seqrcvtab_entry2param
 * Description:
 *      convert Sequence recovery entry to parameter.
 * Input:
 *      seqrRcvEntry     -Sequence recovery entry
 * Output:
 *      seqRcvparam     -Sequence recovery parameter
 * Return:
 *      RT_ERR_OK - Success.
 *      -RT_ERR_NULL_POINTER    -null pointer
 * Note:
 *
 *
 */

RtkApiRet dva_drv_cb_seqrcvtab_entry2param(dva_CbSeqRcvTabEntry_t *const seqrRcvEntry, CbSeqRcvTabParam_t *const seqRcvparam)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /*check NULL pointer*/
        if((NULL == seqrRcvEntry) || (NULL == seqRcvparam))
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        seqRcvparam->hisLen = (uint16)((seqrRcvEntry->val[1] >> 3u) & 0xFFFFu);
        seqRcvparam->hisIdx = ((uint16)((seqrRcvEntry->val[1] & 0x0007u) << 13u) | (uint16)((seqrRcvEntry->val[0] >> 19u) & 0x1FFFu));
        seqRcvparam->rcvNum = (uint16)((seqrRcvEntry->val[0] >> 3u) & 0xFFFFu);
        seqRcvparam->takeno = (uint8)((seqrRcvEntry->val[0] >> 2u) & 0x0001u);
        seqRcvparam->alg    = (uint8)((seqrRcvEntry->val[0] >> 1u) & 0x0001u);
        seqRcvparam->valid  = (uint8)(seqrRcvEntry->val[0] & 0x0001u);
    } while(0u == 1u);
    return retVal;
}
RtkApiRet dva_drv_cb_egress_seqrcv_intf_chk(uint32 index, CbSeqRcvTabParam_t *seqTabentry)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input parameters */
        if(DVA_FRER_SEQRCV_INSTANCE_NUM_MAX <= index)
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
        if((DVA_DISABLED != seqTabentry->valid) && (DVA_ENABLED != seqTabentry->valid))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_DISABLED != seqTabentry->takeno) && (DVA_ENABLED != seqTabentry->takeno))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_DISABLED != seqTabentry->laten) && (DVA_ENABLED != seqTabentry->laten))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_MATCH != seqTabentry->alg) && (DVA_VECTOR != seqTabentry->alg))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_FRER_HISLEN_MAX < seqTabentry->hisLen) || (DVA_FRER_HISLEN_MIN > seqTabentry->hisLen))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_FRER_LATHPATH_MAX < seqTabentry->latPaths) || (DVA_FRER_LATHPATH_MIN > seqTabentry->latPaths))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);
    return retVal;
}
/* Function Name:
 *      dva_drv_cb_seqrcvtab_entry_set
 * Description:
 *      set sequence recovery entry.
 * Input:
 *      index       -sequence recovery entry index
 *      seqParam    -sequence recovery entry parameter
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK - Success.
 *      -RT_ERR_CB_SEQRCV_INVALID_ENTRY_INDEX    -index is out of range of sequence recovery entry
 *      -RT_ERR_NULL_POINTER                  -null pointer
 *      -RT_ERR_FAILED                        -failed
 * Note:
 *
 */

RtkApiRet dva_drv_cb_seqrcvtab_entry_set(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *const seqParam)
{
    dva_CbSeqRcvTabEntry_t seqEntry;
    RtkApiRet              retVal = RT_ERR_OK;
    dva_table_list_t       tableType;
    uint32                 tableAddress  = 0;
    uint32                 registerIndex = 0;
    uint32                 expDiffl      = 0;
    uint32                 expDiffh      = 0;
    uint32                 regLsp        = 0;
    uint32                 regLen        = 0;
    uint32                 seqRcvPathLsp[4] =
        {
            DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_0_LSP,
            DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_1_LSP,
            DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_2_LSP,
            DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_3_LSP};
    uint32 latenRegIndex[3] = {DVA_FRER_LATT_CONTROL0, DVA_FRER_LATT_CONTROL1, 0};

    do
    {
        CHK_FUN_CONTINUE(retVal, drv_memset(&seqEntry, 0, sizeof(seqEntry)));
        /*check NULL pointer*/
        if(NULL == seqParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* convert param to Entry */
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_seqrcvtab_param2entry(&seqEntry, seqParam));

        /* Write table */
        tableType    = DVA_CB_SEQUENCY_RECOVERY_TABLE;
        tableAddress = index;
        CHK_FUN_CONTINUE(retVal, dva_table_write(unit, tableType, tableAddress, (uint32 *)seqEntry.val));
        /* write expectdiff */
        expDiffl      = ((uint32)seqParam->expDiff & 0xFFFFFFFFu);
        expDiffh      = ((uint32)(seqParam->expDiff >> 32u) & 0xFFFFFFFFu);
        registerIndex = DVA_FRER_LATT_CONFIGURE_EXPDIFF_0 + (((uint32)index * 2u) * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, registerIndex, DVA_FRER_LATT_CONFIGURE_EXPDIFF_0_CB_CFG_EXPDIFF_L_LSP, DVA_FRER_LATT_CONFIGURE_EXPDIFF_0_CB_CFG_EXPDIFF_L_LEN, expDiffl));
        registerIndex = DVA_FRER_LATT_CONFIGURE_EXPDIFF_0 + ((((uint32)index * 2u) + 1u) * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, registerIndex, DVA_FRER_LATT_CONFIGURE_EXPDIFF_1_CB_CFG_EXPDIFF_H_LSP, DVA_FRER_LATT_CONFIGURE_EXPDIFF_1_CB_CFG_EXPDIFF_H_LEN, expDiffh));

        /*write RSTLOAD*/
        registerIndex = DVA_FRER_SEQRCV_CONFIGURE_RSTLOAD_0 + ((uint32)index * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, registerIndex, DVA_FRER_SEQRCV_CONFIGURE_RSTLOAD_0_CB_SEQRCV_RSTLOAD_LSP, DVA_FRER_SEQRCV_CONFIGURE_RSTLOAD_0_CB_SEQRCV_RSTLOAD_LEN, seqParam->rstLoad));
        /*write latPath*/
        regLen        = DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_0_LEN;
        registerIndex = DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0 + (((uint32)index / 4u) * 4u);
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_write_8bits(unit, index, registerIndex, seqRcvPathLsp, regLen, seqParam->latPaths));

        /*write laten*/
        regLsp = DVA_FRER_LATT_CONTROL0_CB_LATERR_DET_EN_31_0_LSP;
        regLen = DVA_FRER_LATT_CONTROL0_CB_LATERR_DET_EN_31_0_LEN;
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_write_1bit(unit, index, latenRegIndex, regLsp, regLen, seqParam->laten));

    } while(0u == 1u);
    return retVal;
}
/* Function Name:
 *      dva_drv_cb_seqrcvtab_entry_get
 * Description:
 *      get sequence recovery entry.
 * Input:
 *      index       -sequence recovery entry index
 * Output:
 *      seqParam    -sequence recovery entry parameter
 * Return:
 *      RT_ERR_OK - Success.
 *      -RT_ERR_CB_SEQRCV_INVALID_ENTRY_INDEX    -index is out of range of sequence recovery entry
 *      -RT_ERR_NULL_POINTER                  -null pointer
 *      -RT_ERR_FAILED                        -failed
 * Note:
 *
 */

RtkApiRet dva_drv_cb_seqrcvtab_entry_get(uint32 unit, const uint32 index, CbSeqRcvTabParam_t *const seqParam)
{

    RtkApiRet              retVal       = RT_ERR_OK;
    uint32                 tableAddress = 0;
    dva_table_list_t       tableType    = DVA_CB_SEQUENCY_RECOVERY_TABLE;
    dva_CbSeqRcvTabEntry_t seqEntry;
    uint32                 registerIndex    = 0;
    uint32                 registerTemp     = 0;
    uint32                 expDifflTemp     = 0;
    uint32                 expDiffhTemp     = 0;
    uint32                 addr             = 0;
    uint32                 regLen           = 0;
    uint8                  data[8]          = {0, 0, 0, 0, 0, 0, 0, 0};
    uint32                 latPathRegLsp[4] = {
        DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_0_LSP,
        DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_1_LSP,
        DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_2_LSP,
        DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_3_LSP};
    uint32 latenRegAddr[3] = {DVA_FRER_LATT_CONTROL0, DVA_FRER_LATT_CONTROL1, 0};
    uint64 cntDiffTemp     = 0;
    do
    {
        /* check NULL pointer */
        if(NULL == seqParam)
        {
            retVal = -RT_ERR_NULL_POINTER;
            continue;
        }
        CHK_FUN_CONTINUE(retVal, drv_memset(&seqEntry, 0, sizeof(dva_CbSeqRcvTabEntry_t)));

        /* read expectdiff */
        registerIndex = DVA_FRER_LATT_CONFIGURE_EXPDIFF_0 + (((uint32)index * 2u) * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, registerIndex, DVA_FRER_LATT_CONFIGURE_EXPDIFF_0_CB_CFG_EXPDIFF_L_LSP, DVA_FRER_LATT_CONFIGURE_EXPDIFF_0_CB_CFG_EXPDIFF_L_LEN, &expDifflTemp));

        registerIndex = DVA_FRER_LATT_CONFIGURE_EXPDIFF_0 + ((((uint32)index * 2u) + 1u) * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, registerIndex, DVA_FRER_LATT_CONFIGURE_EXPDIFF_1_CB_CFG_EXPDIFF_H_LSP, DVA_FRER_LATT_CONFIGURE_EXPDIFF_1_CB_CFG_EXPDIFF_H_LEN, &expDiffhTemp));

        seqParam->expDiff = ((uint64)expDiffhTemp << 32u) | ((uint64)expDifflTemp);

        /*read RSTLOAD*/
        registerIndex = DVA_FRER_SEQRCV_CONFIGURE_RSTLOAD_0 + ((uint32)index * 4u);
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, registerIndex, DVA_FRER_SEQRCV_CONFIGURE_RSTLOAD_0_CB_SEQRCV_RSTLOAD_LSP, DVA_FRER_SEQRCV_CONFIGURE_RSTLOAD_0_CB_SEQRCV_RSTLOAD_LEN, &registerTemp));
        seqParam->rstLoad = (uint32)registerTemp;
        /*read latPath*/
        registerIndex = DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0 + (((uint32)index / 4u) * 4u);
        regLen        = DVA_FRER_SEQRCV_CONFIGURE_LATPATHS_0_CB_SEQRCV_LATPATHS_0_LEN;
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_read_8bits(unit, index, registerIndex, latPathRegLsp, regLen, &registerTemp));
        seqParam->latPaths = (uint8)registerTemp;

        /*read laten*/
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_read_1bit(unit, index, latenRegAddr, 0u, 32u, &seqParam->laten));

        /* read curdiff from internal channel*/
        /* write internal addr */
        addr = ((uint32)DVA_SEQRCV_CURDIFF_INERNAL_ADDR + (uint32)index);
        CHK_FUN_CONTINUE(retVal, dva_asic_cb_read_internal_channel(unit, index, addr, data, 8u));
        cntDiffTemp       = ((uint64)data[7] << 56) | ((uint64)data[6] << 48) | ((uint64)data[5] << 40) | ((uint64)data[4] << 32) | ((uint64)data[3] << 24) | ((uint64)data[2] << 16) | ((uint64)data[1] << 8) | (uint64)data[0];
        seqParam->cntDiff = (int64)cntDiffTemp;

        /* read rsttime from internal channel*/
        /* write internal addr */
        addr = ((uint32)DVA_SEQRCV_RSTTIME_INERNAL_ADDR + ((uint32)index / 2u));
        CHK_FUN_CONTINUE(retVal, dva_asic_cb_read_internal_channel(unit, index, addr, data, 4u));
        seqParam->rsttime = ((uint32)data[3] << 24) | ((uint32)data[2] << 16) | ((uint32)data[1] << 8) | (uint32)data[0];

        /* read takeany from internal channel*/
        /* write internal addr */
        addr = DVA_SEQRCV_TAKEANY_INERNAL_ADDR;
        CHK_FUN_CONTINUE(retVal, dva_asic_cb_read_internal_channel(unit, index, addr, data, 1u));
        seqParam->takeany = (uint8)data[0];

        /* read table */
        tableAddress = index;
        CHK_FUN_CONTINUE(retVal, dva_table_read(unit, tableType, tableAddress, seqEntry.val));
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_seqrcvtab_entry2param(&seqEntry, seqParam));

    } while(0u == 1u);
    return retVal;
}

/**
 * @brief Set Rtag
 * @param[in] None
 * @param[out] status Latent error status for all sequence recovery entries
 * @retval RT_ERR_OK
 * @retval -RT_ERR_FAILED
 * @note status:
 *      0: this entry has no latent error
 *      1: this entry has latent error
 */
RtkApiRet dva_drv_cb_rtag_set(uint32 unit, uint16 rtag)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(unit, DVA_FRER_BASIC_CONTROL, DVA_FRER_BASIC_CONTROL_RG_RTAG_LSP, DVA_FRER_BASIC_CONTROL_RG_RTAG_LEN, rtag));
    } while(0u == 1u);
    return retVal;
}

/* Function Name:
 *      rtk_cb_rtag_Get
 * Description:
 *      Get cb R-tag(defult:0xF1C1)
 * Input:
 *      Nones
 * Output:
 *      rtag    -cb r-tag
 * Return:
 *      RT_ERR_OK - Success.
 * Note:
 *
 *
 */

RtkApiRet dva_drv_cb_rtag_get(uint32 unit, uint32 *rtag)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(unit, DVA_FRER_BASIC_CONTROL, DVA_FRER_BASIC_CONTROL_RG_RTAG_LSP, DVA_FRER_BASIC_CONTROL_RG_RTAG_LEN, rtag));
    } while(0u == 1u);
    return retVal;
}

/*for test*/
static RtkApiRet dva_drv_cb_write_internal_channel(uint32 unit, uint32 addr, uint64 data)
{
    RtkApiRet retVal        = RT_ERR_OK;
    uint32    registerIndex = 0;
    uint8     enInteRw      = 1;
    uint32    intercmd      = 0;
    do
    {
        /* ------------Step1 Set cb_indr_test_mode------------ */
        registerIndex = DVA_FRER_BASIC_CONTROL;
        intercmd      = enInteRw;
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(DVA_UNIT, registerIndex, DVA_FRER_BASIC_CONTROL_CB_INDR_TEST_MODE_LSP, DVA_FRER_BASIC_CONTROL_CB_INDR_TEST_MODE_LEN, intercmd));
        /* ------------STEP 2 write cb_indr_data ------------ */
        registerIndex = DVA_FRER_INTERNAL_ACCESS_CHANNEL_1;
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(DVA_UNIT, registerIndex, DVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LSP, DVA_FRER_INTERNAL_ACCESS_CHANNEL_1_CB_INDR_DATA_31_0_LEN, (uint32)(data & 0xFFFFFFFF)));

        registerIndex = DVA_FRER_INTERNAL_ACCESS_CHANNEL_2;
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(DVA_UNIT, registerIndex, DVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LSP, DVA_FRER_INTERNAL_ACCESS_CHANNEL_2_CB_INDR_DATA_63_32_LEN, (uint32)((data >> 32) & 0xFFFFFFFF)));
        /* ------------Step3 write write command ------------ */
        registerIndex = DVA_FRER_INTERNAL_ACCESS_CHANNEL_0;
        /* write internal addr */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(DVA_UNIT, registerIndex, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_ADDR_LSP, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_ADDR_LEN, addr));

        /* write write command */
        intercmd = DVA_INTERNALWRITE;
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(DVA_UNIT, registerIndex, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_WR_LSP, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_WR_LEN, intercmd));

        /* write Execute */
        intercmd = DVA_EXECUTE_ACCESS;
        CHK_FUN_CONTINUE(retVal, dva_reg_field_write(DVA_UNIT, registerIndex, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LEN, intercmd));

        /* ------------Step2 Check execute if complete it will 0------------ */
        CHK_FUN_CONTINUE(retVal, dva_reg_field_read(DVA_UNIT, registerIndex, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LSP, DVA_FRER_INTERNAL_ACCESS_CHANNEL_0_CB_INDR_EXECUTE_LEN, &intercmd));

        if(DVA_COMPLETE_ACCESS != intercmd)
        {
            retVal = -RT_ERR_CB_INTERNAL_CMD;
            continue;
        }
    } while(0u == 1u);
    return retVal;
}
RtkApiRet dva_drv_cb_seqrcv_cnt_set(uint32 unit, const uint32 index, uint64 cntPass, uint64 cntDrop)
{
    RtkApiRet retVal = RT_ERR_OK;
    uint32    addr   = 0;

    do
    {
        /* write cntDrop to internal channel*/
        addr = (uint32)DVA_SEQRCV_CNTDROP_INERNAL_ADDR + (uint32)index;
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_write_internal_channel(unit, addr, cntDrop));

        /* write passCnt to internal channel*/
        addr = (uint32)DVA_SEQRCV_CNTPASS_INERNAL_ADDR + (uint32)index;
        CHK_FUN_CONTINUE(retVal, dva_drv_cb_write_internal_channel(unit, addr, cntPass));

    } while(0u == 1u);
    return retVal;
}
RtkApiRet dva_drv_cb_actsid_egiftbl_intf_chk(uint32 egifIndex, CbActiveEgifTblParm_t *actegifParm)
{
    RtkApiRet retVal = RT_ERR_OK;
    do
    {
        /* Check input */
        if(DVA_FRER_EGIF_INSTANCE <= egifIndex)
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
        if((DVA_ENABLED != actegifParm->saReplaceEn) && (DVA_DISABLED != actegifParm->saReplaceEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_ENABLED != actegifParm->daReplaceEn) && (DVA_DISABLED != actegifParm->daReplaceEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_ENABLED != actegifParm->vidReplaceEn) && (DVA_DISABLED != actegifParm->vidReplaceEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if((DVA_ENABLED != actegifParm->pcpReplaceEn) && (DVA_DISABLED != actegifParm->pcpReplaceEn))
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if(DVA_FRER_VID_MAX < actegifParm->vid)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
        if(DVA_FRER_PCP_MAX < actegifParm->pcp)
        {
            retVal = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);
    return retVal;
}
/**@}*/  // cb