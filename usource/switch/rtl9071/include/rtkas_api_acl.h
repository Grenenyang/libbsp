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

#ifndef RTKAS_API_ACL_H__
#define RTKAS_API_ACL_H__

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/* Table Structures */
/********* PIE *********/
typedef struct
{
    uint16 data0;  /**< PIE data0  field (0 to 0xFFFF) */
    uint16 data1;  /**< PIE data1  field (0 to 0xFFFF) */
    uint16 data2;  /**< PIE data2  field (0 to 0xFFFF) */
    uint16 data3;  /**< PIE data3  field (0 to 0xFFFF) */
    uint16 data4;  /**< PIE data4  field (0 to 0xFFFF) */
    uint16 data5;  /**< PIE data5  field (0 to 0xFFFF) */
    uint16 data6;  /**< PIE data6  field (0 to 0xFFFF) */
    uint16 data7;  /**< PIE data7  field (0 to 0xFFFF) */
    uint16 data8;  /**< PIE data8  field (0 to 0xFFFF) */
    uint16 data9;  /**< PIE data9  field (0 to 0xFFFF) */
    uint16 data10; /**< PIE data10 field (0 to 0xFFFF) */
    uint16 data11; /**< PIE data11 field (0 to 0xFFFF) */
    uint32 valid;  /**< PIE valid bit (0 to 1) */
    uint16 mask0;  /**< PIE mask0  field (0 to 0xFFFF) */
    uint16 mask1;  /**< PIE mask1  field (0 to 0xFFFF) */
    uint16 mask2;  /**< PIE mask2  field (0 to 0xFFFF) */
    uint16 mask3;  /**< PIE mask3  field (0 to 0xFFFF) */
    uint16 mask4;  /**< PIE mask4  field (0 to 0xFFFF) */
    uint16 mask5;  /**< PIE mask5  field (0 to 0xFFFF) */
    uint16 mask6;  /**< PIE mask6  field (0 to 0xFFFF) */
    uint16 mask7;  /**< PIE mask7  field (0 to 0xFFFF) */
    uint16 mask8;  /**< PIE mask8  field (0 to 0xFFFF) */
    uint16 mask9;  /**< PIE mask9  field (0 to 0xFFFF) */
    uint16 mask10; /**< PIE mask10 field (0 to 0xFFFF) */
    uint16 mask11; /**< PIE mask11 field (0 to 0xFFFF) */

} AclTblAccessPieParam_t;

/********* Action *********/
typedef struct
{
    uint32 withdraw;   /**< withdraw outer VLAN tag operation (0 to 1) */
    uint32 outVidCtl;  /**< outer VLAN control (0 to 3): 00: reserved (0); 01: provide new outer VLAN ID as OutVIDInfo (1); 10: packet's new outer VLAN ID = packet's outer tag VLAN ID - OutVIDInfo (2); 11: packet's new outer VLAN ID = packet's outer tag VLAN ID + OutVIDInfo (3) */
    uint32 outVidInfo; /**< outer VLAN infomation (0 to 0xFFF): outVidCtl = 0, ignore; outVidCtl = 1, new outer VLAN ID value; outVidCtl = 2, offset (-); outVidCtl = 3, offset (+) */
    uint32 outTagOp;   /**< outer VLAN operation (0 to 3): 00: packet sent without outer VLAN tag (0); 01: packet sent with modified outer VLAN tag (1); 10: packet sent with original outer VLAN tag (before ACL modify), but priority could be changed (2); 11: Nop (3) */

} AclTblAccessOtagOpParam_t;

typedef struct
{
    uint32 withdraw;  /**< withdraw inner VLAN tag operation (0 to 1) */
    uint32 inVidCtl;  /**< inner VLAN control (0 to 3): 00: reserved (0); 01: provide new inner VLAN ID as inVidInfo (1); 10: packet's new inner VLAN ID = packet's inner tag VLAN ID - inVidInfo (2); 11: packet's new inner VLAN ID = packet's inner tag VLAN ID + inVidInfo (3) */
    uint32 inVidInfo; /**< inner VLAN infomation (0 to 0xFFF): inVidCtl = 0, ignore; inVidCtl = 1, new inner VLAN ID value; inVidCtl = 2, offset (-); inVidCtl = 3, offset (+) */
    uint32 inTagOp;   /**< inner VLAN operation (0 to 3): 00: packet sent without inner VLAN tag (0); 01: packet sent with modified inner VLAN tag (1); 10: packet sent with original inner VLAN tag (before ACL modify), but priority could be changed (2); 11: Nop (3) */

} AclTblAccessIntagOpParam_t;

typedef struct
{
    uint32 withdraw; /**< withdraw vlan priority remarking operation (0 to 1) */
    uint32 tagSel;   /**< tag selected (0 to 0xF): b'xxx1: remark inner priority; b'xx1x: remark outer priority; b'x1xx: remark outer DEI; b'1xxx: remark inner DEI */
    uint32 inPri;    /**< tagSel = b'xxx1, remark inner priority (0 to 7) */
    uint32 outPri;   /**< tagSel = b'xx1x, remark outer priority (0 to 7) */
    uint32 odei;     /**< tagSel = b'x1xx, remark outer DEI (0 to 1) */
    uint32 idei;     /**< tagSel = b'1xxx, remark inner DEI (0 to 1) */

} AclTblAccessVlanPriorityRmkParam_t;

typedef struct
{
    uint32  withdraw;    /**< withdraw redirection action (0 to 1) */
    uint32  opcode;      /**< operation code of the redirection (0 to 3): 00: normal forwarding (0); 01: trap (1); 10: copy to cpu (2); 11: Nop (3) */
    RtkPmap dpm;         /**< destination port mask, dmp 0 means drop (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */
    uint32  bypassLaLut; /**< bypass link aggregation (0 to 1) */
    uint32  crsvlan;     /**< cross ingress/egress vlan filter (0 to 1): 0: forward in VLAN; 1: cross VLAN filter */

} AclTblAccessRedirectParam_t;

typedef struct
{
    uint32  withdraw;    /**< withdraw redirection action (0 to 1) */
    uint32  opcode;      /**< operation code of the redirection (0 to 3): 00: normal forwarding (0, according to dpm); 01: trap (1, according to dpm); 10: recovery dpm & operation selection (2, according to recDpm & actOpSel); 11: reserve (3) */
    RtkPmap dpm;         /**< destination port mask, dmp 0 means drop (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */
    uint32  bypassLaLut; /**< bypass link aggregation (0 to 1) */
    uint32  recDpm;      /**< recovery dpm (0 to 3), this field is valid, when opcode is 2: 00: DPM from PPI3 (0, EACL input); 01: DPM from PPI2 (1, IACL output); 10: DPM from PPI1 (2, L2 LUT output); 11: reserved (3, DPM = 0) */
    uint32  actOpSel;    /**< action operation select (0 to 3), this field is valid, when opcode is 2: 00: output ingress DPM (0, according to "recDpm" setting); 01: output egress DPM (1, according to "dpm" setting); 10: output ingress DPM & egress DPM (2); 11: output ingress DPM | egress DPM (3) */

} AclTblAccessEgrRedirectParam_t;

