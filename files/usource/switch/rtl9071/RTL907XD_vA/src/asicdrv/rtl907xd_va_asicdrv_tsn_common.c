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

#include "rtl907xd_va_asicdrv_tsn_common.h"
#include "rtkas_types.h"
#include "rtl907xd_va_reg_list.h"
#include "rtl907xd_va_regField_list.h"
#include "rtl907xd_va_api.h"

static void  dva_tsn_simple_bigint_multiplication(dva_RtkTsnLongNanosecond_t multiplicand, uint32 multiplier, dva_RtkTsnLongNanosecond_t *product);
static void  dva_tsn_simple_bigint_division(dva_RtkTsnLongNanosecond_t dividend, uint32 divisor, dva_RtkTsnLongNanosecond_t *quotient);
static void  dva_tsn_simple_bigint_subtraction(dva_RtkTsnLongNanosecond_t minuend, dva_RtkTsnLongNanosecond_t subtrahend, dva_RtkTsnLongNanosecond_t *difference);
static int32 dva_tsn_compare_ptptime(RtkTsnPtpTime_t time0, RtkTsnPtpTime_t time1);
static void  dva_tsn_convert_ptptime_to_long_nanosecond(RtkTsnPtpTime_t ptptime, dva_RtkTsnLongNanosecond_t *longNs);
/* note. this is for 'rough estimation' purpose. */
/* if you need PTP time, please refer the PTP SDK. */

/**
 * @brief
 *
 * @param[in] currentTime
 * @retval
 */
