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

#ifndef DVA_PTP_H
#define DVA_PTP_H
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xd_va_api_ptp.h"

#include "rtkas_types.h"
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_table_struct.h"
#include "rtkas_debug.h"
#include "rtl907xd_va_api_acl.h"
#include "rtl907xd_va_asicdrv_acl.h"

#define DVA_PTP_POLLING_INTERVAL (1000u)
#define DVA_PTP_CFG_TYPE_NORMAL  (0u)
#define DVA_PTP_CFG_TYPE_MIB     (1u)
#define DVA_PTP_CFG_TYPE_LOG     (2u)
#define DVA_PTP_CFG_TYPE_TIME    (3u)
#define DVA_PTP_CFG_REINIT       (4u)
#define DVA_PTP_CFG_DEFAULT      (5u)

#define DVA_PTP_CFG_TIM_INC    (4u)
#define DVA_PTP_CFG_TIM_STABLE (2u)
#define DVA_PTP_CFG_TIM_DEC    (1u)

#define DVA_PTP_CFG_TIM_GET (3u)
#define DVA_PTP_CFG_REINIT  (4u)

#define DVA_PTP_STAMP_MAX_NS (1000000000u)
#define DVA_PTP_STAMP_SUB    (0u)
#define DVA_PTP_STAMP_ADD    (1u)
#define DVA_PTP_STAMP_COPY   (3u)
#define DVA_PTP_STAMP_RESET  (4u)
#define DVA_PPS_MAC_LATENCY  (58u)

#define DVA_ptppatch_PTP_CFG_ENABLETIMER (4u)

#define DVA_PTP_CTRL_CLK_TIME_CFG_SEL_MASTER (0u)
#define DVA_PTP_CTRL_CLK_TIME_CFG_SEL_LOCAL  (1u)
#define DVA_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE1 (2u)
#define DVA_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE2 (3u)
#define DVA_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE4 (4u)

/* PTP ACL Index */
#define DVA_PTP_ACL_IDX0 (544u)
#define DVA_PTP_ACL_IDX1 (551u)
#define DVA_PTP_ACL_IDX2 (552u)

/* PTP SDK TODO: Unnecessary define. Remove it when refine CLE. */
#define DVA_PTP_PATCH_SRAM (0x0u)
#define DVA_PTP_SRAM       (0x0u)
/****************************************************************/
/* Dummy Reg */
#define DVA_PTP_CFG_001 (DVA_HW_MISC_DMY8)
#define DVA_PTP_CFG_002 (DVA_HW_MISC_DMY9)
#define DVA_PTP_CFG_003 (DVA_HW_MISC_DMY10)
#define DVA_PTP_CFG_004 (DVA_HW_MISC_DMY11)

/* PtpStatus_t */
#define DVA_PTP_STATUS_SRAM (0x11058D8u)

