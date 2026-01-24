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

#ifndef RTKAS_API_QOS_H__
#define RTKAS_API_QOS_H__

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/
#define QOS_PRI_MAX              (7u)
#define QOS_DEI_MAX              (1u)
#define QOS_QUE_MAX              (7u)
#define QOS_DSCP_MAX             (63u)
#define QOS_TBL_IDX_MAX          (1u)
#define QOS_PRI_WT_MAX           (7u)
#define QOS_WT_MAX               (0x3Fu)
#define QOS_MINBKTSCKTYPE_OTHERS (0u)
#define QOS_MINBKTSCKTYPE_SP     (1u)
#define QOS_MINBKTSCKTYPE_END    (QOS_MINBKTSCKTYPE_SP)
#define QOS_SCHTYPESEL_WFQ       (0u)
#define QOS_SCHTYPESEL_WRR       (1u)
#define QOS_SCHTYPESEL_OTHERS    (2u)
#define QOS_SCHTYPESEL_END       (QOS_SCHTYPESEL_OTHERS)

/******************************************************************************/
/* Variable Type Declaration                                                  */
/******************************************************************************/
typedef uint32 RtkQoSPri;    /**< [in,out] Priority vlaue (0 to QOS_PRI_MAX) */
typedef uint32 RtkQoSDei;    /**< [in,out] Dei vlaue (0 to QOS_DEI_MAX) */
typedef uint32 RtkQoSQue;    /**< [in,out] Tx queue number (0 to QOS_QUE_MAX)*/
typedef uint32 RtkQoSSch;    /**< [in,out] Scheduler value (<CHIPID>_QOS_SCH_SP to <CHIPID>_QOS_SCH_END) */
typedef uint32 RtkQoSTblIdx; /**< [in,out] Table index (0 to QOS_TBL_IDX_MAX) */
typedef uint32 RtkQoSWt;     /**< [in,out] Weight value of WRR or WFQ (0 to QOS_WT_MAX) */
typedef uint32 RtkQoSDscp;   /**< [in,out] Dscp vlaue (0 to QOS_DSCP_MAX) */
typedef uint32 RtkQoSBw;     /**< [in,out] Bandwidth of the port or queue (0 to <CHIPID>_QOS_PORT_BW_MAX or <CHIPID>_QOS_QUE_BW_MAX) */

/******************************************************************************/
/* Structures                                                                 */
/******************************************************************************/
/* Ingress Bandwidth Control */
/**
 * @brief This struct is used for the ingress bandwidth control.
 * The bwCtrlEn0, the igrBw0, the flowCtrlEn0, the bwCtrlEn1, the igrBw1 and the flowCtrlEn1 can each be either an input or an output.
 */
typedef struct
{
    RtkEnable bwCtrlEn0;   /**< [in,out] Bandwidth control enable bit for LB0 (DISABLED to ENABLED) */
    RtkQoSBw  igrBw0;      /**< [in,out] Ingress bw at LB0 in units of 8Kbps (0 to <CHIPID>_QOS_PORT_BW_MAX) */
    RtkEnable flowCtrlEn0; /**< [in,out] Flow control enable bit at LB0 (DISABLED to ENABLED) */
    RtkEnable bwCtrlEn1;   /**< [in,out] Bandwidth control enable bit for LB1 (DISABLED to ENABLED) */
    RtkQoSBw  igrBw1;      /**< [in,out] Ingress bw at LB1 in units of 8Kbps (0 to <CHIPID>_QOS_PORT_BW_MAX) */
    RtkEnable flowCtrlEn1; /**< [in,out] Flow control enable bit at LB1 (DISABLED to ENABLED) */
} QosIgrBwCtrl_t;

/**
 * @brief This struct is used for the bandwidth contorl information.
 * The port is an input, and the qosIgrBwCtrl can be either an input or an output.
 */
typedef struct
{
    RtkPort        port;         /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    QosIgrBwCtrl_t qosIgrBwCtrl; /**< [in,out] QoS Ingress Bandwidth Control (N/A) */
} QosIgrBwCtrlInfo_t;

/* Traffic Classification */
/**
 * @brief This struct is used for the bandwidth contorl information.
 * The port is an input, and the pri can be either an input or an output.
 */