typedef struct
{
    uint32 withdraw;      /**< withdraw dscp remark operation (0 to 1) */
    uint32 opcode;        /**< operation code of the dscp remark (0 to 3): 00: DSCP remarking (0); 01: modify IP precedence (1); 10: modify DTR bits (2); 11: Nop (3) */
    uint32 dscpIpPrecDtr; /**< dscp remark options: opcode = 0, define ACL-based DSCP (0 to 63); opcode = 1, modify IP precedence (0 to 7); opcode = 2, modify DTR bits (0 to 7) */

} AclTblAccessDscpRmkParam_t;

typedef struct
{
    uint32 withdraw; /**< withdraw forward priority assignment operation (0 to 1) */
    uint32 priority; /**< assign Flow-based priority (0 to 7) */

} AclTblAccessFwdPriorityParam_t;

typedef struct
{
    uint32 withdraw; /**< withdraw L3 routing unicast operation (0 to 1) */
    uint32 nhIdx;    /**< This field indicates an index to the next-hop entry, but this is internal use, no more guideline here. (0 to 4095) */

} AclTblAccessUcParam_t;

typedef struct
{
    uint32  withdraw; /**< withdraw L3 routing unicast operation (0 to 1) */
    uint32  rhIdx;    /**< This field indicates an index to the replication header table entry, but this is internal use, no more guideline here. (0 to 4095) */
    RtkPmap eppDpm;   /**< The port mask which join this multicast group, but this is internal use, no more guideline here. (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */

} AclTblAccessMcParam_t;

typedef struct
{
    uint32 fno;          /**< flow number (0 to 1) */
    uint32 drop;         /**< drop ation, 00b: permit; 01b: drop; 10b: withdraw drop; 11b: reserved (0 to 3) */
    uint32 copytocpu;    /**< CPU port mask when packets copied (0 to 1) */
    uint32 mirror1;      /**< ACL Mirror1 action (0 to 1) */
    uint32 mirror0;      /**< ACL Mirror0 action (0 to 1) */
    uint32 otag;         /**< outer tag operation (0 to 1) */
    uint32 itag;         /**< inner tag operation (0 to 1) */
    uint32 priormk;      /**< priority/DEI remarking (0 to 1) */
    uint32 redir;        /**< redirection (0 to 1) */
    uint32 dscprmk;      /**< DSCP remarking (0 to 1) */
    uint32 prioasn;      /**< forwarding priority (tx queue) assignment (0 to 1) */
    uint32 byratelmt;    /**< bypass ingress rate limit (0 to 1) */
    uint32 uc;           /**< routing unicast replacement (0 to 1) */
    uint32 mc;           /**< routing multicast replacement (0 to 1) */
    uint32 timerDiscard; /**< timer discard (0 to 1) */
    uint32 headerDump;   /**< header dump (0 to 1) */

    uint32 copyCpuDpm; /**< CPU port mask, please note that this parameter is set per PIE, not per entry. Entries within the same PIE (e.g., index 0 - 15) will all refer to the same value (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */

    AclTblAccessOtagOpParam_t          outTagOpInfo; /**< AIF information of outer tag remark action */
    AclTblAccessIntagOpParam_t         inTagOpInfo;  /**< AIF information of inner tag remark action */
    AclTblAccessVlanPriorityRmkParam_t priRmkInfo;   /**< AIF information of vlan priority remark action */
    AclTblAccessRedirectParam_t        redirInfo;    /**< AIF information of redirection action */
    AclTblAccessDscpRmkParam_t         dscpRmkInfo;  /**< AIF information of dscp remark action */
    AclTblAccessFwdPriorityParam_t     prioInfo;     /**< AIF information of forward priority remark action */
    AclTblAccessUcParam_t              ucInfo;       /**< AIF information of uc action */
    AclTblAccessMcParam_t              mcInfo;       /**< AIF information of mc action */

} AclTblAccessActParam_t;

typedef struct
{
    uint32 drop;         /**< drop ation, 00b: permit; 01b: drop; 10b: withdraw drop; 11b: reserved (0 to 3) */
    uint32 copytocpu;    /**< CPU port mask when packets copied (0 to 1) */
    uint32 mirror1;      /**< ACL Mirror1 action (0 to 1) */
    uint32 mirror0;      /**< ACL Mirror0 action (0 to 1) */
    uint32 otag;         /**< outer tag operation (0 to 1) */
    uint32 itag;         /**< inner tag operation (0 to 1) */
    uint32 priormk;      /**< priority/DEI remarking (0 to 1) */
    uint32 redir;        /**< redirection (0 to 1) */
    uint32 dscprmk;      /**< DSCP remarking (0 to 1) */
    uint32 prioasn;      /**< forwarding priority (tx queue) assignment (0 to 1) */
    uint32 timerDiscard; /**< timer discard (0 to 1) */
    uint32 headerDump;   /**< header dump (0 to 1) */

    uint32 copyCpuDpm; /**< CPU port mask, please note that this parameter is set per PIE, not per entry. Entries within the same PIE (e.g., index 0 - 15) will all refer to the same value (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */

    AclTblAccessOtagOpParam_t          outTagOpInfo; /**< AIF information of outer tag remark action */
    AclTblAccessIntagOpParam_t         inTagOpInfo;  /**< AIF information of inner tag remark action */
    AclTblAccessVlanPriorityRmkParam_t priRmkInfo;   /**< AIF information of vlan priority remark action */
    AclTblAccessEgrRedirectParam_t     egrRedirInfo; /**< AIF information of redirection action */
    AclTblAccessDscpRmkParam_t         dscpRmkInfo;  /**< AIF information of dscp remark action */
    AclTblAccessFwdPriorityParam_t     prioInfo;     /**< AIF information of forward priority remark action */

} EaclTblAccessActParam_t;

/********* Counter *********/
typedef struct
{
    uint32 counterL; /**< LSB 32-bit of counter (0 to 0xFFFFFFFF) */
    uint32 counterH; /**< MSB 16-bit of counter (0 to 0xFFFF) */
    uint32 cntmode;  /**< counter mode, 0: packet-based counter; 1: byte-based counter (0 to 1) */
    uint32 enable;   /**< enable/disable counter function, 0: disable, 1: enable (0 to 1) */

} AclTblAccessCounterParam_t;