#define DVA_PTPCFG_GLENABLE                                (DVA_PTP_STATUS_SRAM + 0x0u)
#define DVA_PTPCFG_CFGGM                                   (DVA_PTP_STATUS_SRAM + 0x1u)
#define DVA_PTPCFG_AUTOASENABLE                            (DVA_PTP_STATUS_SRAM + 0x2u)
#define DVA_PTPCFG_PPSENABLE                               (DVA_PTP_STATUS_SRAM + 0x11u)
#define DVA_PTPCFG_SIGENABLE                               (DVA_PTP_STATUS_SRAM + 0x13u)
#define DVA_PTPCFG_SLAVEPORTID                             (DVA_PTP_STATUS_SRAM + 0x14u)
#define DVA_PTPCFG_SLAVEPORTMASK                           (DVA_PTP_STATUS_SRAM + 0x18u)
#define DVA_PTPCFG_PRIORITY1                               (DVA_PTP_STATUS_SRAM + 0x1Au)
#define DVA_PTPCFG_SYNCRXTIMOUTTXSYNCEN                    (DVA_PTP_STATUS_SRAM + 0x1Du)
#define DVA_PTPCFG_ALLOWLOSTRESPNUM                        (DVA_PTP_STATUS_SRAM + 0x2Cu)
#define DVA_PTPCFG_DELAYTHRESH                             (DVA_PTP_STATUS_SRAM + 0x30u)
#define DVA_PTPCFG_INITLOGSYNCINTVAL                       (DVA_PTP_STATUS_SRAM + 0x3Cu)
#define DVA_PTPCFG_INITLOGPDELAYINTVAL                     (DVA_PTP_STATUS_SRAM + 0x69u)
#define DVA_PTPCFG_OPERLOGSYNCINTVAL                       (DVA_PTP_STATUS_SRAM + 0x78u)
#define DVA_PTPCFG_OPERLOGPDELAYINTVAL                     (DVA_PTP_STATUS_SRAM + 0x7Bu)
#define DVA_PTPCFG_SYNCRXTIMERNUMINTVAL                    (DVA_PTP_STATUS_SRAM + 0x8Au)
#define DVA_PTPCFG_SIGNALTIMERTARGET                       (DVA_PTP_STATUS_SRAM + 0x9Au)
#define DVA_PTPCFG_FISTBOOTUPTIMTARGET                     (DVA_PTP_STATUS_SRAM + 0xA0u)
#define DVA_PTPCFG_PORTENABLEMASK                          (DVA_PTP_STATUS_SRAM + 0xA2u)
#define DVA_PTPCFG_ASCAPABLEMASK                           (DVA_PTP_STATUS_SRAM + 0xA4u)
#define DVA_PTPCFG_CALDELAYENMASK                          (DVA_PTP_STATUS_SRAM + 0xAAu)
#define DVA_PTPCFG_CLKSTABLE                               (DVA_PTP_STATUS_SRAM + 0xACu)
#define DVA_PTPCFG_PID                                     (DVA_PTP_STATUS_SRAM + 0xB0u)
#define DVA_PTPCFG_DELAYCOMBGATE                           (DVA_PTP_STATUS_SRAM + 0xE0u)
#define DVA_PTPCFG_DOMAINMAP                               (DVA_PTP_STATUS_SRAM + 0xE2u)
#define DVA_PTPCFG_FLASHERASE                              (DVA_PTP_STATUS_SRAM + 0xE8u)
#define DVA_PTPCFG_ONESTEPTXSYNC                           (DVA_PTP_STATUS_SRAM + 0xE9u)
#define DVA_PTPCFG_ONESTEPRXSYNC                           (DVA_PTP_STATUS_SRAM + 0x116u)
#define DVA_PTPCFG_INITONESTEPTXOPER                       (DVA_PTP_STATUS_SRAM + 0x143u)
#define DVA_PTPCFG_TAG                                     (DVA_PTP_STATUS_SRAM + 0x170u)
#define DVA_PTPCFG_EXCHANGEINFO                            (DVA_PTP_STATUS_SRAM + 0x194u)
#define DVA_PTPCFG_CHECKAVNUIOL                            (DVA_PTP_STATUS_SRAM + 0x195u)
#define DVA_PTPCFG_SKIPLOADCFG                             (DVA_PTP_STATUS_SRAM + 0x196u)
#define DVA_PTPCFG_SEND2DOWNPORTS                          (DVA_PTP_STATUS_SRAM + 0x197u)
#define DVA_PTPCFG_CHECKRXTS                               (DVA_PTP_STATUS_SRAM + 0x198u)
#define DVA_PTPCFG_DASEL                                   (DVA_PTP_STATUS_SRAM + 0x1A7u)
#define DVA_PTPCFG_OPTIONDA                                (DVA_PTP_STATUS_SRAM + 0x1AAu)
#define DVA_PTPCFG_OPTIONETHERHEADER                       (DVA_PTP_STATUS_SRAM + 0x1B0u)
#define DVA_PTPCFG_ISOPTIONETHERHEADER                     (DVA_PTP_STATUS_SRAM + 0x1B2u)
#define DVA_PTPCFG_CUSTOMRECOVERYENABLE                    (DVA_PTP_STATUS_SRAM + 0x1B5u)
#define DVA_PTPCFG_TIMEOUTFACTOR                           (DVA_PTP_STATUS_SRAM + 0x1B8u)
#define DVA_PTPCFG_CALRATIOENMASK                          (DVA_PTP_STATUS_SRAM + 0x1BCu)
#define DVA_PTPCFG_PORTMODE                                (DVA_PTP_STATUS_SRAM + 0x1BEu)
#define DVA_PTPCFG_EMPTYDOMAINNOPDELAY                     (DVA_PTP_STATUS_SRAM + 0x1EBu)
#define DVA_PTPCFG_ADJ_DIFF_NS                             (DVA_PTP_STATUS_SRAM + 0x1ECu)
#define DVA_PTPCFG_ADJ_LOCK_TARGET_NS                      (DVA_PTP_STATUS_SRAM + 0x1F0u)
#define DVA_PTPCFG_ADJ_LOCK_STABLE_TIMES                   (DVA_PTP_STATUS_SRAM + 0x1F4u)
#define DVA_PTPCFG_REMAPPINGPORTNUM                        (DVA_PTP_STATUS_SRAM + 0x1F5u)
#define DVA_PTPCFG_ENABLE_PORTREMAPPING                    (DVA_PTP_STATUS_SRAM + 0x204u)
#define DVA_PTPCFG_SIP                                     (DVA_PTP_STATUS_SRAM + 0x205u)
#define DVA_PTPCFG_DIP                                     (DVA_PTP_STATUS_SRAM + 0x20Du)
#define DVA_PTPCFG_PDELAYDIP                               (DVA_PTP_STATUS_SRAM + 0x225u)
#define DVA_PTPCFG_SELL2L3                                 (DVA_PTP_STATUS_SRAM + 0x22Du)
#define DVA_PTPCFG_RATIO                                   (DVA_PTP_STATUS_SRAM + 0x230u)
#define DVA_PTPCFG_NEIRATIO                                (DVA_PTP_STATUS_SRAM + 0x2A8u)
#define DVA_PTPCFG_SYNCRCVD                                (DVA_PTP_STATUS_SRAM + 0x320u)
#define DVA_PTPCFG_ONESTEPSYNCRCVD                         (DVA_PTP_STATUS_SRAM + 0x32Fu)
#define DVA_PTPCFG_SYNCROUND                               (DVA_PTP_STATUS_SRAM + 0x332u)
#define DVA_PTPCFG_FOLLOWUPRCVD                            (DVA_PTP_STATUS_SRAM + 0x341u)
#define DVA_PTPCFG_RCVDPSSYNC                              (DVA_PTP_STATUS_SRAM + 0x350u)
#define DVA_PTPCFG_NEIWASUPDATED                           (DVA_PTP_STATUS_SRAM + 0x35Fu)
#define DVA_PTPCFG_FIRSTBOOTUP                             (DVA_PTP_STATUS_SRAM + 0x38Cu)
#define DVA_PTPCFG_LOSSOFSYNCFLAG                          (DVA_PTP_STATUS_SRAM + 0x39Bu)
#define DVA_PTPCFG_LASTSYNCSEQID                           (DVA_PTP_STATUS_SRAM + 0x3AAu)
#define DVA_PTPCFG_LASTFOLOUPCFADDDELAY                    (DVA_PTP_STATUS_SRAM + 0x3B0u)
#define DVA_PTPCFG_SYNCRESNOTGOOD                          (DVA_PTP_STATUS_SRAM + 0x464u)
#define DVA_PTPCFG_LASTSEQIDFROMHW                         (DVA_PTP_STATUS_SRAM + 0x474u)
#define DVA_PTPCFG_LASTSYNCINTS                            (DVA_PTP_STATUS_SRAM + 0x47Cu)
#define DVA_PTPCFG_LASTSYNCSLAVEIN                         (DVA_PTP_STATUS_SRAM + 0x4A0u)
#define DVA_PTPCFG_RELAYLASTSYNCINTS                       (DVA_PTP_STATUS_SRAM + 0x4C4u)
#define DVA_PTPCFG_RELAYLASTSYNCSLAVEINTS                  (DVA_PTP_STATUS_SRAM + 0x578u)
#define DVA_PTPCFG_RELAYLASTFOLOUPCFADDDELAY               (DVA_PTP_STATUS_SRAM + 0x59Cu)
#define DVA_PTPCFG_DOMAINNUM                               (DVA_PTP_STATUS_SRAM + 0x650u)
#define DVA_PTPCFG_OFFSETSIGN                              (DVA_PTP_STATUS_SRAM + 0x67Du)
#define DVA_PTPCFG_OFFSETFROMMASTER                        (DVA_PTP_STATUS_SRAM + 0x680u)
#define DVA_PTPCFG_MINMASTERSYNCINTVALPORTID               (DVA_PTP_STATUS_SRAM + 0x6A4u)
#define DVA_PTPCFG_CURNTSYNCINTVAL                         (DVA_PTP_STATUS_SRAM + 0x6A8u)
#define DVA_PTPCFG_CURNTPDELAYREQINTVAL                    (DVA_PTP_STATUS_SRAM + 0x75Cu)
#define DVA_PTPCFG_PPSTRIGTIMER                            (DVA_PTP_STATUS_SRAM + 0x798u)
#define DVA_PTPCFG_PPSTRIGTIMERTARGET                      (DVA_PTP_STATUS_SRAM + 0x7A0u)
#define DVA_PTPCFG_PPSTRIGADJ                              (DVA_PTP_STATUS_SRAM + 0x7A8u)
#define DVA_PTPCFG_SYNCRXTIMER                             (DVA_PTP_STATUS_SRAM + 0x7ACu)
#define DVA_PTPCFG_SYNCRXTIMERTARGET                       (DVA_PTP_STATUS_SRAM + 0x7B8u)
#define DVA_PTPCFG_FOLLOWUPTIMER                           (DVA_PTP_STATUS_SRAM + 0x7C4u)
#define DVA_PTPCFG_SIGNALTIMER                             (DVA_PTP_STATUS_SRAM + 0x7D0u)
#define DVA_PTPCFG_SYNCTIMER                               (DVA_PTP_STATUS_SRAM + 0x7DCu)
#define DVA_PTPCFG_PDELAYREQTIMER                          (DVA_PTP_STATUS_SRAM + 0x890u)
#define DVA_PTPCFG_PORTACTIVEMASK                          (DVA_PTP_STATUS_SRAM + 0x8CCu)
#define DVA_PTPCFG_PORTMACORPHYMASK                        (DVA_PTP_STATUS_SRAM + 0x8CEu)
#define DVA_PTPCFG_LINKDNANDUPMASK                         (DVA_PTP_STATUS_SRAM + 0x8D0u)
#define DVA_PTPCFG_RCVDPSSYNCPERPORTMASK                   (DVA_PTP_STATUS_SRAM + 0x8D2u)
#define DVA_PTPCFG_INITPDELAYMASK                          (DVA_PTP_STATUS_SRAM + 0x8D8u)
#define DVA_PTPCFG_PDELAYSTABLEMASK                        (DVA_PTP_STATUS_SRAM + 0x8DAu)
#define DVA_PTPCFG_PDELAYRESPRCVDMASK                      (DVA_PTP_STATUS_SRAM + 0x8DCu)
#define DVA_PTPCFG_PDELAYREQROUNDMASK                      (DVA_PTP_STATUS_SRAM + 0x8DEu)
#define DVA_PTPCFG_PDELAYTIMOUTLOGFLAGMASK                 (DVA_PTP_STATUS_SRAM + 0x8E0u)
#define DVA_PTPCFG_SAVEDFLASHMASK                          (DVA_PTP_STATUS_SRAM + 0x8E2u)
#define DVA_PTPCFG_LASTSIGNALSEQID                         (DVA_PTP_STATUS_SRAM + 0x8E4u)
#define DVA_PTPCFG_LASTRCVDPDELAYSEQID                     (DVA_PTP_STATUS_SRAM + 0x902u)
#define DVA_PTPCFG_LOSTRESPADDER                           (DVA_PTP_STATUS_SRAM + 0x920u)
#define DVA_PTPCFG_SYNCSEQIDADDER                          (DVA_PTP_STATUS_SRAM + 0x930u)
#define DVA_PTPCFG_PDELAYSEQIDADDER                        (DVA_PTP_STATUS_SRAM + 0x98Au)
#define DVA_PTPCFG_SIGNALSEQID                             (DVA_PTP_STATUS_SRAM + 0x9A8u)
#define DVA_PTPCFG_T1DELAY                                 (DVA_PTP_STATUS_SRAM + 0xA04u)
#define DVA_PTPCFG_T2DELAY                                 (DVA_PTP_STATUS_SRAM + 0xAB8u)
#define DVA_PTPCFG_T3DELAY                                 (DVA_PTP_STATUS_SRAM + 0xB6Cu)
#define DVA_PTPCFG_T4DELAY                                 (DVA_PTP_STATUS_SRAM + 0xC20u)
#define DVA_PTPCFG_NEIDELAY                                (DVA_PTP_STATUS_SRAM + 0xCD4u)
#define DVA_PTPCFG_ACCESSFLASH                             (DVA_PTP_STATUS_SRAM + 0xD88u)
#define DVA_PTPCFG_PDELAYTHRESHOLD                         (DVA_PTP_STATUS_SRAM + 0xD8Cu)
#define DVA_PTPCFG_LOCKFLASHSTATUS                         (DVA_PTP_STATUS_SRAM + 0xD90u)
#define DVA_PTPCFG_LASTPDELAYRESPT3                        (DVA_PTP_STATUS_SRAM + 0xD94u)
#define DVA_PTPCFG_LASTPDELAYRESPT4                        (DVA_PTP_STATUS_SRAM + 0xE48u)
#define DVA_PTPCFG_SLDELAYFISTCTRL                         (DVA_PTP_STATUS_SRAM + 0xEFCu)
#define DVA_PTPCFG_SLDELAYFISTSET                          (DVA_PTP_STATUS_SRAM + 0xEFFu)
#define DVA_PTPCFG_FINETUADJ                               (DVA_PTP_STATUS_SRAM + 0xF02u)
#define DVA_PTPCFG_RATEVAL                                 (DVA_PTP_STATUS_SRAM + 0xF08u)
#define DVA_PTPCFG_DELAYSTABLETIMES                        (DVA_PTP_STATUS_SRAM + 0xF14u)
#define DVA_PTPCFG_DELAYFILADDER                           (DVA_PTP_STATUS_SRAM + 0xF24u)
#define DVA_PTPCFG_LASTDELAY                               (DVA_PTP_STATUS_SRAM + 0xF44u)
#define DVA_PTPCFG_DELAYFILTERVAL                          (DVA_PTP_STATUS_SRAM + 0xFF8u)
#define DVA_PTPCFG_LASTSUCCESSFULSYN                       (DVA_PTP_STATUS_SRAM + 0x1034u)
#define DVA_PTPCFG_LASTSUCCESSFULPATHDELY                  (DVA_PTP_STATUS_SRAM + 0x1064u)
#define DVA_PTPCFG_NEEDRECOVERYSYNC                        (DVA_PTP_STATUS_SRAM + 0x1154u)
#define DVA_PTPCFG_SEAMLESS_ORIGINALTS                     (DVA_PTP_STATUS_SRAM + 0x1158u)
#define DVA_PTPCFG_SEAMLESS_LASTFOLOUPCFADDDELAY           (DVA_PTP_STATUS_SRAM + 0x11A0u)
#define DVA_PTPCFG_SEAMLESS_LASTSYNCSLAVEIN                (DVA_PTP_STATUS_SRAM + 0x11E8u)
#define DVA_PTPCFG_EXPLOG                                  (DVA_PTP_STATUS_SRAM + 0x1230u)
#define DVA_PTPCFG_URGETTXINFO                             (DVA_PTP_STATUS_SRAM + 0x18A8u)
#define DVA_PTPCFG_TXTIMESUPTHRESHOLD                      (DVA_PTP_STATUS_SRAM + 0x18B8u)
#define DVA_PTPCFG_TXTIMESUPCNT                            (DVA_PTP_STATUS_SRAM + 0x18BCu)
#define DVA_SIGENABLE_D1D2                                 (DVA_PTP_STATUS_SRAM + 0x18C0u)
#define DVA_ASSIGNTXPRI_ENABLE                             (DVA_PTP_STATUS_SRAM + 0x18C2u)
#define DVA_PTP_PRIORITY                                   (DVA_PTP_STATUS_SRAM + 0x18C4u)
#define DVA_PTPCFG_NORECOVERYFIRSTBOOT                     (DVA_PTP_STATUS_SRAM + 0x18C5u)
#define DVA_PTPCFG_SILENCEIFNEVERSYNCED                    (DVA_PTP_STATUS_SRAM + 0x18C6u)
#define DVA_PTPCFG_USEMYCLOCKID                            (DVA_PTP_STATUS_SRAM + 0x18C9u)
#define DVA_PTPCFG_PDELAYTOMASTERPORT                      (DVA_PTP_STATUS_SRAM + 0x18CAu)
#define DVA_GOTSYNC_D0                                     (DVA_PTP_STATUS_SRAM + 0x18CBu)
#define DVA_PTPCFG_SYNCLOCKSWITCH                          (DVA_PTP_STATUS_SRAM + 0x18CEu)
#define DVA_PTPCFG_SYNCSLOWDOWNSWITCH                      (DVA_PTP_STATUS_SRAM + 0x18CFu)
#define DVA_PTPCFG_TXLATENCY                               (DVA_PTP_STATUS_SRAM + 0x18D0u)
#define DVA_PTPCFG_RXLATENCY                               (DVA_PTP_STATUS_SRAM + 0x190Cu)
#define DVA_PTPCFG_SEAMLESS1EN                             (DVA_PTP_STATUS_SRAM + 0x1948u)
#define DVA_PTPCFG_SEAMLESS1CHKPORTDOMAIN                  (DVA_PTP_STATUS_SRAM + 0x194Cu)
#define DVA_PTPCFG_SEAMLESS2EN                             (DVA_PTP_STATUS_SRAM + 0x1950u)
#define DVA_PTPCFG_SEAMLESS2CHKPORTDOMAIN                  (DVA_PTP_STATUS_SRAM + 0x1954u)
#define DVA_PTPCFG_PPSMACLATENCY                           (DVA_PTP_STATUS_SRAM + 0x1958u)
#define DVA_PTPCFG_PPSTRIGLOWTHRESHOLD                     (DVA_PTP_STATUS_SRAM + 0x195Cu)
#define DVA_PTPCFG_PPSTRIGHIGHTHRESHOLD                    (DVA_PTP_STATUS_SRAM + 0x1960u)
#define DVA_PTPCFG_ALLOWEDFAULTS                           (DVA_PTP_STATUS_SRAM + 0x1964u)
#define DVA_PTPCFG_DETECTEDFAULTS                          (DVA_PTP_STATUS_SRAM + 0x1973u)
#define DVA_PTPCFG_ASCAPABLEACROSSDOMAINS                  (DVA_PTP_STATUS_SRAM + 0x1982u)
#define DVA_PTPCFG_ASYMMETRYMEASUREMENTMODE                (DVA_PTP_STATUS_SRAM + 0x1991u)
#define DVA_PTPCFG_DELAYMEASUREMECH                        (DVA_PTP_STATUS_SRAM + 0x19A0u)
#define DVA_PTPCFG_DELAYASYMMETRY                          (DVA_PTP_STATUS_SRAM + 0x19A4u)
#define DVA_PTPCFG_RXDELAYASYMMETRY                        (DVA_PTP_STATUS_SRAM + 0x1A58u)
#define DVA_PTPCFG_HWPDELAYDOMAIN                          (DVA_PTP_STATUS_SRAM + 0x1B0Cu)
#define DVA_PTPCFG_PARENTLOGSYNCINTERVAL                   (DVA_PTP_STATUS_SRAM + 0x1B1Cu)
#define DVA_PTPCFG_SYNCLOCKED                              (DVA_PTP_STATUS_SRAM + 0x1B28u)
#define DVA_PTPCFG_SYNCSLOWDOWN                            (DVA_PTP_STATUS_SRAM + 0x1B55u)
#define DVA_PTPCFG_OLDSYNCINTERVAL                         (DVA_PTP_STATUS_SRAM + 0x1B84u)
#define DVA_PTPCFG_USEMGTSETTABLELOGSYNCINTERVAL           (DVA_PTP_STATUS_SRAM + 0x1C38u)
#define DVA_PTPCFG_MGTSETTABLELOGSYNCINTERVAL              (DVA_PTP_STATUS_SRAM + 0x1C3Bu)
#define DVA_PTPCFG_USEMGTSETTABLELOGPDELAYREQINTERVAL      (DVA_PTP_STATUS_SRAM + 0x1C3Eu)
#define DVA_PTPCFG_MGTSETTABLELOGPDELAYREQINTERVAL         (DVA_PTP_STATUS_SRAM + 0x1C4Du)
#define DVA_PTPCFG_USEMGTSETTABLECOMPUTENEIGHBORRATERATIO  (DVA_PTP_STATUS_SRAM + 0x1C5Cu)
#define DVA_PTPCFG_MGTSETTABLECOMPUTENEIGHBORRATERATIO     (DVA_PTP_STATUS_SRAM + 0x1C6Bu)
#define DVA_PTPCFG_USEMGTSETTABLECOMPUTEMEANLINKDELAY      (DVA_PTP_STATUS_SRAM + 0x1C7Au)
#define DVA_PTPCFG_MGTSETTABLECOMPUTEMEANLINKDELAY         (DVA_PTP_STATUS_SRAM + 0x1C89u)
#define DVA_PTPCFG_USEMGTSETTABLEONESTEPTXOPER             (DVA_PTP_STATUS_SRAM + 0x1C98u)
#define DVA_PTPCFG_MGTSETTABLEONESTEPTXOPER                (DVA_PTP_STATUS_SRAM + 0x1CA7u)
#define DVA_PTPCFG_USEMGTSETTABLELOGGPTPCAPABLEMSGINTERVAL (DVA_PTP_STATUS_SRAM + 0x1CD4u)
#define DVA_PTPCFG_MGTSETTABLELOGGPTPCAPABLEMSGINTERVAL    (DVA_PTP_STATUS_SRAM + 0x1CE3u)
#define DVA_PTPCFG_INITLOGGPTPCAPABLEMSGINTVAL             (DVA_PTP_STATUS_SRAM + 0x1D10u)
#define DVA_PTPCFG_GPTPCAPABLEMSGINTERVAL                  (DVA_PTP_STATUS_SRAM + 0x1D40u)
#define DVA_PTPCFG_OLDGPTPCAPABLEMSGINTERVAL               (DVA_PTP_STATUS_SRAM + 0x1DF4u)
#define DVA_PTPCFG_GPTPSLOWDOWN                            (DVA_PTP_STATUS_SRAM + 0x1EA8u)
#define DVA_PTPCFG_NEIGHBORGPTPCAPABLE                     (DVA_PTP_STATUS_SRAM + 0x1ED5u)
#define DVA_PTPCFG_NEIGHBORGPTPRXTIMER                     (DVA_PTP_STATUS_SRAM + 0x1F04u)
#define DVA_PTPCFG_NEIGHBORGPTPRXTIMERTARGET               (DVA_PTP_STATUS_SRAM + 0x1FB8u)
#define DVA_PTPCFG_NEIGHBORGPTPRXTIMEOUT                   (DVA_PTP_STATUS_SRAM + 0x206Cu)
#define DVA_PTPCFG_LATESTNEIGHGPTPRXINTERVALLOG            (DVA_PTP_STATUS_SRAM + 0x207Bu)
#define DVA_PTPCFG_RXCMLDSPDELAYREQ                        (DVA_PTP_STATUS_SRAM + 0x20A8u)
#define DVA_PTPCFG_RXPDELAYREQDOMAINNUM                    (DVA_PTP_STATUS_SRAM + 0x20B7u)
#define DVA_PTPCFG_AS2020                                  (DVA_PTP_STATUS_SRAM + 0x20C6u)
#define DVA_PTPCFG_GPTPCAPABLETIMER                        (DVA_PTP_STATUS_SRAM + 0x20C8u)
#define DVA_PTPCFG_POLLINGERRORRECOVERCNT                  (DVA_PTP_STATUS_SRAM + 0x217Cu)
#define DVA_PTPCFG_MDPDELAYREQRESETSTATUS                  (DVA_PTP_STATUS_SRAM + 0x219Cu)

