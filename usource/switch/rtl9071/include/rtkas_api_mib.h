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

#ifndef RTK_API_MIB_H__
#define RTK_API_MIB_H__

#define MIB_PORT_OFFSET  0x800u
#define MIB_RESET_MIN_US (1u)
#define MIB_RESET_MAX_US (600u)

typedef enum
{
    EM_PORTLINKDOWNDROP = 0,            /**< PortLinkDownDrop MIB Counter */
    EM_FLOWCTRLDROP,                    /**< FlowCtrlDrop MIB Counter */
    EM_TXDISABLEDROP,                   /**< TxDisableDrop MIB Counter */
    EM_MIRROREGRESSFILTERDROP,          /**< MirrorEgressFilterDrop MIB Counter */
    EM_LINKAGGRLINKDOWNDROP,            /**< LinkAggrLinkDownDrop MIB Counter */
    EM_CPUPORTSELFFILTERDROP,           /**< CPUPortSelfFilterDrop MIB Counter */
    EM_SYS_DMY_1,                       /**< Dummy */
    EM_PORTISOLATIONDROP,               /**< PortIsolationDrop MIB Counter */
    EM_SPANTREEEGRESSDROP,              /**< SpanTreeEgressDrop MIB Counter */
    EM_EGRESSVLANFILTERDROP,            /**< EgressVLANFilterDrop MIB Counter */
    EM_STORMSUPPRESSIONDROP,            /**< StormSuppressionDrop MIB Counter */
    EM_SOURCEPORTSELFFILTERDROP,        /**< SourcePortSelfFilterDrop MIB Counter */
    EM_MULTICASTEGRESSFILTERDROP,       /**< MulticastEgressFilterDrop MIB Counter */
    EM_MACCONSTRAINDROP,                /**< MACConstrainDrop MIB Counter */
    EM_SAMOVINGDROP,                    /**< SAMovingDrop MIB Counter */
    EM_SABLOCKDROP,                     /**< SABlockDrop MIB Counter */
    EM_L3NHAGEOUTDROP,                  /**< L3NHAgeoutDrop MIB Counter */
    EM_INVALIDSADROP,                   /**< InvalidSADrop MIB Counter */
    EM_LOOKUPMISSDROP,                  /**< LookupMissDrop MIB Counter */
    EM_DABLOCKDROP,                     /**< DABlockDrop MIB Counter */
    EM_L2MULTCASTDMACMULTICASIPDROP,    /**< L2MultcastDMACMulticasIPDrop MIB Counter */
    EM_L2UNICASTDMACMULTICASTIPDROP,    /**< L2UnicastDMACMulticastIPDrop MIB Counter */
    EM_TTLDROP,                         /**< TTLDrop MIB Counter */
    EM_IGMPMLDDROP,                     /**< IGMPMLDDrop MIB Counter */
    EM_SWITCHOWNMACDROP,                /**< SwitchOwnMACDrop MIB Counter */
    EM_SPANTREEINGRESSDROP,             /**< SpanTreeIngressDrop MIB Counter */
    EM_DOT1XEGRESSFILTERDROP,           /**< Dot1XEgressFilterDrop MIB Counter */
    EM_DOT1XINGRESSFILTERDROP,          /**< Dot1XIngressFilterDrop MIB Counter */
    EM_VLANINGRESSFILTERDROP,           /**< VLANIngressFilterDrop MIB Counter */
    EM_VLANTABLELOOKUPMISSDROP,         /**< VLANTableLookupMissDrop MIB Counter */
    EM_VLANACCEPTTAGDROP,               /**< VLANAcceptTagDrop MIB Counter */
    EM_SYS_DMY_3,                       /**< Dummy */
    EM_EGRESSACLCNTDISCARDDROP,         /**< Egress ACL count discard drop MIB Counter */
    EM_INGRESSACLCNTDISCARDDROP,        /**< Ingress ACL count discard drop MIB Counter */
    EM_EGRESSACLLOOKUPMISSDROP,         /**< Egress ACLLookupMissDrop MIB Counter */
    EM_EGRESSACLPOLICEDROP,             /**< Egress ACLPoliceDrop MIB Counter */
    EM_EGRESSACLDROPACTIONDROP,         /**< Egress ACLDropActionDrop MIB Counter */
    EM_EGRESSACLREDIRECTACTIONDROP,     /**< Egress ACLRedirectActionDrop MIB Counter */
    EM_INGRESSBANDWIDTHCTRLDROP,        /**< IngressBandwidthCtrlDrop MIB Counter */
    EM_INGRESSACLLOOKUPMISSDROP,        /**< Ingress ACLLookupMissDrop MIB Counter */
    EM_INGRESSACLPOLICEDROP,            /**< Ingress ACLPoliceDrop MIB Counter */
    EM_INGRESSACLDROPACTIONDROP,        /**< Ingress ACLDropActionDrop MIB Counter */
    EM_INGRESSACLREDIRECTACTIONDROP,    /**< Ingress ACLRedirectActionDrop MIB Counter */
    EM_RMADROP,                         /**< RMADrop MIB Counter */
    EM_SYS_DMY_10,                      /**< Dummy */
    EM_CFIDROP,                         /**< CFIDrop MIB Counter */
    EM_RLDPTXFRAMES,                    /**< RLDPTxFrames MIB Counter */
    EM_RLDPRXFRAMES,                    /**< RLDPRxFrames MIB Counter */
    EM_DOT1DTPLEARNEDENTRYDISCARDS,     /**< dot1dTpLearnedEntryDiscards MIB Counter */
    EM_ROUTE_MC_HDR_OPT,                /**< MC Header option extension */
    EM_ROUTE_MC_ILLEGAL_IP,             /**< MC illegal IP drop */
    EM_ROUTE_MC_DMAC_MISMATCH,          /**< MC DIP DMAC  mismatch drop */
    EM_ROUTE_MC_HDR_ERR,                /**< MC Header error drop */
    EM_ROUTE_UC_URPF,                   /**< UC uRPF drop */
    EM_ROUTE_UC_HDR_OPT,                /**< UC Header option extension drop */
    EM_ROUTE_UC_ILLEGAL_IP,             /**< UC illegal IP drop */
    EM_ROUTE_UC_DMAC_MISMATCH,          /**< UC DIP/DMAC mismatch drop */
    EM_ROUTE_UC_HDR_ERR,                /**< UC Header error drop */
    EM_ROUTE_UC_TTLHL,                  /**< UC TTL HL fail drop */
    EM_ROUTE_UC_ROUTE_DIS,              /**< UC route disable drop */
    EM_ROUTE_UC_DIP_ACT,                /**< UC hit action drop */
    EM_ROUTE_UC_LKMISS,                 /**< UC lookup miss */
    EM_QMAXSDU_P0Q0_DROP,               /**< Port0 Queue 0 Max SDU drop */
    EM_QMAXSDU_P0Q1_DROP,               /**< Port0 Queue 1 Max SDU drop */
    EM_QMAXSDU_P0Q2_DROP,               /**< Port0 Queue 2 Max SDU drop */
    EM_QMAXSDU_P0Q3_DROP,               /**< Port0 Queue 3 Max SDU drop */
    EM_QMAXSDU_P0Q4_DROP,               /**< Port0 Queue 4 Max SDU drop */
    EM_QMAXSDU_P0Q5_DROP,               /**< Port0 Queue 5 Max SDU drop */
    EM_QMAXSDU_P0Q6_DROP,               /**< Port0 Queue 6 Max SDU drop */
    EM_QMAXSDU_P0Q7_DROP,               /**< Port0 Queue 7 Max SDU drop */
    EM_QMAXSDU_P1Q0_DROP,               /**< Port1 Queue 0 Max SDU drop */
    EM_QMAXSDU_P1Q1_DROP,               /**< Port1 Queue 1 Max SDU drop */
    EM_QMAXSDU_P1Q2_DROP,               /**< Port1 Queue 2 Max SDU drop */
    EM_QMAXSDU_P1Q3_DROP,               /**< Port1 Queue 3 Max SDU drop */
    EM_QMAXSDU_P1Q4_DROP,               /**< Port1 Queue 4 Max SDU drop */
    EM_QMAXSDU_P1Q5_DROP,               /**< Port1 Queue 5 Max SDU drop */
    EM_QMAXSDU_P1Q6_DROP,               /**< Port1 Queue 6 Max SDU drop */
    EM_QMAXSDU_P1Q7_DROP,               /**< Port1 Queue 7 Max SDU drop */
    EM_QMAXSDU_P2Q0_DROP,               /**< Port2 Queue 0 Max SDU drop */
    EM_QMAXSDU_P2Q1_DROP,               /**< Port2 Queue 1 Max SDU drop */
    EM_QMAXSDU_P2Q2_DROP,               /**< Port2 Queue 2 Max SDU drop */
    EM_QMAXSDU_P2Q3_DROP,               /**< Port2 Queue 3 Max SDU drop */
    EM_QMAXSDU_P2Q4_DROP,               /**< Port2 Queue 4 Max SDU drop */
    EM_QMAXSDU_P2Q5_DROP,               /**< Port2 Queue 5 Max SDU drop */
    EM_QMAXSDU_P2Q6_DROP,               /**< Port2 Queue 6 Max SDU drop */
    EM_QMAXSDU_P2Q7_DROP,               /**< Port2 Queue 7 Max SDU drop */
    EM_QMAXSDU_P3Q0_DROP,               /**< Port3 Queue 0 Max SDU drop */
    EM_QMAXSDU_P3Q1_DROP,               /**< Port3 Queue 1 Max SDU drop */
    EM_QMAXSDU_P3Q2_DROP,               /**< Port3 Queue 2 Max SDU drop */
    EM_QMAXSDU_P3Q3_DROP,               /**< Port3 Queue 3 Max SDU drop */
    EM_QMAXSDU_P3Q4_DROP,               /**< Port3 Queue 4 Max SDU drop */
    EM_QMAXSDU_P3Q5_DROP,               /**< Port3 Queue 5 Max SDU drop */
    EM_QMAXSDU_P3Q6_DROP,               /**< Port3 Queue 6 Max SDU drop */
    EM_QMAXSDU_P3Q7_DROP,               /**< Port3 Queue 7 Max SDU drop */
    EM_QMAXSDU_P4Q0_DROP,               /**< Port4 Queue 0 Max SDU drop */
    EM_QMAXSDU_P4Q1_DROP,               /**< Port4 Queue 1 Max SDU drop */
    EM_QMAXSDU_P4Q2_DROP,               /**< Port4 Queue 2 Max SDU drop */
    EM_QMAXSDU_P4Q3_DROP,               /**< Port4 Queue 3 Max SDU drop */
    EM_QMAXSDU_P4Q4_DROP,               /**< Port4 Queue 4 Max SDU drop */
    EM_QMAXSDU_P4Q5_DROP,               /**< Port4 Queue 5 Max SDU drop */
    EM_QMAXSDU_P4Q6_DROP,               /**< Port4 Queue 6 Max SDU drop */
    EM_QMAXSDU_P4Q7_DROP,               /**< Port4 Queue 7 Max SDU drop */
    EM_QMAXSDU_P5Q0_DROP,               /**< Port5 Queue 0 Max SDU drop */
    EM_QMAXSDU_P5Q1_DROP,               /**< Port5 Queue 1 Max SDU drop */
    EM_QMAXSDU_P5Q2_DROP,               /**< Port5 Queue 2 Max SDU drop */
    EM_QMAXSDU_P5Q3_DROP,               /**< Port5 Queue 3 Max SDU drop */
    EM_QMAXSDU_P5Q4_DROP,               /**< Port5 Queue 4 Max SDU drop */
    EM_QMAXSDU_P5Q5_DROP,               /**< Port5 Queue 5 Max SDU drop */
    EM_QMAXSDU_P5Q6_DROP,               /**< Port5 Queue 6 Max SDU drop */
    EM_QMAXSDU_P5Q7_DROP,               /**< Port5 Queue 7 Max SDU drop */
    EM_QMAXSDU_P6Q0_DROP,               /**< Port6 Queue 0 Max SDU drop */
    EM_QMAXSDU_P6Q1_DROP,               /**< Port6 Queue 1 Max SDU drop */
    EM_QMAXSDU_P6Q2_DROP,               /**< Port6 Queue 2 Max SDU drop */
    EM_QMAXSDU_P6Q3_DROP,               /**< Port6 Queue 3 Max SDU drop */
    EM_QMAXSDU_P6Q4_DROP,               /**< Port6 Queue 4 Max SDU drop */
    EM_QMAXSDU_P6Q5_DROP,               /**< Port6 Queue 5 Max SDU drop */
    EM_QMAXSDU_P6Q6_DROP,               /**< Port6 Queue 6 Max SDU drop */
    EM_QMAXSDU_P6Q7_DROP,               /**< Port6 Queue 7 Max SDU drop */
    EM_QMAXSDU_P7Q0_DROP,               /**< Port7 Queue 0 Max SDU drop */
    EM_QMAXSDU_P7Q1_DROP,               /**< Port7 Queue 1 Max SDU drop */
    EM_QMAXSDU_P7Q2_DROP,               /**< Port7 Queue 2 Max SDU drop */
    EM_QMAXSDU_P7Q3_DROP,               /**< Port7 Queue 3 Max SDU drop */
    EM_QMAXSDU_P7Q4_DROP,               /**< Port7 Queue 4 Max SDU drop */
    EM_QMAXSDU_P7Q5_DROP,               /**< Port7 Queue 5 Max SDU drop */
    EM_QMAXSDU_P7Q6_DROP,               /**< Port7 Queue 6 Max SDU drop */
    EM_QMAXSDU_P7Q7_DROP,               /**< Port7 Queue 7 Max SDU drop */
    EM_QMAXSDU_P8Q0_DROP,               /**< Port8 Queue 0 Max SDU drop */
    EM_QMAXSDU_P8Q1_DROP,               /**< Port8 Queue 1 Max SDU drop */
    EM_QMAXSDU_P8Q2_DROP,               /**< Port8 Queue 2 Max SDU drop */
    EM_QMAXSDU_P8Q3_DROP,               /**< Port8 Queue 3 Max SDU drop */
    EM_QMAXSDU_P8Q4_DROP,               /**< Port8 Queue 4 Max SDU drop */
    EM_QMAXSDU_P8Q5_DROP,               /**< Port8 Queue 5 Max SDU drop */
    EM_QMAXSDU_P8Q6_DROP,               /**< Port8 Queue 6 Max SDU drop */
    EM_QMAXSDU_P8Q7_DROP,               /**< Port8 Queue 7 Max SDU drop */
    EM_QMAXSDU_P9Q0_DROP,               /**< Port9 Queue 0 Max SDU drop */
    EM_QMAXSDU_P9Q1_DROP,               /**< Port9 Queue 1 Max SDU drop */
    EM_QMAXSDU_P9Q2_DROP,               /**< Port9 Queue 2 Max SDU drop */
    EM_QMAXSDU_P9Q3_DROP,               /**< Port9 Queue 3 Max SDU drop */
    EM_QMAXSDU_P9Q4_DROP,               /**< Port9 Queue 4 Max SDU drop */
    EM_QMAXSDU_P9Q5_DROP,               /**< Port9 Queue 5 Max SDU drop */
    EM_QMAXSDU_P9Q6_DROP,               /**< Port9 Queue 6 Max SDU drop */
    EM_QMAXSDU_P9Q7_DROP,               /**< Port9 Queue 7 Max SDU drop */
    EM_QMAXSDU_P10Q0_DROP,              /**< Port10 Queue 0 Max SDU drop */
    EM_QMAXSDU_P10Q1_DROP,              /**< Port10 Queue 1 Max SDU drop */
    EM_QMAXSDU_P10Q2_DROP,              /**< Port10 Queue 2 Max SDU drop */
    EM_QMAXSDU_P10Q3_DROP,              /**< Port10 Queue 3 Max SDU drop */
    EM_QMAXSDU_P10Q4_DROP,              /**< Port10 Queue 4 Max SDU drop */
    EM_QMAXSDU_P10Q5_DROP,              /**< Port10 Queue 5 Max SDU drop */
    EM_QMAXSDU_P10Q6_DROP,              /**< Port10 Queue 6 Max SDU drop */
    EM_QMAXSDU_P10Q7_DROP,              /**< Port10 Queue 7 Max SDU drop */
    EM_QMAXSDU_P11Q0_DROP,              /**< Port11 Queue 0 Max SDU drop */
    EM_QMAXSDU_P11Q1_DROP,              /**< Port11 Queue 1 Max SDU drop */
    EM_QMAXSDU_P11Q2_DROP,              /**< Port11 Queue 2 Max SDU drop */
    EM_QMAXSDU_P11Q3_DROP,              /**< Port11 Queue 3 Max SDU drop */
    EM_QMAXSDU_P11Q4_DROP,              /**< Port11 Queue 4 Max SDU drop */
    EM_QMAXSDU_P11Q5_DROP,              /**< Port11 Queue 5 Max SDU drop */
    EM_QMAXSDU_P11Q6_DROP,              /**< Port11 Queue 6 Max SDU drop */
    EM_QMAXSDU_P11Q7_DROP,              /**< Port11 Queue 7 Max SDU drop */
    EM_QMAXSDU_P12Q0_DROP,              /**< Port12 Queue 0 Max SDU drop */
    EM_QMAXSDU_P12Q1_DROP,              /**< Port12 Queue 1 Max SDU drop */
    EM_QMAXSDU_P12Q2_DROP,              /**< Port12 Queue 2 Max SDU drop */
    EM_QMAXSDU_P12Q3_DROP,              /**< Port12 Queue 3 Max SDU drop */
    EM_QMAXSDU_P12Q4_DROP,              /**< Port12 Queue 4 Max SDU drop */
    EM_QMAXSDU_P12Q5_DROP,              /**< Port12 Queue 5 Max SDU drop */
    EM_QMAXSDU_P12Q6_DROP,              /**< Port12 Queue 6 Max SDU drop */
    EM_QMAXSDU_P12Q7_DROP,              /**< Port12 Queue 7 Max SDU drop */
    EM_QMAXSDU_P13Q0_DROP,              /**< Port13 Queue 0 Max SDU drop */
    EM_QMAXSDU_P13Q1_DROP,              /**< Port13 Queue 1 Max SDU drop */
    EM_QMAXSDU_P13Q2_DROP,              /**< Port13 Queue 2 Max SDU drop */
    EM_QMAXSDU_P13Q3_DROP,              /**< Port13 Queue 3 Max SDU drop */
    EM_QMAXSDU_P13Q4_DROP,              /**< Port13 Queue 4 Max SDU drop */
    EM_QMAXSDU_P13Q5_DROP,              /**< Port13 Queue 5 Max SDU drop */
    EM_QMAXSDU_P13Q6_DROP,              /**< Port13 Queue 6 Max SDU drop */
    EM_QMAXSDU_P13Q7_DROP,              /**< Port13 Queue 7 Max SDU drop */
    EM_QMAXSDU_P14Q0_DROP,              /**< Port14 Queue 0 Max SDU drop */
    EM_QMAXSDU_P14Q1_DROP,              /**< Port14 Queue 1 Max SDU drop */
    EM_QMAXSDU_P14Q2_DROP,              /**< Port14 Queue 2 Max SDU drop */
    EM_QMAXSDU_P14Q3_DROP,              /**< Port14 Queue 3 Max SDU drop */
    EM_QMAXSDU_P14Q4_DROP,              /**< Port14 Queue 4 Max SDU drop */
    EM_QMAXSDU_P14Q5_DROP,              /**< Port14 Queue 5 Max SDU drop */
    EM_QMAXSDU_P14Q6_DROP,              /**< Port14 Queue 6 Max SDU drop */
    EM_QMAXSDU_P14Q7_DROP,              /**< Port14 Queue 7 Max SDU drop */
    EM_QMAXSDU_P15Q0_DROP,              /**< Port15 Queue 0 Max SDU drop */
    EM_QMAXSDU_P15Q1_DROP,              /**< Port15 Queue 1 Max SDU drop */
    EM_QMAXSDU_P15Q2_DROP,              /**< Port15 Queue 2 Max SDU drop */
    EM_QMAXSDU_P15Q3_DROP,              /**< Port15 Queue 3 Max SDU drop */
    EM_QMAXSDU_P15Q4_DROP,              /**< Port15 Queue 4 Max SDU drop */
    EM_QMAXSDU_P15Q5_DROP,              /**< Port15 Queue 5 Max SDU drop */
    EM_QMAXSDU_P15Q6_DROP,              /**< Port15 Queue 6 Max SDU drop */
    EM_QMAXSDU_P15Q7_DROP,              /**< Port15 Queue 7 Max SDU drop */
    EM_IFOUTOCTETS_LSB = 1000,          /**< ifOutOctets MIB Counter [31:0] */
    EM_IFOUTOCTETS_MSB,                 /**< ifOutOctets MIB Counter [63:32] */
    EM_DOT1DTPPORTOUTFRAMES,            /**< dot1dTpPortOutFrames MIB Counter */
    EM_DOT3OUTPAUSEFRAMES,              /**< dot3OutPauseFrames MIB Counter */
    EM_IFOUTUCASTPKTS,                  /**< ifOutUcastPkts MIB Counter */
    EM_IFOUTMULTICASTPKTS,              /**< ifOutMulticastPkts MIB Counter */
    EM_IFOUTBROADCASTPKTS,              /**< ifOutBroadcastPkts MIB Counter */
    EM_DOT3STATSSINGLECOLLISIONFRAMES,  /**< dot3StatsSingleCollisionFrames MIB Counter */
    EM_DOT3STATMULTIPLECOLLISIONFRAMES, /**< dot3StatsMultipleCollisionFrames MIB Counter */
    EM_ETHERSTATSCOLLISIONS,            /**< etherStatsCollisions MIB Counter */
    EM_DOT3STATSDEFERREDTRANSMISSIONS,  /**< dot3StatsDeferredTransmissions MIB Counter */
    EM_DOT3STATSLATECOLLISIONS,         /**< dot3StatsLateCollisions MIB Counter */
    EM_DOT3STATSEXCESSIVECOLLISIONS,    /**< dot3StatsExcessiveCollisions MIB Counter */
    EM_TXPKTS,                          /**< TxPkt MIB Counter */
    EM_IFINOCTETS_LSB,                  /**< ifInOctets MIB Counter [31:0] */
    EM_IFINOCTETS_MSB,                  /**< ifInOctets MIB Counter [63:32] */
    EM_ETHERSTATSOCTETS_LSB,            /**< etherStatsOctets MIB Counter [31:0] */
    EM_ETHERSTATSOCTETS_MSB,            /**< etherStatsOctets MIB Counter [63:32] */
    EM_DOT1DTPPORTINFRAMES,             /**< dot1dTpPortInFrames MIB Counter */
    EM_DOT3INPAUSEFRAMES,               /**< dot3InPauseFrames MIB Counter */
    EM_IFINUCASTPKTS,                   /**< ifInUcastPkts MIB Counter */
    EM_ETHERSTATSMULTICASTPKTS,         /**< etherStatsMulticastPkts MIB Counter */
    EM_ETHERSTATSBROADCASTPKTS,         /**< etherStatsBroadcastPkts MIB Counter */
    EM_ETHERSTATSFRAGMENTS,             /**< etherStatsFragments MIB Counter */
    EM_RXPKTBELOW64,                    /**< RxPktBelow64 MIB Counter */
    EM_ETHERSTATSJABBERS,               /**< etherStatsJabbers MIB Counter */
    EM_DOT3STATSFRAMETOOLONGS,          /**< dot3StatsFrameTooLongs MIB Counter */
    EM_DOT3STATSFCSERRORS,              /**< dot3StatsFCSErrors MIB Counter */
    EM_DOT3STATSALIGNMENTERRORS,        /**< dot3StatsAlignmentErrors MIB Counter */
    EM_DOT3STATSSYMBOLERRORS,           /**< dot3StatsSymbolErrors MIB Counter */
    EM_DOT3CONTROLINUNKNOWNOPCODES,     /**< dot3ControlInUnknownOpcodes MIB Counter */
    EM_ETHERSTATSDROPEVENTS,            /**< etherStatsDropEvents MIB Counter */
    EM_DOT1DTPPORTINDISCATDS,           /**< dot1dTpPortInDiscards MIB Counter */
    EM_IFOUTDISCARDS,                   /**< ifOutDiscards MIB Counter */
    EM_ETHERSTATSPKTS64OCTETS,          /**< etherStatsPkts64Octets MIB Counter */
    EM_ETHERSTATSPKTS65TO127OCTETS,     /**< etherStatsPkts65to127Octets MIB Counter */
    EM_ETHERSTATSPKTS128TO255OCTETS,    /**< etherStatsPkts128to255Octets MIB Counter */
    EM_ETHERSTATSPKTS256TO511OCTETS,    /**< etherStatsPkts256to511Octets MIB Counter */
    EM_ETHERSTATSPKTS512TO1023OCTETS,   /**< etherStatsPkts512to1023Octets MIB Counter */
    EM_ETHERSTATSPKTS1024TO1518OCTETS,  /**< etherStatsPkts1024to1518Octets MIB Counter */
    EM_ETHERSTATSOVERSIZEPKTS,          /**< etherStatsOversizePkts MIB Counter */
    EM_ETHERSTATSUNDERSIZEPKTS,         /**< etherStatsUnderSizePkts MIB Counter */
    EM_ETHERSTATSPKTS,                  /**< etherStatsPkts MIB Counter */
    EM_ETHERSTATSCRCALIGNERRORS,        /**< etherStatsCRCAlignErrors MIB Counter */
    EM_PORTOUTUCASTPKTS,                /**< ALE port congestion dropped unicast packets MIB Counter */
    EM_PORTOUTMULTICASTPKTS,            /**< ALE port congestion dropped multicast packets MIB Counter */
    EM_PORTOUTBROADCASTPKTS,            /**< ALE port congestion dropped broadcast packets MIB Counter */
    EM_PORTOUTDISCARDS,                 /**< ALE port congestion dropped all packets MIB Counter */
    EM_ALEBWLIMDROPPKTCNT,              /**< ALEBWLimDropPktCnt MIB Counter */
    EM_MIB48PASS1DROPPKTCNT,            /**< 48pass1DropPktCnt MIB Counter */
    EM_IOLLENERRDROPPKTCNT,             /**< IOLLenErrDropPktCnt MIB Counter */
    EM_IOLMAXLENDROPPKTCNT,             /**< IOLMaxLenDropPktCnt MIB Counter */
    EM_FLOWCTRLONRXDROPPKTCNT,          /**< FlowCtrlOnRxDropPktCnt MIB Counter */
    EM_PNSHORTIFGRXDROPPKTCNT,          /**< The number of frames discarded due to short interframe gap */
    EM_EGRESSDROPPKTCNT,                /**< EgressDropPktCnt MIB Counter */
    EM_Q0TXPKTCNT,                      /**< Q0TxPktcnt MIB Counter */
    EM_Q1TXPKTCNT,                      /**< Q1TxPktcnt MIB Counter */
    EM_Q2TXPKTCNT,                      /**< Q2TxPktcnt MIB Counter */
    EM_Q3TXPKTCNT,                      /**< Q3TxPktcnt MIB Counter */
    EM_Q4TXPKTCNT,                      /**< Q4TxPktcnt MIB Counter */
    EM_Q5TXPKTCNT,                      /**< Q5TxPktcnt MIB Counter */
    EM_Q6TXPKTCNT,                      /**< Q6TxPktcnt MIB Counter */
    EM_Q7TXPKTCNT,                      /**< Q7TxPktcnt MIB Counter */
    EM_LINKDOWNTXDROPCNT,               /**< pcs link down tx drop MIB counter */
    EM_Q0TOVERRUNCNT_LSB,               /**< Q0 TransmissionOverrun MIB counter */
    EM_Q0TOVERRUNCNT_MSB,               /**< Q0 TransmissionOverrun MIB counter */
    EM_Q1TOVERRUNCNT_LSB,               /**< Q1 TransmissionOverrun MIB counter */
    EM_Q1TOVERRUNCNT_MSB,               /**< Q1 TransmissionOverrun MIB counter */
    EM_Q2TOVERRUNCNT_LSB,               /**< Q2 TransmissionOverrun MIB counter */
    EM_Q2TOVERRUNCNT_MSB,               /**< Q2 TransmissionOverrun MIB counter */
    EM_Q3TOVERRUNCNT_LSB,               /**< Q3 TransmissionOverrun MIB counter */
    EM_Q3TOVERRUNCNT_MSB,               /**< Q3 TransmissionOverrun MIB counter */
    EM_Q4TOVERRUNCNT_LSB,               /**< Q4 TransmissionOverrun MIB counter */
    EM_Q4TOVERRUNCNT_MSB,               /**< Q4 TransmissionOverrun MIB counter */
    EM_Q5TOVERRUNCNT_LSB,               /**< Q5 TransmissionOverrun MIB counter */
    EM_Q5TOVERRUNCNT_MSB,               /**< Q5 TransmissionOverrun MIB counter */
    EM_Q6TOVERRUNCNT_LSB,               /**< Q6 TransmissionOverrun MIB counter */
    EM_Q6TOVERRUNCNT_MSB,               /**< Q6 TransmissionOverrun MIB counter */
    EM_Q7TOVERRUNCNT_LSB,               /**< Q7 TransmissionOverrun MIB counter */
    EM_Q7TOVERRUNCNT_MSB,               /**< Q7 TransmissionOverrun MIB counter */
    EM_STCONFIGCHGERRCNT_LSB,           /**< STConfigChangeError MIB counter */
    EM_STCONFIGCHGERRCNT_MSB,           /**< STConfigChangeError MIB counter */
    EM_TXOCTETS_LSB,                    /**< Routing EPP packet byte counter */
    EM_TXOCTETS_MSB,                    /**< Routing EPP packet byte counter */
    EM_TXUNICASTPKTS,                   /**< Routing EPP unicast pkcket counter */
    EM_TXMULTICASTPKTS,                 /**< Routing EPP multicast packet counter */
    EM_ROUTE_MC_RPF_DROP,               /**< Routing EPP MC RPF drop */
    EM_ROUTE_MC_MTU_FAIL_DROP,          /**< Routing EPP MC MTU fail drop */
    EM_ROUTE_MC_TTLHL_FAIL_DROP,        /**< Routing EPP MC TTL HL fail drop */
    EM_ROUTE_UC_ICMP_REDIR_DROP,        /**< Routing EPP UC ICMP redir drop */
    EM_ROUTE_UC_MTU_DROP,               /**< Routing EPP UC MTU fail drop */
    EM_IPV6_VERSION_ERR_DROP,           /**< ipv6 version error drop */
    EM_IPV4_VERSION_ERR_DROP,           /**< ipv4 version error drop */
    EM_IPV4_LENGTH_ERR_DROP,            /**< ipv4 length error drop */
    EM_IPV4_CHKSUM_ERR_DROP,            /**< ipv4 checksum error drop */
    EM_IPV4_UNDER_SIZE_DROP,            /**< ipv4 header under size drop */
    EM_WIRE_FRAMELEN_ERROR_DROP = 1103, /**< wire frame length error drop counter */
    EM_SOMEIP_DOIP_WHITELIST_HIT0,      /**< SomeIP/DoIP Whitelist0  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT1,      /**< SomeIP/DoIP Whitelist1  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT2,      /**< SomeIP/DoIP Whitelist2  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT3,      /**< SomeIP/DoIP Whitelist3  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT4,      /**< SomeIP/DoIP Whitelist4  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT5,      /**< SomeIP/DoIP Whitelist5  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT6,      /**< SomeIP/DoIP Whitelist6  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT7,      /**< SomeIP/DoIP Whitelist7  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT8,      /**< SomeIP/DoIP Whitelist8  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT9,      /**< SomeIP/DoIP Whitelist9  hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT10,     /**< SomeIP/DoIP Whitelist10 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT11,     /**< SomeIP/DoIP Whitelist11 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT12,     /**< SomeIP/DoIP Whitelist12 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT13,     /**< SomeIP/DoIP Whitelist13 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT14,     /**< SomeIP/DoIP Whitelist14 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT15,     /**< SomeIP/DoIP Whitelist15 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT16,     /**< SomeIP/DoIP Whitelist16 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT17,     /**< SomeIP/DoIP Whitelist17 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT18,     /**< SomeIP/DoIP Whitelist18 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT19,     /**< SomeIP/DoIP Whitelist19 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT20,     /**< SomeIP/DoIP Whitelist20 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT21,     /**< SomeIP/DoIP Whitelist21 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT22,     /**< SomeIP/DoIP Whitelist22 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT23,     /**< SomeIP/DoIP Whitelist23 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT24,     /**< SomeIP/DoIP Whitelist24 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT25,     /**< SomeIP/DoIP Whitelist25 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT26,     /**< SomeIP/DoIP Whitelist26 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT27,     /**< SomeIP/DoIP Whitelist27 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT28,     /**< SomeIP/DoIP Whitelist28 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT29,     /**< SomeIP/DoIP Whitelist29 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT30,     /**< SomeIP/DoIP Whitelist30 hit count */
    EM_SOMEIP_DOIP_WHITELIST_HIT31,     /**< SomeIP/DoIP Whitelist31 hit count */
} RtkMibCounterType_e;

