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

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_api_ext.h"
#include "rtl907xd_va_asicdrv_tcam.h"
#include "rtl907xd_va_asicdrv_tcam_irq.h"

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
/******************* ACL *******************/
RtkApiRet dva_drv_tcam_get_pie_fail_irpt(uint32 unit, uint32 block, uint32 tcamNum, uint32 *pFail)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    bitVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(block > DVA_TCAM_PIE_BLOCK_BOUND)
        {
            ret = -(RT_ERR_TCAM_PIE_BLOCK);
            continue;
        }

        if(tcamNum > DVA_TCAM_PIE_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pFail)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_128X96_0_0_CTRL + (DVA_TCAM_PIE_BLOCK_REG_OFFSET * block) + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_128X96_0_0_CTRL_RG_TCAM128X96_0_0_ACL_CRC_FAIL_IRPT_LSP, &bitVal));
        *pFail = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_pie_clr_irpt(uint32 unit, uint32 block, uint32 tcamNum, uint32 *pClear)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    bitVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(block > DVA_TCAM_PIE_BLOCK_BOUND)
        {
            ret = -(RT_ERR_TCAM_PIE_BLOCK);
            continue;
        }

        if(tcamNum > DVA_TCAM_PIE_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pClear)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_128X96_0_0_CTRL + (DVA_TCAM_PIE_BLOCK_REG_OFFSET * block) + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_128X96_0_0_CTRL_RG_TCAM128X96_0_0_ACL_CLR_IRPT_LSP, &bitVal));
        *pClear = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_pie_clr_irpt(uint32 unit, uint32 block, uint32 tcamNum, uint32 clear)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(block > DVA_TCAM_PIE_BLOCK_BOUND)
        {
            ret = -(RT_ERR_TCAM_PIE_BLOCK);
            continue;
        }

        if(tcamNum > DVA_TCAM_PIE_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(clear > DVA_TCAM_IRQ_CLEAR)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_128X96_0_0_CTRL + (DVA_TCAM_PIE_BLOCK_REG_OFFSET * block) + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_set(unit, regAddr, DVA_SAFETY_TCAM_128X96_0_0_CTRL_RG_TCAM128X96_0_0_ACL_CLR_IRPT_LSP, clear));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_acl_ip(uint32 unit, uint32 *pIP)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        if(NULL == pIP)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, DVA_TCAM_ERR_ACL_IP, &regVal));
        *pIP = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_acl_ie(uint32 unit, uint32 *pIE)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        if(NULL == pIE)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, DVA_TCAM_ERR_ACL_IE, &regVal));
        *pIE = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_acl_ie(uint32 unit, uint32 ie)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(ie > DVA_TCAM_ERROR_ACL_EN_MSK)
        {
            ret = -(RT_ERR_TCAM_ERROR_EN_MASK);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_write(unit, DVA_TCAM_ERR_ACL_IE, ie));
    } while(0u == 1u);

    return ret;
}

/******************* Routing *******************/
RtkApiRet dva_drv_tcam_get_netroute_fail_irpt(uint32 unit, uint32 tcamNum, uint32 *pFail)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    bitVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_NETROUTE_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pFail)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL_RG_TCAM256X128_0_ROUTE_CRC_FAIL_IRPT_LSP, &bitVal));
        *pFail = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_netroute_clr_irpt(uint32 unit, uint32 tcamNum, uint32 *pClear)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    bitVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_NETROUTE_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pClear)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL_RG_TCAM256X128_0_ROUTE_CLR_IRPT_LSP, &bitVal));
        *pClear = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_netroute_clr_irpt(uint32 unit, uint32 tcamNum, uint32 clear)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_NETROUTE_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(clear > DVA_TCAM_IRQ_CLEAR)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_set(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL_RG_TCAM256X128_0_ROUTE_CLR_IRPT_LSP, clear));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_ingif_fail_irpt(uint32 unit, uint32 tcamNum, uint32 *pFail)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    bitVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_INGIF_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pFail)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL_RG_TCAM64X48_0_ROUTE_CRC_FAIL_IRPT_LSP, &bitVal));
        *pFail = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_ingif_clr_irpt(uint32 unit, uint32 tcamNum, uint32 *pClear)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    bitVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_INGIF_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pClear)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL_RG_TCAM64X48_0_ROUTE_CLR_IRPT_LSP, &bitVal));
        *pClear = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_ingif_clr_irpt(uint32 unit, uint32 tcamNum, uint32 clear)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_INGIF_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(clear > DVA_TCAM_IRQ_CLEAR)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_set(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL_RG_TCAM64X48_0_ROUTE_CLR_IRPT_LSP, clear));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_route_ip(uint32 unit, uint32 *pIP)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        if(NULL == pIP)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, DVA_TCAM_ERR_ROUTE_IP, &regVal));
        *pIP = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_route_ie(uint32 unit, uint32 *pIE)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        if(NULL == pIE)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, DVA_TCAM_ERR_ROUTE_IE, &regVal));
        *pIE = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_route_ie(uint32 unit, uint32 ie)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(ie > DVA_TCAM_ERROR_ROUTE_EN_MSK)
        {
            ret = -(RT_ERR_TCAM_ERROR_EN_MASK);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_write(unit, DVA_TCAM_ERR_ROUTE_IE, ie));
    } while(0u == 1u);

    return ret;
}

/******************* Stream Identification *******************/
RtkApiRet dva_drv_tcam_get_sid_fail_irpt(uint32 unit, uint32 tcamNum, uint32 *pFail)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    bitVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_SID_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pFail)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_96X120_0_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_96X120_0_0_CTRL_RG_TCAM96X120_0_0_CB_CRC_FAIL_IRPT_LSP, &bitVal));
        *pFail = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_sid_clr_irpt(uint32 unit, uint32 tcamNum, uint32 *pClear)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    bitVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_SID_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pClear)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_96X120_0_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_96X120_0_0_CTRL_RG_TCAM96X120_0_0_CB_CLR_IRPT_LSP, &bitVal));
        *pClear = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_sid_clr_irpt(uint32 unit, uint32 tcamNum, uint32 clear)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_SID_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(clear > DVA_TCAM_IRQ_CLEAR)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_96X120_0_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_set(unit, regAddr, DVA_SAFETY_TCAM_96X120_0_0_CTRL_RG_TCAM96X120_0_0_CB_CLR_IRPT_LSP, clear));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_cb_ip(uint32 unit, uint32 *pIP)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        if(NULL == pIP)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, DVA_TCAM_ERR_CB_IP, &regVal));
        *pIP = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_cb_ie(uint32 unit, uint32 *pIE)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    regVal = 0u;

    do
    {
        if(NULL == pIE)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, DVA_TCAM_ERR_CB_IE, &regVal));
        *pIE = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_cb_ie(uint32 unit, uint32 ie)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(ie > DVA_TCAM_ERROR_ROUTE_EN_MSK)
        {
            ret = -(RT_ERR_TCAM_ERROR_EN_MASK);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_write(unit, DVA_TCAM_ERR_CB_IE, ie));
    } while(0u == 1u);

    return ret;
}