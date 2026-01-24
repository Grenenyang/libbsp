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

#ifndef RTK_TIMER_H__
#define RTK_TIMER_H__
/************ ACL ************/

/************ DOT1X ************/

/************ ETHERNET-ACCESS ************/
#define RTK_ETH_ACC_RETRY                   (0x10u)
#define RTK_ETH_ACC_FLASH_WRITE_TIME        (0u)
#define RTK_ETH_ACC_FLASH_ERASE_SEC4K_TIME  (45000u)
#define RTK_ETH_ACC_FLASH_ERASE_BLK64K_TIME (450000u)
#define RTK_ETH_ACC_FLASH_ERASE_ALL_TIME    (7000000u)
#define RTK_ETH_ACC_FLASH_READ_BASE_TIME    (20000u)
#define RTK_ETH_ACC_FLASH_WRITE_BASE_TIME   (20000u)
#define RTK_ETH_ACC_FLASH_POLLING_TIME      (2000u)
#define RTK_ETH_ACC_REBOOT_DELAY            (100u)

/************ INTERFACE ************/
/* Experimental data */
#define LOCK_TIMEOUT (0x7FFFu)

/************ L2 ************/
/* Experimental data */
#define LUT_SEARCH_TIMEOUT (0x1000u)

/************ LINK-AGGREGATION ************/

/************ MIB ************/

/************ OPFSM ************/

/************ PCIE ************/

/************ PHY-TEST ************/
/* The RTCT should be done in 10ms (10us*1000)*/
#define RTK_TIMER_RTCT_USECOUND          (10u)
#define RTK_TIMER_RTCT_TIMEOUT_COUNT_NUM (0x3e8u)
#define RTK_TIMER_SQI_USECOUND           (1000u)
#define RTK_TIMER_SQI_TIMEOUT_COUNT_NUM  (0x3e8u)

/************ PORT-ISOLATION ************/

/************ STORM-CTL ************/

/************ MIRRORING ************/

/************ PTP ************/
/* Experimental Data */
#define PTP_TIME_DIRECT_GET_TIMEOUT (10000u)
/* Experimental Data */
#define PTP_TIME_DIRECT_SET_TIMEOUT (10000u)
/* Experimental Data */
#define PTP_TIME_GET_TIMEOUT (10000u)
/* Experimental Data */
#define PTP_TIME_SET_TIMEOUT (20000u)
/* Experimental Data */
#define PTP_PORT_TX_RX_GET_TIMEOUT (20000u)
/* Evaluate Data. Self set then get. */
#define PTP_TAI_GET_TIME_TIMEOUT (10000u)

/************ QOS ************/

/************ ROUTING ************/
/* The Routing should be done (worst case) in 1.1s (1us * 1100000) */
#define RT_INIT_TIMEOUT (1100000u)

/************ SMI ************/

/************ SPI-FLASH ************/

/************ STACKING ************/
#define STACKING_TIMEOUT (1000u)

/************ STP ************/

/************ SYSTEM ************/
/* Experimental Data */
#define RTK_TIMER_SYSTEM_SERDES_MDIO_CHANNEL (10000u)

/* Experiment Data */
#define RTK_TIMER_SYSTEM_VIRTUAL_CHANNEL (2000u)

/* Experimental data */
#define RTK_TIMER_OTP_INDIRECT_CHANNEL (2000u)

/************ TSN ************/
/* Experimental Data */
#define PSFP_CONFIG_CHANGE_TIMEOUT (1000u)
/************ VLAN ************/
/* Experimental data */
#define VLAN_FLUSH_TIMEOUT (0x1000u)

/************ FLOWCTRL ************/

/************ IGMP ************/

#endif
