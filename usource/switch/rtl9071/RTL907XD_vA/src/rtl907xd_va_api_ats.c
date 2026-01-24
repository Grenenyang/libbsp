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

#include "rtl907xd_va_api_ats.h"
#include "rtl907xd_va_api_qos.h"
#include "rtl907xd_va_api_otp.h"
#include "rtkas_api_ext.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"

/**
 * @defgroup RTK_MODULE_PSCM PSCM(QCI/PSFP & QCR/ATS)
 * @{
 */

/**
 * @defgroup ATS_SCHEDULER ATS Scheduler
 * @{
 */

/**
 * @addtogroup ATS_SCHEDULER_ASIC Low Level Driver
 * @brief ATS Scheduler Low Level Driver
 * @{
 */

/**
 * @brief This API inits the ATS scheduler.
 * @param[in] unit        Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK      The ATS has been successfully initialized. \n
 * @retval Others         Refer to dva_drv_indirect_reg_field_set() for the error.
 */
RtkApiRet dva_ats_init(uint32 unit)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint8     processingDelayMax = 52u;

    do
    {
        /* Set the registers */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_PROCESSING_DELAY_MAX, DVA_ATS_PROCESSING_DELAY_MAX_PROCESSINGDELAYMAX_LSP, DVA_ATS_PROCESSING_DELAY_MAX_PROCESSINGDELAYMAX_LEN, processingDelayMax));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets the information including receive port number, transmit port number, maximum variation of clock offset, maximum deviation of clock rate, maximum recognition delay of arrival time, maximum processing delay, minimum processing delay for the ATS scheduler.
 * @param[in] unit                           Stacking unit (1 to 7) \n
 *                                           Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[out] rxPortNumber                  Rx port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) \n
 *                                           Number of reception port.
 * @param[out] txPortNumber                  Tx port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) \n
 *                                           Number of transmission port.
 * @param[out] clockOffsetVariationMax       Clock offset variation max (0 to 0xFFFFFFFF) \n
 *                                           The maximum variation of clock (in ns).
 * @param[out] clockRateDeviationMax         Clock rate deviation max (0 to 0xFFFFFFFF) \n
 *                                           The maximum deviation of clock rate (in ppm).
 * @param[out] arrivalRecognitionDelayMax    Arrival recognition delay max (0 to 0xFFFFFFFF) \n
 *                                           The maximum delay between the time that the frame is arrived and recognition by ATS scheduler (in ns).
 * @param[out] processingDelayMax            Processing delay maximum (0 to 0xFFFFFFFF) \n
 *                                           The maximum delay between the time that the frame is recognition by ATS scheduler and selection algorithm (in ns).
 * @param[out] processingDelayMin            Processing delay minimum (0 to 0xFFFFFFFF) \n
 *                                           The minimum delay between the time that the frame is recognition by ATS scheduler and selection algorithm (in ns).
 * @retval RT_ERR_OK                         The informations of ATS scheduler were obtained. \n
 * @retval -RT_ERR_NULL_POINTER              Null pointer for parameter. \n
 * @retval Others                            Refer to dva_drv_indirect_reg_field_get() for the error.
 */
