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

#ifndef DV1_PTP_H
#define DV1_PTP_H
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_v1_api_ptp.h"

#include "rtkas_types.h"
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_table_struct.h"
#include "rtkas_debug.h"
#include "rtl907xd_v1_api_acl.h"
#include "rtl907xd_v1_asicdrv_acl.h"

#define DV1_PTP_POLLING_INTERVAL (1000u)
#define DV1_PTP_CFG_TYPE_NORMAL  (0u)
#define DV1_PTP_CFG_TYPE_MIB     (1u)
#define DV1_PTP_CFG_TYPE_LOG     (2u)
#define DV1_PTP_CFG_TYPE_TIME    (3u)
#define DV1_PTP_CFG_REINIT       (4u)
#define DV1_PTP_CFG_DEFAULT      (5u)

#define DV1_PTP_CFG_TIM_INC    (4u)
#define DV1_PTP_CFG_TIM_STABLE (2u)
#define DV1_PTP_CFG_TIM_DEC    (1u)

#define DV1_PTP_CFG_TIM_GET (3u)
#define DV1_PTP_CFG_REINIT  (4u)

#define DV1_PTP_STAMP_MAX_NS (1000000000u)
#define DV1_PTP_STAMP_SUB    (0u)
#define DV1_PTP_STAMP_ADD    (1u)
#define DV1_PTP_STAMP_COPY   (3u)
#define DV1_PTP_STAMP_RESET  (4u)
#define DV1_PPS_MAC_LATENCY  (58u)

#define DV1_ptppatch_PTP_CFG_ENABLETIMER (4u)

#define DV1_PTP_CTRL_CLK_TIME_CFG_SEL_MASTER (0u)
#define DV1_PTP_CTRL_CLK_TIME_CFG_SEL_LOCAL  (1u)
#define DV1_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE1 (2u)
#define DV1_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE2 (3u)
#define DV1_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE4 (4u)

/* PTP ACL Index */
#define DV1_PTP_ACL_IDX (545u)

/* PTP SDK TODO: Unnecessary define. Remove it when refine CLE. */
#define DV1_PTP_PATCH_SRAM (0x0u)
#define DV1_PTP_SRAM       (0x0u)
/****************************************************************/
/* Dummy Reg */
#define DV1_PTP_CFG_001 DV1_HW_MISC_DMY_RESIGSER_8
#define DV1_PTP_CFG_002 DV1_HW_MISC_DMY_RESIGSER_9
#define DV1_PTP_CFG_003 DV1_HW_MISC_DMY_RESIGSER_10
#define DV1_PTP_CFG_004 DV1_HW_MISC_DMY_RESIGSER_11

/* PtpStatus_t */
#define DV1_PTP_STATUS_SRAM (0x10C05D0u)

