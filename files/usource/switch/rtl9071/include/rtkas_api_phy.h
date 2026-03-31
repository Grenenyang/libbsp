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

#ifndef RTKAS_API_PHY_H
#define RTKAS_API_PHY_H

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common_types.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define PHY_BMCR_SPEED_10M     (0u) /**< PHY BMCR speed 10M */
#define PHY_BMCR_SPEED_100M    (1u) /**< PHY BMCR speed 100M */
#define PHY_BMCR_SPEED_1000M   (2u) /**< PHY BMCR speed 1000M */
#define PHY_BMCR_SPEED_500M    (3u) /**< PHY BMCR speed 500M */
#define PHY_LINKDOWN           (0u) /* PHY link down */
#define PHY_LINKUP             (1u) /* PHY link up */
#define PHY_ROLE_AUTO          (0u) /* Auto role mode */
#define PHY_ROLE_MANUAL_SLAVE  (2u) /* Manual slave mode */
#define PHY_ROLE_MANUAL_MASTER (3u) /* Manual master mode */
#define PHY_AUTO_MDIX_MODE     (0u) /* Auto MDI/MDIX mode */
#define PHY_FORCE_MDIX_MODE    (2u) /* Manual force MDIX mode */
#define PHY_FORCE_MDI_MODE     (3u) /* Manual force MDI mode */

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/**
 * @brief PHY role structure
 *
 * This structure is used to setting and getting the T1-PHY role.
 */
typedef struct
{
    RtkPort port;     /**< [in]     The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint32  isMaster; /**< [in,out] The PHY role (0 to 1) */
} PhyRoleCfgInfo_t;

/**
 * @brief PHY Identifier structure
 *
 * This structure is used to getting the T1-PHY identifier.
 */
typedef struct
{
    RtkPort port;     /**< [in]  The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint32  oui;      /**< [out] The PHY identifier (0 to 0xFFFFFFFF) */
    uint8   modleNum; /**< [out] The Manufacture’s Model Number (0 to 0x3F) */
    uint8   revNum;   /**< [out] The Revision number (0 to 0xF) */
} PhyIdInfo_t;

/**
 * @brief PHY reset structure
 *
 * This structure is used to reset the T1-PHY of specific ports.
 */
typedef struct
{
    RtkPmap portMap; /**< [in] The port map to trigger software reset (0 to <CHIPID>_PHY_T1_USER_PORTMAP_ALL) */
} PhyResetInfo_t;

/**
 * @brief PHY PCS loopback structure
 *
 * This structure is used to set or get the T1-PHY PCS loopback configuration of specific ports.
 */
typedef struct
{
    RtkPmap   portMap;   /**< [in,out] The port map (0 to <CHIPID>_PHY_T1_USER_PORTMAP_ALL) */
    RtkEnable enableCfg; /**< [in,out] The PCS loopback set and get configuration (DISABLE/ENABLE) */
} PhyPcsLoopback_t;

/**
 * @brief PHY speed configuration structure
 *
 * This structure is used to force the speed of T1-PHY of the specific port.
 */
typedef struct
{
    RtkPort port;  /**< [in]     The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint32  speed; /**< [in,out] The speed (PHY_BMCR_SPEED_10M/PHY_BMCR_SPEED_100M/PHY_BMCR_SPEED_1000M) */
} PhySpd_t;

/**
 * @brief PHY AN mode configuration structure
 *
 * This structure is for enable auto-negotiation of specific ports.
 */
typedef struct
{
    RtkPmap   portMap;     /**< [in,out] The port map (0 to <CHIPID>_PHY_T1_USER_PORTMAP_ALL) */
    RtkEnable anEnableCfg; /**< [in,out] The auto-negotiation enable setting(DISABLE or ENABLE) */
} PhyAnMode_t;

/**
 * @brief Structure for restart PHY auto-negotiation
 *
 * This structure is for restart PHY auto-negotiation of specific ports.
 */
typedef struct
{
    RtkPmap portMap; /**< [in] The port map (0 to <CHIPID>_PHY_T1_USER_PORTMAP_ALL) */
} PhyRestartAn_t;

/**
 * @brief Structure for power down T1-PHY
 *
 * This structure is for power down T1-PHY of specific ports.
 */
typedef struct
{
    RtkPmap   portMap; /**< [in] The port map (0 to <CHIPID>_PHY_T1_USER_PORTMAP_ALL) */
    RtkEnable pwdCfg;  /**< [in,out] The PHY power down config (DISABLE or ENABLE) */
} PhyPwd_t;