RtkApiRet dva_ats_get_info(uint32 unit, AtsGetInfo_t *atsGetInfo)
{
    RtkApiRet ret                    = RT_ERR_OK;
    uint32    processingDelayMaxTick = 0u;
    uint32    addr                   = 0u;
    uint8     buf                    = 0u;
    uint32    len                    = 0u;

    do
    {
        /* Check if the parameters is valid */
        if(NULL == atsGetInfo)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        addr = 0x166u;
        len  = 1u;

        CHK_FUN_CONTINUE(ret, dva_otp_read(unit, addr, &buf, len));

        if(RTL9072D_VA_PACKAGE == buf)
        {
            atsGetInfo->rxPortNumber = RTL9072D_PORT_NUMBER;
            atsGetInfo->txPortNumber = RTL9072D_PORT_NUMBER;
        }
        if(RTL9070D_VA_PACKAGE == buf)
        {
            atsGetInfo->rxPortNumber = RTL9070D_PORT_NUMBER;
            atsGetInfo->txPortNumber = RTL9070D_PORT_NUMBER;
        }

        atsGetInfo->clockOffsetVariationMax    = 0u;
        atsGetInfo->clockRateDeviationMax      = 100u;    /* ppm */
        atsGetInfo->arrivalRecognitionDelayMax = 108000u; /* ns */
        /* Get registers */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_ATS_PROCESSING_DELAY_MAX, DVA_ATS_PROCESSING_DELAY_MAX_PROCESSINGDELAYMAX_LSP, DVA_ATS_PROCESSING_DELAY_MAX_PROCESSINGDELAYMAX_LEN, &processingDelayMaxTick));
        atsGetInfo->processingDelayMax = (processingDelayMaxTick * TICK_NS_CONVERT);
        atsGetInfo->processingDelayMin = (PROCESS_DELAY_MIN_TICK * TICK_NS_CONVERT);

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API sets the configurations including scheduler ID, group ID, CBS and CIR for an ATS scheduler.
 * @param[in] unit        Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] schedulerId Scheduler identification (0 to 95) \n
 *                        The scheduler identification of ATS.
 * @param[in] groupId     Group identification (0 to 95) \n
 *                        The group identification of ATS.
 * @param[in] cbs         Committed burst size, in bytes (0x54 to 0xFFFFFFFF) \n
 *                        The CommittedBurstSize parameter defines the maximum token capacity of the token bucket.
 * @param[in] cirKbps     Committed Information Rate least significant bit, in kbps (500 to 10000000) \n
 *                        The CommittedInformationRate parameter defines the rate at which the token bucket is refilled with tokens until the maximum token capacity of the token bucket is reached.
 * @param[out] None
 * @retval RT_ERR_OK                       The settings of ATS were configured. \n
 * @retval -RT_ERR_ATS_SCHEDULER_ID         The ATS scheduler ID was out of range. \n
 * @retval -RT_ERR_ATS_SCHEDULER_GROUP_ID   The ATS scheduler group ID was out of range. \n
 * @retval -RT_ERR_ATS_CBS                  The cbs was out of range. \n
 * @retval -RT_ERR_ATS_CIR                  The cir was out of range. \n
 * @retval -RT_ERR_ATS_EMPTY_TO_FULL_TIME   The empty to full time can't exceeds 10 millisecond \n
 * @retval Others                          Refer to dva_drv_indirect_reg_field_set() for the error.
 */
