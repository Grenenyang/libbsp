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

#ifndef RTKAS_API_INTERFACE_H
#define RTKAS_API_INTERFACE_H

/* Sys port */
#define INTF_HW_PORT0    (0u)
#define INTF_HW_PORT1    (1u)
#define INTF_HW_PORT2    (2u)
#define INTF_HW_PORT3    (3u)
#define INTF_HW_PORT4    (4u)
#define INTF_HW_PORT5    (5u)
#define INTF_HW_PORT6    (6u)
#define INTF_HW_PORT7    (7u)
#define INTF_HW_PORT8    (8u)
#define INTF_HW_PORT9    (9u)
#define INTF_HW_PORT10   (10u)
#define INTF_HW_PORT11   (11u)
#define INTF_HW_PORT12   (12u)
#define INTF_HW_PORT13   (13u)
#define INTF_HW_PORT14   (14u)
#define INTF_HW_PORT15   (15u)
#define INTF_HW_PORT_END (16u)

/* ETH interface ability */
#define AN_10M_SPEED_HALF_DUPLEX   (0x1u)
#define AN_10M_SPEED_FULL_DUPLEX   (0x2u)
#define AN_100M_SPEED_HALF_DUPLEX  (0x4u)
#define AN_100M_SPEED_FULL_DUPLEX  (0x8u)
#define AN_1000M_SPEED_FULL_DUPLEX (0x10u)
#define PORT_HALF_DUPLEX           (0x1u)
#define PORT_FULL_DUPLEX           (0x2u)
#define PORT_DUPLEX_END            (0x3u)

/* define combo port configuration*/
#define COMBO_INTF_DISB          (0u)
#define COMBO_INTF_XMII_RGMII    (1u)
#define COMBO_INTF_XMII_RMII     (2u)
#define COMBO_INTF_XMII_MII      (3u)
#define COMBO_INTF_T1PHY_MII     (4u)
#define COMBO_INTF_T1PHY_GMII    (5u)
#define COMBO_INTF_SGMII_HISGMII (6u)
#define COMBO_INTF_XGMII_USXG    (7u)
#define COMBO_INTF_PCIE_GMII     (8u)
#define COMBO_INTF_PCIE_XGMII    (9u)
#define COMBO_INTF_SPI_MACPHY    (10u)

/* Link status */
#define PORT_LINKDOWN       (0x0u)
#define PORT_LINKUP         (0x1u)
#define PORT_LINKSTATUS_END (0x2u)

#define RTK_PORT_SPEED_10M    (0u)
#define RTK_PORT_SPEED_100M   (1u)
#define RTK_PORT_SPEED_1000M  (2u)
#define RTK_PORT_SPEED_2500M  (3u)
#define RTK_PORT_SPEED_5000M  (4u)
#define RTK_PORT_SPEED_10000M (5u)
#define RTK_PORT_SPEED_END    (RTK_PORT_SPEED_10000M)

/**
 * @brief Enum of port speed
 */
typedef enum
{
    EM_PORT_SPEED_10M    = 0x1,  /**< Port speed 10M */
    EM_PORT_SPEED_100M   = 0x2,  /**< Port speed 100M */
    EM_PORT_SPEED_1000M  = 0x4,  /**< Port speed 1000M */
    EM_PORT_SPEED_2500M  = 0x8,  /**< Port speed 2500M */
    EM_PORT_SPEED_5000M  = 0x10, /**< Port speed 5000M */
    EM_PORT_SPEED_10000M = 0x20, /**< Port speed 10000M */
    EM_PORT_SPEED_END    = 0xFF
} RtkPortSpeed_e;

/**
 * @brief Enum of port modes
 */