typedef struct
{
    RtkPort   port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSPri pri;  /**< [in,out] Priority value (0 to QOS_PRI_MAX) */
} QosPortPri_t;

/* Priority Source Select */
/**
 * @brief This struct is used for the priority selection.
 * The portPri, the hybridPri, the dscpPri, the cvlanPri and the svlanPri can each be either an input or an output.
 */
typedef struct
{
    RtkQoSPri portPri;   /**< [in,out] The weight of Port-based priority (0 to QOS_PRI_MAX) */
    RtkQoSPri hybridPri; /**< [in,out] The weight of Hybrid-base priority (0 to QOS_PRI_MAX) */
    RtkQoSPri dscpPri;   /**< [in,out] The weight of Dscp priority (0 to QOS_PRI_MAX) */
    RtkQoSPri cvlanPri;  /**< [in,out] The weight of Cvlan priority (0 to QOS_PRI_MAX) */
    RtkQoSPri svlanPri;  /**< [in,out] The weight of Svlan priority (0 to QOS_PRI_MAX) */
} QosPriSel_t;

/**
 * @brief This struct is used for the priority selection information.
 * The group is an input, and the qosPriSel can be either an input or an output.
 */
typedef struct
{
    RtkQoSTblIdx group;     /**< [in] Arbitration table index (DISABLED to ENABLED) */
    QosPriSel_t  qosPriSel; /**< [in,out] QoS Priority Source Select (N/A) */
} QosPriSelInfo_t;

/**
 * @brief This struct is used for the priority weight table.
 * The port is an input, and the idx can be either an input or an output.
 */
typedef struct
{
    RtkPort      port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSTblIdx idx;  /**< [in,out] Table index (0 to QOS_TBL_IDX_MAX) */
} QosPriWtTbl_t;

/**
 * @brief This struct is used for the cvlan priority remappoing table.
 * The idx, the iPri and the iDei are inputs, and the mappingPri can be either an input or an output.
 */
typedef struct
{
    RtkQoSTblIdx idx;        /**< [in] Table index (0 to QOS_TBL_IDX_MAX) */
    RtkQoSPri    iPri;       /**< [in] Inner priority value (0 to QOS_PRI_MAX) */
    RtkQoSDei    iDei;       /**< [in] Inner dei value (0 to QOS_DEI_MAX) */
    RtkQoSPri    mappingPri; /**< [in,out] Mapping priority value (0 to QOS_PRI_MAX) */
} QosCvlanPriRemap_t;

/**
 * @brief This struct is used for the svlan priority remappoing table.
 * The idx, the oPri and the oDei are inputs, and the mappingPri can be either an input or an output.
 */
typedef struct
{
    RtkQoSTblIdx idx;        /**< [in] Table index (0 to QOS_TBL_IDX_MAX)) */
    RtkQoSPri    oPri;       /**< [in] Outer priority value (0 to QOS_PRI_MAX) */
    RtkQoSDei    oDei;       /**< [in] Outer dei value (0 to QOS_DEI_MAX) */
    RtkQoSPri    mappingPri; /**< [in,out] Mapping priority value (0 to QOS_PRI_MAX) */
} QosSvlanPriRemap_t;

/**
 * @brief This struct is used for the dscp priority remappoing table.
 * The idx and the dscp are inputs, and the mappingPri can be either an input or an output.
 */
typedef struct
{
    RtkQoSTblIdx idx;        /**< [in] Table index (0 to QOS_TBL_IDX_MAX) */
    RtkQoSDscp   dscp;       /**< [in] Dscp value (0 to QOS_DSCP_MAX) */
    RtkQoSPri    mappingPri; /**< [in,out] Mapping priority value (0 to QOS_PRI_MAX) */
} QosDscpPriRemap_t;

/**
 * @brief This struct is used for the cvlan remapping table.
 * The port is an input, and the idx can be either an input or an output.
 */
typedef struct
{
    RtkPort      port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSTblIdx idx;  /**< [in,out] Table index (0 to QOS_TBL_IDX_MAX) */
} QosCvlanRemapTbl_t;