RtkApiRet dva_ats_cbs_cir_set(uint32 unit, uint8 schedulerId, uint8 groupId, uint32 cbs, uint64 cirKbps)
{
    RtkApiRet ret                = RT_ERR_OK;
    uint64    empty2FullTimeTick = 0u;
    uint8     offsetIndex        = 0u;
    uint8     bitOffset          = 0u;
    uint8     groupLen           = 0u;
    uint64    systemCIR          = 0u;
    uint64    sysClk250          = SYSTEM_CLOCK_250M;
    uint64    fractionalBits29   = FRACTIONAL_BITS_29;
    uint32    cirMsb             = 0u;
    uint32    cirLsb             = 0u;

    do
    {
        /* Check if the schedulerId is valid */
        if(QCR_SCHEDULER_MAX < schedulerId)
        {
            ret = -RT_ERR_ATS_SCHEDULER_ID;
            continue;
        }

        /* Check if the groupId is valid */
        if(QCR_GROUPID_MAX < groupId)
        {
            ret = -RT_ERR_ATS_SCHEDULER_GROUP_ID;
            continue;
        }

        /* Check if the cbs is valid */
        if(QCR_CBS_MIN > cbs)
        {
            ret = -RT_ERR_ATS_CBS;
            continue;
        }

        /* Check if the cir is valid */
        if((QCR_CIR_MIN > cirKbps) || (QCR_CIR_MAX < cirKbps))
        {
            ret = -RT_ERR_ATS_CIR;
            continue;
        }

        /* CIR kbps -> bps -> Bps -> tick per byte */
        systemCIR = (sysClk250 << fractionalBits29) / ((cirKbps * 1000u) >> BIT2BYTE);
        cirLsb    = (uint32)(systemCIR & 0xFFFFFFFFu);
        cirMsb    = (uint32)((systemCIR >> 32u) & 0x7FFFFu);

        /* Calculate scheduler empty to full time */
        empty2FullTimeTick = (cbs * systemCIR) >> FRACTIONAL_BITS_29;

        /* empty2FullTimeTick can't exceeds 10 millisecond */
        if(empty2FullTimeTick > SYSTEM_CLOCK_10MS_TICK)
        {
            ret = -RT_ERR_ATS_EMPTY_TO_FULL_TIME;
            continue;
        }

        /* Calculate the ATS schduler group reg offset index and bit offset */
        if(0u == (schedulerId % 2u))
        {
            offsetIndex = (uint8)((schedulerId / 2u) & 0xFFu);
            bitOffset   = DVA_ATS_SCHEDULER_GROUP_0_GROUPID_0_LSP;
            groupLen    = DVA_ATS_SCHEDULER_GROUP_0_GROUPID_0_LEN;
        }
        else
        {
            offsetIndex = (uint8)(((schedulerId - 1u) / 2u) & 0xFFu);
            bitOffset   = DVA_ATS_SCHEDULER_GROUP_0_GROUPID_1_LSP;
            groupLen    = DVA_ATS_SCHEDULER_GROUP_0_GROUPID_1_LEN;
        }

        /* Set register */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_SCHEDULER_COMMITTED_BURST_SIZE_0 + (REG_OFFSET_4 * (uint32)schedulerId), DVA_ATS_SCHEDULER_COMMITTED_BURST_SIZE_0_COMMITTEDBURSTSIZE_LSP, DVA_ATS_SCHEDULER_COMMITTED_BURST_SIZE_0_COMMITTEDBURSTSIZE_LEN, cbs));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_LSB_0 + (REG_OFFSET_8 * (uint32)schedulerId), DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_LSB_0_COMMITTEDINFORMATIONRATE_LSB_LSP, DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_LSB_0_COMMITTEDINFORMATIONRATE_LSB_LEN, cirLsb));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_MSB_0 + (REG_OFFSET_8 * (uint32)schedulerId), DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_MSB_0_COMMITTEDINFORMATIONRATE_MSB_LSP, DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_MSB_0_COMMITTEDINFORMATIONRATE_MSB_LEN, cirMsb));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_SCHEDULER_EMPTY_TO_FULL_TIME_0 + (REG_OFFSET_4 * (uint32)schedulerId), DVA_ATS_SCHEDULER_EMPTY_TO_FULL_TIME_0_EMPTYTOFULLTIME_LSP, DVA_ATS_SCHEDULER_EMPTY_TO_FULL_TIME_0_EMPTYTOFULLTIME_LEN, (uint32)(empty2FullTimeTick & 0xFFFFFFFF)));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_SCHEDULER_GROUP_0 + (REG_OFFSET_4 * (uint32)offsetIndex), bitOffset, groupLen, groupId));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets the configurations including scheduler ID, group ID, CBS and CIR for an ATS scheduler.
 * @param[in]  unit                    Stacking unit (1 to 7) \n
 *                                     Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in]  schedulerId             Scheduler identification (0 to 95) \n
 *                                     The scheduler identification of ATS.
 * @param[out] pGroupId                Group identification (0 to 95) \n
 *                                     The group identification of ATS.
 * @param[out] pCbs                    Committed burst size, in bytes (0x54 to 0xFFFFFFFF) \n
 *                                     The CommittedBurstSize parameter defines the maximum token capacity of the token bucket.
 * @param[out] pCirKbps                Committed Information Rate, in kbps (500 to 10000000) \n
 *                                     The CommittedInformationRate parameter defines the rate at which the token bucket is refilled with tokens until the maximum token capacity of the token bucket is reached.
 * @retval RT_ERR_OK                   The settings of ATS scheduler were obtained. \n
 * @retval -RT_ERR_ATS_SCHEDULER_ID     The ATS scheduler ID was out of range. \n
 * @retval -RT_ERR_NULL_POINTER        Null pointer for parameter. \n
 * @retval Others                      Refer to dva_drv_indirect_reg_field_get() for the error.
 */