#define DVA_PTP_ISREADY_ADDRESS (0x0113E980u)

/*Black magic address. It may changes*/
#define DVA_BM_PTP_PID0_kP            (0x48234u)
#define DVA_BM_PTP_PID1_kP            (0x48244u)
#define DVA_BM_PTP_PID2_kP            (0x48254u)
#define DVA_BM_PTP_DELAYTHRESH_NS     (0x481C0u)
#define DVA_BM_PTP_AS_ENABLE_PORTMASK (0x4822Au)
#define DVA_BM_PTP_START              (0x48188u)

/* Patch Configuration address. */
#define DVA_CUSTOMIZED_DA        (0x010FEBD4u)
#define DVA_DOMAIN_PROFILES      (0x010FE81Cu)
#define DVA_AUTOSAR_CFG_ADDR     (0x010FEB70u)
#define DVA_PTP_SYNC_SLOWDOWN    (0x010FEBE6u)
#define DVA_PTP_SYNC_LOCK_CONFIG (0x010FE818u)
#define DVA_PTP_AUTOSAR_DBG_ADDR (0x010FEBECu)
#define DVA_PTP_1588MSG_MIB      (0x010FE828u)

#define DVA_AUTOSAR_CRCSUPPORT                                 (DVA_AUTOSAR_CFG_ADDR + 0u)
#define DVA_AUTOSAR_RXRESIDENCETIME                            (DVA_AUTOSAR_CFG_ADDR + 0x4u)
#define DVA_AUTOSAR_TXRESIDENCETIME                            (DVA_AUTOSAR_CFG_ADDR + 0x8u)
#define DVA_AUTOSAR_FRAMEPRIO                                  (DVA_AUTOSAR_CFG_ADDR + 0xCu)
#define DVA_AUTOSAR_GLOBALTIMETXPDELAYREQPERIOD                (DVA_AUTOSAR_CFG_ADDR + 0x10u)
#define DVA_AUTOSAR_PDELAYLATENCYTHRESHOLD                     (DVA_AUTOSAR_CFG_ADDR + 0x14u)
#define DVA_AUTOSAR_PDELAYRESPANDRESPFOLLOWUPTIMEOUT           (DVA_AUTOSAR_CFG_ADDR + 0x18u)
#define DVA_AUTOSAR_GLOBALTIMEPROPAGATIONDELAY                 (DVA_AUTOSAR_CFG_ADDR + 0x1Cu)
#define DVA_AUTOSAR_GLOBALTIMEPDELAYRESPENABLE                 (DVA_AUTOSAR_CFG_ADDR + 0x20u)
#define DVA_AUTOSAR_GLOBALTIMETXPERIOD                         (DVA_AUTOSAR_CFG_ADDR + 0x24u)
#define DVA_AUTOSAR_GLOBALTIMEFOLLOWUPTIMEOUT                  (DVA_AUTOSAR_CFG_ADDR + 0x28u)
#define DVA_AUTOSAR_MASTERSLAVECONFLICTDETECTION               (DVA_AUTOSAR_CFG_ADDR + 0x2Cu)
#define DVA_AUTOSAR_MESSAGECOMPLIANCE                          (DVA_AUTOSAR_CFG_ADDR + 0x2Du)
#define DVA_AUTOSAR_RXCRCVALIDATED                             (DVA_AUTOSAR_CFG_ADDR + 0x30u)
#define DVA_AUTOSAR_CRCFLAGSRXVALIDATED                        (DVA_AUTOSAR_CFG_ADDR + 0x34u)
#define DVA_AUTOSAR_CRCMESSAGELENGTH                           (DVA_AUTOSAR_CFG_ADDR + 0x35u)
#define DVA_AUTOSAR_CRCDOMAINNUMBER                            (DVA_AUTOSAR_CFG_ADDR + 0x36u)
#define DVA_AUTOSAR_CRCCORRECTIONFIELD                         (DVA_AUTOSAR_CFG_ADDR + 0x37u)
#define DVA_AUTOSAR_CRCSOURCEPORTIDENTITY                      (DVA_AUTOSAR_CFG_ADDR + 0x38u)
#define DVA_AUTOSAR_CRCSEQUENCEID                              (DVA_AUTOSAR_CFG_ADDR + 0x39u)
#define DVA_AUTOSAR_CRCPRECISEORIGINTIMESTAMP                  (DVA_AUTOSAR_CFG_ADDR + 0x3Au)
#define DVA_AUTOSAR_GLOBALTIMEUPLINKTOTXSWITCHRESIDENCETIME    (DVA_AUTOSAR_CFG_ADDR + 0x3Cu)
#define DVA_AUTOSAR_TLVFOLLOWUPTIMESUBTLV                      (DVA_AUTOSAR_CFG_ADDR + 0x40u)
#define DVA_AUTOSAR_TLVFOLLOWUPSTATUSSUBTLV                    (DVA_AUTOSAR_CFG_ADDR + 0x41u)
#define DVA_AUTOSAR_TLVFOLLOWUPUSERDATASUBTLV                  (DVA_AUTOSAR_CFG_ADDR + 0x42u)
#define DVA_AUTOSAR_TSYNTLVFOLLOWUPOFSSUBTLV                   (DVA_AUTOSAR_CFG_ADDR + 0x43u)
#define DVA_AUTOSAR_CRCTIMEFLAGSTXSECURED                      (DVA_AUTOSAR_CFG_ADDR + 0x44u)
#define DVA_AUTOSAR_GLOBALTIMETXCRCSECURED                     (DVA_AUTOSAR_CFG_ADDR + 0x45u)
#define DVA_AUTOSAR_GLOBALTIMESEQCOUNTERJUMPWIDTH              (DVA_AUTOSAR_CFG_ADDR + 0x48u)
#define DVA_AUTOSAR_GLOBALTIMEPDELAYRESPANDRESPFOLLOWUPTIMEOUT (DVA_AUTOSAR_CFG_ADDR + 0x4Cu)
#define DVA_AUTOSAR_OFSMULTIPLICITY                            (DVA_AUTOSAR_CFG_ADDR + 0x50u)
#define DVA_AUTOSAR_DATAIDLIST                                 (DVA_AUTOSAR_CFG_ADDR + 0x51u)
#define DVA_AUTOSAR_TSYNC_SUPPORT                              (DVA_AUTOSAR_CFG_ADDR + 0x61u)
/* Next Start from DVA_AUTOSAR_CFG_ADDR + 0x62u */

