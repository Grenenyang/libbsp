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
#include "rtl907xd_va_asicdrv_tcam_ctrl.h"

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
RtkApiRet dva_drv_tcam_get_safety(uint32 unit, uint32 *pEnable)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    bitVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == pEnable)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, DVA_INTERNAL_CPU_VARIABLE22, DVA_TCAM_SAFETY_ENABLE_BIT, &bitVal));
        if(0u == bitVal)
        {
            /* 0 is enable tcam safety */
            *pEnable = ENABLE;
        }
        else
        {
            /* 1 is disable tcam safety */
            *pEnable = DISABLE;
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_safety(uint32 unit, uint32 enable)
{
    RtkApiRet ret    = RT_ERR_OK;
    uint32    bitVal = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(enable > ENABLE)
        {
            ret = -(RT_ERR_TCAM_SAFETY_ENABLE);
            continue;
        }

        if(ENABLE == enable)
        {
            /* 0 is enable tcam safety */
            bitVal = 0u;
        }
        else
        {
            /* 1 is disable tcam safety */
            bitVal = 1u;
        }
        CHK_FUN_CONTINUE(ret, dva_reg_bit_set(unit, DVA_INTERNAL_CPU_VARIABLE22, DVA_TCAM_SAFETY_ENABLE_BIT, bitVal));
    } while(0u == 1u);

    return ret;
}

/******************* PIE *******************/
RtkApiRet dva_drv_tcam_get_pie_check_en(uint32 unit, uint32 block, uint32 tcamNum, uint32 *pEnable)
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

        if(NULL == pEnable)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_128X96_0_0_CTRL + (DVA_TCAM_PIE_BLOCK_REG_OFFSET * block) + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_128X96_0_0_CTRL_RG_TCAM128X96_0_0_ACL_CHECK_EN_LSP, &bitVal));
        *pEnable = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_pie_check_en(uint32 unit, uint32 block, uint32 tcamNum, uint32 enable)
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

        if(enable > ENABLE)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_128X96_0_0_CTRL + (DVA_TCAM_PIE_BLOCK_REG_OFFSET * block) + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_set(unit, regAddr, DVA_SAFETY_TCAM_128X96_0_0_CTRL_RG_TCAM128X96_0_0_ACL_CHECK_EN_LSP, enable));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_pie_all_check_en(uint32 unit, uint32 enable)
{
    RtkApiRet ret   = RT_ERR_OK;
    uint32    block = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(enable > ENABLE)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        for(block = 0u; block <= DVA_TCAM_PIE_BLOCK_BOUND; block++)
        {
            ret = dva_drv_tcam_set_pie_check_en(unit, block, DVA_TCAM_PIE_TCAM0, enable);
            CHK_FUN_RET(ret, dva_drv_tcam_set_pie_check_en(unit, block, DVA_TCAM_PIE_TCAM1, enable));
            if(RT_ERR_OK != ret)
            {
                break;
            }
        }
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_pie_golden_crc(uint32 unit, uint32 block, uint32 tcamNum, uint32 *pGoldenCrc)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    regVal  = 0u;

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

        if(NULL == pGoldenCrc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_128X96_0_0_CTRL_CRC_GOLDEN + (DVA_TCAM_PIE_BLOCK_REG_OFFSET * block) + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, regAddr, &regVal));
        *pGoldenCrc = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_pie_golden_crc(uint32 unit, uint32 block, uint32 tcamNum, uint32 goldenCrc)
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

        regAddr = DVA_SAFETY_TCAM_128X96_0_0_CTRL_CRC_GOLDEN + (DVA_TCAM_PIE_BLOCK_REG_OFFSET * block) + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_write(unit, regAddr, goldenCrc));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_pie_block_golden_crc(uint32 unit, uint32 block, dva_drv_TcamPieCrcVal_t *pGoldenCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(block > DVA_TCAM_PIE_BLOCK_BOUND)
        {
            ret = -(RT_ERR_TCAM_PIE_BLOCK);
            continue;
        }

        if(NULL == pGoldenCrc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_pie_golden_crc(unit, block, DVA_TCAM_PIE_TCAM0, pGoldenCrc->val[0]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_pie_golden_crc(unit, block, DVA_TCAM_PIE_TCAM1, pGoldenCrc->val[1]));
    } while(0u == 1u);

    return ret;
}