RtkApiRet dva_ats_cbs_cir_get(uint32 unit, uint8 schedulerId, uint8 *pGroupId, uint32 *pCbs, uint64 *pCirKbps)
{
    RtkApiRet ret               = RT_ERR_OK;
    uint64    cirTpb            = 0u;
    uint64    cirBitsPerSecond  = 0u;
    uint64    cirBytesPerSecond = 0u;
    uint8     offsetIndex       = 0u;
    uint8     bitOffset         = 0u;
    uint8     groupLen          = 0u;
    uint32    cirMsb            = 0u;
    uint32    cirLsb            = 0u;
    uint32    data              = 0u;
    uint64    sysClk250         = SYSTEM_CLOCK_250M;
    uint64    fractionalBits29  = FRACTIONAL_BITS_29;

    do
    {
        /* Check if the schedulerId is valid */
        if(QCR_SCHEDULER_MAX < schedulerId)
        {
            ret = -RT_ERR_ATS_SCHEDULER_ID;
            continue;
        }

        /* Check if the parameters is valid */
        if((NULL == pGroupId) || (NULL == pCbs) || (NULL == pCirKbps))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Calculate the ATS schduler group reg offset index and bit offset */
        if(0u == (schedulerId % 2u))
        {
            offsetIndex = (uint8)((schedulerId / 2u) & 0xFFu);
            bitOffset   = DVA_ATS_SCHEDULER_GROUP_0_GROUPID_0_LSP;
            groupLen    = DVA_ATS_SCHEDULER_GROUP_0_GROUPID_0_LEN;
        }
        else
        {
            offsetIndex = (uint8)(((schedulerId - 1u) / 2u) & 0xFFu);
            bitOffset   = DVA_ATS_SCHEDULER_GROUP_0_GROUPID_1_LSP;
            groupLen    = DVA_ATS_SCHEDULER_GROUP_0_GROUPID_1_LEN;
        }

        /* Get registers */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_ATS_SCHEDULER_COMMITTED_BURST_SIZE_0 + (REG_OFFSET_4 * (uint32)schedulerId), DVA_ATS_SCHEDULER_COMMITTED_BURST_SIZE_0_COMMITTEDBURSTSIZE_LSP, DVA_ATS_SCHEDULER_COMMITTED_BURST_SIZE_0_COMMITTEDBURSTSIZE_LEN, pCbs));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_LSB_0 + (REG_OFFSET_8 * (uint32)schedulerId), DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_LSB_0_COMMITTEDINFORMATIONRATE_LSB_LSP, DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_LSB_0_COMMITTEDINFORMATIONRATE_LSB_LEN, &cirLsb));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_MSB_0 + (REG_OFFSET_8 * (uint32)schedulerId), DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_MSB_0_COMMITTEDINFORMATIONRATE_MSB_LSP, DVA_ATS_SCHEDULER_COMMITTED_INFORMATION_RATE_MSB_0_COMMITTEDINFORMATIONRATE_MSB_LEN, &cirMsb));

        if((0u != cirLsb) || (0u != cirMsb))
        {
            cirTpb            = (((uint64)cirMsb << 32u) | cirLsb);
            cirBytesPerSecond = (sysClk250 << fractionalBits29) / cirTpb;
            cirBitsPerSecond  = cirBytesPerSecond << BIT2BYTE;
            *pCirKbps         = cirBitsPerSecond / 1000u;
        }
        else
        {
            *pCirKbps = 0u;
        }

        /* Get registers */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_ATS_SCHEDULER_GROUP_0 + (REG_OFFSET_4 * (uint32)offsetIndex), bitOffset, groupLen, &data));

        *pGroupId = (uint8)(data & 0xFFu);
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API sets the group residence time for the ATS scheduler. \n
 * @param[in] unit                           Stacking unit (1 to 7) \n
 *                                           Stacking unit ID is used for indicating which switch to be accessed.
 * @param[in] groupId                        Group identification (0 to 95) \n
 *                                           The group identification of ATS.
 * @param[in] maxResidenceTimeNs             Maximum residence time, in nanoseconds (0 to 10000000) \n
 *                                           The maximum residence time limits the duration for which frames can reside in a Bridge.
 * @param[out] None
 * @retval RT_ERR_OK                         Successfully set the residence time for the ATS Scheduler group. \n
 * @retval -RT_ERR_ATS_SCHEDULER_GROUP_ID     The ATS scheduler group ID was out of range. \n
 * @retval -RT_ERR_MAX_RESIDENCE_TIME         The max residence time was out of range. \n
 * @retval Others                            Refer to dva_drv_indirect_reg_field_set() for the error.
 */