/********* Policer *********/
typedef struct
{
    uint32       rate;      /**< Rate in units of 8Kbps (1 to 0x3FFFFF) */
    AclThreshold threshold; /**< burst size, in units of 8Kbytes (0 to 0xFF) */
    uint32       tokencnt;  /**< leaky bucket used token counter, in unit of 1byte (0 to 0x1FFFFF) */
    uint32       enable;    /**< enable/disable policer function, 0: disable, 1: enable (0 to 1) */

} AclTblAccessPolicerParam_t;

/********* Abnormal Times *********/
typedef struct
{
    uint8        enable;    /**< enable abnormal function (0 to 1) */
    uint32       counter;   /**< abnormal counter value (0 to 0xFFFF) */
    AclThreshold threshold; /**< abnormal counter threshold (0 to 0xFFFF) */

} AclTblAccessAbnormalTimesParam_t;

/********* Timer Discard *********/
typedef struct
{
    uint8  withdraw;        /**< withdraw timer discard action (0 to 1) */
    uint8  monitorRun;      /**< monitor indication, 0: monitor timer is not running, 1: monitor timer is running (0 to 1) */
    uint8  guardRun;        /**< guard indication, 0: guard timer is not running, 1: guard timer is running (0 to 1) */
    uint8  threshold;       /**< permit packet number in monitor time (0 to 0xF) */
    uint32 monitorTime;     /**< monitor time, in CHIP_907XC_VA and CHIP_907XC_VB, the unit of time is 8ns. In CHIP_907XD_V1 and CHIP_907XD_VA, the unit of time is 4ns (0 to 0xFFFFFFFF) */
    uint32 guardTime;       /**< guard time, in CHIP_907XC_VA and CHIP_907XC_VB, the unit of time is 8ns. In CHIP_907XD_V1 and CHIP_907XD_VA, the unit of time is 4ns (0 to 0xFFFFFFFF) */
    uint32 monitorEnd;      /**< end time of monitor time (0 to 0xFFFFFFFF) */
    uint8  monitorOverflow; /**< overflow bit for monitor timer (0 to 1) */
    uint32 guardEnd;        /**< end time of guard time (0 to 0xFFFFFFFF) */
    uint8  guardOverflow;   /**< overflow bit for guard timer (0 to 1) */

} AclTblAccessTimerDiscardParam_t;

/********* Header Dump *********/
typedef struct
{
    uint8  sphy;             /**< ingress physical port number (0 to 0xF) */
    uint64 timestamp;        /**< timestamp (0 to 0xFFFFFFFFFFFFFFFF) */
    uint16 iaclidx;          /**< header dump hit iacl index, 0x3FF means no-hit (0 to 0x3FF) */
    uint16 eaclidx;          /**< header dump hit eacl index, 0x3FF means no-hit (0 to 0x3FF) */
    uint8  da[MAC_ADDR_LEN]; /**< destination MAC address (0x0 to 0xFF for 6 elements) */
    uint8  sa[MAC_ADDR_LEN]; /**< source MAC address (0x0 to 0xFF for 6 elements) */
    uint8  otagExist;        /**< indicates if otag exists (0 to 1) */
    uint8  otagPcp;          /**< tag format: otagPcp (0 to 7) */
    uint8  otagCfi;          /**< tag format: otagCfi (0 to 1) */
    uint16 otagVid;          /**< tag format: otagVid (0 to 0xFFF) */
    uint8  itagExist;        /**< indicates if itag exists (0 to 1) */
    uint8  itagPcp;          /**< tag format: itagPcp (0 to 7) */
    uint8  itagCfi;          /**< tag format: itagCfi (0 to 1) */
    uint16 itagVid;          /**< tag format: itagVid (0 to 0xFFF) */
    uint16 ethertype;        /**< ethertype (0 to 0xFFFF) */
#if defined(RTL_907XD_V1) || defined(RTL_907XD_VA)
    uint32 sip_31_0;   /**< SIP[31:0] (0 to 0xFFFFFFFF) */
    uint32 sip_63_32;  /**< SIP[63:32] (0 to 0xFFFFFFFF) */
    uint32 sip_95_64;  /**< SIP[95:64] (0 to 0xFFFFFFFF) */
    uint32 sip_127_96; /**< SIP[127:96] (0 to 0xFFFFFFFF) */
    uint32 dip_31_0;   /**< DIP[31:0] (0 to 0xFFFFFFFF) */
    uint32 dip_63_32;  /**< DIP[63:32] (0 to 0xFFFFFFFF) */
    uint32 dip_95_64;  /**< DIP[95:64] (0 to 0xFFFFFFFF) */
    uint32 dip_127_96; /**< DIP[127:96] (0 to 0xFFFFFFFF) */
#endif
    uint16 offsetField0;  /**< 2 bytes after IPv4/IPv6 byte 0  (0 to 0xFFFF) */
    uint16 offsetField1;  /**< 2 bytes after IPv4/IPv6 byte 2  (0 to 0xFFFF) */
    uint16 offsetField2;  /**< 2 bytes after IPv4/IPv6 byte 4  (0 to 0xFFFF) */
    uint16 offsetField3;  /**< 2 bytes after IPv4/IPv6 byte 6  (0 to 0xFFFF) */
    uint16 offsetField4;  /**< 2 bytes after IPv4/IPv6 byte 8  (0 to 0xFFFF) */
    uint16 offsetField5;  /**< 2 bytes after IPv4/IPv6 byte 10 (0 to 0xFFFF) */
    uint16 offsetField6;  /**< 2 bytes after IPv4/IPv6 byte 12 (0 to 0xFFFF) */
    uint16 offsetField7;  /**< 2 bytes after IPv4/IPv6 byte 14 (0 to 0xFFFF) */
    uint16 offsetField8;  /**< 2 bytes after IPv4/IPv6 byte 16 (0 to 0xFFFF) */
    uint16 offsetField9;  /**< 2 bytes after IPv4/IPv6 byte 18 (0 to 0xFFFF) */
    uint16 offsetField10; /**< 2 bytes after IPv4/IPv6 byte 20 (0 to 0xFFFF) */
    uint16 offsetField11; /**< 2 bytes after IPv4/IPv6 byte 22 (0 to 0xFFFF) */
    uint16 offsetField12; /**< 2 bytes after IPv4/IPv6 byte 24 (0 to 0xFFFF) */
    uint16 offsetField13; /**< 2 bytes after IPv4/IPv6 byte 26 (0 to 0xFFFF) */
    uint16 offsetField14; /**< 2 bytes after IPv4/IPv6 byte 28 (0 to 0xFFFF) */
    uint16 offsetField15; /**< 2 bytes after IPv4/IPv6 byte 30 (0 to 0xFFFF) */
    uint16 offsetField16; /**< 2 bytes after IPv4/IPv6 byte 32 (0 to 0xFFFF) */
    uint16 offsetField17; /**< 2 bytes after IPv4/IPv6 byte 34 (0 to 0xFFFF) */
    uint16 offsetField18; /**< 2 bytes after IPv4/IPv6 byte 36 (0 to 0xFFFF) */
    uint16 offsetField19; /**< 2 bytes after IPv4/IPv6 byte 38 (0 to 0xFFFF) */
    uint16 offsetField20; /**< 2 bytes after L4 other byte 0  (0 to 0xFFFF) */
    uint16 offsetField21; /**< 2 bytes after L4 other byte 2  (0 to 0xFFFF) */
    uint16 offsetField22; /**< 2 bytes after L4 other byte 4  (0 to 0xFFFF) */
    uint16 offsetField23; /**< 2 bytes after L4 other byte 6  (0 to 0xFFFF) */
#if defined(RTL_907XC_VA) || defined(RTL_907XC_VB)
    uint16 offsetField24; /**< 2 bytes after L4 other byte 8  (0 to 0xFFFF) */
    uint16 offsetField25; /**< 2 bytes after L4 other byte 10 (0 to 0xFFFF) */
    uint16 offsetField26; /**< 2 bytes after L4 other byte 12 (0 to 0xFFFF) */
    uint16 offsetField27; /**< 2 bytes after L4 other byte 14 (0 to 0xFFFF) */
    uint16 offsetField28; /**< 2 bytes after L4 other byte 16 (0 to 0xFFFF) */
    uint16 offsetField29; /**< 2 bytes after L4 other byte 18 (0 to 0xFFFF) */
#endif
} AclTblAccessHeaderDumpParam_t;