RtkApiRet dva_drv_tsn_current_time_get(uint32 unit, RtkTsnPtpTime_t *currentTime)
{
    RtkApiRet result          = RT_ERR_OK;
    uint32    registerAddress = 0u;
    uint32    selectedClock   = 0u;
    uint32    ns = 0u, sLsb = 0u, sMsb = 0u, sLsbCheck = 0u;
    int32     breakFlag = 0;

    do
    {
        if(currentTime == NULL)
        {
            result = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* check mux for selected clock */
        CHK_FUN_CONTINUE(result, dva_reg_field_read(unit,
                                                    DVA_QBV_CONTROL0,
                                                    DVA_QBV_CONTROL0_QBV_PTP_SLAVE_TIME_SEL_LSP,
                                                    DVA_QBV_CONTROL0_QBV_PTP_SLAVE_TIME_SEL_LEN,
                                                    &selectedClock));

        /* read PTP time */
        registerAddress = DVA_SLAVE0_TIME_NS + ((5u * 4u) * selectedClock);
        do
        {
            result = dva_reg_read(unit, (registerAddress + (2u * 4u)), &sMsb);
            if(result != RT_ERR_OK)
            {
                breakFlag = 1;
                continue;
            }
            result = dva_reg_read(unit, (registerAddress + (1u * 4u)), &sLsb);
            if(result != RT_ERR_OK)
            {
                breakFlag = 1;
                continue;
            }
            result = dva_reg_read(unit, registerAddress, &ns);
            if(result != RT_ERR_OK)
            {
                breakFlag = 1;
                continue;
            }
            result = dva_reg_read(unit, (registerAddress + (1u * 4u)), &sLsbCheck);
            if(result != RT_ERR_OK)
            {
                breakFlag = 1;
                continue;
            }
            /* ensure no ns -> second happen during sequential read. */
        } while(sLsb != sLsbCheck);

        if(breakFlag == 0)
        {
            currentTime->nanosecond = ns;
            currentTime->lsbSecond  = sLsb;
            currentTime->msbSecond  = (uint16)sMsb;
        }

    } while(0u == 1u);

    return result;
}

/**
 * @brief
 *
 * @param[in] ptptime
 * @param[in] longNs
 */
static void dva_tsn_convert_ptptime_to_long_nanosecond(RtkTsnPtpTime_t ptptime, dva_RtkTsnLongNanosecond_t *longNs)
{
    uint64 temp0 = 0uLL, temp1 = 0uLL;

    temp0 = (((uint64)ptptime.lsbSecond) * ((uint64)DVA_TSN_1B_NANOSECOND)) + ((uint64)ptptime.nanosecond);
    temp1 = (((uint64)ptptime.msbSecond) * ((uint64)DVA_TSN_1B_NANOSECOND)) + (temp0 >> 32);

    longNs->section[0] = ((uint32)(temp0 & 0xFFFFFFFFuLL));
    longNs->section[1] = ((uint32)(temp1 & 0xFFFFFFFFuLL));
    longNs->section[2] = ((uint32)(temp1 >> 32));
}

/**
 * @brief
 *
 * @param[in] longNs
 * @param[in] ptptime
 */
void dva_drv_tsn_cov_ns_to_ptptime(dva_RtkTsnLongNanosecond_t longNs, RtkTsnPtpTime_t *ptptime)
{
    uint64 temp0 = 0uLL, temp1 = 0uLL;

    temp0 = (((uint64)longNs.section[2]) << 32) | ((uint64)longNs.section[1]);
    temp1 = ((temp0 % (uint64)DVA_TSN_1B_NANOSECOND) << 32) | ((uint64)longNs.section[0]);

    ptptime->msbSecond  = (uint16)(temp0 / (uint64)DVA_TSN_1B_NANOSECOND);
    ptptime->lsbSecond  = (uint32)(temp1 / (uint64)DVA_TSN_1B_NANOSECOND);
    ptptime->nanosecond = (uint32)(temp1 % (uint64)DVA_TSN_1B_NANOSECOND);
}

/**
 * @brief
 *
 * @param[in] time0
 * @param[in] time1
 * @retval
 */
static int32 dva_tsn_compare_ptptime(RtkTsnPtpTime_t time0, RtkTsnPtpTime_t time1)
{
    uint64 second0 = 0uLL;
    uint64 second1 = 0uLL;

    int32 ret = 0;

    /* convert type explicitly */
    second0 = (((uint64)time0.msbSecond) << 32) | ((uint64)time0.lsbSecond);
    second1 = (((uint64)time1.msbSecond) << 32) | ((uint64)time1.lsbSecond);

    do
    {
        if(second0 > second1)
        {
            ret = 1;
            continue;
        }
        else if(second0 < second1)
        {
            ret = -1;
            continue;
        }
        else
        {
            /* second are equal, compare nanosecond */
            if(time0.nanosecond > time1.nanosecond)
            {
                ret = 1;
                continue;
            }
            else if(time0.nanosecond < time1.nanosecond)
            {
                ret = 1;
                continue;
            }
            else
            {
                /* time1.nanosecond == time_2.nanosecond */
                ret = 0;
                continue;
            }
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief
 *
 * @param[in] a
 * @param[in] b
 * @retval
 */
int32 dva_drv_tsn_bigint_comparison(dva_RtkTsnLongNanosecond_t const *const a, dva_RtkTsnLongNanosecond_t const *const b)
{
    int32 i = 0, ret = 0, breakFlag = 0;

    do
    {
        for(i = 0; (i != 3); ++i)
        {
            if(0 != breakFlag)
            {
                break;
            }
            if(a->section[(2 - i)] > b->section[(2 - i)])
            {
                /* check section[2], section[1], section[0] in order. */
                ret       = 1;
                breakFlag = 1;
                continue;
            }
            else if(a->section[(2 - i)] < b->section[(2 - i)])
            {
                ret       = -1;
                breakFlag = 1;
                continue;
            }
            else
            {
                if(i == 2)
                {
                    /* when reach here, a == b */
                    ret       = 0;
                    breakFlag = 1;
                    continue;
                }
            }
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief
 *
 * @param[in] addened1st
 * @param[in] addened2nd
 * @param[in] sum
 */
void dva_drv_tsn_bigint_addition(dva_RtkTsnLongNanosecond_t addened1st, dva_RtkTsnLongNanosecond_t addened2nd, dva_RtkTsnLongNanosecond_t *sum)
{
    /* Note. this is not complete bigint implementation. Only considered for API in this c file. */
    uint64 extendedSum = 0uLL;
    uint64 carry       = 0uLL;

    extendedSum     = (uint64)addened1st.section[0] + (uint64)addened2nd.section[0];
    sum->section[0] = (uint32)(extendedSum & 0xFFFFFFFFuLL);
    carry           = ((extendedSum >> 32) & 0xFFFFFFFFuLL);

    extendedSum     = (uint64)addened1st.section[1] + (uint64)addened2nd.section[1] + carry;
    sum->section[1] = (uint32)(extendedSum & 0xFFFFFFFFuLL);
    carry           = ((extendedSum >> 32) & 0xFFFFFFFFuLL);

    extendedSum     = (uint64)addened1st.section[2] + (uint64)addened2nd.section[2] + carry;
    sum->section[2] = (uint32)(extendedSum & 0xFFFFFFFFuLL);
}

/**
 * @brief
 *
 * @param[in] minuend
 * @param[in] subtrahend
 * @param[in] difference
 */
static void dva_tsn_simple_bigint_subtraction(dva_RtkTsnLongNanosecond_t minuend, dva_RtkTsnLongNanosecond_t subtrahend, dva_RtkTsnLongNanosecond_t *difference)
{
    dva_RtkTsnLongNanosecond_t minuendformodify;
    (void)dva_drv_memcpy(&minuendformodify, &minuend, sizeof(dva_RtkTsnLongNanosecond_t));
    /* Note. this is not complete bigint implementation. Only considered for API in this c file. */
    /* difference = minuend - subtrahend, assume caller ensure minued >= subtrahend */

    if(minuendformodify.section[0] >= subtrahend.section[0])
    {
        difference->section[0] = minuendformodify.section[0] - subtrahend.section[0];
    }
    else
    {
        difference->section[0]      = minuendformodify.section[0] + 1u + (0xFFFFFFFFu - subtrahend.section[0]);
        minuendformodify.section[1] = minuendformodify.section[1] - 1u;
    }

    if(minuendformodify.section[1] >= subtrahend.section[1])
    {
        difference->section[1] = minuendformodify.section[1] - subtrahend.section[1];
    }
    else
    {
        difference->section[1]      = minuendformodify.section[1] + 1u + (0xFFFFFFFFu - subtrahend.section[1]);
        minuendformodify.section[2] = minuendformodify.section[2] - 1u;
    }

    difference->section[2] = minuendformodify.section[2] - subtrahend.section[2];
}

/**
 * @brief
 *
 * @param[in] dividend
 * @param[in] divisor
 * @param[in] quotient
 */
static void dva_tsn_simple_bigint_division(dva_RtkTsnLongNanosecond_t dividend, uint32 divisor, dva_RtkTsnLongNanosecond_t *quotient)
{
    /* Note. this is not complete bigint implementation. Only considered for API in this c file. */
    uint64 temp[2]     = {0uLL, 0uLL};
    uint64 castDivisor = (uint64)divisor;

    quotient->section[2] = dividend.section[2] / divisor;

    temp[0]              = ((((uint64)dividend.section[2] % (uint64)divisor)) << 32) | (uint64)dividend.section[1];
    quotient->section[1] = (uint32)(temp[0] / castDivisor);

    temp[1]              = ((temp[0] % castDivisor) << 32) | (uint64)dividend.section[0];
    quotient->section[0] = (uint32)(temp[1] / castDivisor);
}

/**
 * @brief
 *
 * @param[in] multiplicand
 * @param[in] multiplier
 * @param[in] product
 */
static void dva_tsn_simple_bigint_multiplication(dva_RtkTsnLongNanosecond_t multiplicand, uint32 multiplier, dva_RtkTsnLongNanosecond_t *product)
{
    /* Note. this is not complete bigint implementation. Only considered for API in this c file. */
    uint64 temp[3]        = {0uLL, 0uLL, 0uLL};
    uint64 castMultiplier = (uint64)multiplier;

    temp[0]             = ((uint64)multiplicand.section[0]) * castMultiplier;
    product->section[0] = (uint32)(temp[0] & 0xFFFFFFFFuLL);

    temp[1]             = (((uint64)multiplicand.section[1]) * castMultiplier) + (temp[0] >> 32);
    product->section[1] = (uint32)(temp[1] & 0xFFFFFFFFuLL);

    temp[2]             = (((uint64)multiplicand.section[2]) * castMultiplier) + (temp[1] >> 32);
    product->section[2] = (uint32)(temp[2] & 0xFFFFFFFFuLL);
}

/**
 * @brief
 *
 * @param[in] baseTime
 * @param[in] currentTime
 * @param[in] cycleTime
 * @param[in] estimatedLongNs
 */
void dva_drv_tsn_multicycle_est(RtkTsnPtpTime_t baseTime, RtkTsnPtpTime_t currentTime, uint32 cycleTime, dva_RtkTsnLongNanosecond_t *estimatedLongNs)
{
    dva_RtkTsnLongNanosecond_t baseTimeLongNs;
    dva_RtkTsnLongNanosecond_t currentTimeLongNs;
    dva_RtkTsnLongNanosecond_t deltaLongNs;
    dva_RtkTsnLongNanosecond_t temp;

    if(dva_tsn_compare_ptptime(baseTime, currentTime) >= 0)
    {
        /* this means base time is equal or greater than current time */
        estimatedLongNs->section[0] = 0u;
        estimatedLongNs->section[1] = 0u;
        estimatedLongNs->section[2] = 0u;
    }
    else
    {
        /* convert PTP format to long ns for calculation */
        dva_tsn_convert_ptptime_to_long_nanosecond(baseTime, &baseTimeLongNs);
        dva_tsn_convert_ptptime_to_long_nanosecond(currentTime, &currentTimeLongNs);

        /* compute delta_time = currentTime - baseTime */
        dva_tsn_simple_bigint_subtraction(currentTimeLongNs, baseTimeLongNs, &deltaLongNs);

        /* compute integer multiple cycle time by cycleTime * (delta_time/cycleTime); this will truncate the remainder. */
        dva_tsn_simple_bigint_division(deltaLongNs, cycleTime, &temp);
        dva_tsn_simple_bigint_multiplication(temp, cycleTime, estimatedLongNs);
    }
}

/**
 * @brief
 * @param[in] ptptime
 * @param[out] outptptime
 */
void dva_drv_tsn_cov_half_ptptime(RtkTsnPtpTime_t *ptptime, RtkTsnPtpTime_t *outptptime)
{
    /* Set second second to (((uint64)ptptime->msbSecond << 32u) | (uint64)ptptime->lsbSecond) : get whole second */
    uint64 second = (((uint64)ptptime->msbSecond << 32u) | (uint64)ptptime->lsbSecond);
    /* Check if second % 2 is not 0 : check if second is odd number */
    if(0u != (second % 2u))
    {
        /* Set outptptime->nanosecond to (ptptime->nanosecond / 2u) + (uint32)500000000u : add half-seconde to nanosecond */
        outptptime->nanosecond = (ptptime->nanosecond / 2u) + (uint32)500000000u;
    }
    else
    {
        /* Second is even number */
        /* Set outptptime->nanosecond to (ptptime->nanosecond / 2u) */
        outptptime->nanosecond = (ptptime->nanosecond / 2u);
    }
    /* Set outptptime->lsbSecond to(uint32)(second / 2u) */
    outptptime->lsbSecond = (uint32)(second / 2u);
    /* Set outptptime->msbSecond to (uint16)((second / 2u) >> 32) & 0xFFFFu */
    outptptime->msbSecond = (uint16)((second / 2u) >> 32) & 0xFFFFu;
}

/**
 * @brief
 * @param[in] ptptime
 * @param[in] baseTimeOffset
 * @param[out] baseTime
 */
void dva_drv_tsn_ptptime_addition(RtkTsnPtpTime_t ptptime, RtkTsnPtpTime_t baseTimeOffset, RtkTsnPtpTime_t *baseTime)
{
    dva_RtkTsnLongNanosecond_t ptptimeLongNs;
    dva_RtkTsnLongNanosecond_t baseTimeTimeLongNs;
    dva_RtkTsnLongNanosecond_t sum;
    /* convert PTP format to bigint for calculation */
    dva_tsn_convert_ptptime_to_long_nanosecond(ptptime, &ptptimeLongNs);
    dva_tsn_convert_ptptime_to_long_nanosecond(baseTimeOffset, &baseTimeTimeLongNs);

    /* compute sum_time = ptptimeLongNs + baseTimeTimeLongNs */
    dva_drv_tsn_bigint_addition(ptptimeLongNs, baseTimeTimeLongNs, &sum);
    /* Covert bigint to ptp format */
    dva_drv_tsn_cov_ns_to_ptptime(sum, baseTime);
}