RtkApiRet dva_ats_group_residence_time_set(uint32 unit, uint8 groupId, uint32 maxResidenceTimeNs)
{
    RtkApiRet ret                  = RT_ERR_OK;
    uint32    maxResidenceTimeTick = 0u;

    do
    {
        /* Check if the groupId is valid */
        if(QCR_GROUPID_MAX < groupId)
        {
            ret = -RT_ERR_ATS_SCHEDULER_GROUP_ID;
            continue;
        }

        /* 0~3=>0, 4~7=>1, 8~11=>2, ... */
        maxResidenceTimeTick = maxResidenceTimeNs / TICK_NS_CONVERT;

        /* Check if the maxResidenceDuration is valid, and  maxResidenceDuration can't greater than 10ms */
        if((MAX_RESIDENCE_TIME < maxResidenceTimeNs) || (maxResidenceTimeTick > SYSTEM_CLOCK_10MS_TICK))
        {
            ret = -RT_ERR_ATS_MAX_RESIDENCE_TIME;
            continue;
        }

        /* Set register */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_GROUP_MAX_RESIDENCE_TIME_0 + (REG_OFFSET_4 * (uint32)groupId), DVA_ATS_GROUP_MAX_RESIDENCE_TIME_0_MAXRESIDENCETIME_LSP, DVA_ATS_GROUP_MAX_RESIDENCE_TIME_0_MAXRESIDENCETIME_LEN, maxResidenceTimeTick));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets the configurations including group residence time for the ATS scheduler. \n
 * @param[in] unit                           Stacking unit (1 to 7) \n
 *                                           Stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] groupId                        Group identification (0 to 95) \n
 *                                           The group identification of ATS.
 * @param[out] pMaxResidenceTimeNs           Maximum residence time, in nanoseconds (0 to 10000000) \n
 *                                           The maximum residence time limits the duration for which frames can reside in a Bridge.
 * @retval RT_ERR_OK                         Obtain the residence time settings for the ATS Scheduler group. \n
 * @retval -RT_ERR_ATS_SCHEDULER_GROUP_ID     The ATS scheduler group ID was out of range. \n
 * @retval -RT_ERR_NULL_POINTER              Null pointer for parameter. \n
 * @retval Others                            Refer to dva_drv_indirect_reg_field_get() for the error.
 */