#define DV1_PTPCFG_GLENABLE                                (DV1_PTP_STATUS_SRAM + 0x0u)
#define DV1_PTPCFG_CFGGM                                   (DV1_PTP_STATUS_SRAM + 0x1u)
#define DV1_PTPCFG_AUTOASENABLE                            (DV1_PTP_STATUS_SRAM + 0x2u)
#define DV1_PTPCFG_PPSENABLE                               (DV1_PTP_STATUS_SRAM + 0x11u)
#define DV1_PTPCFG_SIGENABLE                               (DV1_PTP_STATUS_SRAM + 0x13u)
#define DV1_PTPCFG_SLAVEPORTID                             (DV1_PTP_STATUS_SRAM + 0x14u)
#define DV1_PTPCFG_SLAVEPORTMASK                           (DV1_PTP_STATUS_SRAM + 0x18u)
#define DV1_PTPCFG_PRIORITY1                               (DV1_PTP_STATUS_SRAM + 0x1Au)
#define DV1_PTPCFG_SYNCRXTIMOUTTXSYNCEN                    (DV1_PTP_STATUS_SRAM + 0x1Du)
#define DV1_PTPCFG_ALLOWLOSTRESPNUM                        (DV1_PTP_STATUS_SRAM + 0x2Cu)
#define DV1_PTPCFG_DELAYTHRESH                             (DV1_PTP_STATUS_SRAM + 0x30u)
#define DV1_PTPCFG_INITLOGSYNCINTVAL                       (DV1_PTP_STATUS_SRAM + 0x3Cu)
#define DV1_PTPCFG_INITLOGPDELAYINTVAL                     (DV1_PTP_STATUS_SRAM + 0x69u)
#define DV1_PTPCFG_OPERLOGSYNCINTVAL                       (DV1_PTP_STATUS_SRAM + 0x78u)
#define DV1_PTPCFG_OPERLOGPDELAYINTVAL                     (DV1_PTP_STATUS_SRAM + 0x7Bu)
#define DV1_PTPCFG_SYNCRXTIMERNUMINTVAL                    (DV1_PTP_STATUS_SRAM + 0x8Au)
#define DV1_PTPCFG_SIGNALTIMERTARGET                       (DV1_PTP_STATUS_SRAM + 0x9Au)
#define DV1_PTPCFG_FISTBOOTUPTIMTARGET                     (DV1_PTP_STATUS_SRAM + 0xA0u)
#define DV1_PTPCFG_PORTENABLEMASK                          (DV1_PTP_STATUS_SRAM + 0xA2u)
#define DV1_PTPCFG_ASCAPABLEMASK                           (DV1_PTP_STATUS_SRAM + 0xA4u)
#define DV1_PTPCFG_CALDELAYENMASK                          (DV1_PTP_STATUS_SRAM + 0xAAu)
#define DV1_PTPCFG_CLKSTABLE                               (DV1_PTP_STATUS_SRAM + 0xACu)
#define DV1_PTPCFG_PID                                     (DV1_PTP_STATUS_SRAM + 0xB0u)
#define DV1_PTPCFG_DELAYCOMBGATE                           (DV1_PTP_STATUS_SRAM + 0xE0u)
#define DV1_PTPCFG_DOMAINMAP                               (DV1_PTP_STATUS_SRAM + 0xE2u)
#define DV1_PTPCFG_FLASHERASE                              (DV1_PTP_STATUS_SRAM + 0xE8u)
#define DV1_PTPCFG_ONESTEPTXSYNC                           (DV1_PTP_STATUS_SRAM + 0xE9u)
#define DV1_PTPCFG_ONESTEPRXSYNC                           (DV1_PTP_STATUS_SRAM + 0x116u)
#define DV1_PTPCFG_INITONESTEPTXOPER                       (DV1_PTP_STATUS_SRAM + 0x143u)
#define DV1_PTPCFG_TAG                                     (DV1_PTP_STATUS_SRAM + 0x170u)
#define DV1_PTPCFG_EXCHANGEINFO                            (DV1_PTP_STATUS_SRAM + 0x194u)
#define DV1_PTPCFG_CHECKAVNUIOL                            (DV1_PTP_STATUS_SRAM + 0x195u)
#define DV1_PTPCFG_SKIPLOADCFG                             (DV1_PTP_STATUS_SRAM + 0x196u)
#define DV1_PTPCFG_SEND2DOWNPORTS                          (DV1_PTP_STATUS_SRAM + 0x197u)
#define DV1_PTPCFG_CHECKRXTS                               (DV1_PTP_STATUS_SRAM + 0x198u)
#define DV1_PTPCFG_DASEL                                   (DV1_PTP_STATUS_SRAM + 0x1A7u)
#define DV1_PTPCFG_OPTIONDA                                (DV1_PTP_STATUS_SRAM + 0x1AAu)
#define DV1_PTPCFG_OPTIONETHERHEADER                       (DV1_PTP_STATUS_SRAM + 0x1B0u)
#define DV1_PTPCFG_ISOPTIONETHERHEADER                     (DV1_PTP_STATUS_SRAM + 0x1B2u)
#define DV1_PTPCFG_CUSTOMRECOVERYENABLE                    (DV1_PTP_STATUS_SRAM + 0x1B5u)
#define DV1_PTPCFG_TIMEOUTFACTOR                           (DV1_PTP_STATUS_SRAM + 0x1B8u)
#define DV1_PTPCFG_CALRATIOENMASK                          (DV1_PTP_STATUS_SRAM + 0x1BCu)
#define DV1_PTPCFG_PORTMODE                                (DV1_PTP_STATUS_SRAM + 0x1BEu)
#define DV1_PTPCFG_EMPTYDOMAINNOPDELAY                     (DV1_PTP_STATUS_SRAM + 0x1EBu)
#define DV1_PTPCFG_ADJ_DIFF_NS                             (DV1_PTP_STATUS_SRAM + 0x1ECu)
#define DV1_PTPCFG_ADJ_LOCK_TARGET_NS                      (DV1_PTP_STATUS_SRAM + 0x1F0u)
#define DV1_PTPCFG_ADJ_LOCK_STABLE_TIMES                   (DV1_PTP_STATUS_SRAM + 0x1F4u)
#define DV1_PTPCFG_REMAPPINGPORTNUM                        (DV1_PTP_STATUS_SRAM + 0x1F5u)
#define DV1_PTPCFG_ENABLE_PORTREMAPPING                    (DV1_PTP_STATUS_SRAM + 0x204u)
#define DV1_PTPCFG_SIP                                     (DV1_PTP_STATUS_SRAM + 0x205u)
#define DV1_PTPCFG_DIP                                     (DV1_PTP_STATUS_SRAM + 0x20Du)
#define DV1_PTPCFG_PDELAYDIP                               (DV1_PTP_STATUS_SRAM + 0x225u)
#define DV1_PTPCFG_SELL2L3                                 (DV1_PTP_STATUS_SRAM + 0x22Du)
#define DV1_PTPCFG_RATIO                                   (DV1_PTP_STATUS_SRAM + 0x230u)
#define DV1_PTPCFG_NEIRATIO                                (DV1_PTP_STATUS_SRAM + 0x2A8u)
#define DV1_PTPCFG_SYNCRCVD                                (DV1_PTP_STATUS_SRAM + 0x320u)
#define DV1_PTPCFG_ONESTEPSYNCRCVD                         (DV1_PTP_STATUS_SRAM + 0x32Fu)
#define DV1_PTPCFG_SYNCROUND                               (DV1_PTP_STATUS_SRAM + 0x332u)
#define DV1_PTPCFG_FOLLOWUPRCVD                            (DV1_PTP_STATUS_SRAM + 0x341u)
#define DV1_PTPCFG_RCVDPSSYNC                              (DV1_PTP_STATUS_SRAM + 0x350u)
#define DV1_PTPCFG_NEIWASUPDATED                           (DV1_PTP_STATUS_SRAM + 0x35Fu)
#define DV1_PTPCFG_FIRSTBOOTUP                             (DV1_PTP_STATUS_SRAM + 0x38Cu)
#define DV1_PTPCFG_LOSSOFSYNCFLAG                          (DV1_PTP_STATUS_SRAM + 0x39Bu)
#define DV1_PTPCFG_LASTSYNCSEQID                           (DV1_PTP_STATUS_SRAM + 0x3AAu)
#define DV1_PTPCFG_LASTFOLOUPCFADDDELAY                    (DV1_PTP_STATUS_SRAM + 0x3B0u)
#define DV1_PTPCFG_SYNCRESNOTGOOD                          (DV1_PTP_STATUS_SRAM + 0x464u)
#define DV1_PTPCFG_LASTSEQIDFROMHW                         (DV1_PTP_STATUS_SRAM + 0x474u)
#define DV1_PTPCFG_LASTSYNCINTS                            (DV1_PTP_STATUS_SRAM + 0x47Cu)
#define DV1_PTPCFG_LASTSYNCSLAVEIN                         (DV1_PTP_STATUS_SRAM + 0x4A0u)
#define DV1_PTPCFG_RELAYLASTSYNCINTS                       (DV1_PTP_STATUS_SRAM + 0x4C4u)
#define DV1_PTPCFG_RELAYLASTSYNCSLAVEINTS                  (DV1_PTP_STATUS_SRAM + 0x578u)
#define DV1_PTPCFG_RELAYLASTFOLOUPCFADDDELAY               (DV1_PTP_STATUS_SRAM + 0x59Cu)
#define DV1_PTPCFG_DOMAINNUM                               (DV1_PTP_STATUS_SRAM + 0x650u)
#define DV1_PTPCFG_OFFSETSIGN                              (DV1_PTP_STATUS_SRAM + 0x67Du)
#define DV1_PTPCFG_OFFSETFROMMASTER                        (DV1_PTP_STATUS_SRAM + 0x680u)
#define DV1_PTPCFG_MINMASTERSYNCINTVALPORTID               (DV1_PTP_STATUS_SRAM + 0x6A4u)
#define DV1_PTPCFG_CURNTSYNCINTVAL                         (DV1_PTP_STATUS_SRAM + 0x6A8u)
#define DV1_PTPCFG_CURNTPDELAYREQINTVAL                    (DV1_PTP_STATUS_SRAM + 0x75Cu)
#define DV1_PTPCFG_PPSTRIGTIMER                            (DV1_PTP_STATUS_SRAM + 0x798u)
#define DV1_PTPCFG_PPSTRIGTIMERTARGET                      (DV1_PTP_STATUS_SRAM + 0x7A0u)
#define DV1_PTPCFG_PPSTRIGADJ                              (DV1_PTP_STATUS_SRAM + 0x7A8u)
#define DV1_PTPCFG_SYNCRXTIMER                             (DV1_PTP_STATUS_SRAM + 0x7ACu)
#define DV1_PTPCFG_SYNCRXTIMERTARGET                       (DV1_PTP_STATUS_SRAM + 0x7B8u)
#define DV1_PTPCFG_FOLLOWUPTIMER                           (DV1_PTP_STATUS_SRAM + 0x7C4u)
#define DV1_PTPCFG_SIGNALTIMER                             (DV1_PTP_STATUS_SRAM + 0x7D0u)
#define DV1_PTPCFG_SYNCTIMER                               (DV1_PTP_STATUS_SRAM + 0x7DCu)
#define DV1_PTPCFG_PDELAYREQTIMER                          (DV1_PTP_STATUS_SRAM + 0x890u)
#define DV1_PTPCFG_PORTACTIVEMASK                          (DV1_PTP_STATUS_SRAM + 0x8CCu)
#define DV1_PTPCFG_PORTMACORPHYMASK                        (DV1_PTP_STATUS_SRAM + 0x8CEu)
#define DV1_PTPCFG_LINKDNANDUPMASK                         (DV1_PTP_STATUS_SRAM + 0x8D0u)
#define DV1_PTPCFG_RCVDPSSYNCPERPORTMASK                   (DV1_PTP_STATUS_SRAM + 0x8D2u)
#define DV1_PTPCFG_INITPDELAYMASK                          (DV1_PTP_STATUS_SRAM + 0x8D8u)
#define DV1_PTPCFG_PDELAYSTABLEMASK                        (DV1_PTP_STATUS_SRAM + 0x8DAu)
#define DV1_PTPCFG_PDELAYRESPRCVDMASK                      (DV1_PTP_STATUS_SRAM + 0x8DCu)
#define DV1_PTPCFG_PDELAYREQROUNDMASK                      (DV1_PTP_STATUS_SRAM + 0x8DEu)
#define DV1_PTPCFG_PDELAYTIMOUTLOGFLAGMASK                 (DV1_PTP_STATUS_SRAM + 0x8E0u)
#define DV1_PTPCFG_SAVEDFLASHMASK                          (DV1_PTP_STATUS_SRAM + 0x8E2u)
#define DV1_PTPCFG_LASTSIGNALSEQID                         (DV1_PTP_STATUS_SRAM + 0x8E4u)
#define DV1_PTPCFG_LASTRCVDPDELAYSEQID                     (DV1_PTP_STATUS_SRAM + 0x902u)
#define DV1_PTPCFG_LOSTRESPADDER                           (DV1_PTP_STATUS_SRAM + 0x920u)
#define DV1_PTPCFG_SYNCSEQIDADDER                          (DV1_PTP_STATUS_SRAM + 0x930u)
#define DV1_PTPCFG_PDELAYSEQIDADDER                        (DV1_PTP_STATUS_SRAM + 0x98Au)
#define DV1_PTPCFG_SIGNALSEQID                             (DV1_PTP_STATUS_SRAM + 0x9A8u)
#define DV1_PTPCFG_T1DELAY                                 (DV1_PTP_STATUS_SRAM + 0xA04u)
#define DV1_PTPCFG_T2DELAY                                 (DV1_PTP_STATUS_SRAM + 0xAB8u)
#define DV1_PTPCFG_T3DELAY                                 (DV1_PTP_STATUS_SRAM + 0xB6Cu)
#define DV1_PTPCFG_T4DELAY                                 (DV1_PTP_STATUS_SRAM + 0xC20u)
#define DV1_PTPCFG_NEIDELAY                                (DV1_PTP_STATUS_SRAM + 0xCD4u)
#define DV1_PTPCFG_ACCESSFLASH                             (DV1_PTP_STATUS_SRAM + 0xD88u)
#define DV1_PTPCFG_PDELAYTHRESHOLD                         (DV1_PTP_STATUS_SRAM + 0xD8Cu)
#define DV1_PTPCFG_LOCKFLASHSTATUS                         (DV1_PTP_STATUS_SRAM + 0xD90u)
#define DV1_PTPCFG_LASTPDELAYRESPT3                        (DV1_PTP_STATUS_SRAM + 0xD94u)
#define DV1_PTPCFG_LASTPDELAYRESPT4                        (DV1_PTP_STATUS_SRAM + 0xE48u)
#define DV1_PTPCFG_SLDELAYFISTCTRL                         (DV1_PTP_STATUS_SRAM + 0xEFCu)
#define DV1_PTPCFG_SLDELAYFISTSET                          (DV1_PTP_STATUS_SRAM + 0xEFFu)
#define DV1_PTPCFG_FINETUADJ                               (DV1_PTP_STATUS_SRAM + 0xF02u)
#define DV1_PTPCFG_RATEVAL                                 (DV1_PTP_STATUS_SRAM + 0xF08u)
#define DV1_PTPCFG_DELAYSTABLETIMES                        (DV1_PTP_STATUS_SRAM + 0xF14u)
#define DV1_PTPCFG_DELAYFILADDER                           (DV1_PTP_STATUS_SRAM + 0xF24u)
#define DV1_PTPCFG_LASTDELAY                               (DV1_PTP_STATUS_SRAM + 0xF44u)
#define DV1_PTPCFG_DELAYFILTERVAL                          (DV1_PTP_STATUS_SRAM + 0xFF8u)
#define DV1_PTPCFG_LASTSUCCESSFULSYN                       (DV1_PTP_STATUS_SRAM + 0x1034u)
#define DV1_PTPCFG_LASTSUCCESSFULPATHDELY                  (DV1_PTP_STATUS_SRAM + 0x1064u)
#define DV1_PTPCFG_NEEDRECOVERYSYNC                        (DV1_PTP_STATUS_SRAM + 0x1154u)
#define DV1_PTPCFG_SEAMLESS_ORIGINALTS                     (DV1_PTP_STATUS_SRAM + 0x1158u)
#define DV1_PTPCFG_SEAMLESS_LASTFOLOUPCFADDDELAY           (DV1_PTP_STATUS_SRAM + 0x11A0u)
#define DV1_PTPCFG_SEAMLESS_LASTSYNCSLAVEIN                (DV1_PTP_STATUS_SRAM + 0x11E8u)
#define DV1_PTPCFG_EXPLOG                                  (DV1_PTP_STATUS_SRAM + 0x1230u)
#define DV1_PTPCFG_URGETTXINFO                             (DV1_PTP_STATUS_SRAM + 0x18A8u)
#define DV1_PTPCFG_TXTIMESUPTHRESHOLD                      (DV1_PTP_STATUS_SRAM + 0x18B8u)
#define DV1_PTPCFG_TXTIMESUPCNT                            (DV1_PTP_STATUS_SRAM + 0x18BCu)
#define DV1_SIGENABLE_D1D2                                 (DV1_PTP_STATUS_SRAM + 0x18C0u)
#define DV1_ASSIGNTXPRI_ENABLE                             (DV1_PTP_STATUS_SRAM + 0x18C2u)
#define DV1_PTP_PRIORITY                                   (DV1_PTP_STATUS_SRAM + 0x18C4u)
#define DV1_PTPCFG_NORECOVERYFIRSTBOOT                     (DV1_PTP_STATUS_SRAM + 0x18C5u)
#define DV1_PTPCFG_SILENCEIFNEVERSYNCED                    (DV1_PTP_STATUS_SRAM + 0x18C6u)
#define DV1_PTPCFG_USEMYCLOCKID                            (DV1_PTP_STATUS_SRAM + 0x18C9u)
#define DV1_PTPCFG_PDELAYTOMASTERPORT                      (DV1_PTP_STATUS_SRAM + 0x18CAu)
#define DV1_GOTSYNC_D0                                     (DV1_PTP_STATUS_SRAM + 0x18CBu)
#define DV1_PTPCFG_SYNCLOCKSWITCH                          (DV1_PTP_STATUS_SRAM + 0x18CEu)
#define DV1_PTPCFG_SYNCSLOWDOWNSWITCH                      (DV1_PTP_STATUS_SRAM + 0x18CFu)
#define DV1_PTPCFG_TXLATENCY                               (DV1_PTP_STATUS_SRAM + 0x18D0u)
#define DV1_PTPCFG_RXLATENCY                               (DV1_PTP_STATUS_SRAM + 0x190Cu)
#define DV1_PTPCFG_SEAMLESS1EN                             (DV1_PTP_STATUS_SRAM + 0x1948u)
#define DV1_PTPCFG_SEAMLESS1CHKPORTDOMAIN                  (DV1_PTP_STATUS_SRAM + 0x194Cu)
#define DV1_PTPCFG_SEAMLESS2EN                             (DV1_PTP_STATUS_SRAM + 0x1950u)
#define DV1_PTPCFG_SEAMLESS2CHKPORTDOMAIN                  (DV1_PTP_STATUS_SRAM + 0x1954u)
#define DV1_PTPCFG_PPSMACLATENCY                           (DV1_PTP_STATUS_SRAM + 0x1958u)
#define DV1_PTPCFG_PPSTRIGLOWTHRESHOLD                     (DV1_PTP_STATUS_SRAM + 0x195Cu)
#define DV1_PTPCFG_PPSTRIGHIGHTHRESHOLD                    (DV1_PTP_STATUS_SRAM + 0x1960u)
#define DV1_PTPCFG_ALLOWEDFAULTS                           (DV1_PTP_STATUS_SRAM + 0x1964u)
#define DV1_PTPCFG_DETECTEDFAULTS                          (DV1_PTP_STATUS_SRAM + 0x1973u)
#define DV1_PTPCFG_ASCAPABLEACROSSDOMAINS                  (DV1_PTP_STATUS_SRAM + 0x1982u)
#define DV1_PTPCFG_ASYMMETRYMEASUREMENTMODE                (DV1_PTP_STATUS_SRAM + 0x1991u)
#define DV1_PTPCFG_DELAYMEASUREMECH                        (DV1_PTP_STATUS_SRAM + 0x19A0u)
#define DV1_PTPCFG_DELAYASYMMETRY                          (DV1_PTP_STATUS_SRAM + 0x19A4u)
#define DV1_PTPCFG_RXDELAYASYMMETRY                        (DV1_PTP_STATUS_SRAM + 0x1A58u)
#define DV1_PTPCFG_HWPDELAYDOMAIN                          (DV1_PTP_STATUS_SRAM + 0x1B0Cu)
#define DV1_PTPCFG_PARENTLOGSYNCINTERVAL                   (DV1_PTP_STATUS_SRAM + 0x1B1Cu)
#define DV1_PTPCFG_SYNCLOCKED                              (DV1_PTP_STATUS_SRAM + 0x1B28u)
#define DV1_PTPCFG_SYNCSLOWDOWN                            (DV1_PTP_STATUS_SRAM + 0x1B55u)
#define DV1_PTPCFG_OLDSYNCINTERVAL                         (DV1_PTP_STATUS_SRAM + 0x1B84u)
#define DV1_PTPCFG_USEMGTSETTABLELOGSYNCINTERVAL           (DV1_PTP_STATUS_SRAM + 0x1C38u)
#define DV1_PTPCFG_MGTSETTABLELOGSYNCINTERVAL              (DV1_PTP_STATUS_SRAM + 0x1C3Bu)
#define DV1_PTPCFG_USEMGTSETTABLELOGPDELAYREQINTERVAL      (DV1_PTP_STATUS_SRAM + 0x1C3Eu)
#define DV1_PTPCFG_MGTSETTABLELOGPDELAYREQINTERVAL         (DV1_PTP_STATUS_SRAM + 0x1C4Du)
#define DV1_PTPCFG_USEMGTSETTABLECOMPUTENEIGHBORRATERATIO  (DV1_PTP_STATUS_SRAM + 0x1C5Cu)
#define DV1_PTPCFG_MGTSETTABLECOMPUTENEIGHBORRATERATIO     (DV1_PTP_STATUS_SRAM + 0x1C6Bu)
#define DV1_PTPCFG_USEMGTSETTABLECOMPUTEMEANLINKDELAY      (DV1_PTP_STATUS_SRAM + 0x1C7Au)
#define DV1_PTPCFG_MGTSETTABLECOMPUTEMEANLINKDELAY         (DV1_PTP_STATUS_SRAM + 0x1C89u)
#define DV1_PTPCFG_USEMGTSETTABLEONESTEPTXOPER             (DV1_PTP_STATUS_SRAM + 0x1C98u)
#define DV1_PTPCFG_MGTSETTABLEONESTEPTXOPER                (DV1_PTP_STATUS_SRAM + 0x1CA7u)
#define DV1_PTPCFG_USEMGTSETTABLELOGGPTPCAPABLEMSGINTERVAL (DV1_PTP_STATUS_SRAM + 0x1CD4u)
#define DV1_PTPCFG_MGTSETTABLELOGGPTPCAPABLEMSGINTERVAL    (DV1_PTP_STATUS_SRAM + 0x1CE3u)
#define DV1_PTPCFG_INITLOGGPTPCAPABLEMSGINTVAL             (DV1_PTP_STATUS_SRAM + 0x1D10u)
#define DV1_PTPCFG_GPTPCAPABLEMSGINTERVAL                  (DV1_PTP_STATUS_SRAM + 0x1D40u)
#define DV1_PTPCFG_OLDGPTPCAPABLEMSGINTERVAL               (DV1_PTP_STATUS_SRAM + 0x1DF4u)
#define DV1_PTPCFG_GPTPSLOWDOWN                            (DV1_PTP_STATUS_SRAM + 0x1EA8u)
#define DV1_PTPCFG_NEIGHBORGPTPCAPABLE                     (DV1_PTP_STATUS_SRAM + 0x1ED5u)
#define DV1_PTPCFG_NEIGHBORGPTPRXTIMER                     (DV1_PTP_STATUS_SRAM + 0x1F04u)
#define DV1_PTPCFG_NEIGHBORGPTPRXTIMERTARGET               (DV1_PTP_STATUS_SRAM + 0x1FB8u)
#define DV1_PTPCFG_NEIGHBORGPTPRXTIMEOUT                   (DV1_PTP_STATUS_SRAM + 0x206Cu)
#define DV1_PTPCFG_LATESTNEIGHGPTPRXINTERVALLOG            (DV1_PTP_STATUS_SRAM + 0x207Bu)
#define DV1_PTPCFG_RXCMLDSPDELAYREQ                        (DV1_PTP_STATUS_SRAM + 0x20A8u)
#define DV1_PTPCFG_RXPDELAYREQDOMAINNUM                    (DV1_PTP_STATUS_SRAM + 0x20B7u)
#define DV1_PTPCFG_AS2020                                  (DV1_PTP_STATUS_SRAM + 0x20C6u)
#define DV1_PTPCFG_GPTPCAPABLETIMER                        (DV1_PTP_STATUS_SRAM + 0x20C8u)
#define DV1_PTPCFG_POLLINGERRORRECOVERCNT                  (DV1_PTP_STATUS_SRAM + 0x217Cu)
#define DV1_PTPCFG_MDPDELAYREQRESETSTATUS                  (DV1_PTP_STATUS_SRAM + 0x219Cu)