/**
 * @brief Structure for T1-PHY isolate configuration
 *
 * This structure is for T1-PHY isolate configuration of specific ports.
 */
typedef struct
{
    RtkPmap   portMap; /**< [in] The port map (0 to <CHIPID>_PHY_T1_USER_PORTMAP_ALL) */
    RtkEnable isoCfg;  /**< [in,out] The PHY isolate configuration (DISABLE or ENABLE) */
} PhyIsolate_t;

/**
 * @brief Structure for T1-PHY Link status configuration
 *
 * This structure is for T1-PHY Link status of specific ports.
 */
typedef struct
{
    RtkPort port;    /**< [in]  The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint32  linkSts; /**< [out] The PHY link status (PHY_LINKDOWN or PHY_LINKUP) */
} PhyLinkStatus_t;

/**
 * @brief Structure for T1-PHY PHYSR1 register value
 *
 * This structure is for T1-PHY PHYSR1 register value
 */
typedef struct
{
    RtkPort port;         /**< [in]  The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint32  regValue;     /**< [out] The register value */
    uint8   roleCfg;      /**< [out] The PHY role config (0 or 1) */
    uint8   locRcvReady;  /**< [out] The PHY local receive status is OK or not (0 or 1) */
    uint8   remRcvReady;  /**< [out] The PHY remote receive status is OK or not (0 or 1) */
    uint8   idleErrorCnt; /**< [out] The Idle Error Counter. 100BASE-T1 only (0 to 255) */
} PhyPHYSR1_t;

/**
 * @brief Structure for T1-PHY PHYSR2 register value
 *
 * This structure is for T1-PHY PHYSR2 register value
 */
typedef struct
{
    RtkPort port;            /**< [in]  The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint32  regValue;        /**< [out] The register value */
    uint8   masterMode;      /**< [out] The Role setting after Auto-Negotiation (0 or 1) */
    uint8   speed;           /**< [out] The PHY local receive status is OK or not (0 or 1) */
    uint8   duplex;          /**< [out] The PHY remote receive status is OK or not (0 or 1) */
    uint8   realTimelinkSts; /**< [out] The Real-time Link Status (0 or 1) */
} PhyPHYSR2_t;

/**
 * @brief Structure for T1-PHY PHYSR4 register value
 *
 * This structure is for T1-PHY PHYSR4 register value
 */
typedef struct
{
    RtkPort port;              /**< [in]  The designated port index (<CHIPID>_PHY_100T1_USER_PORT_MIN to <CHIPID>_PHY_100T1_USER_PORT_MAX) */
    uint32  regValue;          /**< [out] The register value */
    uint8   rxPolarity;        /**< [out] Receive polarity for 100BASE-T1. (0 or 1) */
    uint8   txPolarity;        /**< [out] Transmit polarity for 100BASE-T1. (0 or 1) */
    uint8   remoteReceiverSts; /**< [out] Remote Receiver Status for 100BASE-T1 (0 or 1) */
    uint8   localReceiverSts;  /**< [out] Local Receiver Status for 100BASE-T1 (0 or 1) */
} PhyPHYSR4_t;

/**
 * @brief Structure for T1-PHY PHYSFR register value
 *
 * This structure is for T1-PHY PHYSFR register value
 */
typedef struct
{
    RtkPort port;       /**< [in]  The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint32  regValue;   /**< [out] The register value */
    uint32  phySubFlag; /**< [out] PHY Status indicator (N/A) */
} PhyPHYSFR_t;

/**
 * @brief Structure for T1-PHY PCS status
 *
 * This structure is for T1-PHY PCS status
 */
typedef struct
{
    RtkPort port;     /**< [in]  The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint32  regValue; /**< [out] The register value */
    uint32  phyPCS;   /**< [out] PHY PCS Status (N/A) */
} PhyPCS_t;

/**
 * @brief Structure for T1-PHY IPCR1 register value
 *
 * This structure is for T1-PHY IPCR1 register value
 */
typedef struct
{
    RtkPort   port;         /**< [in] The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    RtkEnable enRemoteLbk;  /**< [in] Enable or disable remote loopback (DISABLE or ENABLE) */
    RtkEnable enPktCounter; /**< [in] Enable or disable PHY internal packet counter (DISABLE or ENABLE) */
    uint8     pathSel;      /**< [in] The selection of monitor path for the internal PHY counter and CRC counter (0 or 1) */
} PhyIPCR1_t;

