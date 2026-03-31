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

#ifndef DV1_RTK_API_PHY_H
#define DV1_RTK_API_PHY_H

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtl907xd_v1_api.h"

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DV1_PHY_T1_USER_PORT_MIN       (DV1_RTK_USER_PORT_1ST) /**< Min user port index with T1PHY */
#define DV1_PHY_T1_USER_PORT_MAX       (DV1_RTK_USER_PORT_6TH) /**< Max user port index with T1PHY */
#define DV1_PHY_T1_RTPGE_USER_PORT_MIN (DV1_RTK_USER_PORT_1ST) /**< Min user port index of T1PHY support 100/1000BASE-T1*/
#define DV1_PHY_T1_RTPGE_USER_PORT_MAX (DV1_RTK_USER_PORT_6TH) /**< Max user port index of T1PHY support 100/1000BASE-T1*/
#define DV1_PHY_ETH_USER_PORT_IDX      (DV1_RTK_USER_PORT_7TH) /**< The user port index of the Ethernet port */
#define DV1_PHY_T1_SYS_PORT_MIN        (DV1_RTK_SYS_PORT_1ST)  /**< Min sys port index with T1PHY */
#define DV1_PHY_T1_SYS_PORT_MAX        (DV1_RTK_SYS_PORT_6TH)  /**< Max sys port index with T1PHY */
#define DV1_PHY_ETH_SYS_PORT_IDX       (DV1_RTK_SYS_PORT_7TH)  /**< The sys port index of the Ethernet port */

