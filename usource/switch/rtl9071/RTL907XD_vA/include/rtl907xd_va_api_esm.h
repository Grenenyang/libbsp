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

#ifndef DVA_RTK_API_ESM_H__
#define DVA_RTK_API_ESM_H__
/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_common.h"
/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/

/******************************************************************************/
/* Enumeration                                                                */
/******************************************************************************/
typedef enum
{
    DVA_EM_EVENT_DFT_POLLING_TIMEOUT = 0,
    DVA_EM_EVENT_NIC_BUFFER_POLLING_TIMEOUT,
    DVA_EM_EVENT_INTERRUPT_POLLING_TIMEOUT,
    DVA_EM_EVENT_L2_POLLING_TIMEOUT,
    DVA_EM_EVENT_QBV_POLLING_TIMEOUT,
    DVA_EM_EVENT_QCI_POLLING_TIMEOUT,
    DVA_EM_EVENT_VLAN_POLLING_TIMEOUT,
    DVA_EM_EVENT_MDX_MASTER_POLLING_TIMEOUT,
    DVA_EM_EVENT_ROUTING_POLLING_TIMEOUT,
    DVA_EM_EVENT_PCIE_POLLING_TIMEOUT,
    DVA_EM_EVENT_CIPHER_POLLING_TIMEOUT,
    DVA_EM_EVENT_MACPHY_POLLING_TIMEOUT,
    DVA_EM_EVENT_SPIC_FLASH_POLLING_TIMEOUT,
    DVA_EM_EVENT_SERDES_POLLING_TIMEOUT,
    DVA_EM_EVENT_PTP_POLLING_TIMEOUT,
    DVA_EM_EVENT_DMAC_POLLING_TIMEOUT,
    DVA_EM_EVENT_TABLE_LIST_POLLING_TIMEOUT,
    DVA_EM_EVENT_SAFETY_POLLING_TIMEOUT,
    DVA_EM_EVENT_MACSEC_POLLING_TIMEOUT,
    DVA_EM_EVENT_OTP_POLLING_TIMEOUT,
    DVA_EM_EVENT_UART_POLLING_TIMEOUT,
    DVA_EM_EVENT_BOOTUP_PHY_POLLING_TIMEOUT,
    DVA_EM_EVENT_RESET_POLLING_TIMEOUT,
    DVA_EM_EVENT_MIB_POLLING_TIMEOUT,
    DVA_EM_EVENT_INDIRECT_ACCESS_TIMEOUT,
    DVA_EM_EVENT_FLASHLESS_TIMEOUT,
    DVA_EM_EVENT_ETHERNET_ACCESS_TIMEOUT,
    DVA_EM_EVENT_STACKING_TIMEOUT,
    DVA_EM_EVENT_ID_MAX,

} dva_EsmEventID_e;

typedef enum
{
    DVA_EM_EVENT_STATUS_OK = 0,
    DVA_EM_EVENT_STATUS_FAILED,
    DVA_EM_EVENT_STATUS_MAX,

} dva_EsmEventStatus_e;

/******************************************************************************/
/* Struct                                                                     */
/******************************************************************************/

/********* Function Declaration *********/
#if defined(ESM_ENABLE)
EXTERN_API void    dva_esm_perform_log_set(dva_EsmEventID_e eventID, dva_EsmEventStatus_e eventStatus);
static inline void dva_esm_event_log_set(dva_EsmEventID_e eventID, dva_EsmEventStatus_e eventStatus)
{
    dva_esm_perform_log_set(eventID, eventStatus);
}
#else
static inline void dva_esm_event_log_set(dva_EsmEventID_e eventID, dva_EsmEventStatus_e eventStatus)
{
    (void)eventID;
    (void)eventStatus;
    /* Do nothing when ESM_ENABLE is not defined */
}
#endif

#endif /* DVA_RTK_API_ESM_H__*/