typedef enum
{
    EM_MODE_EXT_PULLDOWN = 0,       /**< [00] Pulldown */
    EM_MODE_EXT_MII_PHY,            /**< [01] MII PHY  */
    EM_MODE_EXT_FAST_ETHERNET,      /**< [02] Fast Ethernet */
    EM_MODE_EXT_100BASE_T1_PHY,     /**< [03] 100BASE-T1 PHY */
    EM_MODE_EXT_SGMII,              /**< [04] SGMII */
    EM_MODE_EXT_GMII,               /**< [05] GMII */
    EM_MODE_EXT_RGMII,              /**< [06] RGMII */
    EM_MODE_EXT_MII,                /**< [07] MII */
    EM_MODE_EXT_RMII,               /**< [08] RMII */
    EM_MODE_EXT_RMII_PHY,           /**< [09] RMII PHY */
    EM_MODE_EXT_RMII_MAC,           /**< [10] RMII MAC */
    EM_MODE_EXT_MII_MAC,            /**< [11] MII MAC */
    EM_MODE_EXT_RTPG_T1_PHY,        /**< [12] 1000BASE-T1 PHY */
    EM_MODE_EXT_RESERVED,           /**< [13] RESERVED Mode */
    EM_MODE_EXT_HISGMII,            /**< [14] HISGMII */
    EM_MODE_EXT_USXGMII,            /**< [15] USXGMII */
    EM_MODE_EXT_5G_KR,              /**< [16] 5GKR */
    EM_MODE_EXT_10G_KR,             /**< [17] 10GKR */
    EM_MODE_EXT_5G_XFI,             /**< [18] 5GXFI */
    EM_MODE_EXT_10G_XFI,            /**< [19] 10GXFI */
    EM_MODE_EXT_PCIE_1G_MAC_GMII,   /**< [20] PCIE 1G MAC GMII */
    EM_MODE_EXT_PCIE_5G_MAC_GMII,   /**< [21] PCIE 5G MAC GMII */
    EM_MODE_EXT_PCIE_5G_MAC_XGMII,  /**< [22] PCIE 5G MAC XGMII */
    EM_MODE_EXT_PCIE_10G_MAC_XGMII, /**< [23] PCIE 10G MAC XGMII */
    EM_MODE_EXT_XMII_DUMMY,         /**< [24] RESERVED Mode */
    EM_MODE_EXT_MACPHY,             /**< [25] MACPHY */
    EM_MODE_EXT_10BASE_T1_PHY,      /**< [26] 10BASE-T1 PHY */
    EM_MODE_EXT_10BASE_T1S_PHY,     /**< [27] 10BASE-T1S PHY */
    EM_MODE_EXT_GIGA_ETHERNET,      /**< [28] Giga Ethernet */
    EM_MODE_EXT_END
} RtkModeExt_e;

/**
 * @brief Structure for port mode set/get.
 *
 * This structure holds the configurations for the port mode of designated ports,
 * including userPort and mode.
 *
 * For set function, this structure holds a userPort indicating the port to configure the mode type according to mode value. \n
 * For get function, this structure holds a mode value indicating the mode type of the designated port.
 */
typedef struct
{
    RtkPort      userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkModeExt_e mode;     /**< [in,out] Set/Get port mode (N/A) */
} PortMode_t;

/**
 * @brief Structure for port link status set/get.
 *
 * This structure holds the configurations for the port link status of the designated ports,
 * including speed, duplex, link, txpause and rxpause.
 */
typedef struct
{
    uint8 speed;   /**< Link speed (EM_PORT_SPEED_10M/EM_PORT_SPEED_100M/EM_PORT_SPEED_1000M/EM_PORT_SPEED_2500M/EM_PORT_SPEED_5000M/EM_PORT_SPEED_10000M) */
    uint8 duplex;  /**< Duplex mode (PORT_HALF_DUPLEX/PORT_FULL_DUPLEX) */
    uint8 link;    /**< Link status (PORT_LINKDOWN/PORT_LINKUP) */
    uint8 txpause; /**< TX pause (0/1) */
    uint8 rxpause; /**< RX pause (0/1) */
} PortLinkStatus_t;