/* ACL Information Structures */
/**
 * @brief This structure represents the ingress ACL enable control information.
 *
 * This structure contains port number, ACL enable status, and default action for ACL.
 */
typedef struct
{
    RtkPort   portNum; /**< [in]Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    AclEnable aclEn;   /**< [in, out]Ingress ACL enable (<CHIPID>_ACL_DISABLE to <CHIPID>_ACL_ENABLE) */
    AclDefAct defAct;  /**< [in, out]The default action of ingress ACL lookup miss (<CHIPID>_ACL_DEF_ACT_PERMIT to <CHIPID>_ACL_DEF_ACT_DROP) */

} AclEnableCtrlInfo_t;

/**
 * @brief This structure represents the egress ACL enable control information.
 *
 * This structure contains port number, ACL enable status, and default action for ACL.
 */
typedef struct
{
    RtkPort   portNum; /**< [in]Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END) */
    AclEnable aclEn;   /**< [in, out]Egress ACL enable (<CHIPID>_ACL_DISABLE to <CHIPID>_ACL_ENABLE) */
    AclDefAct defAct;  /**< [in, out]The default action of egress ACL lookup miss (<CHIPID>_ACL_DEF_ACT_PERMIT to <CHIPID>_ACL_DEF_ACT_DROP) */

} EaclEnableCtrlInfo_t;

/**
 * @brief This structure represents the ingress ACL template information.
 *
 * This structure contains template index, field index, and filter key type for ACL.
 */
typedef struct
{
    AclTemplateId      tpIdx;    /**< [in]Template index of ACL (0 to <CHIPID>_ACL_TEMPLATE_BOUND) */
    AclTemplateFieldId fieldIdx; /**< [in]Field index (0 to <CHIPID>_ACL_TEMPLATE_FIELD_BOUND) */
    AclFilterFieldType type;     /**< [in, out]Filter key type (<CHIPID>_IACL_FLD_LBOUND to <CHIPID>_IACL_FLD_HBOUND) */

} AclTemplateInfo_t;

/**
 * @brief This structure represents the egress ACL template information.
 *
 * This structure contains template index, field index, and filter key type for ACL.
 */
typedef struct
{
    AclTemplateId      tpIdx;    /**< [in]Template index of ACL (0 to <CHIPID>_ACL_TEMPLATE_BOUND) */
    AclTemplateFieldId fieldIdx; /**< [in]Field index (0 to <CHIPID>_ACL_TEMPLATE_FIELD_BOUND) */
    AclFilterFieldType type;     /**< [in, out]Filter key type (<CHIPID>_EACL_FLD_HBOUND to <CHIPID>_EACL_FLD_LBOUND) */

} EaclTemplateInfo_t;

/**
 * @brief This structure represents the ACL PIE entry information.
 *
 * This structure contains ACL rule index and parameters for the ACL PIE table.
 */
typedef struct
{
    AclRuleId               aclRuleIndex; /**< [in]ACL rule index (0 to <CHIPID>_ACL_RULE_BOUND) */
    AclTblAccessPieParam_t *pPieParam;    /**< [in, out]The parameters of ACL PIE table (N/A) */
} AclPieEntryInfo_t;

/**
 * @brief This structure represents the ACL PIE priority information.
 *
 * This structure contains PIE block index and priority value for the ACL PIE.
 */
typedef struct
{
    AclPieBlockId pieBlock; /**< [in]PIE index (0 to <CHIPID>_ACL_PIE_BLOCK_BOUND) */
    AclPriVal     priVal;   /**< [in, out]PIE priority value (0 to <CHIPID>_ACL_PIE_PRIORITY_BOUND) */

} AclPriorityPieInfo_t;

/**
 * @brief This structure represents the ACL result reverse information.
 *
 * This structure contains ACL rule index and reverse configuration for the ACL.
 */
typedef struct
{
    AclRuleId  aclRuleIndex; /**< [in]ACL rule index (0 to <CHIPID>_ACL_RULE_BOUND) */
    AclReverse isReverse;    /**< [in, out]Reverse configuration (<CHIPID>_ACL_NORMAL to <CHIPID>_ACL_REVERSE) */

} AclReverseInfo_t;

/**
 * @brief This structure represents the ACL result aggregation information.
 *
 * This structure contains ACL rule index and aggregator operation code for the ACL.
 */
typedef struct
{
    AclRuleId aclRuleIndex; /**< [in]ACL rule index (even-numbered TG's entry index, e.g. 0-31, 64-95, 128-159 and so on) */
    AclAggOp  aggOp;        /**< [in, out]Aggregator operation code (<CHIPID>_ACL_NOT_AGG to <CHIPID>_ACL_SEL_DOWN_PIE_AGG) */

} AclAggregatorInfo_t;

/**
 * @brief This structure represents the ingress ACL action entry information.
 *
 * This structure contains ACL rule index and parameters for the ingress ACL action table.
 */
typedef struct
{
    AclRuleId               aclRuleIndex; /**< [in]ACL rule index (0 to <CHIPID>_ACL_RULE_BOUND) */
    AclTblAccessActParam_t *pActParam;    /**< [in, out]The ingress ACL parameters of ACL action table (N/A) */
} AclActionEntryInfo_t;