/******************* Netroute *******************/
RtkApiRet dva_drv_tcam_get_netroute_check_en(uint32 unit, uint32 tcamNum, uint32 *pEnable)
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

        if(NULL == pEnable)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL_RG_TCAM256X128_0_ROUTE_CHECK_EN_LSP, &bitVal));
        *pEnable = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_netroute_check_en(uint32 unit, uint32 tcamNum, uint32 enable)
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

        if(enable > ENABLE)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_set(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL_RG_TCAM256X128_0_ROUTE_CHECK_EN_LSP, enable));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_netroute_all_check_en(uint32 unit, uint32 enable)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(enable > ENABLE)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_check_en(unit, DVA_TCAM_NETROUTE_TCAM0, enable));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_check_en(unit, DVA_TCAM_NETROUTE_TCAM1, enable));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_check_en(unit, DVA_TCAM_NETROUTE_TCAM2, enable));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_check_en(unit, DVA_TCAM_NETROUTE_TCAM3, enable));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_check_en(unit, DVA_TCAM_NETROUTE_TCAM4, enable));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_check_en(unit, DVA_TCAM_NETROUTE_TCAM5, enable));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_netroute_golden_crc(uint32 unit, uint32 tcamNum, uint32 *pGoldenCrc)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    regVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_NETROUTE_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pGoldenCrc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL_CRC_GOLDEN + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, regAddr, &regVal));
        *pGoldenCrc = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_netroute_golden_crc(uint32 unit, uint32 tcamNum, uint32 goldenCrc)
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

        regAddr = DVA_SAFETY_TCAM_ROUTE_256X128_0_CTRL_CRC_GOLDEN + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_write(unit, regAddr, goldenCrc));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_netroute_all_golden_crc(uint32 unit, dva_drv_TcamNetRouteCrcVal_t *pGoldenCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == pGoldenCrc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_golden_crc(unit, DVA_TCAM_NETROUTE_TCAM0, pGoldenCrc->val[0]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_golden_crc(unit, DVA_TCAM_NETROUTE_TCAM1, pGoldenCrc->val[1]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_golden_crc(unit, DVA_TCAM_NETROUTE_TCAM2, pGoldenCrc->val[2]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_golden_crc(unit, DVA_TCAM_NETROUTE_TCAM3, pGoldenCrc->val[3]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_golden_crc(unit, DVA_TCAM_NETROUTE_TCAM4, pGoldenCrc->val[4]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_netroute_golden_crc(unit, DVA_TCAM_NETROUTE_TCAM5, pGoldenCrc->val[5]));
    } while(0u == 1u);

    return ret;
}

/******************* Ingress Interface *******************/
RtkApiRet dva_drv_tcam_get_ingif_check_en(uint32 unit, uint32 tcamNum, uint32 *pEnable)
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

        if(NULL == pEnable)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL_RG_TCAM64X48_0_ROUTE_CHECK_EN_LSP, &bitVal));
        *pEnable = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_ingif_check_en(uint32 unit, uint32 tcamNum, uint32 enable)
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

        if(enable > ENABLE)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_set(unit, regAddr, DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL_RG_TCAM64X48_0_ROUTE_CHECK_EN_LSP, enable));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_ingif_all_check_en(uint32 unit, uint32 enable)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(enable > ENABLE)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_ingif_check_en(unit, DVA_TCAM_INGIF_TCAM0, enable));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_ingif_check_en(unit, DVA_TCAM_INGIF_TCAM1, enable));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_ingif_golden_crc(uint32 unit, uint32 tcamNum, uint32 *pGoldenCrc)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    regVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_INGIF_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pGoldenCrc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL_CRC_GOLDEN + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, regAddr, &regVal));
        *pGoldenCrc = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_ingif_golden_crc(uint32 unit, uint32 tcamNum, uint32 goldenCrc)
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

        regAddr = DVA_SAFETY_TCAM_ROUTE_64X48_0_CTRL_CRC_GOLDEN + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_write(unit, regAddr, goldenCrc));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_ingif_all_golden_crc(uint32 unit, dva_drv_TcamIngifCrcVal_t *pGoldenCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == pGoldenCrc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_ingif_golden_crc(unit, DVA_TCAM_INGIF_TCAM0, pGoldenCrc->val[0]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_ingif_golden_crc(unit, DVA_TCAM_INGIF_TCAM1, pGoldenCrc->val[1]));
    } while(0u == 1u);

    return ret;
}