/**
 * @brief Structure of port media capability.
 *
 * This structure holds the port media capability configuration of designated ports,
 * including speed, duplex, link, txpause and rxpause.
 */
typedef struct
{
    uint32 forcemode; /**< Force mode (0/1) */
    uint32 speed;     /**< Link speed (EM_PORT_SPEED_10M/EM_PORT_SPEED_100M/EM_PORT_SPEED_1000M) */
    uint32 duplex;    /**< Duplex mode (PORT_HALF_DUPLEX/PORT_FULL_DUPLEX)*/
    uint32 link;      /**< Set Link status (PORT_LINKDOWN/PORT_LINKUP) */
    uint32 nway;      /**< Set AN mode (0/1) */
    uint32 txpause;   /**< Set TX pause (0/1) */
    uint32 rxpause;   /**< Set RX pause (0/1) */
} PortMacAbility_t;

/**
 * @brief Structure of fast ethernet link status.
 *
 * This structure holds the link status of the designated fast ethernet port,
 * including speed, duplex, autoNegoCapability and autoNego.
 */
typedef struct
{
    uint8  speed;              /**< ETH Link speed (EM_PORT_SPEED_10M/EM_PORT_SPEED_100M/EM_PORT_SPEED_1000M) */
    uint8  duplex;             /**< ETH port link duplex status (PORT_HALF_DUPLEX/PORT_FULL_DUPLEX) */
    uint16 autoNegoCapability; /**< AN mode capability (AN_10M_SPEED_HALF_DUPLEX/AN_10M_SPEED_FULL_DUPLEX/AN_100M_SPEED_HALF_DUPLEX/AN_100M_SPEED_FULL_DUPLEX/AN_1000M_SPEED_FULL_DUPLEX)) */
    uint8  autoNego;           /**< Set AN mode (0/1) */
} PortRtkEthLinkStatus_t;

/**
 * @brief Structure of fast ethernet media capability.
 *
 * This structure holds the media capability configuration of the designated fast ethernet port,
 * including speed, duplex, autoNegoCapability and autoNego.
 */
typedef struct
{
    uint32 speed;              /**< ETH Link speed (EM_PORT_SPEED_10M/EM_PORT_SPEED_100M/EM_PORT_SPEED_1000M) */
    uint32 duplex;             /**< ETH Duplex ability (PORT_HALF_DUPLEX/PORT_FULL_DUPLEX)*/
    uint16 autoNegoCapability; /**< AN mode capability (AN_10M_SPEED_HALF_DUPLEX/AN_10M_SPEED_FULL_DUPLEX/AN_100M_SPEED_HALF_DUPLEX/AN_100M_SPEED_FULL_DUPLEX/AN_1000M_SPEED_FULL_DUPLEX)) */
    uint8  autoNego;           /**< AN mode (0/1) */
} PortEthLinkAbility_t;

/**
 * @brief Structure for setting the link configuration of the designated port.
 *
 * This structure holds the designated port and port media capability configuration,
 * including userPort and pointer of PortMacAbility_t.
 */
typedef struct
{
    RtkPort          userPort;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    PortMacAbility_t macAbility; /**< [in] Port MAC capability (N/A) */
} PortLinkConfigurationSet_t;

/**
 * @brief Structure for getting the link status of the designated port.
 *
 * This structure holds the designated port and link status,
 * including userPort and pointer of PortLinkStatus_t.
 */
typedef struct
{
    RtkPort          userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    PortLinkStatus_t status;   /**< [out] Port link status (N/A) */
} PortLinkStatusGet_t;

/**
 * @brief Structure for setting the fast ethernet media capability.
 *
 * This structure holds the designated port and ethernet media capability configuration,
 * including userPort and pointer of PortEthLinkAbility_t.
 */
