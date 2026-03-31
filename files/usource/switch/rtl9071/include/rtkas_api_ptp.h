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

#ifndef RTKAS_API_PTP_H__
#define RTKAS_API_PTP_H__

#include "rtkas_common.h"

#define MIN_TIMEOUT (1u)

#define PTP_MASTER_CLOCK (0u)
#define PTP_LOCAL_CLOCK  (1u)
#define PTP_SLAVE0_CLOCK (2u)
#define PTP_SLAVE1_CLOCK (3u)
#define PTP_SLAVE2_CLOCK (4u)

typedef struct
{
    uint16 up16Sec;  /**< Up 16 bit second part (0 to 0xffff) */
    uint32 low32Sec; /**< Low 32 bit second part (0 to 0xffffffff) */
    uint32 nanoSec;  /**< Nano Second part, (0 to 999999999) */
} PtpTs_t;

typedef struct
{
    uint16 tci;    /**< Tag control information. */
    uint8  enable; /**< Enable status. */
} PtpQTag;

typedef struct
{
    PtpQTag qtag[2];   /**< qtag[0] innter, qtag[1] outter*/
    uint8   fix;       /**< Fix length flag */
    uint8   enable;    /**< Tag global enable status */
    uint8   tagLength; /**< Tag length  */
} ptpqTaginfo;

typedef struct
{
    uint16 up16BitNs;  /**< Up 16 bit nano second part (0 to 0xffff) */
    uint32 mid32BitNs; /**< Low 32 bit nano second part (0 to 0xffffffff) */
    uint16 sub16bitNs; /**< Nano second subtraction part, (0 to 0xffffffff) */
} CFiled_t;

typedef enum
{
    EM_PTP_CRC_IGNORED = 0,   /**< CRC Ignored. */
    EM_PTP_CRC_NOT_VALIDATED, /**< CRC Not Validated. */
    EM_PTP_CRC_OPTIONAL,      /**< CRC Optional. */
    EM_PTP_CRC_VALIDATED,     /**< CRC Validated. */
    EM_PTP_CRC_END,           /**< CRC End. */
} PtpTpCrcValidated_e;

typedef struct
{
    uint16 pcp0 : 3;  /**< The Priority Code Point in 802.1Qtag0. */
    uint16 cfi0 : 1;  /**< The Canonical Format Indicator in Qtag0. */
    uint16 vid0 : 12; /**< The VLAN Identifier in Qtag0. */
    uint8  enable0;   /**< The Qtag0 Enable status. */
    uint16 pcp1 : 3;  /**< The Priority Code Point in 802.1Qtag1. */
    uint16 cfi1 : 1;  /**< The Canonical Format Indicator in Qtag1. */
    uint16 vid1 : 12; /**< The VLAN Identifier in Qtag1. */
    uint8  enable1;   /**< The Qtag1 Enable status. */
    uint8  fix;       /**< Fix length flag */
    uint8  enable;    /**< Tag global enable status */
    uint8  tagLength; /**< Tag length  */
} ptpNewQTaginfo;

typedef enum
{
    EM_PORT_TYPE_START = 0,
    EM_PORT_TYPE_PURE_100BASE_T1,               /**< Interface 100BASE_T1 */
    EM_PORT_TYPE_PURE_10BASE_T1,                /**< Interface 10BASE_T1 */
    EM_PORT_TYPE_FAST_ETHERNET_100,             /**< Interface FAST_ETHERNET 100M */
    EM_PORT_TYPE_FAST_ETHERNET_10,              /**< Interface FAST_ETHERNET 10M */
    EM_PORT_TYPE_FAST_ETHERNET_AN,              /**< Interface FAST_ETHERNET AN */
    EM_PORT_TYPE_MII_100,                       /**< Interface MII 100M */
    EM_PORT_TYPE_MII_10,                        /**< Interface MII 10M */
    EM_PORT_TYPE_RMII_100,                      /**< Interface RMII 100M */
    EM_PORT_TYPE_RMII_10,                       /**< Interface RMII 10M */
    EM_PORT_TYPE_RGMII_1000,                    /**< Interface RGMII 1000M */
    EM_PORT_TYPE_RGMII_100,                     /**< Interface RGMII 100M */
    EM_PORT_TYPE_RGMII_10,                      /**< Interface RGMII 10M */
    EM_PORT_TYPE_SGMII_WRAP_1000,               /**< Interface SGMII WRAP 1000M */
    EM_PORT_TYPE_SGMII_WRAP_100,                /**< Interface SGMII WRAP 100M */
    EM_PORT_TYPE_SGMII_10,                      /**< Interface SGMII 10M */
    EM_PORT_TYPE_SGMII_AN,                      /**< Interface SGMII AN */
    EM_PORT_TYPE_RTPGE_1000BASE_T1,             /**< Interface 1000BASE_T1 */
    EM_PORT_TYPE_HISGMII_WTG_2500,              /**< Interface HISGMII WTG 2500M */
    EM_PORT_TYPE_BaseKR_10000,                  /**< Interface 10G KR */
    EM_PORT_TYPE_BaseKR_5000,                   /**< Interface 5G KR */
    EM_PORT_TYPE_USXGMII_10000,                 /**< Interface USXGMII 10G */
    EM_PORT_TYPE_USXGMII_5000,                  /**< Interface USXGMII 5G */
    EM_PORT_TYPE_USXGMII_2500,                  /**< Interface USXGMII 2.5G */
    EM_PORT_TYPE_USXGMII_1000,                  /**< Interface USXGMII 1G */
    EM_PORT_TYPE_USXGMII_AN,                    /**< Interface USXGMII AN */
    EM_PORT_TYPE_RTPGE_100BASE_T1,              /**< Interface RTPGE 100BASE */
    EM_PORT_TYPE_MACPHY,                        /**< Interface MACPHY */
    EM_PORT_TYPE_SGMII_WTG_1000,                /**< Interface SGMII 1000M */
    EM_PORT_TYPE_SGMII_WTG_100,                 /**< Interface SGMII 100M */
    EM_PORT_TYPE_HISGMII_WRAP_2500,             /**< Interface HISGMII WRAP 2500M */
    EM_PORT_TYPE_XFI_WTG_10000,                 /**< Interface XFI 10G */
    EM_PORT_TYPE_XFI_WTG_5000,                  /**< Interface XFI 5G */
    EM_PORT_TYPE_GIGABIT_ETHERNET,              /**< Interface 1000Base-T */
    EM_PORT_TYPE_PURE_100BASE_T1_WTIH_MACSEC,   /**< Interface 1000BASE_T1 WITH_MACSEC */
    EM_PORT_TYPE_RTPGE_1000BASE_T1_WITH_MACSEC, /**< Interface RTPGE 1000BASE_T1 WITH_MACSEC */
    EM_PORT_TYPE_RTPGE_100BASE_T1_WITH_MACSEC,  /**< Interface RTPGE 100BASE_T1 WITH_MACSEC */
    EM_PORT_TYPE_MII_100_WITH_MACSEC,           /**< Interface MII 100M WITH_MACSEC */
    EM_PORT_TYPE_RMII_100_WITH_MACSEC,          /**< Interface RMII 100M WITH_MACSEC */
    EM_PORT_TYPE_RGMII_1000_WITH_MACSEC,        /**< Interface RGMII 1000M WITH_MACSEC */
    EM_PORT_TYPE_RGMII_100_WITH_MACSEC,         /**< Interface RGMII 100M WITH_MACSEC */
    EM_PORT_TYPE_SGMII_WRAP_1000_WITH_MACSEC,   /**< Interface SGMII WRAP 1000M WITH_MACSEC */
    EM_PORT_TYPE_SGMII_WRAP_100_WITH_MACSEC,    /**< Interface SGMII WRAP 100M WITH_MACSEC */
    EM_PORT_TYPE_HISGMII_WRAP_2500_WITH_MACSEC, /**< Interface HISGMII WRAP 2500M WITH_MACSEC */
    EM_PORT_TYPE_SGMII_WTG_1000_WITH_MACSEC,    /**< Interface SGMII WTG 1000M WITH_MACSEC */
    EM_PORT_TYPE_SGMII_WTG_100_WITH_MACSEC,     /**< Interface SGMII WTG 100M WITH_MACSEC */
    EM_PORT_TYPE_HISGMII_WTG_2500_WITH_MACSEC,  /**< Interface HISGMII WTG 2500M WITH_MACSEC */
    EM_PORT_TYPE_USXGMII_WTG_5000_WITH_MACSEC,  /**< Interface USXGMII WTG 5000M WITH_MACSEC */
    EM_PORT_TYPE_USXGMII_WTG_2500_WITH_MACSEC,  /**< Interface USXGMII WTG 2500M WITH_MACSEC */
    EM_PORT_TYPE_BaseKR_WTG_5000_WITH_MACSEC,   /**< Interface KR WTG 5000M WITH_MACSEC */
    EM_PORT_TYPE_NOT_SUPPORT,
    EM_PORT_TYPE_END /**< End */
} PtpPortType_e;