/**
 * @brief This struct is used for the svlan remapping table.
 * The port is an input, and the idx can be either an input or an output.
 */
typedef struct
{
    RtkPort      port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSTblIdx idx;  /**< [in,out] Table index (0 to QOS_TBL_IDX_MAX) */
} QosSvlanRemapTbl_t;

/**
 * @brief This struct is used for the dscp remapping table.
 * The port is an input, and the idx can be either an input or an output.
 */
typedef struct
{
    RtkPort      port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSTblIdx idx;  /**< [in,out] Table index (0 to QOS_TBL_IDX_MAX)) */
} QosDscpRemapTbl_t;

/**
 * @brief This struct is used for the priority queue mapping table.
 * The port and the pri are inputs, and the que can be either an input or an output.
 */
typedef struct
{
    RtkPort   port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSPri pri;  /**< [in] Priority value (0 to QOS_PRI_MAX) */
    RtkQoSQue que;  /**< [in,out] Tx queue number (0 to QOS_QUE_MAX) */
} QosPriQueMapping_t;

/* Traffic Scheduling */
/**
 * @brief This struct is used for the queue scheduling information registers.
 * The schType, the schSel and schAlgo pri are outputs.
 */
typedef struct
{
    uint32 schType; /**< [out] QoS queue schedule type value (QOS_MINBKTSCKTYPE_OTHERS to QOS_MINBKTSCKTYPE_END) */
    uint32 schSel;  /**< [out] QoS queue schedule select value (QOS_SCHTYPESEL_WFQ to QOS_SCHTYPESEL_END) */
    uint32 schAlgo; /**< [out] QoS queue schedule algorithm value (<CHIPID>_QOS_DOT1QSCKALGTYPE_CBS to <CHIPID>_QOS_DOT1QSCKALGTYPE_END) */
} QosQueSchInfoReg_t;

/**
 * @brief This struct is used for the queue scheduling information.
 * The port and the que are inputs, and the queSch can be either an input or an output.
 */
typedef struct
{
    RtkPort   port;   /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSQue que;    /**< [in] Tx queue number (0 to QOS_QUE_MAX) */
    RtkQoSSch queSch; /**< [in,out] QoS/Qav queue scheduler value (<CHIPID>_QOS_SCH_SP to <CHIPID>_QOS_SCH_END) */
} QosQueSchInfo_t;

/**
 * @brief This struct is used for the queue wfq/wrr weight.
 * The port and the que are inputs, and the queWt can be either an input or an output.
 */
typedef struct
{
    RtkPort   port;  /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSQue que;   /**< [in] Tx queue number (0 to QOS_QUE_MAX) */
    RtkQoSWt  queWt; /**< [in,out] Weight value of WRR or WFQ (0 to QOS_WT_MAX) */
} QosQueWfqWrrWt_t;

/* Egress Bandwidth Control */
/**
 * @brief This struct is used for the egress port bandwidth control.
 * The port and the en are inputs, and the bw can be either an input or an output.
 */
typedef struct
{
    RtkPort   port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable en;   /**< [in,out] Enable status (DISABLED to ENABLED) */
    RtkQoSBw  bw;   /**< [in,out] Bandwidth of the port (0 to <CHIPID>_QOS_PORT_BW_MAX) */
} QosEgrBwCtrl_t;

/**
 * @brief This struct is used for the egress queue bandwidth control.
 * The port and the que and the en are inputs, and the bw can be either an input or an output.
 */
typedef struct
{
    RtkPort   port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSQue que;  /**< [in] Tx queue number (0 to QOS_QUE_MAX) */
    RtkEnable en;   /**< [in,out] Enable status (DISABLED to ENABLED) */
    RtkQoSBw  bw;   /**< [in,out] Rate of queue (0 to <CHIPID>_QOS_QUE_BW_MAX) */
} QosEgrQueBwCtrl_t;

/* Traffic Priority Remarking */
/**
 * @brief This struct is used for the cvaln remarking enable.
 * The port is an inputs, and the en can be either an input or an output.
 */
typedef struct
{
    RtkPort   port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable en;   /**< [in,out] Enable status (DISABLED to ENABLED) */
} QosCvlanRmkEn_t;