RtkApiRet dva_ats_group_residence_time_get(uint32 unit, uint8 groupId, uint32 *pMaxResidenceTimeNs)
{
    RtkApiRet ret                  = RT_ERR_OK;
    uint32    maxResidenceTimeTick = 0u;

    do
    {
        /* Check if the groupId is valid */
        if(QCR_GROUPID_MAX < groupId)
        {
            ret = -RT_ERR_ATS_SCHEDULER_GROUP_ID;
            continue;
        }

        /* Check if the parameters is valid */
        if(NULL == pMaxResidenceTimeNs)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }
        /* Get register */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_ATS_GROUP_MAX_RESIDENCE_TIME_0 + (REG_OFFSET_4 * (uint32)groupId), DVA_ATS_GROUP_MAX_RESIDENCE_TIME_0_MAXRESIDENCETIME_LSP, DVA_ATS_GROUP_MAX_RESIDENCE_TIME_0_MAXRESIDENCETIME_LEN, &maxResidenceTimeTick));

        *pMaxResidenceTimeNs = maxResidenceTimeTick * TICK_NS_CONVERT;

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets the drop counter for a specific port of ATS scheduler. \n
 * @param[in] unit             Stacking unit (1 to 7) \n
 *                             Stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] port             Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) \n
 *                             Obtain the drop counter for a specific port.
 * @param[in] pCounter         Counter (0 to 0xFFFFFFFF) \n
 *                             Discard frames count.
 * @param[out] None
 * @retval RT_ERR_OK             Successfully obtained the drop counter for a specific port in the ATS Scheduler. \n
 * @retval -RT_ERR_PORT_ID       The port was out of range. \n
 * @retval -RT_ERR_NULL_POINTER  Null pointer for parameter. \n
 * @retval Others                Refer to dva_drv_indirect_reg_field_get() for the error.
 */
RtkApiRet dva_ats_drop_counter_get(uint32 unit, RtkPort port, uint64 *pCounter)
{
    RtkApiRet ret         = RT_ERR_OK;
    uint32    pCounterLsb = 0u;
    uint32    pCounterMsb = 0u;

    do
    {
        /* Check if the port is valid */
        if(0u == DVA_IS_VALID_PORT_ID_WO_CPUPORT(port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if the groupId is valid */
        if(NULL == pCounter)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get register */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_ATS_DROP_COUNTER_PORT_LSB_0 + (REG_OFFSET_8 * port), DVA_ATS_DROP_COUNTER_PORT_LSB_0_DROPCOUNTER_LSB_LSP, DVA_ATS_DROP_COUNTER_PORT_LSB_0_DROPCOUNTER_LSB_LEN, &pCounterLsb));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_ATS_DROP_COUNTER_PORT_MSB_0 + (REG_OFFSET_8 * port), DVA_ATS_DROP_COUNTER_PORT_MSB_0_DROPCOUNTER_MSB_LSP, DVA_ATS_DROP_COUNTER_PORT_MSB_0_DROPCOUNTER_MSB_LEN, &pCounterMsb));

        *pCounter = (((uint64)pCounterMsb << 32u) | pCounterLsb);

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API clears the drop counter for a specific port of ATS scheduler. \n
 * @param[in] unit                   Stacking unit (1 to 7) \n
 *                                   Stacking unit ID is used for indicating which switch is to be accessed.
 * @param[in] port                   Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) \n
 *                                   The port for which the drop counter needs to be cleared.
 * @param[out] None
 * @retval RT_ERR_OK                 The drop counter for a specific port of ATS were cleared. \n
 * @retval -RT_ERR_PORT_ID           The port was out of range. \n
 * @retval Others                    Refer to dva_drv_indirect_reg_field_set() for the error.
 */