typedef struct
{
    RtkPort               userPort;    /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    PortEthLinkAbility_t *pEthAbility; /**< [out] Ethernet Port Link ability (N/A) */
} PortEthLinkAbilitySet_t;

/**
 * @brief Structure for getting the fast ethernet media capability.
 *
 * This structure holds the designated port and ethernet media capability,
 * including userPort and pointer of PortRtkEthLinkStatus_t.
 */
typedef struct
{
    RtkPort                 userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    PortRtkEthLinkStatus_t *pStatus;  /**< [out] Ethernet Port Link status (N/A) */
} PortEthLinkAbilityGet_t;

/**
 * @brief Structure for the max packet length set/get functions.
 *
 * this structure holds a userPort value indicating the port to enable or disable the max packet length configuration,
 * including userPort and enable.
 */
typedef struct
{
    RtkPort   userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable enable;   /**< [in] Enable config (0/1) */
} PortPktMaxlenSet_t;

/**
 * @brief Structure for the max packet length set/get functions.
 *
 * This structure holds the designated port and the max packet length,
 * including userPort and maxLen.
 */
typedef struct
{
    RtkPort userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  maxLen;   /**< [out] Max packet length (0x05F2 to 0x2800) */
} PortPktMaxlenGet_t;

/**
 * @brief Structure for the RGMII TX delay set/get functions.
 *
 * This structure holds the designated port and delay configuration,
 * including userPort and delay.
 *
 * For set function, this structure holds a userPort value indicating the port to configure the RGMII delay. \n
 * For get function, this structure holds a userPort value indicating the port to return the RGMII delay.
 */
typedef struct
{
    RtkPort userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   delay;    /**< [in,out] TX delay (0 to 1) */
} PortRgmiiTxdly_t;

/**
 * @brief Structure for the RGMII RX delay set functions.
 *
 * This structure holds the designated port, speed and delay configuration,
 * including userPort, speed and delay.
 */
typedef struct
{
    RtkPort        userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkPortSpeed_e speed;    /**< [in] Link speed (EM_PORT_SPEED_10M, EM_PORT_SPEED_100M and EM_PORT_SPEED_1000M)*/
    uint8          delay;    /**< [in] RX delay (0~3 nanoseconds) */
} PortRgmiiRxdlySet_t;

/**
 * @brief Structure for the RGMII RX delay get functions.
 *
 * This structure holds the designated port and delay configuration,
 * including userPort and delay.
 */
typedef struct
{
    RtkPort userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   delay;    /**< [out] RX delay (0~3 nanoseconds) */
} PortRgmiiRxdlyGet_t;

/**
 * @brief Structure for the XMII driving set/get functions.
 *
 * This structure holds the designated port and driving configuration,
 * including userPort, base, rise and fall.
 */
typedef struct
{
    RtkPort userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   base;     /**< [in] Driving base (0 ~ 3) */
    uint8   rise;     /**< [in] Driving rise (0 ~ 7) */
    uint8   fall;     /**< [in] Driving fall (0 ~ 7) */
} PortXmiiDriving_t;

/**
 * @brief Structure for the XMII RX/TX sampling edge set/get functions.
 *
 * This structure holds the designated port and RX/TX sampling edge configuration,
 * including userPort, isRxNegSample and isTxNegSample.
 */
typedef struct
{
    RtkPort userPort;      /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    bool    isRxNegSample; /**< [in] RX sampling edge (false = Negative sampling, true = Positive sampling) */
    bool    isTxNegSample; /**< [in] TX sampling edge (false = Negative sampling, true = Positive sampling) */
} PortXmiiLatchEdge_t;

/**
 * @brief Structure for the RMII reference clock set functions.
 *
 * This structure holds the designated port and reference clock selection,
 * including userPort, phyModePhase and internalPhase.
 */