/**
 * @brief This struct is used for the svaln remarking enable.
 * The port is an inputs, and the en can be either an input or an output.
 */
typedef struct
{
    RtkPort   port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable en;   /**< [in,out] Enable status (DISABLED to ENABLED) */
} QosSvlanRmkEn_t;

/**
 * @brief This struct is used for the dscp remarking enable.
 * The port is an inputs, and the en can be either an input or an output.
 */
typedef struct
{
    RtkPort   port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkEnable en;   /**< [in,out] Enable status (DISABLED to ENABLED) */
} QosDscpRmkEn_t;

/**
 * @brief This struct is used for the cvaln remarking.
 * The idx and the oriPri are inputs, and the rmkPri and the rmkDei can be either an input or an output.
 */
typedef struct
{
    RtkQoSTblIdx idx;    /**< [in] Table index (0 to QOS_TBL_IDX_MAX) */
    RtkQoSPri    oriPri; /**< [in] Original priority value (0 to QOS_PRI_MAX) */
    RtkQoSPri    rmkPri; /**< [in,out] Remarked priority value (0 to QOS_PRI_MAX) */
    RtkQoSDei    rmkDei; /**< [in,out] Remarked dei value (0 to QOS_DEI_MAX) */
} QosCvlanRmk_t;

/**
 * @brief This struct is used for the svaln remarking.
 * The idx and the oriPri are inputs, and the rmkPri and the rmkDei can be either an input or an output.
 */
typedef struct
{
    RtkQoSTblIdx idx;    /**< [in] Table index (0 to QOS_TBL_IDX_MAX) */
    RtkQoSPri    oriPri; /**< [in] Original priority value (0 to QOS_PRI_MAX) */
    RtkQoSPri    rmkPri; /**< [in,out] Remarked priority value (0 to QOS_PRI_MAX) */
    RtkQoSDei    rmkDei; /**< [in,out] Remarked dei value (0 to QOS_DEI_MAX) */
} QosSvlanRmk_t;

/**
 * @brief This struct is used for the dscp remarking.
 * The idx and the oriPri are inputs, and the rmkDscp can be either an input or an output.
 */
typedef struct
{
    RtkQoSTblIdx idx;     /**< [in] Table index (0 to QOS_TBL_IDX_MAX) */
    RtkQoSPri    oriPri;  /**< [in] Original priority value (0 to QOS_PRI_MAX) */
    RtkQoSDscp   rmkDscp; /**< [in,out] Dscp value (0 to QOS_DSCP_MAX) */
} QosDscpRmk_t;

/**
 * @brief This struct is used for the cvaln remarking table.
 * The port is an inputs, and the idx can be either an input or an output.
 */
typedef struct
{
    RtkPort      port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSTblIdx idx;  /**< [in,out] Table index (0 to QOS_TBL_IDX_MAX) */
} QosCvlanRmkTbl_t;

/**
 * @brief This struct is used for the svaln remarking table.
 * The port is an inputs, and the idx can be either an input or an output.
 */
typedef struct
{
    RtkPort      port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSTblIdx idx;  /**< [in,out] Table index (0 to QOS_TBL_IDX_MAX) */
} QosSvlanRmkTbl_t;

/**
 * @brief This struct is used for the dscp remarking table.
 * The port is an inputs, and the idx can be either an input or an output.
 */
typedef struct
{
    RtkPort      port; /**< [in] Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    RtkQoSTblIdx idx;  /**< [in,out] Table index (0 to QOS_TBL_IDX_MAX) */
} QosDscpRmkTbl_t;

