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

#ifndef RTKAS_API_PHY_TEST_H__
#define RTKAS_API_PHY_TEST_H__

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"
#include "rtkas_common.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define PHY_TESTMODE_BIT_MASK      (0xE000u)
#define PHY_TESTMODE_LSB           (13u)
#define RTK_RTCT_ARRAY             (3u)
#define OCR_PORT_SHIFT_LSB         (16u)
#define PHY_TEST_USER_PM_ALL       (RTK_USER_PMAP_1ST | RTK_USER_PMAP_2ND | RTK_USER_PMAP_3RD | RTK_USER_PMAP_4TH | RTK_USER_PMAP_5TH | RTK_USER_PMAP_6TH)
#define PHY_TEST_USER_PORT_MIN     (RTK_USER_PORT_1ST)
#define PHY_TEST_USER_PORT_MAX     (RTK_USER_PORT_6TH)
#define PHY_TEST_USER_PORT_END     (RTK_USER_PORT_6TH + 1)
#define PHY_TESTMODE_SPEED_100M    (0u)
#define PHY_TESTMODE_SPEED_1000M   (1u)
#define PHY_TESTMODE_SPEED_NUM_END (2u)
#define PHY_TESTMODE_ROLE_NUM_END  (2u)
#define PHY_TESTMODE_NUM_END       (8u)

/******************************************************************************/
/* Typedef Data                                                              */
/******************************************************************************/
typedef uint32 RtkRtctCableDefect;

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
typedef enum
{
    EM_RTK_RTCT_PORT1 = 1, /**< Cable test on port 1 */
    EM_RTK_RTCT_PORT2,     /**< Cable test on port 2 */
    EM_RTK_RTCT_PORT3,     /**< Cable test on port 3 */
    EM_RTK_RTCT_PORT4,     /**< Cable test on port 4 */
    EM_RTK_RTCT_PORT5,     /**< Cable test on port 5 */
    EM_RTK_RTCT_PORT6,     /**< Cable test on port 6 */
    EM_RTK_RTCT_PORTMAX,
} RtkRtctPortNumber_e;

typedef enum
{
    EM_RTK_RTCT_NORMAL = 0, /**< The cable status is normal */
    EM_RTK_RTCT_OPEN,       /**< The cable status is open */
    EM_RTK_RTCT_SHORT,      /**< The cable status is short */
    EM_RTK_RTCT_ERROR,      /**< Cannot detect the cable status */
} RtkRtctCableStatus_e;

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/**
 * @brief RTCT translation structure
 *
 * This structure is used to categorize RTCT status.
 */
typedef struct
{
    uint16               level;  /**< The level value read from the RTCT detection register (0x4800u to 0x6000u) */
    RtkRtctCableStatus_e result; /**< The cable status (EM_RTK_RTCT_NORMAL to EM_RTK_RTCT_SHORT) */
} RtkRtctCompare_t;

/**
 * @brief RTCT information structure
 *
 * This structure is used to trigger RTCT and get results.
 */
typedef struct
{
    RtkRtctPortNumber_e port;    /**< [in] The port index to be obtained (EM_RTK_RTCT_PORT1 to EM_RTK_RTCT_PORT6) \n
                                  EM_RTK_RTCT_PORT1 corresponds to <CHIPID>_RTK_USER_PORT_1ST \n
                                  EM_RTK_RTCT_PORT2 corresponds to <CHIPID>_RTK_USER_PORT_2ND \n
                                  EM_RTK_RTCT_PORT3 corresponds to <CHIPID>_RTK_USER_PORT_3RD \n
                                  EM_RTK_RTCT_PORT4 corresponds to <CHIPID>_RTK_USER_PORT_4TH \n
                                  EM_RTK_RTCT_PORT5 corresponds to <CHIPID>_RTK_USER_PORT_5TH \n
                                  EM_RTK_RTCT_PORT6 corresponds to <CHIPID>_RTK_USER_PORT_6TH \n
                                  */
    RtkRtctCableStatus_e status; /**< [out] RTCT result (EM_RTK_RTCT_NORMAL to EM_RTK_RTCT_ERROR) */
    RtkRtctCableDefect   defeat; /**< [out] RTCT defect location in meters */
} RTCTInfo_t;

/**
 * @brief PHY test mode information structure
 *
 * This structure is used to access PHY test mode.
 */
typedef struct
{
    uint8 port;     /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint8 speed;    /**< [in] 10BASE-T1S/100BASE-T1/1000BASE-T1 (0 to 1) \n
                     - In RTL907XC_VA and RTL907XC_VB, \n
                     -- for <CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_4TH: \n
                     --- 0: 10BASE-T1S \n
                     --- 1: 100BASE-T1 \n
                     -- for <CHIPID>_RTK_USER_PORT_5TH to <CHIPID>_RTK_USER_PORT_6TH: \n
                     --- 0: 100BASE-T1 \n
                     --- 1: 1000BASE-T1 \n
                     - In RTL907XD_V1 and RTL907XD_VA, \n
                     -- for <CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH: \n
                     --- 0: 100BASE-T1 \n
                     --- 1: 1000BASE-T1
                     */
    uint8 testMode; /**< [in] PHY test mode (0 to 7) */
    uint8 isMaster; /**< [in] PHY master/slave (0 to 1) \n
                     0: slave \n
                     1: master
                     */
} PhyTestModeInfo_t;