RtkApiRet dva_ats_drop_counter_clear(uint32 unit, RtkPort port)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the port is valid */
        if(0u == DVA_IS_VALID_PORT_ID_WO_CPUPORT(port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Set register */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_DROP_COUNTER_PORT_LSB_0 + (REG_OFFSET_8 * port), 0u, 32u, 1u));
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_DROP_COUNTER_PORT_MSB_0 + (REG_OFFSET_8 * port), 0u, 32u, 1u));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API sets the transmission selection algorithm to Asynchronous Traffic Shaping for a specific port and specific queue.
 * @param[in] unit                  Stacking unit (1 to 7) \n
 *                                  Stacking unit ID is used to indicate which switch to access.
 * @param[in] port                  Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) \n
 *                                  Egress port number.
 * @param[in] que                   Queue (0 to 7) \n
 *                                  Queue identification for a specific port.
 * @param[in] mode                  Mode (0 to 1) \n
 *                                  Queue valid bit for a specific port.
 * @param[out] None
 * @retval RT_ERR_OK                The settings of the ATS scheduler for a specific queue on a specific port were configured.\n
 * @retval -RT_ERR_PORT_ID          The port was out of range. \n
 * @retval -RT_ERR_QUEUE_ID         The queue ID was out of range.
 * @retval -RT_ERR_INPUT                    The index was out of range. \n
 * @retval Others                   Refer to dva_drv_indirect_reg_field_set(), dva_drv_indirect_reg_field_get() for the error.
 */
RtkApiRet dva_ats_scheduler_set(uint32 unit, RtkPort port, QosQueNum queue, RtkEnable mode)
{
    RtkApiRet ret                  = RT_ERR_OK;
    RtkQoSSch queSch               = DVA_QOS_SCH_ATS;
    uint32    atsQueueValidBitmask = 0u;

    do
    {
        /* Check if the port is valid */
        if(0u == DVA_IS_VALID_PORT_ID_WO_CPUPORT(port))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        if(queue > QOS_QUE_MAX)
        {
            ret = -RT_ERR_QUEUE_ID;
        }

        if((ENABLE != mode) && (DISABLE != mode))
        {
            ret = -RT_ERR_INPUT;
        }

        /* Set tx algorithm*/
        CHK_FUN_CONTINUE(ret, dva_qos_que_sch_set(unit, port, queue, queSch));

        if(ENABLE == mode)
        {
            /* Set the ats queue valid bit 1 */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_QUEUE_PORT_0 + (NEXT_REG_OFFSET * port), DVA_ATS_QUEUE_PORT_0_ATS_QUEUE_VALID_LSP, DVA_ATS_QUEUE_PORT_0_ATS_QUEUE_VALID_LEN, (uint32)(1UL << ((uint32)queue))));
        }
        else
        {
            /* Set the ATS queue valid bit 0 */
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_get(unit, DVA_ATS_QUEUE_PORT_0 + (NEXT_REG_OFFSET * port), DVA_ATS_QUEUE_PORT_0_ATS_QUEUE_VALID_LSP, DVA_ATS_QUEUE_PORT_0_ATS_QUEUE_VALID_LEN, &atsQueueValidBitmask));
            atsQueueValidBitmask &= (uint32)(~((uint32)1u << (uint32)queue));
            CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_QUEUE_PORT_0 + (NEXT_REG_OFFSET * port), DVA_ATS_QUEUE_PORT_0_ATS_QUEUE_VALID_LSP, DVA_ATS_QUEUE_PORT_0_ATS_QUEUE_VALID_LEN, atsQueueValidBitmask));
        }

        /* Set the mismatch policy is pass - default */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_ATS_QUEUE_PORT_0 + (NEXT_REG_OFFSET * port), DVA_ATS_QUEUE_PORT_0_ATS_MISMATCH_POLICY_LSP, DVA_ATS_QUEUE_PORT_0_ATS_MISMATCH_DETECT_LEN, DISABLE));

        /* Set the ATS port queue enable */
        CHK_FUN_CONTINUE(ret, dva_drv_indirect_reg_field_set(unit, DVA_PORT0_ATS_QUEUE_CONTROL + (PORT_OFFSET * (uint32)dva_g_userPort2sysPort[port]), DVA_PORT0_ATS_QUEUE_CONTROL_ATSQ_EN_LSP, DVA_PORT0_ATS_QUEUE_CONTROL_ATSQ_EN_LEN, ENABLE));
    } while(0u == 1u);

    return ret;
}

/**@}*/ /*ATS_SCHEDULER_ASIC*/

/**@}*/ /* ATS_SCHEDULER */

/**@}*/ /* RTK_MODULE_PSCM */
