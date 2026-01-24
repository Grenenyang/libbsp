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

#ifndef RTKAS_API_ATS_H__
#define RTKAS_API_ATS_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/

#include "rtkas_common.h"

#define QOS_QUE_MAX (7u)

typedef uint32 QosQueNum; /**< Tx queue number (0 to 7)*/

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define QCR_SCHEDULER_MAX      (0x5Fu)
#define QCR_GROUPID_MAX        (0x5Fu)
#define QCR_CBS_MAX            (0xFFFFFFFFu)
#define QCR_CBS_MIN            (0x54u)     /* 84bytes = min packet 64B + media overhead 20B */
#define QCR_CIR_MAX            (10000000u) /* 10,000,000 Kbps = 10G*/
#define QCR_CIR_MIN            (500u)      /* TC11 500kpbs */
#define BIT2BYTE               (3u)
#define FRACTIONAL_BITS_29     (29u)
#define SYSTEM_CLOCK_250M      (250u * 1000000u) /* 250 * (10 ^ 6) */
#define SYSTEM_CLOCK           (SYSTEM_CLOCK_250M)
#define TICK_NS_CONVERT        (uint32)((1000000000u) / SYSTEM_CLOCK) /* (10 ^ 9) / SYSTEM_CLOCK */
#define SYSTEM_CLOCK_10MS_TICK (uint32)((10u * (1000000u)) / TICK_NS_CONVERT)
#define MAX_RESIDENCE_TIME     (0x989680u) /* 10,000,000ns = 10ms */
#define NS2MS                  (1000000u)
#define REG_OFFSET_4           (4u)
#define REG_OFFSET_8           (8u)
#define ATS_RELEASE            (0u)

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/

/**
 * @brief This is an ATS get information structure.
 *
 * This structure is used to obtain information about the number of ports on the chip package as well as time-related information.
 */
typedef struct
{
    uint32 rxPortNumber;               /**< [out] Number of reception port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32 txPortNumber;               /**< [out] Number of transmission port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32 clockOffsetVariationMax;    /**< [out] The maximum variation of clock offset, and the unit is ns. (0 to 0xFFFFFFFF) */
    uint32 clockRateDeviationMax;      /**< [out] The maximum deviation of clock rate, and the unit is ppm. (0 to 0xFFFFFFFF) */
    uint32 arrivalRecognitionDelayMax; /**< [out] The maximum delay between the time that the frame is arrived and recongniton by ATS scheduler, and the unit is ns. (0 to 0xFFFFFFFF) */
    uint32 processingDelayMax;         /**< [out] The maximum delay between the time that the frame is recongniton by ATS scheduler and selection algorithm, and the unit is ns. (0 to 0xFFFFFFFF) */
    uint32 processingDelayMin;         /**< [out] The minimum delay between the time that the frame is recongniton by ATS scheduler and selection algorithm, and the unit is ns. (0 to 0xFFFFFFFF) */
} AtsGetInfo_t;

/**
 * @brief This is an ATS set/get CBS and CIR information structure.
 *
 * This structure is used to set/get the ATS scheduler ID, group ID, CBS, and CIR kbps.
 */
typedef struct
{
    uint8  schedulerId; /**< [in] The scheduler identification of ATS. (0 to 95) */
    uint8  groupId;     /**< [in,out] The group identification of ATS. (0 to 95) */
    uint32 cbs;         /**< [in,out] The CommittedBurstSize parameter defines the maximum token capacity of the token bucket. (0x54 to 0xFFFFFFFF) */
    uint64 cirKbps;     /**< [in,out] The CommittedInformationRate parameter defines the rate at which the token bucket is refilled with tokens until the maximum token capacity of the token bucket is reached. (500 to 10000000) */
} AtsCbsCirInfo_t;

/**
 * @brief This is an ATS set/get group residence time duration information structure.
 *
 * This structure is used to set/get the ATS group residence time duration.
 */
typedef struct
{
    uint8  groupId;            /**< [in] The group identification of ATS. (0 to 95) */
    uint32 maxResidenceTimeNs; /**< [in,out] The maximum residence time limits the duration for which frames can reside in a Bridge. (0 to 10000000) */
} AtsGroupResidenceTimeInfo_t;

/**
 * @brief This is an ATS get drop counter information structure.
 *
 * This structure is used to get ATS drop counter for a specific port.
 */
typedef struct
{
    RtkPort port;    /**< [in] Obtain the drop counter for a specific port. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint64  counter; /**< [out] Discard frames count. (0 to 0xFFFFFFFFFFFFFFFF) */
} AtsDropCounterGet_t;

/**
 * @brief This is an ATS clear drop counter information structure.
 *
 * This structure is used to clear ATS drop counter for a specific port.
 */
typedef struct
{
    RtkPort port; /**< [in] The port for which the drop counter needs to be cleared. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
} AtsDropCounterClear_t;

/**
 * @brief This is an ATS scheduler set information structure.
 *
 * This structure is used to set the transmission selection of a specific queue on a specific port to the ATS scheduler.
 */
typedef struct
{
    RtkPort   port;  /**< [in] Egress port number. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    QosQueNum queue; /**< [in] Queue identification for a specific port. (0 to 7) */
    RtkEnable mode;  /**< [in] Used to set whether the ATS queue is valid or not. */
} AtsSetInfo_t;

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_ats_init(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_ats_get_info(UnitChip_t unitChip, AtsGetInfo_t *atsGetInfo);
EXTERN_API RtkApiRet rtk_ats_cbs_cir_set(UnitChip_t unitChip, AtsCbsCirInfo_t *atsCbsCirInfo);
EXTERN_API RtkApiRet rtk_ats_cbs_cir_get(UnitChip_t unitChip, AtsCbsCirInfo_t *atsCbsCirInfo);
EXTERN_API RtkApiRet rtk_ats_group_residence_time_set(UnitChip_t unitChip, AtsGroupResidenceTimeInfo_t *atsGroupResidenceTimeInfo);
EXTERN_API RtkApiRet rtk_ats_group_residence_time_get(UnitChip_t unitChip, AtsGroupResidenceTimeInfo_t *atsGroupResidenceTimeInfo);
EXTERN_API RtkApiRet rtk_ats_drop_counter_get(UnitChip_t unitChip, AtsDropCounterGet_t *atsDropCounterGet);
EXTERN_API RtkApiRet rtk_ats_drop_counter_clear(UnitChip_t unitChip, AtsDropCounterClear_t *atsDropCounterClear);
EXTERN_API RtkApiRet rtk_ats_scheduler_set(UnitChip_t unitChip, AtsSetInfo_t *atsSetInfo);

#endif /* RTKAS_API_ATS_H__ */