typedef enum
{
    EM_PTP_MAX_MS  = 65535, /**< Interval Type for maximum time interval */
    EM_PTP_8000_MS = 8001,  /**< Interval Type for 8000ms time interval */
    EM_PTP_4000_MS = 4001,  /**< Interval Type for 4000ms time interval */
    EM_PTP_2000_MS = 2001,  /**< Interval Type for 2000ms time interval */
    EM_PTP_1000_MS = 1001,  /**< Interval Type for 1000ms time interval */
    EM_PTP_500_MS  = 501,   /**< Interval Type for 500ms time interval */
    EM_PTP_250_MS  = 251,   /**< Interval Type for 250ms time interval */
    EM_PTP_125_MS  = 126,   /**< Interval Type for 125ms time interval */
    EM_PTP_62_MS   = 62,    /**< Interval Type for 62ms time interval */
    EM_PTP_31_MS   = 32     /**< Interval Type for 32ms time interval */
} PtpIntvaType_e;

typedef struct
{
    uint16 ppsIdx;    /**< [in] Pulse Per Second index (0 to 1) */
    uint8  ppsenable; /**< [in,out] Pulse Per Second enable status (0, 1) */
} PtpPpsenable_t;

typedef struct
{
    uint8 ppsidx; /**< [in] Pulse Per Second index (0 to 1) */
    uint8 ppsEn;  /**< [in,out] Pulse Per Second enable status (0 to 1) */
} PtpTaiPpsEn_t;

typedef struct
{
    uint8 ppsidx; /**< [in] Pulse Per Second index (0 to 1) */
    uint8 ppsEn;  /**< [in,out] Pulse Per Second enable status (0 to 1) */
} PtpSysPpsEn_t;

typedef struct
{
    RtkPort port;       /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint8   timeSyncEn; /**< [in,out] Time sync enable status (0 to 1) */
} PtpPortTimeSyncEn_t;

typedef struct
{
    uint8 ptpEnable; /**< [in,out] PTP hardware enable status (0 to 1) */
} PtpHwEnable_t;

typedef struct
{
    uint8 sysTimeEn; /**< [in,out] System time enable status (0 to 1) */
} PtpSysTimeEn_t;

typedef struct
{
    uint8 masterEn; /**< [in,out] Master clock enable status (0 to 1) */
} PtpMasterEn_t;

typedef struct
{
    uint8 slave2En; /**< [in,out] SLAVE2 enable status (0 to 1) */
} PtpSlave2En_t;

typedef struct
{
    uint8 slave1En; /**< [in,out] SLAVE1 enable status (0 to 1) */
} PtpSlave1En_t;

typedef struct
{
    uint8 slave0En; /**< [in,out] SLAVE0 clock enable status (0 to 1) */
} PtpSlave0En_t;

typedef struct
{
    uint32 domainid;  /**< [in] Domain index (0 to 2) */
    uint8  clkstable; /**< [Out] Clock statble status (0 to 0xFF) */
} PtpClkstable_t;

typedef struct
{
    uint16 cpumask; /**< [in,out] CPU mask (0 to <CHIPID>_RTK_USER_PMAP_ALL) */
} PtpRedirectCpumask_t;

typedef struct
{
    uint8 ptpRedirAct; /**< [in,out] Redirect action enable status (0 to 1) */
} PtpRedirectAct_t;

typedef struct
{
    RtkPort port;      /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint8   rtkEnable; /**< [in,out] Check enable status (0 to 1) */
} PtpEtherCheckEnable0_t;

typedef struct
{
    RtkPort port;     /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint8   ptp100tx; /**< [in,out] tp_100tx timing (0 to 2) */
} PtpPortPtp100Tx_t;

typedef struct
{
    RtkPort port;      /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint16  rxLatency; /**< [in,out] Receipt latency (0 to 0xFFFF) */
} PtpPortRxlatency_t;

typedef struct
{
    RtkPort port;      /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint16  txLatency; /**< [in,out] Transmit latency (0 to 0xFFFF) */
} PtpPortTxlatency_t;

typedef struct
{
    RtkPort port;        /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint32  domainid;    /**< [in] Domain index (0 to 2) */
    uint8   domainvalue; /**< [in,out] Domain value (0 to 0xFF) */
} PtpPortSlaveTimeDomain_t;

typedef struct
{
    RtkPort port;      /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint16  ethertype; /**< [in,out] Customized ethertype (>= 0x600) */
} PtpCustomEthertype_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpCustomEthertypeEnable_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpCustomVlanEnable_t;

typedef struct
{
#if defined(RTL_907XD_VA)
    uint32 domainid; /**< [in] Domain index (0 to 2) */
#endif
    RtkPort port;     /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint8   damac[6]; /**< [in,out] Destination MAC address. (N/A) */
} PtpCustomDamac01_t;

typedef struct
{

#if defined(RTL_907XD_VA)
    uint32 domainid; /**< [in] Domain index (0 to 2) */
#endif
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpCustomDamac01Enable_t;

typedef struct
{
    uint32      domainid; /**< [in] Domain index (0 to 2) */
    ptpqTaginfo taginfo;  /**< [in,out] 802.1Q tag information (N/A) */
} PtpTag_t;

typedef struct
{
    uint8         direction; /**< [in] Direction (0 to 1) */
    PtpPortType_e interface; /**< [in] Interface (EM_PORT_TYPE_RTGPE_1000BASE_T1 to EM_PORT_TYPE_END) */
    uint16        latency;   /**< [out] Latency */
} PtpInterfaceLatency_t;

typedef struct
{
    int8   loginterval; /**< [in] log interval (-5 to 3, and 127) */
    uint16 intval;      /**< [out] interval (2^-5 to 2^3 in ms, and 0xFFFF) */
} PtpLogToIntval_t;

typedef struct
{
    RtkPort port;                 /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  curntPdelayReqIntval; /**< [in,out] Current pdelay request interval (ref to rtk_ptp_log_to_intval) */
} PtpCurntPdelayreqIntval_t;

typedef struct
{
    RtkPort port;     /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END & RTK_CPU_PORT) */
    uint8   portmode; /**< [in,out] port mode. 0: Disable, 1: BC mode, 2: TC p2p mode, 3: TC e2e mode, 4:AS mode */
} PtpPortmode_t;

typedef struct
{
    uint8 glenable; /**< [in,out] global enable status (0 to 1) */
} PtpGlenable_t;

typedef struct
{
    uint8 cfggm; /**< [in,out] GM mode configuration (0 to 1) */
} PtpCfggm_t;

typedef struct
{
    RtkPort port;         /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   autoasenable; /**< [in,out] Automotive profile enable status (0 to 1) */
} PtpAutoasenable_t;