/**
 * @brief This structure represents the egress ACL action entry information.
 *
 * This structure contains ACL rule index and parameters for the egress ACL action table.
 */
typedef struct
{
    AclRuleId                aclRuleIndex; /**< [in]ACL rule index (0 to <CHIPID>_ACL_RULE_BOUND) */
    EaclTblAccessActParam_t *pEactParam;   /**< [in, out]The egress ACL parameters of ACL action table (N/A) */

} EaclActionEntryInfo_t;

/**
 * @brief This structure represents the ACL any offset information.
 *
 * This structure contains any offset parser index, type, and shift position for the ACL.
 */
typedef struct
{
    AclOffsetIndex    offsetIndex;    /**< [in]Any offset parser index (0 to <CHIPID>_ACL_OFFSET_BOUND) */
    AclOffsetType     offsetType;     /**< [in, out]Any offset parser type (<CHIPID>_ACL_OFFSET_TYPE_LBOUND to <CHIPID>_ACL_OFFSET_TYPE_HBOUND) */
    AclOffsetPosition offsetPosition; /**< [in, out]Shift position of any offset parser (0 to <CHIPID>_ACL_OFFSET_POSITION_BOUND) */
#if defined(RTL_907XD_V1) || defined(RTL_907XD_VA)
    RtkPort portNum; /**< [in]Port number (<CHIPID>_RTK_USER_PORT_1ST to <CHIPID>_RTK_USER_PORT_END, <CHIPID>_RTK_CPU_PORT) */
#endif
} AclOffsetInfo_t;

/**
 * @brief This structure represents the ACL Layer4 TCP/UDP port range information.
 *
 * This structure contains Layer4 port range check index, type, upper bound, and lower bound for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx; /**< [in]Layer4 port range check index (0 to <CHIPID>_FILTER_L4PORTRANGE_BOUND) */
    AclRangeType   type;     /**< [in, out]Range check types of Layer4 port (<CHIPID>_L4PORT_TYPE_RANGE_LBOUND to <CHIPID>_L4PORT_TYPE_RANGE_HBOUND) */
    AclFilterBound upper;    /**< [in, out]Layer4 port upper bound (0 to <CHIPID>_L4PORT_RANGE_BOUND) */
    AclFilterBound lower;    /**< [in, out]Layer4 port lower bound (0 to <CHIPID>_L4PORT_RANGE_BOUND) */

} AclL4PortRangeInfo_t;

/**
 * @brief This structure represents the ACL VLAN ID range information.
 *
 * This structure contains VLAN ID range check index, type, upper bound, and lower bound for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx; /**< [in]VLAN ID range check index (0 to <CHIPID>_FILTER_VIDRANGE_BOUND) */
    AclRangeType   type;     /**< [in, out]Range check types of VLAN ID (<CHIPID>_VID_RANGE_TYPE_LBOUND to <CHIPID>_VID_RANGE_TYPE_HBOUND) */
    AclFilterBound upper;    /**< [in, out]VLAN ID upper bound (0 to <CHIPID>_VID_RANGE_BOUND) */
    AclFilterBound lower;    /**< [in, out]VLAN ID lower bound (0 to <CHIPID>_VID_RANGE_BOUND) */

} AclVidRangeInfo_t;

/**
 * @brief This structure represents the ACL IPv4 range information.
 *
 * This structure contains IPv4 range check index, type, upper bound, and lower bound for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx; /**< [in]IPv4 range check index (0 to <CHIPID>_FILTER_IPRANGE_BOUND) */
    AclRangeType   type;     /**< [in, out]Range check types of IPv4 (<CHIPID>_IP_RANGE_TYPE_LBOUND to <CHIPID>_IP_RANGE_TYPE_HBOUND) */
    AclIpv4        upper;    /**< [in, out]IPv4 upper bound (0 to <CHIPID>_IP_RANGE_BOUND) */
    AclIpv4        lower;    /**< [in, out]IPv4 lower bound (0 to <CHIPID>_IP_RANGE_BOUND) */

} AclIpRangeInfo_t;

/**
 * @brief This structure represents the ACL packet length range information.
 *
 * This structure contains packet length range check index, upper bound, and lower bound for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx; /**< [in]packet length range check index (0 to <CHIPID>_FILTER_PKTLENRANGE_BOUND) */
    AclFilterBound upper;    /**< [in, out]packet length upper bound (0 to <CHIPID>_PKTLEN_RANGE_BOUND) */
    AclFilterBound lower;    /**< [in, out]packet length lower bound (0 to <CHIPID>_PKTLEN_RANGE_BOUND) */

} AclPktLenRangeInfo_t;

/**
 * @brief This structure represents the ACL IPv6 range information.
 *
 * This structure contains IPv6 range check index, type, upper bound, and lower bound for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx;             /**< [in]IPv6 range check index (0 to <CHIPID>_FILTER_IP6RANGE_BOUND) */
    AclRangeType   type;                 /**< [in, out]Range check types of IPv6 (<CHIPID>_IP6_RANGE_TYPE_LBOUND to <CHIPID>_IP6_RANGE_TYPE_HBOUND) */
    AclIpv6        upper[IPV6_ADDR_LEN]; /**< [in, out]IPv6 upper bound (0x0 - 0xFF for 16 elements) */
    AclIpv6        lower[IPV6_ADDR_LEN]; /**< [in, out]IPv6 lower bound (0x0 - 0xFF for 16 elements) */

} AclIp6RangeInfo_t;

/**
 * @brief This structure represents the ACL MAC address range information.
 *
 * This structure contains MAC address range check index, type, upper bound, and lower bound for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx;            /**< [in]MAC address range check index (0 to <CHIPID>_FILTER_MACRANGE_BOUND) */
    AclRangeType   type;                /**< [in, out]MAC address (<CHIPID>_MAC_RANGE_TYPE_LBOUND to <CHIPID>_MAC_RANGE_TYPE_HBOUND) */
    AclMac         upper[MAC_ADDR_LEN]; /**< [in, out]MAC address upper bound (0x0 - 0xFF for 6 elements) */
    AclMac         lower[MAC_ADDR_LEN]; /**< [in, out]MAC address lower bound (0x0 - 0xFF for 6 elements) */

} AclMacRangeInfo_t;

/**
 * @brief This structure represents the ACL Layer4 TCP/UDP port member information.
 *
 * This structure contains Layer4 port member check index, type, and member value for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx; /**< [in]Layer4 port member check index (0 to <CHIPID>_FILTER_L4PORTMEMBER_BOUND) */
    AclMemberType  type;     /**< [in, out]Member check types of Layer4 port (<CHIPID>_L4PORT_MEMBER_TYPE_LBOUND to <CHIPID>_L4PORT_MEMBER_TYPE_HBOUND) */
    AclMember      member;   /**< [in, out]Layer4 port member (0 to <CHIPID>_L4PORT_MEMBER_BOUND) */

} AclL4PortMemberInfo_t;