/******************************************************************************/
/* Function                                                                   */
/******************************************************************************/
/* Ingress Bandwidth Control */
EXTERN_API RtkApiRet rtk_qos_igr_bw_ctrl_set(UnitChip_t unitChip, const QosIgrBwCtrlInfo_t *qosIgrBwCtrlInfo);
EXTERN_API RtkApiRet rtk_qos_igr_bw_ctrl_get(UnitChip_t unitChip, QosIgrBwCtrlInfo_t *qosIgrBwCtrlInfo);
/* Traffic Classification */
EXTERN_API RtkApiRet rtk_qos_port_pri_set(UnitChip_t unitChip, const QosPortPri_t *qosPortPri);
EXTERN_API RtkApiRet rtk_qos_port_pri_get(UnitChip_t unitChip, QosPortPri_t *qosPortPri);
EXTERN_API RtkApiRet rtk_qos_pri_sel_set(UnitChip_t unitChip, const QosPriSelInfo_t *qosPriSelInfo);
EXTERN_API RtkApiRet rtk_qos_pri_sel_get(UnitChip_t unitChip, QosPriSelInfo_t *qosPriSelInfo);
EXTERN_API RtkApiRet rtk_qos_pri_wt_tbl_set(UnitChip_t unitChip, const QosPriWtTbl_t *qosPriWtTbl);
EXTERN_API RtkApiRet rtk_qos_pri_wt_tbl_get(UnitChip_t unitChip, QosPriWtTbl_t *qosPriWtTbl);
EXTERN_API RtkApiRet rtk_qos_cvlan_pri_remap_set(UnitChip_t unitChip, const QosCvlanPriRemap_t *qosCvlanPriRemap);
EXTERN_API RtkApiRet rtk_qos_cvlan_pri_remap_get(UnitChip_t unitChip, QosCvlanPriRemap_t *qosCvlanPriRemap);
EXTERN_API RtkApiRet rtk_qos_svlan_pri_remap_set(UnitChip_t unitChip, const QosSvlanPriRemap_t *qosSvlanPriRemap);
EXTERN_API RtkApiRet rtk_qos_svlan_pri_remap_get(UnitChip_t unitChip, QosSvlanPriRemap_t *qosSvlanPriRemap);
EXTERN_API RtkApiRet rtk_qos_dscp_pri_remap_set(UnitChip_t unitChip, const QosDscpPriRemap_t *qosDscpPriRemap);
EXTERN_API RtkApiRet rtk_qos_dscp_pri_remap_get(UnitChip_t unitChip, QosDscpPriRemap_t *qosDscpPriRemap);
EXTERN_API RtkApiRet rtk_qos_cvlan_remap_tbl_set(UnitChip_t unitChip, const QosCvlanRemapTbl_t *qosCvlanRemapTbl);
EXTERN_API RtkApiRet rtk_qos_cvlan_remap_tbl_get(UnitChip_t unitChip, QosCvlanRemapTbl_t *qosCvlanRemapTbl);
EXTERN_API RtkApiRet rtk_qos_svlan_remap_tbl_set(UnitChip_t unitChip, const QosSvlanRemapTbl_t *qosSvlanRemapTbl);
EXTERN_API RtkApiRet rtk_qos_svlan_remap_tbl_get(UnitChip_t unitChip, QosSvlanRemapTbl_t *qosSvlanRemapTbl);
EXTERN_API RtkApiRet rtk_qos_dscp_remap_tbl_set(UnitChip_t unitChip, const QosDscpRemapTbl_t *qosDscpRemapTbl);
EXTERN_API RtkApiRet rtk_qos_dscp_remap_tbl_get(UnitChip_t unitChip, QosDscpRemapTbl_t *qosDscpRemapTbl);
EXTERN_API RtkApiRet rtk_qos_pri_que_mapping_set(UnitChip_t unitChip, const QosPriQueMapping_t *qosPriQueMapping);
EXTERN_API RtkApiRet rtk_qos_pri_que_mapping_get(UnitChip_t unitChip, QosPriQueMapping_t *qosPriQueMapping);
/* Traffic Scheduling */
EXTERN_API RtkApiRet rtk_qos_que_sch_set(UnitChip_t unitChip, const QosQueSchInfo_t *qosQueSchInfo);
EXTERN_API RtkApiRet rtk_qos_que_sch_get(UnitChip_t unitChip, QosQueSchInfo_t *qosQueSchInfo);
EXTERN_API RtkApiRet rtk_qos_que_wfq_wrr_wt_set(UnitChip_t unitChip, const QosQueWfqWrrWt_t *qosQueWfqWrrWt);
EXTERN_API RtkApiRet rtk_qos_que_wfq_wrr_wt_get(UnitChip_t unitChip, QosQueWfqWrrWt_t *qosQueWfqWrrWt);
/* Egress Bandwidth Control */
EXTERN_API RtkApiRet rtk_qos_egr_bw_ctrl_set(UnitChip_t unitChip, const QosEgrBwCtrl_t *qosEgrBwCtrl);
EXTERN_API RtkApiRet rtk_qos_egr_bw_ctrl_get(UnitChip_t unitChip, QosEgrBwCtrl_t *qosEgrBwCtrl);
EXTERN_API RtkApiRet rtk_qos_egr_que_bw_ctrl_set(UnitChip_t unitChip, const QosEgrQueBwCtrl_t *qosEgrQueBwCtrl);
EXTERN_API RtkApiRet rtk_qos_egr_que_bw_ctrl_get(UnitChip_t unitChip, QosEgrQueBwCtrl_t *qosEgrQueBwCtrl);
/* Traffic Priority Remarking */
EXTERN_API RtkApiRet rtk_qos_cvlan_rmk_en_set(UnitChip_t unitChip, const QosCvlanRmkEn_t *qosCvlanRmkEn);
EXTERN_API RtkApiRet rtk_qos_cvlan_rmk_en_get(UnitChip_t unitChip, QosCvlanRmkEn_t *qosCvlanRmkEn);
EXTERN_API RtkApiRet rtk_qos_svlan_rmk_en_set(UnitChip_t unitChip, const QosSvlanRmkEn_t *qosSvlanRmkEn);
EXTERN_API RtkApiRet rtk_qos_svlan_rmk_en_get(UnitChip_t unitChip, QosSvlanRmkEn_t *qosSvlanRmkEn);
EXTERN_API RtkApiRet rtk_qos_dscp_rmk_en_set(UnitChip_t unitChip, const QosDscpRmkEn_t *qosDscpRmkEn);
EXTERN_API RtkApiRet rtk_qos_dscp_rmk_en_get(UnitChip_t unitChip, QosDscpRmkEn_t *qosDscpRmkEn);
EXTERN_API RtkApiRet rtk_qos_cvlan_rmk_set(UnitChip_t unitChip, const QosCvlanRmk_t *qosCvlanRmk);
EXTERN_API RtkApiRet rtk_qos_cvlan_rmk_get(UnitChip_t unitChip, QosCvlanRmk_t *qosCvlanRmk);
EXTERN_API RtkApiRet rtk_qos_svlan_rmk_set(UnitChip_t unitChip, const QosSvlanRmk_t *qosSvlanRmk);
EXTERN_API RtkApiRet rtk_qos_svlan_rmk_get(UnitChip_t unitChip, QosSvlanRmk_t *qosSvlanRmk);
EXTERN_API RtkApiRet rtk_qos_dscp_rmk_set(UnitChip_t unitChip, const QosDscpRmk_t *qosDscpRmk);
EXTERN_API RtkApiRet rtk_qos_dscp_rmk_get(UnitChip_t unitChip, QosDscpRmk_t *qosDscpRmk);
EXTERN_API RtkApiRet rtk_qos_cvlan_rmk_tbl_set(UnitChip_t unitChip, const QosCvlanRmkTbl_t *qosCvlanRmkTbl);
EXTERN_API RtkApiRet rtk_qos_cvlan_rmk_tbl_get(UnitChip_t unitChip, QosCvlanRmkTbl_t *qosCvlanRmkTbl);
EXTERN_API RtkApiRet rtk_qos_svlan_rmk_tbl_set(UnitChip_t unitChip, const QosSvlanRmkTbl_t *qosSvlanRmkTbl);
EXTERN_API RtkApiRet rtk_qos_svlan_rmk_tbl_get(UnitChip_t unitChip, QosSvlanRmkTbl_t *qosSvlanRmkTbl);
EXTERN_API RtkApiRet rtk_qos_dscp_rmk_tbl_set(UnitChip_t unitChip, const QosDscpRmkTbl_t *qosDscpRmkTbl);
EXTERN_API RtkApiRet rtk_qos_dscp_rmk_tbl_get(UnitChip_t unitChip, QosDscpRmkTbl_t *qosDscpRmkTbl);

#endif /* RTKAS_API_QOS_H__ */