typedef struct
{
    uint32 domainid; /**< [in] Domain index (0 to 2) */
    uint8  enable;   /**< [in,out] Enable status (0 to 1) */
} PtpSignalingen_t;

typedef struct
{
    uint32 domainid;    /**< [in] Domain index (0 to 2) */
    uint8  slaveportid; /**< [in,out] The slave index (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
} PtpSlaveportid_t;

typedef struct
{
    uint16 slvMask; /**< [in,out] Slave port mask (0 to <CHIPID>_RTK_USER_PMAP_ALL) */
} PtpSlaveportmask_t;

typedef struct
{
    uint32 domainid;  /**< [in] Domain index (0 to 2) */
    uint8  priority1; /**< [in,out] Priority1 value (0 to 255) */
} PtpPriority1_t;

typedef struct
{
    PtpTs_t delaythresh; /**< [in,out] Pdelay threshold (N/A) */
} PtpDelaythresh_t;

typedef struct
{
    RtkPort port;         /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    int8    pdelayintval; /**< [in,out] Pdelay request message interval (-5 to 3, -128, 127)  */
} PtpInitlogpdelayintval_t;

typedef struct
{
    uint32 domainid;   /**< [in] Domain index (0 to 2) */
    int8   syncintval; /**< [in,out] Sync message interval (-5 to 3, -128, 127) */
} PtpOperlogsyncintval_t;

typedef struct
{
    RtkPort port;         /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    int8    pdelayintval; /**< [in,out] Pdelay request message interval (-5 to 3, -128, 127) */
} PtpOperlogpdelayintval_t;

typedef struct
{
    uint8 exchangeInfo; /**< [in,out] Exchange Info flag (0 to 1) */
} PtpExchangeinfo_t;

typedef struct
{
    uint16 timetarget; /**< [in,out] First boot up time target (0 to 0xFFFF) */
} PtpFirstbootuptimetarget_t;

typedef struct
{
    RtkPort port;            /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domainid;        /**< [in] Domain index (0 to 2) */
    uint32  curntsyncintval; /**< [in,out] Current sync interval (ref to rtk_ptp_log_to_intval) */
} PtpCurntsyncintval_t;

typedef struct
{
    RtkPort port;     /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domainid; /**< [in] Domain index (0 to 2) */
    uint8   portmode; /**< [in,out] Port mode (0 to 3) */
} PtpSwPortmode_t;

typedef struct
{
    RtkPort port;              /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domainid;          /**< [in] Domain index (0 to 2) */
    int8    initLogSyncIntval; /**< [in,out] Initial log sync interval (-5 to 3, -128, 127) */
} PtpInitlogsyncintval_t;

typedef struct
{
    RtkPort port;      /**< [in] ort (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domainid;  /**< [in] Domain index (0 to 2) */
    uint8   domainnum; /**< [in,out] Domain number (0 to 255) */
} PtpDomainnum_t;

typedef struct
{
    uint32 domainid;  /**< [in]  Domain index (0 to 2) */
    uint16 domainmap; /**< [in,out] Domain port map. (0 to <CHIPID>_RTK_USER_PMAP_ALL) */
} PtpDomainmap_t;

typedef struct
{
    RtkPort port;      /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkPort remapping; /**< [in,out] Remapping port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
} PtpPortremapping_t;

typedef struct
{
    uint8 enable; /**< [in,out] Enable status (0 to 1) */
} PtpPortremappingen_t;

typedef struct
{
    RtkPort port;          /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   oneStepTxSync; /**< [in,out] 1-step transmition sync message operability (0 to 1) */
} PtpOnesteptxsync_t;

typedef struct
{
    uint32  domainid; /**< [in] Domain index (0 to 2) */
    uint16 *portmask; /**< [in,out] Portmask array (0 to <CHIPID>_RTK_USER_PMAP_ALL) */
} PtpProtocolportmask_t;

typedef struct
{
    uint8 enable; /**< [in,out] Enable status (0 to 1) */
} PtpSkipfirstbootrecovery_t;

typedef struct
{
    uint32 domainid; /**< [in] Domain index (0 to 2) */
    uint8  enable;   /**< [in,out] Enable status (0 to 1) */
} PtpSilenceifneversynced_t;

typedef struct
{
    uint8 enable; /**< [in,out] Enable status (0 to 1) */
} PtpOverwritePtpid_t;

typedef struct
{
    uint8 enable; /**< [in,out] Enable status (0 to 1) */
} PtpSendPdelayFromMasterport_t;

typedef struct
{
    RtkPort port;          /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domainid;      /**< [in] Domain index (0 to 2) */
    uint8   oneStepTxSync; /**< [in,out] 1-Step transmition sync message operability (0 to 1) */
} PtpOnesteptxenSw_t;

typedef struct
{
    RtkPort port;     /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domainid; /**< [in] Domain index (0 to 2) */
    uint32  mib;      /**< [out] Mib counter (0 to 0xFFFFFFFF) */
} PtpMib_t;

typedef struct
{
    RtkPort port;          /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domainid;      /**< [in] Domain index (0 to 2) */
    uint8   oneStepRxSync; /**< [in,out] 1-Step received sync operability. (0 to 1) */
} PtpOnesteprxenSw_t;

typedef struct
{
    RtkPort port;                /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domainid;            /**< [in] Domain index (0 to 2) */
    uint8   initOneStepTXOperEn; /**< [in,out] Initial one stap transmission operability enable status (0 to 1) */
} PtpInitonesteptxoper_t;

typedef struct
{
    uint8 flashAccess; /**< [in,out] Flash access flag status (0 to 1) */
} PtpFlashRuntimeAccess_t;

typedef struct
{
    uint32 threshold; /**< [in,out] Write flash threshold (0 to 0xFFFFFFFF) */
} PtpPdelayWrflashThres_t;

typedef struct
{
    uint8 synclockStatus; /**< [in,out] Sync locked status (0 to 2) */
} PtpSynclockConfig_t;

typedef struct
{
    uint8 syncSlowdownStatus; /**< [in,out] Sync slowdown status (0 to 1) */
} PtpSyncslowdownConfig_t;

typedef struct
{
    RtkPort port;          /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   allowedfaults; /**< [in,out] The pdelay response allowed faults (0 to 0xFF) */
} PtpAllowedfaults_t;

typedef struct
{
    uint32 mechnisam; /**< [in,out] The delay measurement mechnisam (2 to 3) */
} PtpDelaymeasureMech_t;

typedef struct
{
    RtkPort  port;           /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    CFiled_t delayAsymmetry; /**< [in,out] The delay asymmetry value (N/A) */
} PtpDelayAsymmetry_t;

typedef struct
{
    uint32 domain; /**< [in] Domain index (0 to 2) */
    uint8  enable; /**< [in,out] Enable status (0 to 1) */
} PtpUseMgtsettableLogsyncintval_t;

typedef struct
{
    uint32 domain;    /**< [in] Domain index (0 to 2) */
    int8   logIntval; /**< [in,out] Log interval (-5 to 3, -128, 127) */
} PtpMgtsettableLogsyncintval_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpUsemgtsettableLogpdelayreqintval_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    int8    intval; /**< [in,out] Log interval (-5 to 3, -128, 127) */
} PtpMgtsettableLogpdelayreqintval_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpUsemgtsettableRateratio_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpMgtsettableRateratio_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpUsemgtsettableLinkdelay_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpMgtsettableLinkdelay_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpUsemgtsettable1Steptxoper_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domain; /**< [in] Domain index (0 to 2) */
    uint8   txOper; /**< [in,out]  The 1-step transmition operability (0 to 1)*/
} PtpMgtsettable1Steptxoper_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   enable; /**< [in,out] Enable status (0 to 1) */
} PtpUsemgtsettableGptpmsgintval_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domain; /**< [in] Domain index (0 to 2) */
    int8    intval; /**< [in,out] Log interval (-5 to 3, -128, 127) */
} PtpMgtsettableGptpmsglogintval_t;