/******************* Stream Identification *******************/
RtkApiRet dva_drv_tcam_get_sid_check_en(uint32 unit, uint32 tcamNum, uint32 *pEnable)
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

        if(NULL == pEnable)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_96X120_0_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_get(unit, regAddr, DVA_SAFETY_TCAM_96X120_0_0_CTRL_RG_TCAM96X120_0_0_CB_CHECK_EN_LSP, &bitVal));
        *pEnable = bitVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_sid_check_en(uint32 unit, uint32 tcamNum, uint32 enable)
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

        if(enable > ENABLE)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_96X120_0_0_CTRL + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_bit_set(unit, regAddr, DVA_SAFETY_TCAM_96X120_0_0_CTRL_RG_TCAM96X120_0_0_CB_CHECK_EN_LSP, enable));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_sid_all_check_en(uint32 unit, uint32 enable)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(enable > ENABLE)
        {
            ret = -(RT_ERR_TCAM_CHECK_ENABLE);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_check_en(unit, DVA_TCAM_SID_TCAM0, enable));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_check_en(unit, DVA_TCAM_SID_TCAM1, enable));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_check_en(unit, DVA_TCAM_SID_TCAM2, enable));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_check_en(unit, DVA_TCAM_SID_TCAM3, enable));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_get_sid_golden_crc(uint32 unit, uint32 tcamNum, uint32 *pGoldenCrc)
{
    RtkApiRet ret     = RT_ERR_OK;
    uint32    regAddr = 0u;
    uint32    regVal  = 0u;

    do
    {
        /* Check if the input parameters are valid */
        if(tcamNum > DVA_TCAM_SID_TCAM_NUM_BOUND)
        {
            ret = -(RT_ERR_TCAM_NUM);
            continue;
        }

        if(NULL == pGoldenCrc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        regAddr = DVA_SAFETY_TCAM_96X120_0_0_CTRL_CRC_GOLDEN + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_read(unit, regAddr, &regVal));
        *pGoldenCrc = regVal;
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_sid_golden_crc(uint32 unit, uint32 tcamNum, uint32 goldenCrc)
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

        regAddr = DVA_SAFETY_TCAM_96X120_0_0_CTRL_CRC_GOLDEN + (DVA_TCAM_CTRL_REG_INTERVAL_NUM * DVA_TCAM_4_BYTE_REG * tcamNum);

        CHK_FUN_CONTINUE(ret, dva_reg_write(unit, regAddr, goldenCrc));
    } while(0u == 1u);

    return ret;
}

RtkApiRet dva_drv_tcam_set_sid_all_golden_crc(uint32 unit, dva_drv_TcamSidCrcVal_t *pGoldenCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == pGoldenCrc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_golden_crc(unit, DVA_TCAM_SID_TCAM0, pGoldenCrc->val[0]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_golden_crc(unit, DVA_TCAM_SID_TCAM1, pGoldenCrc->val[1]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_golden_crc(unit, DVA_TCAM_SID_TCAM2, pGoldenCrc->val[2]));
        CHK_FUN_CONTINUE(ret, dva_drv_tcam_set_sid_golden_crc(unit, DVA_TCAM_SID_TCAM3, pGoldenCrc->val[3]));
    } while(0u == 1u);

    return ret;
}