typedef struct
{
    RtkPort userPort;      /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  phyModePhase;  /**< [in,out] PHY mode reference clock selection (0 = original phase , 1 = Invert clock phase of output reference clock) */
    uint32  internalPhase; /**< [in,out] Internal reference clock selection (0 = Negative sampling, 1 = Invert clock phase for internal MAC) */
} PortRmiiRefclkSel_t;

/**
 * @brief Structure for the XMII spread signal controller get function.
 *
 * This structure holds the designated port.
 */
typedef struct
{
    RtkPort   userPort;           /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable isEnable;           /**< [in,out] SSC controller state (0 = disable , 1 = enable SSC controller) */
    uint32    sequenceModeOffset; /**< [in,out] Phase offset in sequence mode (3 ~ 7 for offset phase) */
    uint32    phaseOffset;        /**< [in,out] SSC controller to output the phase offset (3 ~ 7 for offset phase) */
} PortXmiiSsc_t;

/**
 * @brief Structure for the XMII config set function.
 *
 * This structure holds the designated port.
 */
typedef struct
{
    RtkPort      userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkModeExt_e portMode; /**< [in] Port mode (EM_MODE_EXT_MII_PHY, EM_MODE_EXT_MII_MAC, EM_MODE_EXT_RGMII, EM_MODE_EXT_RMII_PHY, EM_MODE_EXT_RMII_MAC) */
} PortXmiiConfigSet_t;

/**
 * @brief Structure for the XMII config set function.
 *
 * This structure holds the designated port.
 */
typedef struct
{
    RtkPort      userPort;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkModeExt_e portMode;   /**< [in] Port mode (EM_MODE_EXT_MII_PHY, EM_MODE_EXT_MII_MAC, EM_MODE_EXT_RGMII, EM_MODE_EXT_RMII_PHY, EM_MODE_EXT_RMII_MAC) */
    uint32       speed;      /**< [in] Link speed (EM_PORT_SPEED_10M, EM_PORT_SPEED_100M and EM_PORT_SPEED_1000M)*/
    uint32       linkStatus; /**< [in] Port Link status (0 = link down, 1 = link up) */
} PortXmiiForceLinkSet_t;

/**
 * @brief Structure for the Fast ethernet config set function.
 *
 * This structure holds the designated port.
 */
typedef struct
{
    RtkPort      userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkModeExt_e portMode; /**< [in] Port mode (EM_MODE_EXT_FAST_ETHERNET,  EM_MODE_EXT_GIGA_ETHERNET) */
    RtkEnable    anEnable; /**< [in] Auto negotiation enable (0 = disable , 1 = enable) */
    uint32       speed;    /**< [in] Link speed (PHY_BMCR_SPEED_10M, PHY_BMCR_SPEED_100M and PHY_BMCR_SPEED_1000M)*/
} PortFeConfigSet_t;

/**
 * @brief Structure for the T1 ethernet config set function.
 *
 * This structure holds the designated port.
 */
typedef struct
{
    RtkPort      userPort; /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkModeExt_e portMode; /**< [in] Port mode (EM_MODE_EXT_10BASE_T1_PHY, EM_MODE_EXT_100BASE_T1_PHY, EM_MODE_EXT_RTPG_T1_PHY) */
    uint32       isMaster; /**< [in] The role setting (1 = Master, 0 = Slave)*/
    uint32       speed;    /**< [in] Link speed (PHY_BMCR_SPEED_10M, PHY_BMCR_SPEED_100M and PHY_BMCR_SPEED_1000M)*/
} PortT1ConfigSet_t;