/**
 * @brief Structure for T1-PHY IPCR2 register value
 *
 * This structure is for T1-PHY IPCR2 register value
 */
typedef struct
{
    RtkPort port;         /**< [in] The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint8   intCntErrClr; /**< [in] Clear the error packet counter. (0 or 1) */
    uint8   intCntClr;    /**< [in] Clear the packet counter (0 or 1) */
    uint8   rxPkCntRst;   /**< [in] Clear the received packet counter (0 or 1) */
} PhyIPCR2_t;

/**
 * @brief Structure for T1-PHY IPCR3 register value
 *
 * This structure is for T1-PHY IPCR3 register value
 */
typedef struct
{
    RtkPort port;      /**< [in] The designated port index (<CHIPID>_PHY_T1_USER_PORT_MIN to <CHIPID>_PHY_T1_USER_PORT_MAX) */
    uint32  intCntLSB; /**< [in] From IPCR3. Received packet counter LSB. (0 to 0xFFFF) */
    uint32  intCntMSB; /**< [in] From IPCR4. Received packet counter MSB. (0 to 0xFFFF) */
    uint32  intErrCnt; /**< [in] From IEPCR. Received error packet counter. (0 to 0xFFFF) */
} PhyIPCRCntVal_t;

/**
 * @brief Structure for Ethernet Power Saving Mode
 *
 * This structure is for Ethernet power saving register value
 */
typedef struct
{
    uint32 mscrRoleCfg; /**< [in,out] Enable Manual master/slave role setting or disable manual role setting. (PHY_ROLE_AUTO/PHY_ROLE_MANUAL_SLAVE/PHY_ROLE_MANUAL_MASTER) */
} PhyMSCRCfg_t;

/**
 * @brief Structure for Ethernet MDI/MDIX register value
 *
 * This structure is for Ethernet MDI/MDIX register value
 */
typedef struct
{
    uint32 mdixMode; /**< [in,out] MDI/MDIX mode. (PHY_AUTO_MDIX_MODE/PHY_FORCE_MDIX_MODE/PHY_FORCE_MDI_MODE) */
} PhyMdixCfg_t;

/**
 * @brief Structure for Ethernet Power Saving Mode
 *
 * This structure is for Ethernet power saving register value
 */
