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

#ifndef RTKAS_API_MIRROR_H__
#define RTKAS_API_MIRROR_H__

#include "rtkas_common.h"

#define MIRROR_CFI_MAX         (0x1u)
#define MIRROR_PRI_MAX         (0x7u)
#define MIRROR_VID_MAX         (0xFFFu)
#define MIRROR_SAMPLE_MAX      (0xFFFFu)
#define MIRROR_FUNCTION_OFFSET (0x2Cu)

/* mirror port_based and flow_based parameter: morg */
#define MIRROR_MODIFIED_PACKET (0u)
#define MIRROR_ORIGINAL_PACKET (1u)

/**
 * @brief Enum of RSPAN tx tag action
 */
typedef enum
{
    EM_RSPAN_TXTAG_RETAIN = 0, /**< Retain the RSPAN tag */
    EM_RSPAN_TXTAG_ADD,        /**< Add the RSPAN tag */
    EM_RSPAN_TXTAG_CHANGE,     /**< Change the RSPAN tag */
    EM_RSPAN_TXTAG_REMOVE,     /**< Remove the RSPAN tag */
    EM_RSPAN_TXTAG_END
} RtkRspanTxtagAction_e;

/**
 * @brief This is a mirror RSPAN tag structure.
 *
 * This structure is used to configure the mirror RSPAN tx tag information, including VLAN ID, CFI, PRI, and TPID.
 */
typedef struct
{
    uint16 vid;  /**< VLAN Identifier */
    uint8  cfi;  /**< Canonical Format Indicator */
    uint8  pri;  /**< User-Priority */
    uint16 tpid; /**< Tag Protocol Identifier */
} RtkRspanTxtag_t;

/**
 * @brief This is a mirror information structure.
 *
 * This structure is used to configure or obtain port-based or flow-based mirroring.
 */
typedef struct
{
    uint8   mirrorNo;                /**< [in] Mirroring function number (1 to 2) */
    RtkPort mirroringPort;           /**< [in,out] Mirroring port is used to monitor the network traffic (RTK_USER_PORT_1ST to RTK_USER_PORT_END) */
    RtkPmap mirroredIngressPortmask; /**< [in,out] The ingress mirrored port mask contains the ports which are monitored for ingress traffic (0 to <CHIP_ID>_RTK_USER_PMAP_ALL, <CHIP_ID>_RTK_CPU_PMAP) */
    RtkPmap mirroredEgressPortmask;  /**< [in,out] The egress mirrored port mask contains the ports which are monitored for egress traffic (0 to <CHIP_ID>_RTK_USER_PMAP_ALL, <CHIP_ID>_RTK_CPU_PMAP) */
    uint32  morg;                    /**< [in,out] Mirror original indicates that the switch mirrors original packets or modified packets. 0: mirror modified packet, 1: mirror original packet (0 to 1) */
} MirrorInfo_t;

/**
 * @brief This is a mirror filter information structure.
 *
 * This structure is used to configure or retrieve mirror filter settings.
 */
typedef struct
{
    uint8     mirrorNo; /**< [in] Mirroring function number (1 to 2) */
    RtkEnable enable;   /**< [in,out] Filter status. DISABLE: The mirroring filter doesn't prevent sending forwarded traffic to the mirroring port, ENABLE: The mirroring filter prevents sending forwarded traffic to the mirroring port (0 to 1) */
} MirrorFilterInfo_t;

/**
 * @brief This is a mirror sample rate information structure.
 *
 * This structure is used to configure or retrieve mirror sample rate settings.
 */
typedef struct
{
    uint8  mirrorNo; /**< [in] Mirroring function number (1 to 2) */
    uint32 samples;  /**< [in,out] Sample size indicates control of the quantity of mirrored packets (0 to 65535) */
} MirrorSampleInfo_t;

/**
 * @brief This is a mirror RSPAN tx information structure.
 *
 * This structure is used to configure or retrieve mirror RSPAN tx settings.
 */
typedef struct
{
    uint8                 mirrorNo;      /**< [in] Mirroring function number (1 to 2) */
    RtkPort               mirroringPort; /**< [in,out] Mirroring port is used to monitor the network traffic (<CHIP_ID>_RTK_USER_PORT_1ST to <CHIP_ID>_RTK_USER_PORT_END) */
    RtkRspanTxtagAction_e action;        /**< [in,out] RSPAN TX action (N/A) */
    RtkRspanTxtag_t       tag;           /**< [in,out] VLAN tag (N/A) */
} MirrorRspanTxInfo_t;

/**
 * @brief This is a mirror RSPAN rx information structure.
 *
 * This structure is used to configure or retrieve mirror RSPAN rx settings.
 */
typedef struct
{
    uint8     mirrorNo;         /**< [in] Mirroring function number (1 to 2) */
    RtkPmap   mirroredPortmask; /**< [in,out] The mirrored port mask contains the ports which are monitored for mirrored traffic (0 to <CHIP_ID>_RTK_USER_PMAP_ALL, <CHIP_ID>_RTK_CPU_PMAP) */
    RtkEnable enable;           /**< [in,out] RSPAN check, DISABLE: no action, ENABLE: packets with matched VID are regarded as mirrored traffic directly (0 to 1) */
    uint16    vid;              /**< [in,out] VLAN identifier (0 to 4095) */
} MirrorRspanRxInfo_t;

/**
 * @brief This is a mirror reset information structure.
 *
 * This structure is used to reset mirror settings.
 */
typedef struct
{
    uint8 mirrorNo; /**< [in] Mirroring function number (1 to 2) */
} MirrorResetInfo_t;

EXTERN_API RtkApiRet rtk_mirror_port_based_set(UnitChip_t unitChip, MirrorInfo_t *mirrorInfo);
EXTERN_API RtkApiRet rtk_mirror_port_based_get(UnitChip_t unitChip, MirrorInfo_t *mirrorInfo);
EXTERN_API RtkApiRet rtk_mirror_flow_based_set(UnitChip_t unitChip, MirrorInfo_t *mirrorInfo);
EXTERN_API RtkApiRet rtk_mirror_flow_based_get(UnitChip_t unitChip, MirrorInfo_t *mirrorInfo);
EXTERN_API RtkApiRet rtk_mirror_filter_set(UnitChip_t unitChip, MirrorFilterInfo_t *filterInfo);
EXTERN_API RtkApiRet rtk_mirror_filter_get(UnitChip_t unitChip, MirrorFilterInfo_t *filterInfo);
EXTERN_API RtkApiRet rtk_mirror_sample_rate_set(UnitChip_t unitChip, MirrorSampleInfo_t *sampleInfo);
EXTERN_API RtkApiRet rtk_mirror_sample_rate_get(UnitChip_t unitChip, MirrorSampleInfo_t *sampleInfo);
EXTERN_API RtkApiRet rtk_rspan_tx_set(UnitChip_t unitChip, MirrorRspanTxInfo_t *rspanTxInfo);
EXTERN_API RtkApiRet rtk_rspan_tx_get(UnitChip_t unitChip, MirrorRspanTxInfo_t *rspanTxInfo);
EXTERN_API RtkApiRet rtk_rspan_rx_set(UnitChip_t unitChip, MirrorRspanRxInfo_t *rspanRxInfo);
EXTERN_API RtkApiRet rtk_rspan_rx_get(UnitChip_t unitChip, MirrorRspanRxInfo_t *rspanRxInfo);
EXTERN_API RtkApiRet rtk_mirror_reset(UnitChip_t unitChip, MirrorResetInfo_t *resetInfo);

#endif /* RTKAS_API_MIRROR_H__ */