typedef enum
{
    EM_RATE_PERIOD_HALF_SECOND = 0, /**< calculate the rate every 0.5 second */
    EM_RATE_PERIOD_1_SECOND,        /**< calculate the rate every 1 second */
    EM_RATE_PERIOD_2_SECOND,        /**< calculate the rate every 2 seconds */
    EM_RATE_PERIOD_END
} RtkMibRatePeriod_e;

/**
 * @brief This struct is used for getting ALE drop reason.
 */
typedef struct
{
    uint32 dropReason; /**< [out] Indicate the drop reason in ALE stage. (0 to 255)\n
                        * 7: Rx Disable \n
                        * 8: Packet parser IDEI(CFI) \n
                        * 9: LA LACP packet \n
                        * 10: LA marker packet \n
                        * 11: RMA \n
                        * 12: ACL redirect drop/trap \n
                        * 13: ACL drop to CPU action \n
                        * 14: ACL policer drop \n
                        * 15: ACL lookup miss \n
                        * 16: Ingress bandwidth control \n
                        * 17: SNMP trap/drop/copy \n
                        * 26: Accept frame tag type \n
                        * 27: VLAN table lookup miss \n
                        * 28: VLAN ingress filter \n
                        * 29: Dot1X ingress check \n
                        * 30: Dot1X egress filter (MAC/Port) \n
                        * 31: Spanning tree ingress filter \n
                        * 32: DSTSWMACCHK(switch DA check) \n
                        * 34: IGMP/MLD/ARP \n
                        * 35: TTL drop \n
                        * 36: L2UCIPMC \n
                        * 37: L2MCIPMC \n
                        * 38: DA block \n
                        * 39: LKMISS(L2\L3, unicast\multicast\broadcast) \n
                        * 40: invalid SA (including DA == SA) \n
                        * 41: Forwarding table PM empty \n
                        * 42: SA block \n
                        * 43: SA moving (legal/illegal) \n
                        * 44: MAC constrain \n
                        * 46: Multicast egress filter (router port/multidisport) \n
                        * 47: Source port self filter \n
                        * 48: storm suppression \n
                        * 49: Egress VLAN filter \n
                        * 50: Egress SPT filter \n
                        * 52: Port isolation \n
                        * 53: CPU port self filter \n
                        * 54: Link aggregation member port change \n
                        * 56: Mirror egress filter \n
                        * 57: Tx disable \n
                        * 58: Flow control \n
                        * 59: Port link down
                        * 71: IPV4 UC ZERO SIP ACTION DROP \n
                        * 74: IPV6 UC ZERO SIP ACTION DROP \n
                        * 92: IPV4 MC HDR ERR ACTION DROP \n
                        * 94: IPV6 MC HDR ERR ACTION DROP \n
                        * 95: IPV4 MC BAD SIP ACTION DROP \n
                        * 97: IPV4 MC ZERO SIP ACTION DROP \n
                        * 99: IPV6 MC BAD SIP ACTION DROP \n
                        * 101: IPV6 MCZERO SIP ACTION DROP \n
                        * 104: IPV4 MC DMAC MISMATH ACTION DROP \n
                        * 106: IPV6 MC DMAC MISMATH ACTION DROP \n
                        * 107: IPV4 MC HDR OPT ACTION DROP \n
                        * 110: IPV6 MC HBH ERR ACTION DROP \n
                        * 113: IPV6 MC HBH ACTION DROP \n
                        * 116: IPV6 MC ROUTE ACTION DROP \n
                        * 120: IPV4 MC ROUTE LU MISS ACTION DROP \n
                        * 122: IPV6 MC ROUTE LU MISS ACTION DROP \n
                        * 123: UNICAST ROUTE DISABLE \n
                        * 129: L3 Unicast lookup miss drop \n
                        * 130: IPv4 TTL check drop \n
                        * 131: IPv6 HL check drop \n
                        * 133: Non IP drop \n
                        * 134: IVID != Ingress table INTFID \n
                        * 135: max sdu drop: the frame exceeds the stream max sdu. \n
                        * 136: gate close drop: the frame is received when stream gate state is close \n
                        * 137: exceed interval octet drop: the frame bandwidth exceeds the interval octect max. \n
                        * 138: yellow frame drop: the flow meter judge this is yellow frame and drop on yellow is enabled. \n
                        * 139: red frame drop: the flow meter judges this is red frame. \n
                        * 140: Inactive flow meter drop: the frame is discarded because assigned to a inactive flow meter \n
                        * 141: ACL TCAM error handling \n
                        * 142: IP Routing TCAM error handling \n
                        * 143: Qci TCAM error handling \n
                        * 144: EACL redirect drop \n
                        * 145: EACL drop \n
                        * 146: EACL policer drop \n
                        * 147: EACL lookup miss drop \n
                        * 149: Invalid SA detection \n
                        * 150: ACL_TIMER_DISCARD \n
                        * 151: Dot1X egress filter \n
                        * 152: L3_NH_AGEOUT_ACT \n
                        * 153: CB drop \n
                        * 154: ACL abnormal drop \n
                        * 155: EACL abnormal drop \n
                        * 156: EACL_TIMER_DISCARD \n
                        * Others: Reserved
                        */
} StatGetAleDropReason_t;