/**
 * @brief This structure represents the ACL VLAN ID member information.
 *
 * This structure contains VLAN ID member check index, type, and member value for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx; /**< [in]VLAN ID member check index (0 to <CHIPID>_FILTER_VIDMEMBER_BOUND) */
    AclMemberType  type;     /**< [in, out]Member check types of VLAN ID (<CHIPID>_VID_MEMBER_TYPE_LBOUND to <CHIPID>_VID_MEMBER_TYPE_HBOUND) */
    AclMember      member;   /**< [in, out]VLAN ID member (0 to <CHIPID>_VID_MEMBER_BOUND) */

} AclVidMemberInfo_t;

/**
 * @brief This structure represents the ACL source port mask information.
 *
 * This structure contains source port mask member check index and source port mask value for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx; /**< [in]Source port mask member check index (0 to <CHIPID>_FILTER_SRCPORTMASK_BOUND) */
    RtkPmap        spm;      /**< [in, out]Source port mask (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL) */

} AclSpmMemberInfo_t;

/**
 * @brief This structure represents the ACL IPv4 member information.
 *
 * This structure contains IPv4 member check index, type, and member value for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx; /**< [in]IPv4 member check index (0 to <CHIPID>_FILTER_IPMEMBER_BOUND) */
    AclMemberType  type;     /**< [in, out]Member check types of IPv4 (<CHIPID>_IP_MEMBER_TYPE_LBOUND to <CHIPID>_IP_MEMBER_TYPE_HBOUND) */
    AclIpv4        member;   /**< [in, out]IPv4 member (0 to <CHIPID>_IP_MEMBER_BOUND) */

} AclIpMemberInfo_t;

/**
 * @brief This structure represents the ACL IPv6 member information.
 *
 * This structure contains IPv6 member check index, type, and member value for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx;              /**< [in]IPv6 member check index (0 to <CHIPID>_FILTER_IP6MEMBER_BOUND) */
    AclMemberType  type;                  /**< [in, out]Member check types of IPv6 (<CHIPID>_IP_MEMBER_TYPE_LBOUND to <CHIPID>_IP_MEMBER_TYPE_HBOUND) */
    AclIpv6        member[IPV6_ADDR_LEN]; /**< [in, out]IPv6 member (0x0 - 0xFF for 16 elements) */

} AclIp6MemberInfo_t;

/**
 * @brief This structure represents the ACL MAC address member information.
 *
 * This structure contains MAC address member check index, type, and member value for the ACL.
 */
typedef struct
{
    AclFilterIndex entryIdx;             /**< [in]MAC address member check index (0 to <CHIPID>_FILTER_MACMEMBER_BOUND) */
    AclMemberType  type;                 /**< [in, out]Member check types of MAC address (<CHIPID>_MAC_MEMBER_TYPE_LBOUND to <CHIPID>_MAC_MEMBER_TYPE_HBOUND) */
    AclMac         member[MAC_ADDR_LEN]; /**< [in, out]MAC address member (0x0 - 0xFF for 6 elements) */

} AclMacMemberInfo_t;

/**
 * @brief This structure represents the ACL counter entry information.
 *
 * This structure contains ACL rule index and parameters for the ACL counter table.
 */
typedef struct
{
    AclRuleId                   aclRuleIndex;  /**< [in]ACL rule index (0 to <CHIPID>_ACL_RULE_BOUND) */
    AclTblAccessCounterParam_t *pCounterParam; /**< [in, out]The parameters of ACL counter table (N/A) */
} AclCounterEntryInfo_t;

/**
 * @brief This structure represents the ACL policer entry information.
 *
 * This structure contains ACL rule index and parameters for the ACL policer table.
 */
typedef struct
{
    AclRuleId                   aclRuleIndex;  /**< [in]ACL rule index (0 to <CHIPID>_ACL_RULE_BOUND) */
    AclTblAccessPolicerParam_t *pPolicerParam; /**< [in, out]The parameters of ACL Policer table (N/A) */
} AclPolicerEntryInfo_t;

/**
 * @brief This structure represents the ACL abnormal times entry information.
 *
 * This structure contains ACL rule index and parameters for the ACL abnormal times table.
 */
typedef struct
{
    AclRuleId                         aclRuleIndex;        /**< [in]ACL rule index (0 to <CHIPID>_ACL_ABNORMAL_TIMES_ENTRY_BOUND, <CHIPID>_ACL_RULE_BOUND-<CHIPID>_ACL_ABNORMAL_TIMES_ENTRY_BOUND to <CHIPID>_ACL_RULE_BOUND) */
    AclTblAccessAbnormalTimesParam_t *pAbnormalTimesParam; /**< [in, out]The parameters of ACL abnormal times table (N/A) */
} AclAbnormalTimesEntryInfo_t;

/**
 * @brief This structure represents the ACL time slot enable information.
 *
 * This structure contains enable status for ingress ACL.
 */
typedef struct
{
    AclEnable enable; /**< [in, out]Enable status for ingress ACL (<CHIPID>_ACL_DISABLE to <CHIPID>_ACL_ENABLE) */

} AclTimeSlotEnInfo_t;

/**
 * @brief This structure represents the egress ACL time slot enable information.
 *
 * This structure contains enable status for egress ACL.
 */
typedef struct
{
    AclEnable enable; /**< [in, out]Enable status for egress ACL (<CHIPID>_ACL_DISABLE to <CHIPID>_ACL_ENABLE) */

} EaclTimeSlotEnInfo_t;

/**
 * @brief This structure represents the ACL time slot threshold information.
 *
 * This structure contains time slot interval for ingress ACL.
 */
typedef struct
{
    AclThreshold threshold; /**< [in, out]Time slot interval for ingress ACL (<CHIPID>_ACL_TIME_SLOT_THRESHOLD_LBOUND to <CHIPID>_ACL_TIME_SLOT_THRESHOLD_HBOUND) */

} AclTimeSlotThresholdInfo_t;

/**
 * @brief This structure represents the egress ACL time slot threshold information.
 *
 * This structure contains time slot interval for egress ACL.
 */
typedef struct
{
    AclThreshold threshold; /**< [in, out]Time slot interval for egress ACL (<CHIPID>_ACL_TIME_SLOT_THRESHOLD_LBOUND to <CHIPID>_ACL_TIME_SLOT_THRESHOLD_HBOUND) */

} EaclTimeSlotThresholdInfo_t;

/**
 * @brief This structure represents the ACL abnormal destination port mask information.
 *
 * This structure contains port mask for ingress ACL.
 */
typedef struct
{
    RtkPmap dpm; /**< [in, out]The port mask for ingress ACL (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */

} AclAbnormalDpmInfo_t;

