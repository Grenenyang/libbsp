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

#ifndef RTK_API_PCIE_H__
#define RTK_API_PCIE_H__

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define DEFAULT_ID_LEN      (1u)
#define MACID_ADDR_LEN      (6u)
#define SMIDSVID_ADDR_LEN   (4u)
#define SERIALNUM_ADDR_LEN  (8u)
#define PCIE_MACID_FLAG     (1u)
#define PCIE_SMIDSVID_FLAG  (2u)
#define PCIE_SERIALNUM_FLAG (4u)

#define PCIe_OTP_START_ADDR  0x180u
#define PCIE_OTP_CONFIG_SIZE (256u)

/* PCIe config dummy register */
#define PCIE_LINK_SPEED_GEN1 1u
#define PCIE_LINK_SPEED_GEN2 2u
#define PCIE_LINK_SPEED_GEN3 3u

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/

/**
 *@brief This structure represents the PCIe parameters.
 *
 * This structure contains the switch port number and related PCIe parameters.
 */
typedef struct
{
    RtkPort pciePort;                      /**< [in] PCIe port (RTK_USER_PORT_9TH or RTK_USER_PORT_11TH) */
    uint8   type;                          /**< [in] Decide whether MACID(0x1), SMIDSVID(0x2) or Serial Number(0x4) should be set*/
    uint8   mac[MAC_ADDR_LEN];             /**< [in, out] PCIe Mac address (0x0 to 0xFF for 6 elements) */
    uint8   smidSvid[SMIDSVID_ADDR_LEN];   /**< [in, out] SMID and SVID (0x0 to 0xFF for 4 elements) */
    uint8   serialNum[SERIALNUM_ADDR_LEN]; /**< [in, out] Serial Number (0x0 to 0xFF for 8 elements) */
    uint8   defaultId[DEFAULT_ID_LEN];     /**< [in, out] PCIe Default ID (0x0 to 0xFF for 1 elements) */
} PcieParm_t;

/**
 *@brief This structure represents the PCIe link speed.
 *
 * This structure contains switch port number and PCIe link speed.
 */
typedef struct
{
    uint32  linkSpeed; /**< [in, out] Link speed (PCIE_LINK_SPEED_GEN1 to PCIE_LINK_SPEED_GEN3) */
    RtkPort pciePort;  /**< [in] PCIe port (RTK_USER_PORT_9TH or RTK_USER_PORT_11TH) */
} PcieLinkspeed_t;

/**
 *@brief This structure represents the number of the PCIe lanes.
 *
 * This structure contains switch port number and related PCIe lanes.
 */
typedef struct
{
    uint32  pcie2LaneEn; /**< [in, out] PCIe port lane (0 to 1) */
    RtkPort pciePort;    /**< [in] PCIe port (RTK_USER_PORT_9TH or RTK_USER_PORT_11TH) */
} PcieLaneInfo_t;

/**
 *@brief This structure represents the information about the PCIe PTM Sync to the Switch.
 *
 * This structure contains switch port number and related PCIe PTM Sync to the Switch.
 */
typedef struct
{
    uint32  pciePtmSync; /**< [out] PCIe port for PTM Sync to the Switch (0 to 1) */
    RtkPort pciePort;    /**< [in] PCIe port (RTK_USER_PORT_9TH or RTK_USER_PORT_11TH) */
} PciePtmInfo_t;

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_pcie_parm_get(UnitChip_t unitChip, PcieParm_t *pcieParam);
EXTERN_API RtkApiRet rtk_pcie_linkspeed_get(UnitChip_t unitChip, PcieLinkspeed_t *pcieLinkspeed);
EXTERN_API RtkApiRet rtk_pcie_lane_get(UnitChip_t unitChip, PcieLaneInfo_t *pcieLaneInfo);
EXTERN_API RtkApiRet rtk_pcie_ptm_sync_info_get(UnitChip_t unitChip, PciePtmInfo_t *pciePtmInfo);
#endif /* RTKAS_API_PCIE_H__ */