/*Black magic address. It may changes*/
#define DV1_BM_PTP_PID0_kP            (0x48234u)
#define DV1_BM_PTP_PID1_kP            (0x48244u)
#define DV1_BM_PTP_PID2_kP            (0x48254u)
#define DV1_BM_PTP_DELAYTHRESH_NS     (0x481C0u)
#define DV1_BM_PTP_AS_ENABLE_PORTMASK (0x4822Au)
#define DV1_BM_PTP_START              (0x48188u)

/* DV1_CHK_FUN_INVALID_RET */
#define DV1_CHK_FUN_INVALID_RET(r, f)                                                                    \
    {                                                                                                    \
        if((int32)(r) != (f))                                                                            \
        {                                                                                                \
            rtlglue_printf("(%d)Invalid input failed, expected=%d, actual return=%d\n", __LINE__, r, f); \
            return FAILED;                                                                               \
        }                                                                                                \
    }

typedef struct
{
    uint32 func_sel;    /**< Selected TAI function register (N/A) */
    uint32 clk_sel;     /**< Selected domain clock (0 to 2) */
    uint32 gene_mode;   /**< Pulse generated mode, 0:Single rising edge", "1:Single falling edge", "2:Single pulse", "3:Periodic pulses. (0 to 3) */
    uint32 dc;          /**< Duty cycle of a Trigger Pulse. 00: 0% (a 10 ns pulse) Available when pulse_amt >=8, 01: 25% Available when pulse_amt >= 8, 10: 50%, 11: 75%. Available when pulse_amt >= 8 (0 to 3) */
    uint32 amt;         /**< Period of periodic pulses / Width of single pulse (N/A) */
    uint32 amt_unit;    /**< Unit, 0: 8 nano-second (8ns~7.999ns)", "1: 1 micro-second (1us~0.999ns)", "2: 1 milli-second (0.999ms)", "3: 1 second (0.999s) (0 to 3) */
    uint32 capture_det; /**< Event Capture rising/falling edge detect, 0:raising 1:Falling (0 to 1) */
} dv1_taicfg_t;