EXTERN_API RtkApiRet rtk_port_t1_mode_set(UnitChip_t unitChip, PortMode_t *pPortModeSet);
EXTERN_API RtkApiRet rtk_port_mode_get(UnitChip_t unitChip, PortMode_t *pPortModeGet);
EXTERN_API RtkApiRet rtk_port_xmii_link_status_set(UnitChip_t unitChip, PortLinkConfigurationSet_t *pPortLinkCfgSet);
EXTERN_API RtkApiRet rtk_port_link_status_get(UnitChip_t unitChip, PortLinkStatusGet_t *pPortLinkStatusGet);
EXTERN_API RtkApiRet rtk_port_eth_link_ability_set(UnitChip_t unitChip, PortEthLinkAbilitySet_t *pEthLinkAbilitySet);
EXTERN_API RtkApiRet rtk_port_eth_link_ability_get(UnitChip_t unitChip, PortEthLinkAbilityGet_t *pEthLinkAbilityGet);
EXTERN_API RtkApiRet rtk_port_pkt_maxlen_set(UnitChip_t unitChip, PortPktMaxlenSet_t *pPortPktMaxlenSet);
EXTERN_API RtkApiRet rtk_port_pkt_maxlen_get(UnitChip_t unitChip, PortPktMaxlenGet_t *pPortPktMaxlenGet);
EXTERN_API RtkApiRet rtk_port_rgmii_txdly_set(UnitChip_t unitChip, PortRgmiiTxdly_t *pPortRgmiiTxdlySet);
EXTERN_API RtkApiRet rtk_port_rgmii_txdly_get(UnitChip_t unitChip, PortRgmiiTxdly_t *pPortRgmiiTxdlyGet);
EXTERN_API RtkApiRet rtk_port_rgmii_rxdly_set(UnitChip_t unitChip, PortRgmiiRxdlySet_t *pPortRgmiiRxdlySet);
EXTERN_API RtkApiRet rtk_port_rgmii_rxdly_get(UnitChip_t unitChip, PortRgmiiRxdlyGet_t *pPortRgmiiRxdlyGet);
EXTERN_API RtkApiRet rtk_port_xmii_driving_set(UnitChip_t unitChip, PortXmiiDriving_t *pPortXmiiDrivingSet);
EXTERN_API RtkApiRet rtk_port_xmii_driving_get(UnitChip_t unitChip, PortXmiiDriving_t *pPortXmiiDrivingGet);
EXTERN_API RtkApiRet rtk_port_xmii_latch_edge_set(UnitChip_t unitChip, PortXmiiLatchEdge_t *pPortXmiiLatchEdgeSet);
EXTERN_API RtkApiRet rtk_port_xmii_latch_edge_get(UnitChip_t unitChip, PortXmiiLatchEdge_t *pPortXmiiLatchEdgeGet);
EXTERN_API RtkApiRet rtk_port_rmii_refclk_sel_set(UnitChip_t unitChip, PortRmiiRefclkSel_t *pPortRmiiRefclkSelSet);
EXTERN_API RtkApiRet rtk_port_rmii_refclk_sel_get(UnitChip_t unitChip, PortRmiiRefclkSel_t *pPortRmiiRefclkSelGet);
EXTERN_API RtkApiRet rtk_port_xmii_ssc_set(UnitChip_t unitChip, PortXmiiSsc_t *pPortXmiiMaxSscSet);
EXTERN_API RtkApiRet rtk_port_xmii_ssc_get(UnitChip_t unitChip, PortXmiiSsc_t *pPortXmiiSscGet);
EXTERN_API RtkApiRet rtk_port_xmii_config_set(UnitChip_t unitChip, PortXmiiConfigSet_t *pPortXmiiConfigSet);
EXTERN_API RtkApiRet rtk_port_xmii_force_link_set(UnitChip_t unitChip, PortXmiiForceLinkSet_t *pPortXmiiForceLinkSet);
EXTERN_API RtkApiRet rtk_port_fe_config_set(UnitChip_t unitChip, PortFeConfigSet_t *pPortFeConfigSet);
EXTERN_API RtkApiRet rtk_port_t1_config_set(UnitChip_t unitChip, PortT1ConfigSet_t *pPortT1ConfigSet);
#endif /* RTKAS_API_INTERFACE_H */