#define DV1_PHY_T1_USER_PORTMAP_ALL (DV1_RTK_USER_PMAP_1ST | DV1_RTK_USER_PMAP_2ND | DV1_RTK_USER_PMAP_3RD | DV1_RTK_USER_PMAP_4TH | DV1_RTK_USER_PMAP_5TH | DV1_RTK_USER_PMAP_6TH) /**< Max user port index with T1PHY */
/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
extern const uint32 dv1_g_sysPortToOcpAddr[DV1_RTK_SYS_PORT_ID_MAX];
/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet dv1_phy_bmcr_reset(uint32 unit, RtkPmap portMap);
EXTERN_API RtkApiRet dv1_phy_bmcr_pcs_loopback_get(uint32 unit, RtkPmap *pPortMap);
EXTERN_API RtkApiRet dv1_phy_bmcr_pcs_loopback_set(uint32 unit, RtkPmap portMap, RtkEnable enableCfg);
EXTERN_API RtkApiRet dv1_phy_bmcr_speed_get(uint32 unit, RtkPort port, uint32 *pSpeed);
EXTERN_API RtkApiRet dv1_phy_bmcr_speed_set(uint32 unit, RtkPort port, uint32 speed);
EXTERN_API RtkApiRet dv1_phy_bmcr_an_mode_get(uint32 unit, RtkPmap *pPortMap);
EXTERN_API RtkApiRet dv1_phy_bmcr_an_mode_set(uint32 unit, RtkPmap portMap, RtkEnable anEnableCfg);
EXTERN_API RtkApiRet dv1_phy_bmcr_restart_an(uint32 unit, RtkPmap portMap);
EXTERN_API RtkApiRet dv1_phy_bmcr_power_down_get(uint32 unit, RtkPmap *pPortMap);
EXTERN_API RtkApiRet dv1_phy_bmcr_power_down_set(uint32 unit, RtkPmap portMap, RtkEnable pwdCfg);
EXTERN_API RtkApiRet dv1_phy_bmcr_isolate_get(uint32 unit, RtkPmap *pPortMap);
EXTERN_API RtkApiRet dv1_phy_bmcr_isolate_set(uint32 unit, RtkPmap portMap, RtkEnable isoCfg);
EXTERN_API RtkApiRet dv1_phy_phyid_get(uint32 unit, RtkPort portId, uint32 *pOui, uint8 *pModleNum, uint8 *pRevNum);
EXTERN_API RtkApiRet dv1_phy_phycr_role_set(uint32 unit, RtkPort portId, uint32 isMaster);
EXTERN_API RtkApiRet dv1_phy_phycr_role_get(uint32 unit, RtkPort portId, uint32 *pIsMaster);
EXTERN_API RtkApiRet dv1_phy_bmsr_link_status_get(uint32 unit, RtkPort portId, uint32 *pLinkSts);
EXTERN_API RtkApiRet dv1_phy_physr1_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint8 *pRoleCfg, uint8 *pLocRcvReady, uint8 *pRemRcvReady, uint8 *pIdleErrorCnt);
EXTERN_API RtkApiRet dv1_phy_physr2_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint8 *pMasterMode, uint8 *pSpeed, uint8 *pDuplex, uint8 *pRealTimelinkSts);
EXTERN_API RtkApiRet dv1_phy_physr4_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint8 *pRxPolarity, uint8 *pTxPolarity, uint8 *pRemoteReceiverSts, uint8 *plocalReceiverSts);
EXTERN_API RtkApiRet dv1_phy_physfr_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint32 *pPhySubFlag);
EXTERN_API RtkApiRet dv1_phy_pcs_info_cfg(uint32 unit, RtkPort portId, uint32 *pRegVal, uint32 *pPhyPCS);
EXTERN_API RtkApiRet dv1_phy_ipcr1_info_cfg(uint32 unit, RtkPort portId, RtkEnable enRemoteLbk, RtkEnable enPktCounter, uint8 pathSel);
EXTERN_API RtkApiRet dv1_phy_ipcr2_info_cfg(uint32 unit, RtkPort portId, uint8 intCntErrClr, uint8 intCntClr, uint8 rxPkCntRst);
EXTERN_API RtkApiRet dv1_phy_ipcr_cnt(uint32 unit, RtkPort portId, uint32 *intCntLSB, uint32 *intCntMSB, uint32 *intErrCnt);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_reset(uint32 unit);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_pcs_loopback_get(uint32 unit, RtkEnable *pEnableCfg);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_pcs_loopback_set(uint32 unit, RtkEnable enableCfg);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_speed_get(uint32 unit, uint32 *pSpeed);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_speed_set(uint32 unit, uint32 speed);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_an_mode_get(uint32 unit, RtkEnable *pAnEnableCfg);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_an_mode_set(uint32 unit, RtkEnable anEnableCfg);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_restart_an(uint32 unit);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_power_down_get(uint32 unit, RtkEnable *pPwdCfg);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_power_down_set(uint32 unit, RtkEnable pwdCfg);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_isolate_get(uint32 unit, RtkEnable *pIsoCfg);
EXTERN_API RtkApiRet dv1_ethphy_bmcr_isolate_set(uint32 unit, RtkEnable isoCfg);
EXTERN_API RtkApiRet dv1_ethphy_bmsr_link_status_get(uint32 unit, uint32 *pLinkSts);
EXTERN_API RtkApiRet dv1_ethphy_physfr_info_cfg(uint32 unit, uint32 *pRegVal, uint32 *pPhySubFlag);
EXTERN_API RtkApiRet dv1_ethphy_pcs_info_cfg(uint32 unit, uint32 *pRegVal, uint32 *pPhyPCS);
EXTERN_API RtkApiRet dv1_ethphy_mdix_mode_get(uint32 unit, uint32 *pMdixMode);
EXTERN_API RtkApiRet dv1_ethphy_mdix_mode_set(uint32 unit, uint32 mdixMode);
EXTERN_API RtkApiRet dv1_ethphy_power_save_en_get(uint32 unit, RtkEnable *pEnPwrSaving);
EXTERN_API RtkApiRet dv1_ethphy_power_save_en_set(uint32 unit, RtkEnable enPwrSaving);
#endif /* DV1_RTK_API_PHY_H */