typedef struct
{
    RtkPort port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  domain; /**< [in] Domain index (0 to 2) */
    int8    intval; /**< [in,out] Log interval (-5 to 3, -128, 127) */
} PtpInitGptpmsgintval_t;

typedef struct
{
    RtkPort port;       /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8   timeoutCnt; /**< [in,out] Timeout count (1 to 255) */
} PtpNeigptprxtimerout_t;

typedef struct
{
    uint8 as2020En; /**< [in,out] AS2020 enable status (0 to 1) */
} PtpAs2020Status_t;

typedef struct
{
    uint8 enable; /**< [out] Enable status (0 to 1) */
} PtpOverwriteClockid_t;

typedef struct
{
    uint16 priority_en; /**< [in,out] Priority enable status (0 to 1) */
} PtpInterServPriEn_t;

typedef struct
{
    uint8 priority; /**< [in,out] Priority (1 to 7) */
} PtpInterServPri_t;

typedef struct
{
    uint32 isPtpRdy; /**< [out] PTP ready status (0 to 1) */
} PtpIsReady_t;

typedef struct
{
    uint8   timesel; /**< [in] PTP clock select (PTP_MASTER_CLOCK to PTP_SLAVE2_CLOCK) */
    PtpTs_t ts;      /**< [out] PTP time Get (N/A) */
} PtpTimeDirectGet_t;

typedef struct
{
    uint8   port;           /*< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32  neiDelay;       /*< [out] Stored neighbor delay (0 to 0xFFFFFFFF) */
    float64 dNeiDelayRatio; /*< [out] Stored neighbor rate ratio (Range of double) */
    float64 dRateRatio[3];  /*< [out] Stored GM rate ratio (Range of double) */
} PtpGetNeidelayAndNeirateratio_t;

typedef enum
{
    EM_PTP_PRF_AS2011 = 0, /**< Profiles 802.1AS 2011 */
    EM_PTP_PRF_AUTOMOTIVE, /**< Profiles AVNU Automotive */
    EM_PTP_PRF_AUTOSAR,    /**< Profiles Autosar Time Sync */
    EM_PTP_PRF_AS2020,     /**< Profiles 802.1AS 2020 */
    EM_PTP_PRF_1588_E2ETC, /**< Profiles IEEE-1588 E2E Transparent Clock */
    EM_PTP_PRF_1588_P2PTC, /**< Profiles IEEE-1588 P2P Transparent Clock */
    EM_PTP_PRF_END,        /**< Profiles ENUM END */
} PtpTpSupportProfiles_e;

typedef struct
{
    uint32                 domainid; /*< [in] Domain index (0 to 2) */
    PtpTpSupportProfiles_e profiles; /*< [in,out] PTP Profiles. Please refer to PtpTpSupportProfiles_e. */
} PtpDomainProfiles_t;

typedef struct
{
    uint32 port;     /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint32 neiDelay; /*< [in,out] Neighbor Delay on the port. Units by nano second. (0 to 999999999) */
} PtpNeidelay_t;

typedef struct
{
    uint32 port;   /**< [in] Port (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    uint8  enable; /**< [in] Enable configuration. (0 to 1) */
} PtpCaldelayEn_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarCrcSupport_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarFramePri_t;

typedef struct
{
    uint32 pdelayIntval; /**< [in] Pdelay request interval. (0, 32, 62, 126, 251, 501, 1001, 2001, 4001, 8001) */
} PtpAtsarTxPdelayReqPeriod_t;

typedef struct
{
    uint32 delayThresh; /**< [in] Threshold of valid pdelay measurement result. (0 to 999999999) */
} PtpAtsarPdelayLatencyThresh_t;

typedef struct
{
    uint32 pDelay; /*< [in] Neighbor Delay on the port. Units by nano second. (0 to 999999999) */
} PtpAtsarPropDelay_t;

typedef struct
{
    uint32 intval; /**< [in] Sync packet interval. (0, 32, 62, 126, 251, 501, 1001, 2001, 4001, 8001) */
} PtpAtsarTxPeriod_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarMsgCompliance_t;

typedef struct
{
    PtpTpCrcValidated_e validated; /**< [in] CRC validation mode. (Please refer to PtpTpCrcValidated_e) */
} PtpAtsarRxCrcValidated_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarCrcFlags_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarTimeSubtlv_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarStatusSubtlv_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarUdataSubtlv_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarOfsSubtlv_t;

typedef struct
{
    uint8 flags; /**< [in] Flags defines which parts of the Follow_Up message elements shall be used for CRC calculation. (0x0 to 0xFF) */
} PtpAtsarTxCrcTimeFlags_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarTxTimeCrcSecured_t;

typedef struct
{
    uint8 enable; /**< [in] Enable configuration. (0 to 1) */
} PtpAtsarPdelayRespEnable_t;

typedef struct
{
    uint32          domainid; /**< [in] Domain index (0 to 2) */
    ptpNewQTaginfo *taginfo;  /**< [in] 802.1Q tag information (Please refer to ptpNewQTaginfo) */
} PtpAtsarTagVidPri_t;

typedef struct
{
    uint8 dataid[16]; /**< [in] Data ID List which is used to calculated CRC. (N/A) */
} PtpAtsarFoloupDataidList_t;

typedef struct
{
    uint8 ofsMulti; /**< [in] Offset synchronization multiplicity existing in OFS sub-TLV. (0 to 16)*/
} PtpAtsarOfsMulticity_t;

typedef struct
{
    uint32 domainid;           /**< [in] Domain index (0 to 2) */
    uint8  syncReceiptTimeout; /**< [in,out] The number of sync intervals to wait without receiving synchronization information. (1 to 255)*/
} PtpSyncReceiptTimeout_t;

typedef struct
{
    uint8 allowedLostResponses; /**< [in,out] The number of Pdelay_Req messages without valid responses above which a port is considered to be not exchanging peer delay messages with its neighbor. (1 to 255)*/
} PtpAllowedLostPrespNum_t;

typedef struct
{
    PtpIntvaType_e curntIntval; /**< [in] Current interval in ms. (refer to PtpIntvaType_e)*/
    int8           logIntval;   /**< [out] Log interval after transforamtion. (127,3,2,1,0,-1,-2,-3,-4,-5)*/
} PtpIntvalToLog_t;