/**
 * @brief This structure represents the egress ACL abnormal destination port mask information.
 *
 * This structure contains port mask for egress ACL.
 */
typedef struct
{
    RtkPmap dpm; /**< [in, out]The port mask for egress ACL (<CHIPID>_RTK_USER_PMAP_1ST to <CHIPID>_RTK_USER_PMAP_ALL, <CHIPID>_RTK_CPU_PMAP) */

} EaclAbnormalDpmInfo_t;

/**
 * @brief This structure represents the ACL timer discard entry information.
 *
 * This structure contains ACL rule index and parameters for the ACL timer discard table.
 */
typedef struct
{
    AclRuleId                        aclRuleIndex;       /**< [in]ACL rule index (0 to <CHIPID>_ACL_RULE_BOUND) */
    AclTblAccessTimerDiscardParam_t *pTimerDiscardParam; /**< [in, out]The parameters of ACL timer discard table (N/A) */
} AclTimerDiscardEntryInfo_t;

/**
 * @brief This structure represents the ACL header dump entry information.
 *
 * This structure contains header dump entry index and parameters for the ACL header dump table.
 */
typedef struct
{
    AclHeaderDumpId                aclHeaderDumpIndex; /**< [in]Header dump entry index (0 to <CHIPID>_ACL_HEADER_DUMP_BOUND) */
    AclTblAccessHeaderDumpParam_t *pHeaderDumpParam;   /**< [in, out]The parameters of ACL header dump table (N/A) */
} AclHeaderDumpEntryInfo_t;

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet rtk_acl_enable_ctrl_get(UnitChip_t unitChip, AclEnableCtrlInfo_t *aclEnableCtrlInfo);
EXTERN_API RtkApiRet rtk_acl_enable_ctrl_set(UnitChip_t unitChip, AclEnableCtrlInfo_t *aclEnableCtrlInfo);
EXTERN_API RtkApiRet rtk_eacl_enable_ctrl_get(UnitChip_t unitChip, EaclEnableCtrlInfo_t *eaclEnableCtrlInfo);
EXTERN_API RtkApiRet rtk_eacl_enable_ctrl_set(UnitChip_t unitChip, EaclEnableCtrlInfo_t *eaclEnableCtrlInfo);
EXTERN_API RtkApiRet rtk_acl_template_getting(UnitChip_t unitChip, AclTemplateInfo_t *aclTemplateInfo);
EXTERN_API RtkApiRet rtk_acl_template_setting(UnitChip_t unitChip, AclTemplateInfo_t *aclTemplateInfo);
EXTERN_API RtkApiRet rtk_eacl_template_getting(UnitChip_t unitChip, EaclTemplateInfo_t *eaclTemplateInfo);
EXTERN_API RtkApiRet rtk_eacl_template_setting(UnitChip_t unitChip, EaclTemplateInfo_t *eaclTemplateInfo);
EXTERN_API RtkApiRet rtk_acl_getting_pie_entry(UnitChip_t unitChip, AclPieEntryInfo_t *aclPieEntryInfo);
EXTERN_API RtkApiRet rtk_acl_setting_pie_entry(UnitChip_t unitChip, AclPieEntryInfo_t *aclPieEntryInfo);
EXTERN_API RtkApiRet rtk_acl_runtime_add_pie_entry(UnitChip_t unitChip, AclPieEntryInfo_t *aclPieEntryInfo);
EXTERN_API RtkApiRet rtk_acl_runtime_trigger_pie_entries(UnitChip_t unitChip);
EXTERN_API RtkApiRet rtk_acl_priority_pie_get(UnitChip_t unitChip, AclPriorityPieInfo_t *aclPriorityPieInfo);
EXTERN_API RtkApiRet rtk_acl_priority_pie_set(UnitChip_t unitChip, AclPriorityPieInfo_t *aclPriorityPieInfo);
EXTERN_API RtkApiRet rtk_acl_reverse_getting(UnitChip_t unitChip, AclReverseInfo_t *aclReverseInfo);
EXTERN_API RtkApiRet rtk_acl_reverse_setting(UnitChip_t unitChip, AclReverseInfo_t *aclReverseInfo);
EXTERN_API RtkApiRet rtk_acl_aggregator_get(UnitChip_t unitChip, AclAggregatorInfo_t *aclAggregatorInfo);
EXTERN_API RtkApiRet rtk_acl_aggregator_set(UnitChip_t unitChip, AclAggregatorInfo_t *aclAggregatorInfo);
EXTERN_API RtkApiRet rtk_acl_getting_action_entry(UnitChip_t unitChip, AclActionEntryInfo_t *aclActionEntryInfo);
EXTERN_API RtkApiRet rtk_acl_setting_action_entry(UnitChip_t unitChip, AclActionEntryInfo_t *aclActionEntryInfo);
EXTERN_API RtkApiRet rtk_eacl_getting_action_entry(UnitChip_t unitChip, EaclActionEntryInfo_t *eaclActionEntryInfo);
EXTERN_API RtkApiRet rtk_eacl_setting_action_entry(UnitChip_t unitChip, EaclActionEntryInfo_t *eaclActionEntryInfo);
EXTERN_API RtkApiRet rtk_acl_offset_get(UnitChip_t unitChip, AclOffsetInfo_t *aclOffsetInfo);
EXTERN_API RtkApiRet rtk_acl_offset_set(UnitChip_t unitChip, AclOffsetInfo_t *aclOffsetInfo);
EXTERN_API RtkApiRet rtk_acl_l4_port_range_get(UnitChip_t unitChip, AclL4PortRangeInfo_t *aclL4PortRangeInfo);
EXTERN_API RtkApiRet rtk_acl_l4_port_range_set(UnitChip_t unitChip, AclL4PortRangeInfo_t *aclL4PortRangeInfo);
EXTERN_API RtkApiRet rtk_acl_vid_range_get(UnitChip_t unitChip, AclVidRangeInfo_t *aclVidRangeInfo);
EXTERN_API RtkApiRet rtk_acl_vid_range_set(UnitChip_t unitChip, AclVidRangeInfo_t *aclVidRangeInfo);
EXTERN_API RtkApiRet rtk_acl_ip_range_get(UnitChip_t unitChip, AclIpRangeInfo_t *aclIpRangeInfo);
EXTERN_API RtkApiRet rtk_acl_ip_range_set(UnitChip_t unitChip, AclIpRangeInfo_t *aclIpRangeInfo);
EXTERN_API RtkApiRet rtk_acl_pkt_len_range_get(UnitChip_t unitChip, AclPktLenRangeInfo_t *aclPktLenRangeInfo);
EXTERN_API RtkApiRet rtk_acl_pkt_len_range_set(UnitChip_t unitChip, AclPktLenRangeInfo_t *aclPktLenRangeInfo);
EXTERN_API RtkApiRet rtk_acl_ip6_range_get(UnitChip_t unitChip, AclIp6RangeInfo_t *aclIp6RangeInfo);
EXTERN_API RtkApiRet rtk_acl_ip6_range_set(UnitChip_t unitChip, AclIp6RangeInfo_t *aclIp6RangeInfo);
EXTERN_API RtkApiRet rtk_acl_mac_range_get(UnitChip_t unitChip, AclMacRangeInfo_t *aclMacRangeInfo);
EXTERN_API RtkApiRet rtk_acl_mac_range_set(UnitChip_t unitChip, AclMacRangeInfo_t *aclMacRangeInfo);
EXTERN_API RtkApiRet rtk_acl_l4_port_member_get(UnitChip_t unitChip, AclL4PortMemberInfo_t *aclL4PortMemberInfo);
EXTERN_API RtkApiRet rtk_acl_l4_port_member_set(UnitChip_t unitChip, AclL4PortMemberInfo_t *aclL4PortMemberInfo);
EXTERN_API RtkApiRet rtk_acl_vid_member_get(UnitChip_t unitChip, AclVidMemberInfo_t *aclVidMemberInfo);
EXTERN_API RtkApiRet rtk_acl_vid_member_set(UnitChip_t unitChip, AclVidMemberInfo_t *aclVidMemberInfo);
EXTERN_API RtkApiRet rtk_acl_spm_member_get(UnitChip_t unitChip, AclSpmMemberInfo_t *aclSpmMemberInfo);
EXTERN_API RtkApiRet rtk_acl_spm_member_set(UnitChip_t unitChip, AclSpmMemberInfo_t *aclSpmMemberInfo);
EXTERN_API RtkApiRet rtk_acl_ip_member_get(UnitChip_t unitChip, AclIpMemberInfo_t *aclIpMemberInfo);
EXTERN_API RtkApiRet rtk_acl_ip_member_set(UnitChip_t unitChip, AclIpMemberInfo_t *aclIpMemberInfo);
EXTERN_API RtkApiRet rtk_acl_ip6_member_get(UnitChip_t unitChip, AclIp6MemberInfo_t *aclIp6MemberInfo);
EXTERN_API RtkApiRet rtk_acl_ip6_member_set(UnitChip_t unitChip, AclIp6MemberInfo_t *aclIp6MemberInfo);
EXTERN_API RtkApiRet rtk_acl_mac_member_get(UnitChip_t unitChip, AclMacMemberInfo_t *aclMacMemberInfo);
EXTERN_API RtkApiRet rtk_acl_mac_member_set(UnitChip_t unitChip, AclMacMemberInfo_t *aclMacMemberInfo);
EXTERN_API RtkApiRet rtk_acl_counter_entry_get(UnitChip_t unitChip, AclCounterEntryInfo_t *aclCounterEntryInfo);
EXTERN_API RtkApiRet rtk_acl_counter_entry_set(UnitChip_t unitChip, AclCounterEntryInfo_t *aclCounterEntryInfo);
EXTERN_API RtkApiRet rtk_acl_policer_entry_get(UnitChip_t unitChip, AclPolicerEntryInfo_t *aclPolicerEntryInfo);
EXTERN_API RtkApiRet rtk_acl_policer_entry_set(UnitChip_t unitChip, AclPolicerEntryInfo_t *aclPolicerEntryInfo);
EXTERN_API RtkApiRet rtk_acl_getting_abnormal_times_entry(UnitChip_t unitChip, AclAbnormalTimesEntryInfo_t *aclAbnormalTimesEntryInfo);
EXTERN_API RtkApiRet rtk_acl_setting_abnormal_times_entry(UnitChip_t unitChip, AclAbnormalTimesEntryInfo_t *aclAbnormalTimesEntryInfo);
EXTERN_API RtkApiRet rtk_acl_time_slot_en_get(UnitChip_t unitChip, AclTimeSlotEnInfo_t *aclTimeSlotEnInfo);
EXTERN_API RtkApiRet rtk_acl_time_slot_en_set(UnitChip_t unitChip, AclTimeSlotEnInfo_t *aclTimeSlotEnInfo);
EXTERN_API RtkApiRet rtk_eacl_time_slot_en_get(UnitChip_t unitChip, EaclTimeSlotEnInfo_t *eaclTimeSlotEnInfo);
EXTERN_API RtkApiRet rtk_eacl_time_slot_en_set(UnitChip_t unitChip, EaclTimeSlotEnInfo_t *eaclTimeSlotEnInfo);
EXTERN_API RtkApiRet rtk_acl_time_slot_threshold_get(UnitChip_t unitChip, AclTimeSlotThresholdInfo_t *aclTimeSlotThresholdInfo);
EXTERN_API RtkApiRet rtk_acl_time_slot_threshold_set(UnitChip_t unitChip, AclTimeSlotThresholdInfo_t *aclTimeSlotThresholdInfo);
EXTERN_API RtkApiRet rtk_eacl_time_slot_threshold_get(UnitChip_t unitChip, EaclTimeSlotThresholdInfo_t *eaclTimeSlotThresholdInfo);
EXTERN_API RtkApiRet rtk_eacl_time_slot_threshold_set(UnitChip_t unitChip, EaclTimeSlotThresholdInfo_t *eaclTimeSlotThresholdInfo);
EXTERN_API RtkApiRet rtk_acl_abnormal_dpm_get(UnitChip_t unitChip, AclAbnormalDpmInfo_t *aclAbnormalDpmInfo);
EXTERN_API RtkApiRet rtk_acl_abnormal_dpm_set(UnitChip_t unitChip, AclAbnormalDpmInfo_t *aclAbnormalDpmInfo);
EXTERN_API RtkApiRet rtk_eacl_abnormal_dpm_get(UnitChip_t unitChip, EaclAbnormalDpmInfo_t *eaclAbnormalDpmInfo);
EXTERN_API RtkApiRet rtk_eacl_abnormal_dpm_set(UnitChip_t unitChip, EaclAbnormalDpmInfo_t *eaclAbnormalDpmInfo);
EXTERN_API RtkApiRet rtk_acl_getting_timer_discard_entry(UnitChip_t unitChip, AclTimerDiscardEntryInfo_t *aclTimerDiscardEntryInfo);
EXTERN_API RtkApiRet rtk_acl_setting_timer_discard_entry(UnitChip_t unitChip, AclTimerDiscardEntryInfo_t *aclTimerDiscardEntryInfo);
EXTERN_API RtkApiRet rtk_acl_getting_header_dump_entry(UnitChip_t unitChip, AclHeaderDumpEntryInfo_t *aclHeaderDumpEntryInfo);

#endif /* RTKAS_API_ACL_H__ */