/* DomainPortMask:
 * D0:bit0 ~ bit15, D1:bit16 ~ bit31, D2:bit32 ~ bit47 */
typedef struct
{
    uint32 remainTxTime;         /**< Remain tx time of next ptp packet. (0 to 0xFFFFFFFF) */
    uint32 remainTxTimeBuffer;   /**< Remain Tx time buffer (0 to 0xFFFFFFFF) */
    uint64 urgentDomainPortMask; /**< Tx Port mask of next ptp packet. */
} dv1_ptpUrgentTxInfo_t;

extern uint32 dv1_g_ptpTimeConfigTimeout;
extern uint32 dv1_g_ptpControlClkConfigTimeout;
extern uint32 dv1_g_ptpConfigMultipleTimeout;
extern uint32 dv1_g_ptpConfigTimeout;
extern uint32 dv1_g_ptpPortTXRXTSTimeout;
extern uint32 dv1_g_ptpTaiConfigTimeout;

EXTERN_API RtkApiRet dv1_drv_ptp_time_direct_set(uint32 unit, uint8 timesel, PtpTs_t ts);
EXTERN_API RtkApiRet dv1_drv_ptp_time_get(uint32 unit, uint8 timesel, PtpTs_t *ts);
EXTERN_API RtkApiRet dv1_drv_ptp_time_set(uint32 unit, uint8 timesel, uint8 timeoper, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dv1_drv_ptp_tai_cfg_set(uint32 unit, uint8 ppsidx, dv1_taicfg_t cfg);
EXTERN_API RtkApiRet dv1_drv_ptp_tai_cfg_get(uint32 unit, uint8 ppsidx, dv1_taicfg_t *cfg);
EXTERN_API RtkApiRet dv1_drv_ptp_tai_run(uint32 unit, uint8 ppsidx);
EXTERN_API RtkApiRet dv1_drv_ptp_tai_done(uint32 unit, uint8 ppsidx, uint8 *ishwRunning);
EXTERN_API RtkApiRet dv1_drv_ptp_tai_get_time(uint32 unit, uint8 ppsidx, PtpTs_t *pts);
EXTERN_API RtkApiRet dv1_drv_ptp_tai_notified(uint32 unit, uint8 ppsidx, uint8 *isnotified);
EXTERN_API RtkApiRet dv1_drv_ptp_get_port_tx_rx(uint32 unit, RtkPort port, uint8 domain, uint8 msgtype, uint8 txrxsel, PtpTs_t *ts);
EXTERN_API RtkApiRet dv1_drv_ptp_get_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 *ptrxts_overwr_en);
EXTERN_API RtkApiRet dv1_drv_ptp_set_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 trxts_overwr_en);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_rx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_rx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_rx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_tx_presonse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_rx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_tx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_get_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_tx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_tx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_tx_presponse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_tx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_sync_receipt_timeout_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_rx_ptp_packet_discard_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_mib_set_pdelay_allowed_lost_response_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dv1_drv_ptp_expLog_loss_of_sync_get(uint32 unit, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet dv1_drv_ptp_set_explog_loss_of_sync(uint32 unit, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dv1_drv_ptp_expLog_link_dn_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet dv1_drv_ptp_set_explog_link_dn_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dv1_drv_ptp_expLog_link_up_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet dv1_drv_ptp_set_explog_link_up_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dv1_drv_ptp_get_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet dv1_drv_ptp_set_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dv1_drv_ptp_get_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 *u8data);
EXTERN_API RtkApiRet dv1_drv_ptp_set_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 u8data);
EXTERN_API RtkApiRet dv1_drv_ptp_allowlostrespnum_get(uint32 unit, uint8 *u8data);
EXTERN_API RtkApiRet dv1_drv_ptp_allowlostrespnum_set(uint32 unit, uint8 u8data);
EXTERN_API RtkApiRet dv1_drv_ptp_protocolPortmask_get(uint32 unit, uint32 domainid, uint16 *portmask);
EXTERN_API RtkApiRet dv1_drv_ptp_onesteptxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepTxSync);
EXTERN_API int8      dv1_drv_ptp_ts_cmp(const PtpTs_t *a, const PtpTs_t *b);
EXTERN_API void      dv1_drv_ptp_ts_trim(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime);
EXTERN_API void      dv1_drv_ptp_ts_trim_sub(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime);
EXTERN_API uint8     dv1_drv_ptp_invalid_ptpdomainid(uint32 domainid);
EXTERN_API RtkApiRet dv1_drv_ptp_acl_redirect_set(uint32 unit, uint8 isEnabled, uint16 ethertype, uint8 const *const DA, uint16 spmask, uint8 priority, uint32 redirectPortMsk);
EXTERN_API RtkApiRet dv1_drv_ptp_acl_redirect_get(uint32 unit, uint8 *isEnabled, uint16 *ethertype, uint8 *DA, uint16 *spmask, uint8 *priority, uint32 *redirectPortMsk);
EXTERN_API RtkApiRet dv1_drv_ptp_onesteprxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepRxSync);
EXTERN_API RtkApiRet dv1_drv_ptp_urgentTXInfo_get(uint32 unit, dv1_ptpUrgentTxInfo_t *urgentTXInfo);
EXTERN_API RtkApiRet dv1_drv_ptp_urgentTXTimeBuffer_set(uint32 unit, uint32 txTimeBuffer);
EXTERN_API RtkApiRet dv1_drv_ptp_set_txtimesup_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet dv1_drv_ptp_get_txtimesup_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet dv1_drv_ptp_tx_times_up_cnt_set(uint32 unit, uint32 count);
EXTERN_API RtkApiRet dv1_drv_ptp_tx_times_up_cnt_get(uint32 unit, uint32 *count);
EXTERN_API RtkApiRet dv1_drv_ptp_txLatency_set(uint32 unit, RtkPort port, uint32 latency);
EXTERN_API RtkApiRet dv1_drv_ptp_txLatency_get(uint32 unit, RtkPort port, uint32 *latency);
EXTERN_API RtkApiRet dv1_drv_ptp_rxLatency_set(uint32 unit, RtkPort port, uint32 latency);
EXTERN_API RtkApiRet dv1_drv_ptp_rxLatency_get(uint32 unit, RtkPort port, uint32 *latency);
EXTERN_API RtkApiRet dv1_drv_ptp_ppsmaclatency_set(uint32 unit, uint32 latency);
EXTERN_API RtkApiRet dv1_drv_ptp_ppsmaclatency_get(uint32 unit, uint32 *latency);
EXTERN_API RtkApiRet dv1_drv_ptp_get_ppstrig_low_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet dv1_drv_ptp_set_ppstrig_low_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet dv1_drv_ptp_get_ppstrig_high_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet dv1_drv_ptp_set_ppstrig_high_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet dv1_drv_ptp_detectedfaults_get(uint32 unit, RtkPort port, uint8 *detectfaults);
EXTERN_API RtkApiRet dv1_drv_ptp_set_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 ascapablecrosdomain);
EXTERN_API RtkApiRet dv1_drv_ptp_get_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 *ascapablecrosdomain);
EXTERN_API RtkApiRet dv1_drv_ptp_get_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 *mode);
EXTERN_API RtkApiRet dv1_drv_ptp_set_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 mode);
EXTERN_API RtkApiRet dv1_drv_ptp_hw_pdelay_domain_get(uint32 unit, RtkPort port, uint8 *domain);
EXTERN_API RtkApiRet dv1_drv_ptp_parentlog_interval_get(uint32 unit, uint32 domain, uint32 *parentlog);
EXTERN_API RtkApiRet dv1_drv_ptp_runtime_synclock_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncLockStatus);
EXTERN_API RtkApiRet dv1_drv_ptp_runtime_syncslowdown_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncSlowdownStatus);
EXTERN_API RtkApiRet dv1_drv_ptp_runtime_gptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval);
EXTERN_API RtkApiRet dv1_drv_ptp_runtime_lastgptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval);
EXTERN_API RtkApiRet dv1_drv_ptp_runtime_gptpslowdown_get(uint32 unit, RtkPort port, uint32 domain, uint8 *gptpSlowdown);
EXTERN_API RtkApiRet dv1_drv_ptp_runtime_neigptpcapable_get(uint32 unit, RtkPort port, uint32 domain, uint8 *neiGptpCapable);
EXTERN_API RtkApiRet dv1_drv_ptp_neigptp_rxtimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt);
EXTERN_API RtkApiRet dv1_drv_ptp_neigptp_rxtimertarget_get(uint32 unit, RtkPort port, uint32 domain, uint32 *targetTime);
EXTERN_API RtkApiRet dv1_drv_ptp_last_neigptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, int8 *logIntval);
EXTERN_API RtkApiRet dv1_drv_ptp_rx_pdelayreq_cmds_get(uint32 unit, RtkPort port, uint8 *cmldsStatus);
EXTERN_API RtkApiRet dv1_drv_ptp_rx_pdelay_domainnum_get(uint32 unit, RtkPort port, uint8 *pdelayDomainNum);
EXTERN_API RtkApiRet dv1_drv_ptp_get_ts_polling_error_recover_count(uint32 unit, uint16 *recoverCnt);
EXTERN_API RtkApiRet dv1_drv_ptp_set_ts_polling_error_recover_count(uint32 unit, uint16 recoverCnt);
EXTERN_API RtkApiRet dv1_drv_ptp_gptpcapabletimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt);
EXTERN_API RtkApiRet dv1_drv_ptp_check_reinitprotocol_done(uint32 unit, uint32 regval);
EXTERN_API RtkApiRet dv1_drv_ptp_customize_get_in_glenable(uint32 unit, uint16 *ethertype, uint8 *da);
EXTERN_API RtkApiRet dv1_drv_ptp_invalid_logintval(int8 logintval);
EXTERN_API RtkApiRet dv1_drv_check_ptp_time_set_done(uint32 unit, uint32 regval);
RtkApiRet            dv1_drv_ptp_if_rxlatency_get(PtpPortType_e interface, uint16 *latency);
RtkApiRet            dv1_drv_ptp_if_txlatency_get(PtpPortType_e interface, uint16 *latency);
#endif