typedef struct
{
    RtkEnable enPwrSaving; /**< [in] Enable or disable power saving mode. (DISABLE to ENABLE) */
} PhyPwrSave_t;

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_phy_bmcr_reset(UnitChip_t unitChip, PhyResetInfo_t *phyResetInfo);
EXTERN_API RtkApiRet rtk_phy_bmcr_pcs_loopback_get(UnitChip_t unitChip, PhyPcsLoopback_t *pPhyPcsLoopback);
EXTERN_API RtkApiRet rtk_phy_bmcr_pcs_loopback_set(UnitChip_t unitChip, PhyPcsLoopback_t *pPhyPcsLoopback);
EXTERN_API RtkApiRet rtk_phy_bmcr_speed_get(UnitChip_t unitChip, PhySpd_t *pPhySpd);
EXTERN_API RtkApiRet rtk_phy_bmcr_speed_set(UnitChip_t unitChip, PhySpd_t *pPhySpd);
EXTERN_API RtkApiRet rtk_phy_bmcr_an_mode_get(UnitChip_t unitChip, PhyAnMode_t *pPhyAnMode);
EXTERN_API RtkApiRet rtk_phy_bmcr_an_mode_set(UnitChip_t unitChip, PhyAnMode_t *pPhyAnMode);
EXTERN_API RtkApiRet rtk_phy_bmcr_restart_an(UnitChip_t unitChip, PhyRestartAn_t *pPhyRestartAn);
EXTERN_API RtkApiRet rtk_phy_bmcr_power_down_set(UnitChip_t unitChip, PhyPwd_t *pPhyPwd);
EXTERN_API RtkApiRet rtk_phy_bmcr_power_down_get(UnitChip_t unitChip, PhyPwd_t *pPhyPwd);
EXTERN_API RtkApiRet rtk_phy_bmcr_isolate_set(UnitChip_t unitChip, PhyIsolate_t *pPhyIsolate);
EXTERN_API RtkApiRet rtk_phy_bmcr_isolate_get(UnitChip_t unitChip, PhyIsolate_t *pPhyIsolate);
EXTERN_API RtkApiRet rtk_phy_phyid_get(UnitChip_t unitChip, PhyIdInfo_t *pPhyIdInfo);
EXTERN_API RtkApiRet rtk_phy_phycr_role_set(UnitChip_t unitChip, PhyRoleCfgInfo_t *pPhyRoleCfgInfo);
EXTERN_API RtkApiRet rtk_phy_phycr_role_get(UnitChip_t unitChip, PhyRoleCfgInfo_t *pPhyRoleCfgInfo);
EXTERN_API RtkApiRet rtk_phy_bmsr_link_status_get(UnitChip_t unitChip, PhyLinkStatus_t *pPhyLinkStatus);
EXTERN_API RtkApiRet rtk_phy_physr1_info_cfg(UnitChip_t unitChip, PhyPHYSR1_t *pPhyPHYSR1);
EXTERN_API RtkApiRet rtk_phy_physr2_info_cfg(UnitChip_t unitChip, PhyPHYSR2_t *pPhyPHYSR2);
EXTERN_API RtkApiRet rtk_phy_physr4_info_cfg(UnitChip_t unitChip, PhyPHYSR4_t *pPhyPHYSR4);
EXTERN_API RtkApiRet rtk_phy_physfr_info_cfg(UnitChip_t unitChip, PhyPHYSFR_t *pPhyPHYSFR);
EXTERN_API RtkApiRet rtk_phy_pcs_info_cfg(UnitChip_t unitChip, PhyPCS_t *pPhyPCS);
EXTERN_API RtkApiRet rtk_phy_ipcr1_info_cfg(UnitChip_t unitChip, PhyIPCR1_t *pPhyIPCR1);
EXTERN_API RtkApiRet rtk_phy_ipcr2_info_cfg(UnitChip_t unitChip, PhyIPCR2_t *pPhyIPCR2);
EXTERN_API RtkApiRet rtk_phy_ipcr_cnt(UnitChip_t unitChip, PhyIPCRCntVal_t *pPhyIPCRCntVal);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_reset(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_pcs_loopback_get(UnitChip_t unitChip, PhyPcsLoopback_t *pPhyPcsLoopback);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_pcs_loopback_set(UnitChip_t unitChip, PhyPcsLoopback_t *pPhyPcsLoopback);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_speed_get(UnitChip_t unitChip, PhySpd_t *pPhySpd);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_speed_set(UnitChip_t unitChip, PhySpd_t *pPhySpd);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_an_mode_get(UnitChip_t unitChip, PhyAnMode_t *pPhyAnMode);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_an_mode_set(UnitChip_t unitChip, PhyAnMode_t *pPhyAnMode);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_restart_an(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_power_down_set(UnitChip_t unitChip, PhyPwd_t *pPhyPwd);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_power_down_get(UnitChip_t unitChip, PhyPwd_t *pPhyPwd);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_isolate_set(UnitChip_t unitChip, PhyIsolate_t *pPhyIsolate);
EXTERN_API RtkApiRet rtk_ethphy_bmcr_isolate_get(UnitChip_t unitChip, PhyIsolate_t *pPhyIsolate);
EXTERN_API RtkApiRet rtk_ethphy_bmsr_link_status_get(UnitChip_t unitChip, PhyLinkStatus_t *pPhyLinkStatus);
EXTERN_API RtkApiRet rtk_ethphy_physfr_info_cfg(UnitChip_t unitChip, PhyPHYSFR_t *pPhyPHYSFR);
EXTERN_API RtkApiRet rtk_ethphy_pcs_info_cfg(UnitChip_t unitChip, PhyPCS_t *pPhyPCS);
EXTERN_API RtkApiRet rtk_ethphy_mscr_role_cfg_get(UnitChip_t unitChip, PhyMSCRCfg_t *pPhyMSCRCfg);
EXTERN_API RtkApiRet rtk_ethphy_mscr_role_cfg_set(UnitChip_t unitChip, PhyMSCRCfg_t *pPhyMSCRCfg);
EXTERN_API RtkApiRet rtk_ethphy_mdix_mode_get(UnitChip_t unitChip, PhyMdixCfg_t *pPhyMdixCfg);
EXTERN_API RtkApiRet rtk_ethphy_mdix_mode_set(UnitChip_t unitChip, PhyMdixCfg_t *pPhyMdixCfg);
EXTERN_API RtkApiRet rtk_ethphy_power_save_en_get(UnitChip_t unitChip, PhyPwrSave_t *pPhyPwrSave);
EXTERN_API RtkApiRet rtk_ethphy_power_save_en_set(UnitChip_t unitChip, PhyPwrSave_t *pPhyPwrSave);
#endif /* RTKAS_API_PHY_H */