EXTERN_API RtkApiRet rtk_ptp_get_ppsenable(UnitChip_t unitChip, PtpPpsenable_t *ptpPpsEnable);
EXTERN_API RtkApiRet rtk_ptp_set_ppsenable(UnitChip_t unitChip, PtpPpsenable_t *ptpPpsEnable);
EXTERN_API RtkApiRet rtk_ptp_get_tai_pps_en(UnitChip_t unitChip, PtpTaiPpsEn_t *ptpTaiPpsEn);
EXTERN_API RtkApiRet rtk_ptp_set_sys_pps_en(UnitChip_t unitChip, PtpSysPpsEn_t *ptpSysPpsEn);
EXTERN_API RtkApiRet rtk_ptp_get_port_time_sync_en(UnitChip_t unitChip, PtpPortTimeSyncEn_t *ptpPortTimeSyncEn);
EXTERN_API RtkApiRet rtk_ptp_set_port_time_sync_en(UnitChip_t unitChip, PtpPortTimeSyncEn_t *ptpPortTimeSyncEn);
EXTERN_API RtkApiRet rtk_ptp_get_hw_enable(UnitChip_t unitChip, PtpHwEnable_t *ptpHwEnable);
EXTERN_API RtkApiRet rtk_ptp_set_hw_enable(UnitChip_t unitChip, PtpHwEnable_t *ptpHwEnable);
EXTERN_API RtkApiRet rtk_ptp_get_sys_time_en(UnitChip_t unitChip, PtpSysTimeEn_t *ptpSysTimeEn);
EXTERN_API RtkApiRet rtk_ptp_set_sys_time_en(UnitChip_t unitChip, PtpSysTimeEn_t *ptpSysTimeEn);
EXTERN_API RtkApiRet rtk_ptp_get_master_en(UnitChip_t unitChip, PtpMasterEn_t *ptpMasterEn);
EXTERN_API RtkApiRet rtk_ptp_set_master_en(UnitChip_t unitChip, PtpMasterEn_t *ptpMasterEn);
EXTERN_API RtkApiRet rtk_ptp_get_slave2_en(UnitChip_t unitChip, PtpSlave2En_t *ptpSlave2En);
EXTERN_API RtkApiRet rtk_ptp_set_slave2_en(UnitChip_t unitChip, PtpSlave2En_t *ptpSlave2En);
EXTERN_API RtkApiRet rtk_ptp_get_slave1_en(UnitChip_t unitChip, PtpSlave1En_t *ptpSlave1En);
EXTERN_API RtkApiRet rtk_ptp_set_slave1_en(UnitChip_t unitChip, PtpSlave1En_t *ptpSlave1En);
EXTERN_API RtkApiRet rtk_ptp_get_slave0_en(UnitChip_t unitChip, PtpSlave0En_t *ptpSlave0En);
EXTERN_API RtkApiRet rtk_ptp_set_slave0_en(UnitChip_t unitChip, PtpSlave0En_t *ptpSlave0En);
EXTERN_API RtkApiRet rtk_ptp_clkstable_get(UnitChip_t unitChip, PtpClkstable_t *ptpClkStable);
EXTERN_API RtkApiRet rtk_ptp_get_redirect_cpumask(UnitChip_t unitChip, PtpRedirectCpumask_t *ptpRedirectCpuMask);
EXTERN_API RtkApiRet rtk_ptp_set_redirect_cpumask(UnitChip_t unitChip, PtpRedirectCpumask_t *ptpRedirectCpuMask);
EXTERN_API RtkApiRet rtk_ptp_get_redirect_act(UnitChip_t unitChip, PtpRedirectAct_t *ptpRedirectAct);
EXTERN_API RtkApiRet rtk_ptp_set_redirect_act(UnitChip_t unitChip, PtpRedirectAct_t *ptpRedirectAct);
EXTERN_API RtkApiRet rtk_ptp_get_ether_check_enable0(UnitChip_t unitChip, PtpEtherCheckEnable0_t *ptpEtherCheckEnable0);
EXTERN_API RtkApiRet rtk_ptp_set_ether_check_enable0(UnitChip_t unitChip, PtpEtherCheckEnable0_t *ptpEtherCheckEnable0);
EXTERN_API RtkApiRet rtk_ptp_get_port_ptp_100tx(UnitChip_t unitChip, PtpPortPtp100Tx_t *ptpPortPtp100Tx);
EXTERN_API RtkApiRet rtk_ptp_set_port_ptp_100tx(UnitChip_t unitChip, PtpPortPtp100Tx_t *ptpPortPtp100Tx);
EXTERN_API RtkApiRet rtk_ptp_get_port_rxlatency(UnitChip_t unitChip, PtpPortRxlatency_t *ptpPortRxLatency);
EXTERN_API RtkApiRet rtk_ptp_set_port_rxlatency(UnitChip_t unitChip, PtpPortRxlatency_t *ptpPortRxLatency);
EXTERN_API RtkApiRet rtk_ptp_get_port_txlatency(UnitChip_t unitChip, PtpPortTxlatency_t *ptpPortTxLatency);
EXTERN_API RtkApiRet rtk_ptp_set_port_txlatency(UnitChip_t unitChip, PtpPortTxlatency_t *ptpPortTxLatency);
EXTERN_API RtkApiRet rtk_ptp_get_port_slave_time_domain(UnitChip_t unitChip, PtpPortSlaveTimeDomain_t *ptpPortSlaveTimeDomain);
EXTERN_API RtkApiRet rtk_ptp_set_port_slave_time_domain(UnitChip_t unitChip, PtpPortSlaveTimeDomain_t *ptpPortSlaveTimeDomain);
EXTERN_API RtkApiRet rtk_ptp_set_custom_ethertype(UnitChip_t unitChip, PtpCustomEthertype_t *ptpCustomEthertype);
EXTERN_API RtkApiRet rtk_ptp_get_custom_ethertype(UnitChip_t unitChip, PtpCustomEthertype_t *ptpCustomEthertype);
EXTERN_API RtkApiRet rtk_ptp_set_ethertype_enable_custom(UnitChip_t unitChip, PtpCustomEthertypeEnable_t *ptpCustomEthertypeEnable);
EXTERN_API RtkApiRet rtk_ptp_get_ethertype_enable_custom(UnitChip_t unitChip, PtpCustomEthertypeEnable_t *ptpCustomEthertypeEnable);
EXTERN_API RtkApiRet rtk_ptp_custom_set_vlan_enable(UnitChip_t unitChip, PtpCustomVlanEnable_t *ptpCustomVlanEnable);
EXTERN_API RtkApiRet rtk_ptp_custom_get_vlan_enable(UnitChip_t unitChip, PtpCustomVlanEnable_t *ptpCustomVlanEnable);
EXTERN_API RtkApiRet rtk_ptp_set_custom_damac01(UnitChip_t unitChip, PtpCustomDamac01_t *ptpCustomDamac01);
EXTERN_API RtkApiRet rtk_ptp_get_custom_damac01(UnitChip_t unitChip, PtpCustomDamac01_t *ptpCustomDamac01);
EXTERN_API RtkApiRet rtk_ptp_set_damac01_enable_custom(UnitChip_t unitChip, PtpCustomDamac01Enable_t *ptpCustomDamac01Enable);
EXTERN_API RtkApiRet rtk_ptp_get_damac01_enable_custom(UnitChip_t unitChip, PtpCustomDamac01Enable_t *ptpCustomDamac01Enable);
EXTERN_API RtkApiRet rtk_ptp_tag_set(UnitChip_t unitChip, PtpTag_t *ptpTag);
EXTERN_API RtkApiRet rtk_ptp_get_interface_latency(UnitChip_t unitChip, PtpInterfaceLatency_t *ptpInterfaceLatency);
EXTERN_API RtkApiRet rtk_ptp_tag_get(UnitChip_t unitChip, PtpTag_t *ptpTag);
EXTERN_API RtkApiRet rtk_ptp_log_to_intval(UnitChip_t unitChip, PtpLogToIntval_t *ptpLogToIntval);
EXTERN_API RtkApiRet rtk_ptp_get_curnt_pdelayreq_intval(UnitChip_t unitChip, PtpCurntPdelayreqIntval_t *ptpCurntPdelayreqIntval);
EXTERN_API RtkApiRet rtk_ptp_set_curnt_pdelayreq_intval(UnitChip_t unitChip, PtpCurntPdelayreqIntval_t *ptpCurntPdelayreqIntval);
EXTERN_API RtkApiRet rtk_ptp_get_portmode(UnitChip_t unitChip, PtpPortmode_t *ptpPortMode);
EXTERN_API RtkApiRet rtk_ptp_set_portmode(UnitChip_t unitChip, PtpPortmode_t *ptpPortMode);
EXTERN_API RtkApiRet rtk_ptp_get_glenable(UnitChip_t unitChip, PtpGlenable_t *ptpGlenable);
EXTERN_API RtkApiRet rtk_ptp_set_glenable(UnitChip_t unitChip, PtpGlenable_t *ptpGlenable);
EXTERN_API RtkApiRet rtk_ptp_get_cfggm(UnitChip_t unitChip, PtpCfggm_t *ptpCfggm);
EXTERN_API RtkApiRet rtk_ptp_set_cfggm(UnitChip_t unitChip, PtpCfggm_t *ptpCfggm);
EXTERN_API RtkApiRet rtk_ptp_get_autoasenable(UnitChip_t unitChip, PtpAutoasenable_t *ptpAutoasenable);
EXTERN_API RtkApiRet rtk_ptp_set_autoasenable(UnitChip_t unitChip, PtpAutoasenable_t *ptpAutoasenable);
EXTERN_API RtkApiRet rtk_ptp_get_signalingen(UnitChip_t unitChip, PtpSignalingen_t *ptpSignalingEn);
EXTERN_API RtkApiRet rtk_ptp_set_signalingen(UnitChip_t unitChip, PtpSignalingen_t *ptpSignalingEn);
EXTERN_API RtkApiRet rtk_ptp_get_slaveportid(UnitChip_t unitChip, PtpSlaveportid_t *ptpSlaveportid);
EXTERN_API RtkApiRet rtk_ptp_set_slaveportid(UnitChip_t unitChip, PtpSlaveportid_t *ptpSlaveportid);
EXTERN_API RtkApiRet rtk_ptp_get_slaveportmask(UnitChip_t unitChip, PtpSlaveportmask_t *ptpSlaveportmask);
EXTERN_API RtkApiRet rtk_ptp_set_slaveportmask(UnitChip_t unitChip, PtpSlaveportmask_t *ptpSlaveportmask);
EXTERN_API RtkApiRet rtk_ptp_get_priority1(UnitChip_t unitChip, PtpPriority1_t *ptpPriority1);
EXTERN_API RtkApiRet rtk_ptp_set_priority1(UnitChip_t unitChip, PtpPriority1_t *ptpPriority1);
EXTERN_API RtkApiRet rtk_ptp_get_delaythresh(UnitChip_t unitChip, PtpDelaythresh_t *ptpDelaythresh);
EXTERN_API RtkApiRet rtk_ptp_set_delaythresh(UnitChip_t unitChip, PtpDelaythresh_t *ptpDelaythresh);
EXTERN_API RtkApiRet rtk_ptp_get_initlogpdelayintval(UnitChip_t unitChip, PtpInitlogpdelayintval_t *ptpInitlogpdelayintval);
EXTERN_API RtkApiRet rtk_ptp_set_initlogpdelayintval(UnitChip_t unitChip, PtpInitlogpdelayintval_t *ptpInitlogpdelayintval);
EXTERN_API RtkApiRet rtk_ptp_get_operlogsyncintval(UnitChip_t unitChip, PtpOperlogsyncintval_t *ptpOperlogsyncintval);
EXTERN_API RtkApiRet rtk_ptp_set_operlogsyncintval(UnitChip_t unitChip, PtpOperlogsyncintval_t *ptpOperlogsyncintval);
EXTERN_API RtkApiRet rtk_ptp_get_operlogpdelayintval(UnitChip_t unitChip, PtpOperlogpdelayintval_t *ptpOperlogpdelayintval);
EXTERN_API RtkApiRet rtk_ptp_set_operlogpdelayintval(UnitChip_t unitChip, PtpOperlogpdelayintval_t *ptpOperlogpdelayintval);
EXTERN_API RtkApiRet rtk_ptp_get_exchangeInfo(UnitChip_t unitChip, PtpExchangeinfo_t *ptpExchangeinfo);
EXTERN_API RtkApiRet rtk_ptp_set_exchangeInfo(UnitChip_t unitChip, PtpExchangeinfo_t *ptpExchangeinfo);
EXTERN_API RtkApiRet rtk_ptp_get_firstbootuptimetarget(UnitChip_t unitChip, PtpFirstbootuptimetarget_t *ptpFirstbootuptimetarget);
EXTERN_API RtkApiRet rtk_ptp_set_firstbootuptimetarget(UnitChip_t unitChip, PtpFirstbootuptimetarget_t *ptpFirstbootuptimetarget);
EXTERN_API RtkApiRet rtk_ptp_get_curntsyncintval(UnitChip_t unitChip, PtpCurntsyncintval_t *ptpCurntsyncintval);
EXTERN_API RtkApiRet rtk_ptp_set_curntsyncintval(UnitChip_t unitChip, PtpCurntsyncintval_t *ptpCurntsyncintval);
EXTERN_API RtkApiRet rtk_ptp_get_sw_portmode(UnitChip_t unitChip, PtpSwPortmode_t *ptpSwPortmode);
EXTERN_API RtkApiRet rtk_ptp_set_sw_portmode(UnitChip_t unitChip, PtpSwPortmode_t *ptpSwPortmode);
EXTERN_API RtkApiRet rtk_ptp_get_initlogsyncintval(UnitChip_t unitChip, PtpInitlogsyncintval_t *ptpInitlogsyncintval);
EXTERN_API RtkApiRet rtk_ptp_set_initlogsyncintval(UnitChip_t unitChip, PtpInitlogsyncintval_t *ptpInitlogsyncintval);
EXTERN_API RtkApiRet rtk_ptp_get_domainnum(UnitChip_t unitChip, PtpDomainnum_t *ptpDomainnum);
EXTERN_API RtkApiRet rtk_ptp_set_domainnum(UnitChip_t unitChip, PtpDomainnum_t *ptpDomainnum);
EXTERN_API RtkApiRet rtk_ptp_get_domainmap(UnitChip_t unitChip, PtpDomainmap_t *ptpDomainmap);
EXTERN_API RtkApiRet rtk_ptp_set_domainmap(UnitChip_t unitChip, PtpDomainmap_t *ptpDomainmap);
EXTERN_API RtkApiRet rtk_ptp_get_portremapping(UnitChip_t unitChip, PtpPortremapping_t *ptpPortRemapping);
EXTERN_API RtkApiRet rtk_ptp_set_portremapping(UnitChip_t unitChip, PtpPortremapping_t *ptpPortRemapping);
EXTERN_API RtkApiRet rtk_ptp_get_en_portremapping(UnitChip_t unitChip, PtpPortremappingen_t *ptpPortRemappingEn);
EXTERN_API RtkApiRet rtk_ptp_set_en_portremapping(UnitChip_t unitChip, PtpPortremappingen_t *ptpPortRemappingEn);
EXTERN_API RtkApiRet rtk_ptp_get_onesteptxsync(UnitChip_t unitChip, PtpOnesteptxsync_t *ptpOnestepTxSync);
EXTERN_API RtkApiRet rtk_ptp_set_onesteptxsync(UnitChip_t unitChip, PtpOnesteptxsync_t *ptpOnestepTxSync);
EXTERN_API RtkApiRet rtk_ptp_reinitprotocol(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_ptp_protocolportmask_set(UnitChip_t unitChip, PtpProtocolportmask_t *ptpProtocolportmask);
EXTERN_API RtkApiRet rtk_ptp_set_skipfirstbootrecovery(UnitChip_t unitChip, PtpSkipfirstbootrecovery_t *ptpSkipfirstbootrecovery);
EXTERN_API RtkApiRet rtk_ptp_set_silenceifneversynced(UnitChip_t unitChip, PtpSilenceifneversynced_t *ptpSilenceifneversynced);
EXTERN_API RtkApiRet rtk_ptp_overwrite_ptpid_set(UnitChip_t unitChip, PtpOverwritePtpid_t *ptpOverwritePtpid);
EXTERN_API RtkApiRet rtk_ptp_set_send_pdelay_from_masterport(UnitChip_t unitChip, PtpSendPdelayFromMasterport_t *ptpSendPdelayFromMasterport);
EXTERN_API RtkApiRet rtk_ptp_onesteptxen_sw_set(UnitChip_t unitChip, PtpOnesteptxenSw_t *ptpOnesteptxenSw);
EXTERN_API RtkApiRet rtk_ptp_rx_sync_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_rx_follow_up_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_rx_pdelay_request_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_rx_pdelay_response_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_rx_pdelay_resp_follow_up_mib_get(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_rx_ptp_packet_discard_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_sync_receipt_timeout_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_pdelay_allowed_lost_response_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_tx_sync_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_tx_follow_up_mib_get_count(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_tx_pdelay_request_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_tx_pdelay_response_mib_get_cont(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_tx_pdelay_resp_follow_up_mib_get(UnitChip_t unitChip, PtpMib_t *ptpMib);
EXTERN_API RtkApiRet rtk_ptp_onesteprxen_sw_set(UnitChip_t unitChip, PtpOnesteprxenSw_t *ptpOnesteprxenSw);
EXTERN_API RtkApiRet rtk_ptp_set_initonesteptxoper(UnitChip_t unitChip, PtpInitonesteptxoper_t *ptpInitonesteptxoper);
EXTERN_API RtkApiRet rtk_ptp_get_initonesteptxoper(UnitChip_t unitChip, PtpInitonesteptxoper_t *ptpInitonesteptxoper);
EXTERN_API RtkApiRet rtk_ptp_get_flash_runtime_access(UnitChip_t unitChip, PtpFlashRuntimeAccess_t *ptpFlashRuntimeAccess);
EXTERN_API RtkApiRet rtk_ptp_set_flash_runtime_access(UnitChip_t unitChip, PtpFlashRuntimeAccess_t *ptpFlashRuntimeAccess);
EXTERN_API RtkApiRet rtk_ptp_set_pdelay_wrflash_thres(UnitChip_t unitChip, PtpPdelayWrflashThres_t *ptpPdelayWrflashThres);
EXTERN_API RtkApiRet rtk_ptp_get_pdelay_wrflash_thres(UnitChip_t unitChip, PtpPdelayWrflashThres_t *ptpPdelayWrflashThres);
EXTERN_API RtkApiRet rtk_ptp_get_synclock_config(UnitChip_t unitChip, PtpSynclockConfig_t *ptpSynclockConfig);
EXTERN_API RtkApiRet rtk_ptp_set_synclock_config(UnitChip_t unitChip, PtpSynclockConfig_t *ptpSynclockConfig);
EXTERN_API RtkApiRet rtk_ptp_get_syncslowdown_config(UnitChip_t unitChip, PtpSyncslowdownConfig_t *ptpSyncslowdownConfig);
EXTERN_API RtkApiRet rtk_ptp_set_syncslowdown_config(UnitChip_t unitChip, PtpSyncslowdownConfig_t *ptpSyncslowdownConfig);
EXTERN_API RtkApiRet rtk_ptp_set_allowedfaults(UnitChip_t unitChip, PtpAllowedfaults_t *ptpAllowedfaults);
EXTERN_API RtkApiRet rtk_ptp_get_allowedfaults(UnitChip_t unitChip, PtpAllowedfaults_t *ptpAllowedfaults);
EXTERN_API RtkApiRet rtk_ptp_set_delaymeasure_mech(UnitChip_t unitChip, PtpDelaymeasureMech_t *ptpDelaymeasureMech);
EXTERN_API RtkApiRet rtk_ptp_get_delaymeasure_mech(UnitChip_t unitChip, PtpDelaymeasureMech_t *ptpDelaymeasureMech);
EXTERN_API RtkApiRet rtk_ptp_set_delay_asymmetry(UnitChip_t unitChip, PtpDelayAsymmetry_t *ptpDelayAsymmetry);
EXTERN_API RtkApiRet rtk_ptp_get_delay_asymmetry(UnitChip_t unitChip, PtpDelayAsymmetry_t *ptpDelayAsymmetry);
EXTERN_API RtkApiRet rtk_ptp_set_use_mgtsettable_logsyncintval(UnitChip_t unitChip, PtpUseMgtsettableLogsyncintval_t *ptpUseMgtsettableLogsyncintval);
EXTERN_API RtkApiRet rtk_ptp_get_use_mgtsettable_logsyncintval(UnitChip_t unitChip, PtpUseMgtsettableLogsyncintval_t *ptpUseMgtsettableLogsyncintval);
EXTERN_API RtkApiRet rtk_ptp_set_logsyncintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLogsyncintval_t *ptpMgtsettableLogsyncintval);
EXTERN_API RtkApiRet rtk_ptp_get_logsyncintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLogsyncintval_t *ptpMgtsettableLogsyncintval);
EXTERN_API RtkApiRet rtk_ptp_set_usemgtsettable_logpdelayreqintval(UnitChip_t unitChip, PtpUsemgtsettableLogpdelayreqintval_t *ptpUseMgtsettableLogpdelayreqintval);
EXTERN_API RtkApiRet rtk_ptp_get_usemgtsettable_logpdelayreqintval(UnitChip_t unitChip, PtpUsemgtsettableLogpdelayreqintval_t *ptpUseMgtsettableLogpdelayreqintval);
EXTERN_API RtkApiRet rtk_ptp_set_logpdelayreqintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLogpdelayreqintval_t *ptpMgtsettableLogpdelayreqintval);
EXTERN_API RtkApiRet rtk_ptp_get_logpdelayreqintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLogpdelayreqintval_t *ptpMgtsettableLogpdelayreqintval);
EXTERN_API RtkApiRet rtk_ptp_set_rateratio_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableRateratio_t *ptpUseMgtsettableRateratio);
EXTERN_API RtkApiRet rtk_ptp_get_rateratio_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableRateratio_t *ptpUseMgtsettableRateratio);
EXTERN_API RtkApiRet rtk_ptp_set_rateratio_mgtsettable(UnitChip_t unitChip, PtpMgtsettableRateratio_t *ptpMgtsettableRateratio);
EXTERN_API RtkApiRet rtk_ptp_get_rateratio_mgtsettable(UnitChip_t unitChip, PtpMgtsettableRateratio_t *ptpMgtsettableRateratio);
EXTERN_API RtkApiRet rtk_ptp_set_linkdelay_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableLinkdelay_t *ptpUseMgtsettableLinkdelay);
EXTERN_API RtkApiRet rtk_ptp_get_linkdelay_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableLinkdelay_t *ptpUseMgtsettableLinkdelay);
EXTERN_API RtkApiRet rtk_ptp_set_linkdelay_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLinkdelay_t *ptpMgtsettableLinkdelay);
EXTERN_API RtkApiRet rtk_ptp_get_linkdelay_mgtsettable(UnitChip_t unitChip, PtpMgtsettableLinkdelay_t *ptpMgtsettableLinkdelay);
EXTERN_API RtkApiRet rtk_ptp_set_1steptxoper_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettable1Steptxoper_t *ptpUseMgtsettable1Steptxoper);
EXTERN_API RtkApiRet rtk_ptp_get_1steptxoper_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettable1Steptxoper_t *ptpUseMgtsettable1Steptxoper);
EXTERN_API RtkApiRet rtk_ptp_set_1steptxoper_mgtsettable(UnitChip_t unitChip, PtpMgtsettable1Steptxoper_t *ptpMgtsettable1Steptxoper);
EXTERN_API RtkApiRet rtk_ptp_get_1steptxoper_mgtsettable(UnitChip_t unitChip, PtpMgtsettable1Steptxoper_t *ptpMgtsettable1Steptxoper);
EXTERN_API RtkApiRet rtk_ptp_set_gptpmsgintval_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableGptpmsgintval_t *ptpUseMgtsettableGptpmsgintval);
EXTERN_API RtkApiRet rtk_ptp_get_gptpmsgintval_usemgtsettable(UnitChip_t unitChip, PtpUsemgtsettableGptpmsgintval_t *ptpUseMgtsettableGptpmsgintval);
EXTERN_API RtkApiRet rtk_ptp_set_gptpmsglogintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableGptpmsglogintval_t *ptpMgtsettableGptpmsglogintval);
EXTERN_API RtkApiRet rtk_ptp_get_gptpmsglogintval_mgtsettable(UnitChip_t unitChip, PtpMgtsettableGptpmsglogintval_t *ptpMgtsettableGptpmsglogintval);
EXTERN_API RtkApiRet rtk_ptp_set_init_gptpmsgintval(UnitChip_t unitChip, PtpInitGptpmsgintval_t *ptpInitGptpmsgintval);
EXTERN_API RtkApiRet rtk_ptp_get_init_gptpmsgintval(UnitChip_t unitChip, PtpInitGptpmsgintval_t *ptpInitGptpmsgintval);
EXTERN_API RtkApiRet rtk_ptp_set_neigptprxtimerout(UnitChip_t unitChip, PtpNeigptprxtimerout_t *ptpNeigptprxtimerout);
EXTERN_API RtkApiRet rtk_ptp_get_neigptprxtimerout(UnitChip_t unitChip, PtpNeigptprxtimerout_t *ptpNeigptprxtimerout);
EXTERN_API RtkApiRet rtk_ptp_get_as2020_status(UnitChip_t unitChip, PtpAs2020Status_t *ptpAs2020Status);
EXTERN_API RtkApiRet rtk_ptp_set_as2020_status(UnitChip_t unitChip, PtpAs2020Status_t *ptpAs2020Status);
EXTERN_API RtkApiRet rtk_ptp_overwrite_clockid_get(UnitChip_t unitChip, PtpOverwriteClockid_t *ptpOverwriteClockid);
EXTERN_API RtkApiRet rtk_ptp_get_send_pdelay_from_masterport(UnitChip_t unitChip, PtpSendPdelayFromMasterport_t *ptpSendPdelayFromMasterport);
EXTERN_API RtkApiRet rtk_ptp_get_skipfirstbootrecovery(UnitChip_t unitChip, PtpSkipfirstbootrecovery_t *ptpSkipfirstbootrecovery);
EXTERN_API RtkApiRet rtk_ptp_get_silenceifneversynced(UnitChip_t unitChip, PtpSilenceifneversynced_t *ptpSilenceifneversynced);
EXTERN_API RtkApiRet rtk_ptp_get_en_inter_serv_pri(UnitChip_t unitChip, PtpInterServPriEn_t *ptpInterServPriEn);
EXTERN_API RtkApiRet rtk_ptp_set_en_inter_serv_pri(UnitChip_t unitChip, PtpInterServPriEn_t *ptpInterServPriEn);
EXTERN_API RtkApiRet rtk_ptp_get_inter_serv_pri(UnitChip_t unitChip, PtpInterServPri_t *ptpInterServPri);
EXTERN_API RtkApiRet rtk_ptp_set_inter_serv_pri(UnitChip_t unitChip, PtpInterServPri_t *ptpInterServPri);
EXTERN_API RtkApiRet rtk_ptp_get_isready(UnitChip_t unitChip, PtpIsReady_t *ptpIsReady);
EXTERN_API RtkApiRet rtk_ptp_time_direct_get(UnitChip_t unitChip, PtpTimeDirectGet_t *ptpTimeDirectGet);
EXTERN_API RtkApiRet rtk_ptp_get_neidelay_and_neirateratio(UnitChip_t unitChip, PtpGetNeidelayAndNeirateratio_t *ptpGetNeidelayAndNeirateratio);
EXTERN_API RtkApiRet rtk_ptp_erase_flash_data(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_ptp_set_domain_profiles(UnitChip_t unitChip, PtpDomainProfiles_t *ptpDomainProfiles);

EXTERN_API RtkApiRet rtk_ptp_intval_to_log(UnitChip_t unitChip, PtpIntvalToLog_t *ptpIntvalToLog);
EXTERN_API RtkApiRet rtk_ptp_set_allowed_lost_presp_num(UnitChip_t unitChip, PtpAllowedLostPrespNum_t *ptpAllowedLostPrespNum);
EXTERN_API RtkApiRet rtk_ptp_get_allowed_lost_presp_num(UnitChip_t unitChip, PtpAllowedLostPrespNum_t *ptpAllowedLostPrespNum);
EXTERN_API RtkApiRet rtk_ptp_set_sync_receipt_timeout(UnitChip_t unitChip, PtpSyncReceiptTimeout_t *ptpSyncReceiptTimeout);
EXTERN_API RtkApiRet rtk_ptp_get_sync_receipt_timeout(UnitChip_t unitChip, PtpSyncReceiptTimeout_t *ptpSyncReceiptTimeout);
EXTERN_API RtkApiRet rtk_ptp_neidelay_get(UnitChip_t unitChip, PtpNeidelay_t *ptpNeidelay);
EXTERN_API RtkApiRet rtk_ptp_neidelay_set(UnitChip_t unitChip, PtpNeidelay_t *ptpNeidelay);
EXTERN_API RtkApiRet rtk_ptp_caldelayen_set(UnitChip_t unitChip, PtpCaldelayEn_t *ptpCaldelayEn);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_crc_support(UnitChip_t unitChip, PtpAtsarCrcSupport_t *ptpAtsarCrcSupport);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_frame_pri(UnitChip_t unitChip, PtpAtsarFramePri_t *ptpAtsarFramePri);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_tx_pdelayreq_period(UnitChip_t unitChip, PtpAtsarTxPdelayReqPeriod_t *ptpAtsarTxPdelayReqPeriod);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_pdelay_latency_thresh(UnitChip_t unitChip, PtpAtsarPdelayLatencyThresh_t *ptpAtsarPdelayLatencyThresh);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_prop_delay(UnitChip_t unitChip, PtpAtsarPropDelay_t *ptpAtsarPropDelay);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_tx_period(UnitChip_t unitChip, PtpAtsarTxPeriod_t *ptpAtsarTxPeriod);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_msg_compliance(UnitChip_t unitChip, PtpAtsarMsgCompliance_t *ptpAtsarMsgCompliance);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_rx_crc_validated(UnitChip_t unitChip, PtpAtsarRxCrcValidated_t *ptpAtsarRxCrcValidated);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_crc_flags(UnitChip_t unitChip, PtpAtsarCrcFlags_t *ptpAtsarCrcFlags);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_time_subtlv(UnitChip_t unitChip, PtpAtsarTimeSubtlv_t *ptpAtsarTimeSubtlv);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_status_subtlv(UnitChip_t unitChip, PtpAtsarStatusSubtlv_t *ptpAtsarStatusSubtlv);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_udata_subtlv(UnitChip_t unitChip, PtpAtsarUdataSubtlv_t *ptpAtsarUdataSubtlv);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_ofs_subtlv(UnitChip_t unitChip, PtpAtsarOfsSubtlv_t *ptpAtsarOfsSubtlv);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_tx_crc_time_flags(UnitChip_t unitChip, PtpAtsarTxCrcTimeFlags_t *ptpAtsarTxCrcTimeFlags);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_tx_time_crc_secured(UnitChip_t unitChip, PtpAtsarTxTimeCrcSecured_t *ptpAtsarTxTimeCrcSecured);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_pdelay_resp_enable(UnitChip_t unitChip, PtpAtsarPdelayRespEnable_t *ptpAtsarPdelayRespEnable);
EXTERN_API RtkApiRet rtk_ptp_tag_vid_pri_set(UnitChip_t unitChip, PtpAtsarTagVidPri_t *ptpAtsarTagVidPri);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_foloup_dataid_list(UnitChip_t unitChip, PtpAtsarFoloupDataidList_t *ptpAtsarFoloupDataidList);
EXTERN_API RtkApiRet rtk_ptp_atsar_set_ofs_multicity(UnitChip_t unitChip, PtpAtsarOfsMulticity_t *ptpAtsarOfsMulticity);

#endif /* RTKAS_API_PTP_H__ */