/* DVA_CHK_FUN_INVALID_RET */
#define DVA_CHK_FUN_INVALID_RET(r, f)                                                                    \
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
} dva_taicfg_t;

/* DomainPortMask:
 * D0:bit0 ~ bit15, D1:bit16 ~ bit31, D2:bit32 ~ bit47 */
typedef struct
{
    uint32 remainTxTime;         /**< Remain tx time of next ptp packet. (0 to 0xFFFFFFFF) */
    uint32 remainTxTimeBuffer;   /**< Remain Tx time buffer (0 to 0xFFFFFFFF) */
    uint64 urgentDomainPortMask; /**< Tx Port mask of next ptp packet. */
} dva_ptpUrgentTxInfo_t;

extern uint32 dva_g_ptpTimeConfigTimeout;
extern uint32 dva_g_ptpControlClkConfigTimeout;
extern uint32 dva_g_ptpConfigMultipleTimeout;
extern uint32 dva_g_ptpConfigTimeout;
extern uint32 dva_g_ptpPortTXRXTSTimeout;
extern uint32 dva_g_ptpTaiConfigTimeout;

EXTERN_API RtkApiRet dva_drv_ptp_time_direct_set(uint32 unit, uint8 timesel, PtpTs_t ts);
EXTERN_API RtkApiRet dva_drv_ptp_time_get(uint32 unit, uint8 timesel, PtpTs_t *ts);
EXTERN_API RtkApiRet dva_drv_ptp_time_set(uint32 unit, uint8 timesel, uint8 timeoper, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dva_drv_ptp_tai_cfg_set(uint32 unit, uint8 ppsidx, dva_taicfg_t cfg);
EXTERN_API RtkApiRet dva_drv_ptp_tai_cfg_get(uint32 unit, uint8 ppsidx, dva_taicfg_t *cfg);
EXTERN_API RtkApiRet dva_drv_ptp_tai_run(uint32 unit, uint8 ppsidx);
EXTERN_API RtkApiRet dva_drv_ptp_tai_done(uint32 unit, uint8 ppsidx, uint8 *ishwRunning);
EXTERN_API RtkApiRet dva_drv_ptp_tai_get_time(uint32 unit, uint8 ppsidx, PtpTs_t *pts);
EXTERN_API RtkApiRet dva_drv_ptp_tai_notified(uint32 unit, uint8 ppsidx, uint8 *isnotified);
EXTERN_API RtkApiRet dva_drv_ptp_get_port_tx_rx(uint32 unit, RtkPort port, uint8 domain, uint8 msgtype, uint8 txrxsel, PtpTs_t *ts);
EXTERN_API RtkApiRet dva_drv_ptp_get_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 *ptrxts_overwr_en);
EXTERN_API RtkApiRet dva_drv_ptp_set_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 trxts_overwr_en);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_rx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_rx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_rx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_tx_presonse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_rx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_tx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_get_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_tx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_tx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_tx_presponse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_tx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_sync_receipt_timeout_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_rx_ptp_packet_discard_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_mib_set_pdelay_allowed_lost_response_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet dva_drv_ptp_expLog_loss_of_sync_get(uint32 unit, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet dva_drv_ptp_set_expLog_loss_of_sync(uint32 unit, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dva_drv_ptp_expLog_link_dn_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet dva_drv_ptp_set_explog_link_dn_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dva_drv_ptp_expLog_link_up_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet dva_drv_ptp_set_explog_link_up_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dva_drv_ptp_get_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet dva_drv_ptp_set_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet dva_drv_ptp_get_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 *u8data);
EXTERN_API RtkApiRet dva_drv_ptp_set_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 u8data);
EXTERN_API RtkApiRet dva_drv_ptp_protocolPortmask_get(uint32 unit, uint32 domainid, uint16 *portmask);
EXTERN_API RtkApiRet dva_drv_ptp_onesteptxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepTxSync);
EXTERN_API int8      dva_drv_ptp_ts_cmp(const PtpTs_t *a, const PtpTs_t *b);
EXTERN_API void      dva_drv_ptp_ts_trim(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime);
EXTERN_API void      dva_drv_ptp_ts_trim_sub(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime);
EXTERN_API uint8     dva_drv_ptp_invalid_ptpdomainid(uint32 domainid);
EXTERN_API RtkApiRet dva_drv_ptp_acl_redirect_set(uint32 unit, dva_ptpAclParams_t *params);
EXTERN_API RtkApiRet dva_drv_ptp_acl_redirect_get(uint32 unit, dva_ptpAclParams_t *params);
EXTERN_API RtkApiRet dva_drv_ptp_onesteprxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepRxSync);
EXTERN_API RtkApiRet dva_drv_ptp_urgentTXInfo_get(uint32 unit, dva_ptpUrgentTxInfo_t *urgentTXInfo);
EXTERN_API RtkApiRet dva_drv_ptp_urgentTXTimeBuffer_set(uint32 unit, uint32 txTimeBuffer);
EXTERN_API RtkApiRet dva_drv_ptp_set_txtimesup_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet dva_drv_ptp_get_txtimesup_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet dva_drv_ptp_tx_times_up_cnt_set(uint32 unit, uint32 count);
EXTERN_API RtkApiRet dva_drv_ptp_tx_times_up_cnt_get(uint32 unit, uint32 *count);
EXTERN_API RtkApiRet dva_drv_ptp_txLatency_set(uint32 unit, RtkPort port, uint32 latency);
EXTERN_API RtkApiRet dva_drv_ptp_txLatency_get(uint32 unit, RtkPort port, uint32 *latency);
EXTERN_API RtkApiRet dva_drv_ptp_rxLatency_set(uint32 unit, RtkPort port, uint32 latency);
EXTERN_API RtkApiRet dva_drv_ptp_rxLatency_get(uint32 unit, RtkPort port, uint32 *latency);
EXTERN_API RtkApiRet dva_drv_ptp_ppsmaclatency_set(uint32 unit, uint32 latency);
EXTERN_API RtkApiRet dva_drv_ptp_ppsmaclatency_get(uint32 unit, uint32 *latency);
EXTERN_API RtkApiRet dva_drv_ptp_get_ppstrig_low_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet dva_drv_ptp_set_ppstrig_low_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet dva_drv_ptp_get_ppstrig_high_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet dva_drv_ptp_set_ppstrig_high_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet dva_drv_ptp_detectedfaults_get(uint32 unit, RtkPort port, uint8 *detectfaults);
EXTERN_API RtkApiRet dva_drv_ptp_set_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 ascapablecrosdomain);
EXTERN_API RtkApiRet dva_drv_ptp_get_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 *ascapablecrosdomain);
EXTERN_API RtkApiRet dva_drv_ptp_get_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 *mode);
EXTERN_API RtkApiRet dva_drv_ptp_set_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 mode);
EXTERN_API RtkApiRet dva_drv_ptp_hw_pdelay_domain_get(uint32 unit, RtkPort port, uint8 *domain);
EXTERN_API RtkApiRet dva_drv_ptp_parentlog_interval_get(uint32 unit, uint32 domain, uint32 *parentlog);
EXTERN_API RtkApiRet dva_drv_ptp_runtime_synclock_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncLockStatus);
EXTERN_API RtkApiRet dva_drv_ptp_runtime_syncslowdown_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncSlowdownStatus);
EXTERN_API RtkApiRet dva_drv_ptp_runtime_gptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval);
EXTERN_API RtkApiRet dva_drv_ptp_runtime_lastgptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval);
EXTERN_API RtkApiRet dva_drv_ptp_runtime_gptpslowdown_get(uint32 unit, RtkPort port, uint32 domain, uint8 *gptpSlowdown);
EXTERN_API RtkApiRet dva_drv_ptp_runtime_neigptpcapable_get(uint32 unit, RtkPort port, uint32 domain, uint8 *neiGptpCapable);
EXTERN_API RtkApiRet dva_drv_ptp_neigptp_rxtimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt);
EXTERN_API RtkApiRet dva_drv_ptp_neigptp_rxtimertarget_get(uint32 unit, RtkPort port, uint32 domain, uint32 *targetTime);
EXTERN_API RtkApiRet dva_drv_ptp_last_neigptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, int8 *logIntval);
EXTERN_API RtkApiRet dva_drv_ptp_rx_pdelayreq_cmds_get(uint32 unit, RtkPort port, uint8 *cmldsStatus);
EXTERN_API RtkApiRet dva_drv_ptp_rx_pdelay_domainnum_get(uint32 unit, RtkPort port, uint8 *pdelayDomainNum);
EXTERN_API RtkApiRet dva_drv_ptp_get_ts_polling_error_recover_count(uint32 unit, uint16 *recoverCnt);
EXTERN_API RtkApiRet dva_drv_ptp_set_ts_polling_error_recover_count(uint32 unit, uint16 recoverCnt);
EXTERN_API RtkApiRet dva_drv_ptp_gptpcapabletimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt);
EXTERN_API RtkApiRet dva_drv_ptp_check_reinitprotocol_done(uint32 unit, uint32 regval);
EXTERN_API RtkApiRet dva_drv_ptp_customize_da_get_in_glenable(uint32 unit, uint8 da[DVA_MAX_SLAVE_DOMAIN][DVA_MAC_ADDR_LEN], uint8 *aclDAcount);
EXTERN_API RtkApiRet dva_drv_ptp_customize_ethertype_get_in_glenable(uint32 unit, uint16 *ethertype);
EXTERN_API RtkApiRet dva_drv_ptp_invalid_logintval(int8 logintval);
EXTERN_API RtkApiRet dva_drv_check_ptp_time_set_done(uint32 unit, uint32 regval);
EXTERN_API RtkApiRet dva_drv_ptp_tag_vid_pri_get(uint32 unit, uint32 domainid, ptpNewQTaginfo *taginfo);
RtkApiRet            dva_drv_ptp_if_rxlatency_get(PtpPortType_e interface, uint16 *latency);
RtkApiRet            dva_drv_ptp_if_txlatency_get(PtpPortType_e interface, uint16 *latency);
RtkApiRet            dva_drv_ptp_invalid_intval(uint32 pdelayIntval);
RtkApiRet            dva_drv_ptp_get_domain_profiles(uint32 unit, uint32 domainid, PtpTpSupportProfiles_e *profiles);
RtkApiRet            dva_drv_ptp_atsar_get_crc_support(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_frame_pri(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_rx_pdelayreq_period(uint32 unit, uint32 *pdelayIntval);
RtkApiRet            dva_drv_ptp_atsar_get_pdelay_latency_thresh(uint32 unit, uint32 *delayThresh);
RtkApiRet            dva_drv_ptp_atsar_get_prop_delay(uint32 unit, uint32 *pDelay);
RtkApiRet            dva_drv_ptp_atsar_get_tx_period(uint32 unit, uint32 *intval);
RtkApiRet            dva_drv_ptp_atsar_get_msg_compliance(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_rx_crc_validated(uint32 unit, PtpTpCrcValidated_e *validated);
RtkApiRet            dva_drv_ptp_atsar_get_crc_flags(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_time_subtlv(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_status_subtlv(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_udata_subtlv(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_ofs_subtlv(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_tx_crc_time_flags(uint32 unit, uint8 *flags);
RtkApiRet            dva_drv_ptp_atsar_get_tx_time_crc_secured(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_pdelay_resp_enable(uint32 unit, uint8 *enable);
RtkApiRet            dva_drv_ptp_atsar_get_foloup_dataid_list(uint32 unit, uint8 dataid[16]);
RtkApiRet            dva_drv_ptp_atsar_get_ofs_multicity(uint32 unit, uint8 *ofsMulti);

#endif