/**
 * @brief MSE information structure
 *
 * This structure is used to get MSE measurement.
 */
typedef struct
{
    RtkPort port;  /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint16  value; /**< [out] MSE value */
} MseGetInfo_t;

/**
 * @brief The structure of the worst case of MSE information
 *
 * This structure is used to get the worst case of MSE measurement.
 */
typedef struct
{
    RtkPort port;  /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint16  value; /**< [out] The value of the worst case of MSE */
} WcMseGetInfo_t;

/**
 * @brief The structure of the peak MSE information
 *
 * This structure is used to get the peak MSE measurement.
 */
typedef struct
{
    RtkPort port;  /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint8   value; /**< [out] The value of the peak MSE */
} PeakMseGetInfo_t;

/**
 * @brief The structure of the worst case of the peak MSE information
 *
 * This structure is used to get the worst case of the peak MSE measurement.
 */
typedef struct
{
    RtkPort port;  /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint8   value; /**< [out] The value of the worst case of the peak MSE */
} WcPeakMseGetInfo_t;

/**
 * @brief The structure of LTT information
 *
 * This structure is used to get LTT measurement.
 */
typedef struct
{
    RtkPort port;    /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint8   lttTime; /**< [out] The value of LTT */
} LttGetInfo_t;

/**
 * @brief The structure of LRT information
 *
 * This structure is used to get LRT measurement.
 */
typedef struct
{
    RtkPort port;    /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint8   lrtTime; /**< [out] The value of LRT */
} LrtGetInfo_t;

/**
 * @brief The structure of RRT information
 *
 * This structure is used to get RRT measurement.
 */
typedef struct
{
    RtkPort port;    /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint8   rrtTime; /**< [out] The value of RRT */
} RrtGetInfo_t;

/**
 * @brief The structure of LFL information
 *
 * This structure is used to get LFL measurement.
 */
typedef struct
{
    RtkPort port;    /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint8   lossCnt; /**< [out] The number of link losses */
    uint16  failCnt; /**< [out] The number of link failures without causing a link loss */
} LflGetInfo_t;

/**
 * @brief PHY SQI information structure
 *
 * This structure is used to get PHY SQI.
 */
typedef struct
{
    RtkPort port;  /**< [in] The port index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_6TH) */
    uint8   speed; /**< [in] PHY speed (0 to 1) */
    uint8   sqi;   /**< [out] SQI */
} SqiGetInfo_t;

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_rtct_run_cable_diag(UnitChip_t unitChip, RTCTInfo_t *rtctInfo);
EXTERN_API RtkApiRet rtk_rtct_get_cable_status(UnitChip_t unitChip, RTCTInfo_t *rtctInfo);
EXTERN_API RtkApiRet rtk_phy_testmode_set(UnitChip_t unitChip, PhyTestModeInfo_t *phyTMInfo);
EXTERN_API RtkApiRet rtk_phy_testmode_get(UnitChip_t unitChip, PhyTestModeInfo_t *phyTMInfo);
EXTERN_API RtkApiRet rtk_port_mse_get(UnitChip_t unitChip, MseGetInfo_t *mseGetInfo);
EXTERN_API RtkApiRet rtk_port_wc_mse_get(UnitChip_t unitChip, WcMseGetInfo_t *wcMseGetInfo);
EXTERN_API RtkApiRet rtk_port_peak_mse_get(UnitChip_t unitChip, PeakMseGetInfo_t *pMseGetInfo);
EXTERN_API RtkApiRet rtk_port_wc_peak_mse_get(UnitChip_t unitChip, WcPeakMseGetInfo_t *wcPkMseGetInfo);
EXTERN_API RtkApiRet rtk_port_sqi_get(UnitChip_t unitChip, SqiGetInfo_t *sqiGetInfo);
EXTERN_API RtkApiRet rtk_port_ltt_get(UnitChip_t unitChip, LttGetInfo_t *lttGetInfo);
EXTERN_API RtkApiRet rtk_port_lrt_get(UnitChip_t unitChip, LrtGetInfo_t *lrtGetInfo);
EXTERN_API RtkApiRet rtk_port_rrt_get(UnitChip_t unitChip, RrtGetInfo_t *rrtGetInfo);
EXTERN_API RtkApiRet rtk_port_lfl_loss_get(UnitChip_t unitChip, LflGetInfo_t *lflGetInfo);
EXTERN_API RtkApiRet rtk_port_lfl_fail_get(UnitChip_t unitChip, LflGetInfo_t *lflGetInfo);

#endif /* RTKAS_API_PHY_TEST_H__ */