/**
 * @brief This struct is used for getting maximum queue level.
 */
typedef struct
{
    RtkPort port;          /**< [in] Indicate the port ID for getting max queue level (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    uint32  maxQueueLevel; /**< [out] Indicate the max queue level of the specified port ID. (0 to 2047)*/
} StatPortMaxQueueLevelGet_t;

/**
 * @brief This struct is used for specifying port for resetting/starting/stopping port counter.
 */
typedef struct
{
    RtkPort port; /**< [in] Indicate the port ID for resetting/starting/stopping port counter. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
} StatPortConfig_t;

/**
 * @brief This struct is used for getting port counter value.
 */
typedef struct
{
    RtkPort             port;         /**< [in] Indicate the port ID for getting port counter. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkMibCounterType_e counterType;  /**< [in] Indicate the counter type for getting counter value. (N/A) */
    uint32              counterValue; /**< [out] Indicate the counter value of specified counter type. (0 to 4294967295) */
} StatPortGet_t;

/**
 * @brief This struct is used for getting port RX drop reason.
 */
typedef struct
{
    RtkPort port;       /**< [in] Indicate the port ID for getting drop reason. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    uint32  dropReason; /**< [out] Indicate the port drop reason. (0 to 31)\n
                         * 1: enrx = 0, rx enable reg is off \n
                         * 2: stop_ale = 1, ale test enable reg(en_aletest) is on \n
                         * 3: sys_flood happened, pkt buffer over the system flow control threshold \n
                         * 4: crc error \n
                         * 5: symbol error \n
                         * 6: alignment error \n
                         * 7: pkt is less than 64 bytes \n
                         * 8: pkt is over max length \n
                         * 9: received the pause pkt \n
                         * 10: iol drop, iol pause function, including unknown op code, unknown da, unknown ether type \n
                         * 11: iol length error \n
                         * 12: iol max length \n
                         * 13: over 48 times conflicts, pass pkt is over 1536 bytes(12 pages) \n
                         * 14: attack prevention drop \n
                         * 15: wol drop \n
                         * 16: packet pass, no drop \n
                         * 17: vlan hopping \n
                         * 19: packet drop by port relay disable \n
                         * 20: ALE bandwidth full drop \n
                         * 21: wire length error drop\n
                         * Others: Reserved
                         */
} StatPortGetRxDropReason_t;

/**
 * @brief This struct is used for port rate meter configuration.
 */
typedef struct
{
    RtkPort            port;       /**< [in] Indicate the port ID for setting or getting port rate configuration. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    RtkEnable          state;      /**< [in, out] Indicate the state of enabling rate meter. (DISABLED, ENABLED) */
    RtkMibRatePeriod_e ratePeriod; /**< [in, out] Indicate the period for calculating RX and TX rate. (N/A) */
} StatPortRateConfig_t;

/**
 * @brief This struct is used for getting port current rate.
 */
typedef struct
{
    RtkPort port;   /**< [in] Indicate the port ID for getting current port rate. (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
    uint32  rxRate; /**< [out] Indicate the RX rate in bytes. (0 to 4294967295) */
    uint32  txRate; /**< [out] Indicate the TX rate in bytes. (0 to 4294967295) */
} StatPortRateGet_t;

EXTERN_API RtkApiRet rtk_stat_reset_all(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_stat_start_all(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_stat_stop_all(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_stat_get_ale_drop_reason(UnitChip_t unitChip, StatGetAleDropReason_t *statGetAleDropReason);
EXTERN_API RtkApiRet rtk_stat_port_max_queue_level_get(UnitChip_t unitChip, StatPortMaxQueueLevelGet_t *statPortMaxQueueLevelGet);
EXTERN_API RtkApiRet rtk_stat_port_reset(UnitChip_t unitChip, StatPortConfig_t *statPortReset);
EXTERN_API RtkApiRet rtk_stat_port_start(UnitChip_t unitChip, StatPortConfig_t *statPortStart);
EXTERN_API RtkApiRet rtk_stat_port_stop(UnitChip_t unitChip, StatPortConfig_t *statPortStop);
EXTERN_API RtkApiRet rtk_stat_port_get(UnitChip_t unitChip, StatPortGet_t *statPortGet);
EXTERN_API RtkApiRet rtk_stat_port_get_rx_drop_reason(UnitChip_t unitChip, StatPortGetRxDropReason_t *statPortGetRxDropReason);
EXTERN_API RtkApiRet rtk_stat_port_rate_config_set(UnitChip_t unitChip, StatPortRateConfig_t *statPortRateConfigSet);
EXTERN_API RtkApiRet rtk_stat_port_rate_config_get(UnitChip_t unitChip, StatPortRateConfig_t *statPortRateConfigGet);
EXTERN_API RtkApiRet rtk_stat_port_rate_get(UnitChip_t unitChip, StatPortRateGet_t *statPortRateGet);
#endif /* RTK_API_MIB_H__ */
