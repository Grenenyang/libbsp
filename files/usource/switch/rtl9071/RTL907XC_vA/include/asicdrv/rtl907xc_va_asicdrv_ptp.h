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

#ifndef CVA_PTP_H
#define CVA_PTP_H
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_va_api_ptp.h"

#include "rtkas_types.h"
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_table_struct.h"
#include "rtkas_debug.h"
#include "rtl907xc_va_api_acl.h"
#include "rtl907xc_va_asicdrv_acl.h"

#define CVA_PTP_POLLING_INTERVAL (1000u)
#define CVA_PTP_CFG_TYPE_NORMAL  (0u)
#define CVA_PTP_CFG_TYPE_MIB     (1u)
#define CVA_PTP_CFG_TYPE_LOG     (2u)
#define CVA_PTP_CFG_TYPE_TIME    (3u)
#define CVA_PTP_CFG_REINIT       (4u)
#define CVA_PTP_CFG_DEFAULT      (5u)

#define CVA_PTP_CFG_TIM_INC    (4u)
#define CVA_PTP_CFG_TIM_STABLE (2u)
#define CVA_PTP_CFG_TIM_DEC    (1u)

#define CVA_PTP_CFG_TIM_GET (3u)
#define CVA_PTP_CFG_REINIT  (4u)

#define CVA_PTP_STAMP_MAX_NS (1000000000u)
#define CVA_PTP_STAMP_SUB    (0u)
#define CVA_PTP_STAMP_ADD    (1u)
#define CVA_PTP_STAMP_COPY   (3u)
#define CVA_PTP_STAMP_RESET  (4u)
#define CVA_PPS_MAC_LATENCY  (58u)

#define CVA_ptppatch_PTP_CFG_ENABLETIMER (4u)

#define CVA_PTP_CTRL_CLK_TIME_CFG_SEL_MASTER (0u)
#define CVA_PTP_CTRL_CLK_TIME_CFG_SEL_LOCAL  (1u)
#define CVA_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE1 (2u)
#define CVA_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE2 (3u)
#define CVA_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE4 (4u)

/* PTP ACL Index */
#define CVA_PTP_ACL_IDX (545u)

/* PTP SDK TODO: Unnecessary define. Remove it when refine CLE. */
#define CVA_PTP_PATCH_SRAM (0x0u)
#define CVA_PTP_SRAM       (0x0u)
/****************************************************************/
/* Dummy Reg */
#define CVA_PTP_CFG_001 CVA_HW_MISC_DMY_RESIGSER_8
#define CVA_PTP_CFG_002 CVA_HW_MISC_DMY_RESIGSER_9
#define CVA_PTP_CFG_003 CVA_HW_MISC_DMY_RESIGSER_10
#define CVA_PTP_CFG_004 CVA_HW_MISC_DMY_RESIGSER_11

/* PtpStatus_t */
#define CVA_PTP_STATUS_SRAM (0x1A4E88u)

#define CVA_PTPCFG_GLENABLE                                (CVA_PTP_STATUS_SRAM + 0x0u)
#define CVA_PTPCFG_CFGGM                                   (CVA_PTP_STATUS_SRAM + 0x1u)
#define CVA_PTPCFG_AUTOASENABLE                            (CVA_PTP_STATUS_SRAM + 0x2u)
#define CVA_PTPCFG_PPSENABLE                               (CVA_PTP_STATUS_SRAM + 0x11u)
#define CVA_PTPCFG_SIGENABLE                               (CVA_PTP_STATUS_SRAM + 0x13u)
#define CVA_PTPCFG_SLAVEPORTID                             (CVA_PTP_STATUS_SRAM + 0x14u)
#define CVA_PTPCFG_SLAVEPORTMASK                           (CVA_PTP_STATUS_SRAM + 0x18u)
#define CVA_PTPCFG_PRIORITY1                               (CVA_PTP_STATUS_SRAM + 0x1Au)
#define CVA_PTPCFG_SYNCRXTIMOUTTXSYNCEN                    (CVA_PTP_STATUS_SRAM + 0x1Du)
#define CVA_PTPCFG_ALLOWLOSTRESPNUM                        (CVA_PTP_STATUS_SRAM + 0x2Cu)
#define CVA_PTPCFG_DELAYTHRESH                             (CVA_PTP_STATUS_SRAM + 0x30u)
#define CVA_PTPCFG_INITLOGSYNCINTVAL                       (CVA_PTP_STATUS_SRAM + 0x3Cu)
#define CVA_PTPCFG_INITLOGPDELAYINTVAL                     (CVA_PTP_STATUS_SRAM + 0x69u)
#define CVA_PTPCFG_OPERLOGSYNCINTVAL                       (CVA_PTP_STATUS_SRAM + 0x78u)
#define CVA_PTPCFG_OPERLOGPDELAYINTVAL                     (CVA_PTP_STATUS_SRAM + 0x7Bu)
#define CVA_PTPCFG_SYNCRXTIMERNUMINTVAL                    (CVA_PTP_STATUS_SRAM + 0x8Au)
#define CVA_PTPCFG_SIGNALTIMERTARGET                       (CVA_PTP_STATUS_SRAM + 0x9Au)
#define CVA_PTPCFG_FISTBOOTUPTIMTARGET                     (CVA_PTP_STATUS_SRAM + 0xA0u)
#define CVA_PTPCFG_PORTENABLEMASK                          (CVA_PTP_STATUS_SRAM + 0xA2u)
#define CVA_PTPCFG_ASCAPABLEMASK                           (CVA_PTP_STATUS_SRAM + 0xA4u)
#define CVA_PTPCFG_CALDELAYENMASK                          (CVA_PTP_STATUS_SRAM + 0xAAu)
#define CVA_PTPCFG_CLKSTABLE                               (CVA_PTP_STATUS_SRAM + 0xACu)
#define CVA_PTPCFG_PID                                     (CVA_PTP_STATUS_SRAM + 0xB0u)
#define CVA_PTPCFG_DELAYCOMBGATE                           (CVA_PTP_STATUS_SRAM + 0xE0u)
#define CVA_PTPCFG_DOMAINMAP                               (CVA_PTP_STATUS_SRAM + 0xE2u)
#define CVA_PTPCFG_FLASHERASE                              (CVA_PTP_STATUS_SRAM + 0xE8u)
#define CVA_PTPCFG_ONESTEPTXSYNC                           (CVA_PTP_STATUS_SRAM + 0xE9u)
#define CVA_PTPCFG_ONESTEPRXSYNC                           (CVA_PTP_STATUS_SRAM + 0x116u)
#define CVA_PTPCFG_INITONESTEPTXOPER                       (CVA_PTP_STATUS_SRAM + 0x143u)
#define CVA_PTPCFG_TAG                                     (CVA_PTP_STATUS_SRAM + 0x170u)
#define CVA_PTPCFG_EXCHANGEINFO                            (CVA_PTP_STATUS_SRAM + 0x194u)
#define CVA_PTPCFG_CHECKAVNUIOL                            (CVA_PTP_STATUS_SRAM + 0x195u)
#define CVA_PTPCFG_SKIPLOADCFG                             (CVA_PTP_STATUS_SRAM + 0x196u)
#define CVA_PTPCFG_SEND2DOWNPORTS                          (CVA_PTP_STATUS_SRAM + 0x197u)
#define CVA_PTPCFG_CHECKRXTS                               (CVA_PTP_STATUS_SRAM + 0x198u)
#define CVA_PTPCFG_DASEL                                   (CVA_PTP_STATUS_SRAM + 0x1A7u)
#define CVA_PTPCFG_OPTIONDA                                (CVA_PTP_STATUS_SRAM + 0x1AAu)
#define CVA_PTPCFG_OPTIONETHERHEADER                       (CVA_PTP_STATUS_SRAM + 0x1B0u)
#define CVA_PTPCFG_ISOPTIONETHERHEADER                     (CVA_PTP_STATUS_SRAM + 0x1B2u)
#define CVA_PTPCFG_CUSTOMRECOVERYENABLE                    (CVA_PTP_STATUS_SRAM + 0x1B5u)
#define CVA_PTPCFG_TIMEOUTFACTOR                           (CVA_PTP_STATUS_SRAM + 0x1B8u)
#define CVA_PTPCFG_CALRATIOENMASK                          (CVA_PTP_STATUS_SRAM + 0x1BCu)
#define CVA_PTPCFG_PORTMODE                                (CVA_PTP_STATUS_SRAM + 0x1BEu)
#define CVA_PTPCFG_EMPTYDOMAINNOPDELAY                     (CVA_PTP_STATUS_SRAM + 0x1EBu)
#define CVA_PTPCFG_ADJ_DIFF_NS                             (CVA_PTP_STATUS_SRAM + 0x1ECu)
#define CVA_PTPCFG_ADJ_LOCK_TARGET_NS                      (CVA_PTP_STATUS_SRAM + 0x1F0u)
#define CVA_PTPCFG_ADJ_LOCK_STABLE_TIMES                   (CVA_PTP_STATUS_SRAM + 0x1F4u)
#define CVA_PTPCFG_REMAPPINGPORTNUM                        (CVA_PTP_STATUS_SRAM + 0x1F5u)
#define CVA_PTPCFG_ENABLE_PORTREMAPPING                    (CVA_PTP_STATUS_SRAM + 0x204u)
#define CVA_PTPCFG_SIP                                     (CVA_PTP_STATUS_SRAM + 0x205u)
#define CVA_PTPCFG_DIP                                     (CVA_PTP_STATUS_SRAM + 0x20Du)
#define CVA_PTPCFG_PDELAYDIP                               (CVA_PTP_STATUS_SRAM + 0x225u)
#define CVA_PTPCFG_SELL2L3                                 (CVA_PTP_STATUS_SRAM + 0x22Du)
#define CVA_PTPCFG_RATIO                                   (CVA_PTP_STATUS_SRAM + 0x230u)
#define CVA_PTPCFG_NEIRATIO                                (CVA_PTP_STATUS_SRAM + 0x2A8u)
#define CVA_PTPCFG_SYNCRCVD                                (CVA_PTP_STATUS_SRAM + 0x320u)
#define CVA_PTPCFG_ONESTEPSYNCRCVD                         (CVA_PTP_STATUS_SRAM + 0x32Fu)
#define CVA_PTPCFG_SYNCROUND                               (CVA_PTP_STATUS_SRAM + 0x332u)
#define CVA_PTPCFG_FOLLOWUPRCVD                            (CVA_PTP_STATUS_SRAM + 0x341u)
#define CVA_PTPCFG_RCVDPSSYNC                              (CVA_PTP_STATUS_SRAM + 0x350u)
#define CVA_PTPCFG_NEIWASUPDATED                           (CVA_PTP_STATUS_SRAM + 0x35Fu)
#define CVA_PTPCFG_FIRSTBOOTUP                             (CVA_PTP_STATUS_SRAM + 0x38Cu)
#define CVA_PTPCFG_LOSSOFSYNCFLAG                          (CVA_PTP_STATUS_SRAM + 0x39Bu)
#define CVA_PTPCFG_LASTSYNCSEQID                           (CVA_PTP_STATUS_SRAM + 0x3AAu)
#define CVA_PTPCFG_LASTFOLOUPCFADDDELAY                    (CVA_PTP_STATUS_SRAM + 0x3B0u)
#define CVA_PTPCFG_SYNCRESNOTGOOD                          (CVA_PTP_STATUS_SRAM + 0x464u)
#define CVA_PTPCFG_LASTSEQIDFROMHW                         (CVA_PTP_STATUS_SRAM + 0x474u)
#define CVA_PTPCFG_LASTSYNCINTS                            (CVA_PTP_STATUS_SRAM + 0x47Cu)
#define CVA_PTPCFG_LASTSYNCSLAVEIN                         (CVA_PTP_STATUS_SRAM + 0x4A0u)
#define CVA_PTPCFG_RELAYLASTSYNCINTS                       (CVA_PTP_STATUS_SRAM + 0x4C4u)
#define CVA_PTPCFG_RELAYLASTSYNCSLAVEINTS                  (CVA_PTP_STATUS_SRAM + 0x578u)
#define CVA_PTPCFG_RELAYLASTFOLOUPCFADDDELAY               (CVA_PTP_STATUS_SRAM + 0x59Cu)
#define CVA_PTPCFG_DOMAINNUM                               (CVA_PTP_STATUS_SRAM + 0x650u)
#define CVA_PTPCFG_OFFSETSIGN                              (CVA_PTP_STATUS_SRAM + 0x67Du)
#define CVA_PTPCFG_OFFSETFROMMASTER                        (CVA_PTP_STATUS_SRAM + 0x680u)
#define CVA_PTPCFG_MINMASTERSYNCINTVALPORTID               (CVA_PTP_STATUS_SRAM + 0x6A4u)
#define CVA_PTPCFG_CURNTSYNCINTVAL                         (CVA_PTP_STATUS_SRAM + 0x6A8u)
#define CVA_PTPCFG_CURNTPDELAYREQINTVAL                    (CVA_PTP_STATUS_SRAM + 0x75Cu)
#define CVA_PTPCFG_PPSTRIGTIMER                            (CVA_PTP_STATUS_SRAM + 0x798u)
#define CVA_PTPCFG_PPSTRIGTIMERTARGET                      (CVA_PTP_STATUS_SRAM + 0x7A0u)
#define CVA_PTPCFG_PPSTRIGADJ                              (CVA_PTP_STATUS_SRAM + 0x7A8u)
#define CVA_PTPCFG_SYNCRXTIMER                             (CVA_PTP_STATUS_SRAM + 0x7ACu)
#define CVA_PTPCFG_SYNCRXTIMERTARGET                       (CVA_PTP_STATUS_SRAM + 0x7B8u)
#define CVA_PTPCFG_FOLLOWUPTIMER                           (CVA_PTP_STATUS_SRAM + 0x7C4u)
#define CVA_PTPCFG_SIGNALTIMER                             (CVA_PTP_STATUS_SRAM + 0x7D0u)
#define CVA_PTPCFG_SYNCTIMER                               (CVA_PTP_STATUS_SRAM + 0x7DCu)
#define CVA_PTPCFG_PDELAYREQTIMER                          (CVA_PTP_STATUS_SRAM + 0x890u)
#define CVA_PTPCFG_PORTACTIVEMASK                          (CVA_PTP_STATUS_SRAM + 0x8CCu)
#define CVA_PTPCFG_PORTMACORPHYMASK                        (CVA_PTP_STATUS_SRAM + 0x8CEu)
#define CVA_PTPCFG_LINKDNANDUPMASK                         (CVA_PTP_STATUS_SRAM + 0x8D0u)
#define CVA_PTPCFG_RCVDPSSYNCPERPORTMASK                   (CVA_PTP_STATUS_SRAM + 0x8D2u)
#define CVA_PTPCFG_INITPDELAYMASK                          (CVA_PTP_STATUS_SRAM + 0x8D8u)
#define CVA_PTPCFG_PDELAYSTABLEMASK                        (CVA_PTP_STATUS_SRAM + 0x8DAu)
#define CVA_PTPCFG_PDELAYRESPRCVDMASK                      (CVA_PTP_STATUS_SRAM + 0x8DCu)
#define CVA_PTPCFG_PDELAYREQROUNDMASK                      (CVA_PTP_STATUS_SRAM + 0x8DEu)
#define CVA_PTPCFG_PDELAYTIMOUTLOGFLAGMASK                 (CVA_PTP_STATUS_SRAM + 0x8E0u)
#define CVA_PTPCFG_SAVEDFLASHMASK                          (CVA_PTP_STATUS_SRAM + 0x8E2u)
#define CVA_PTPCFG_LASTSIGNALSEQID                         (CVA_PTP_STATUS_SRAM + 0x8E4u)
#define CVA_PTPCFG_LASTRCVDPDELAYSEQID                     (CVA_PTP_STATUS_SRAM + 0x902u)
#define CVA_PTPCFG_LOSTRESPADDER                           (CVA_PTP_STATUS_SRAM + 0x920u)
#define CVA_PTPCFG_SYNCSEQIDADDER                          (CVA_PTP_STATUS_SRAM + 0x930u)
#define CVA_PTPCFG_PDELAYSEQIDADDER                        (CVA_PTP_STATUS_SRAM + 0x98Au)
#define CVA_PTPCFG_SIGNALSEQID                             (CVA_PTP_STATUS_SRAM + 0x9A8u)
#define CVA_PTPCFG_T1DELAY                                 (CVA_PTP_STATUS_SRAM + 0xA04u)
#define CVA_PTPCFG_T2DELAY                                 (CVA_PTP_STATUS_SRAM + 0xAB8u)
#define CVA_PTPCFG_T3DELAY                                 (CVA_PTP_STATUS_SRAM + 0xB6Cu)
#define CVA_PTPCFG_T4DELAY                                 (CVA_PTP_STATUS_SRAM + 0xC20u)
#define CVA_PTPCFG_NEIDELAY                                (CVA_PTP_STATUS_SRAM + 0xCD4u)
#define CVA_PTPCFG_ACCESSFLASH                             (CVA_PTP_STATUS_SRAM + 0xD88u)
#define CVA_PTPCFG_PDELAYTHRESHOLD                         (CVA_PTP_STATUS_SRAM + 0xD8Cu)
#define CVA_PTPCFG_LOCKFLASHSTATUS                         (CVA_PTP_STATUS_SRAM + 0xD90u)
#define CVA_PTPCFG_LASTPDELAYRESPT3                        (CVA_PTP_STATUS_SRAM + 0xD94u)
#define CVA_PTPCFG_LASTPDELAYRESPT4                        (CVA_PTP_STATUS_SRAM + 0xE48u)
#define CVA_PTPCFG_SLDELAYFISTCTRL                         (CVA_PTP_STATUS_SRAM + 0xEFCu)
#define CVA_PTPCFG_SLDELAYFISTSET                          (CVA_PTP_STATUS_SRAM + 0xEFFu)
#define CVA_PTPCFG_FINETUADJ                               (CVA_PTP_STATUS_SRAM + 0xF02u)
#define CVA_PTPCFG_RATEVAL                                 (CVA_PTP_STATUS_SRAM + 0xF08u)
#define CVA_PTPCFG_DELAYSTABLETIMES                        (CVA_PTP_STATUS_SRAM + 0xF14u)
#define CVA_PTPCFG_DELAYFILADDER                           (CVA_PTP_STATUS_SRAM + 0xF24u)
#define CVA_PTPCFG_LASTDELAY                               (CVA_PTP_STATUS_SRAM + 0xF44u)
#define CVA_PTPCFG_DELAYFILTERVAL                          (CVA_PTP_STATUS_SRAM + 0xFF8u)
#define CVA_PTPCFG_LASTSUCCESSFULSYN                       (CVA_PTP_STATUS_SRAM + 0x1034u)
#define CVA_PTPCFG_LASTSUCCESSFULPATHDELY                  (CVA_PTP_STATUS_SRAM + 0x1064u)
#define CVA_PTPCFG_NEEDRECOVERYSYNC                        (CVA_PTP_STATUS_SRAM + 0x1154u)
#define CVA_PTPCFG_SEAMLESS_ORIGINALTS                     (CVA_PTP_STATUS_SRAM + 0x1158u)
#define CVA_PTPCFG_SEAMLESS_LASTFOLOUPCFADDDELAY           (CVA_PTP_STATUS_SRAM + 0x11A0u)
#define CVA_PTPCFG_SEAMLESS_LASTSYNCSLAVEIN                (CVA_PTP_STATUS_SRAM + 0x11E8u)
#define CVA_PTPCFG_EXPLOG                                  (CVA_PTP_STATUS_SRAM + 0x1230u)
#define CVA_PTPCFG_URGETTXINFO                             (CVA_PTP_STATUS_SRAM + 0x18A8u)
#define CVA_PTPCFG_TXTIMESUPTHRESHOLD                      (CVA_PTP_STATUS_SRAM + 0x18B8u)
#define CVA_PTPCFG_TXTIMESUPCNT                            (CVA_PTP_STATUS_SRAM + 0x18BCu)
#define CVA_SIGENABLE_D1D2                                 (CVA_PTP_STATUS_SRAM + 0x18C0u)
#define CVA_ASSIGNTXPRI_ENABLE                             (CVA_PTP_STATUS_SRAM + 0x18C2u)
#define CVA_PTP_PRIORITY                                   (CVA_PTP_STATUS_SRAM + 0x18C4u)
#define CVA_PTPCFG_NORECOVERYFIRSTBOOT                     (CVA_PTP_STATUS_SRAM + 0x18C5u)
#define CVA_PTPCFG_SILENCEIFNEVERSYNCED                    (CVA_PTP_STATUS_SRAM + 0x18C6u)
#define CVA_PTPCFG_USEMYCLOCKID                            (CVA_PTP_STATUS_SRAM + 0x18C9u)
#define CVA_PTPCFG_PDELAYTOMASTERPORT                      (CVA_PTP_STATUS_SRAM + 0x18CAu)
#define CVA_GOTSYNC_D0                                     (CVA_PTP_STATUS_SRAM + 0x18CBu)
#define CVA_PTPCFG_SYNCLOCKSWITCH                          (CVA_PTP_STATUS_SRAM + 0x18CEu)
#define CVA_PTPCFG_SYNCSLOWDOWNSWITCH                      (CVA_PTP_STATUS_SRAM + 0x18CFu)
#define CVA_PTPCFG_TXLATENCY                               (CVA_PTP_STATUS_SRAM + 0x18D0u)
#define CVA_PTPCFG_RXLATENCY                               (CVA_PTP_STATUS_SRAM + 0x190Cu)
#define CVA_PTPCFG_SEAMLESS1EN                             (CVA_PTP_STATUS_SRAM + 0x1948u)
#define CVA_PTPCFG_SEAMLESS1CHKPORTDOMAIN                  (CVA_PTP_STATUS_SRAM + 0x194Cu)
#define CVA_PTPCFG_SEAMLESS2EN                             (CVA_PTP_STATUS_SRAM + 0x1950u)
#define CVA_PTPCFG_SEAMLESS2CHKPORTDOMAIN                  (CVA_PTP_STATUS_SRAM + 0x1954u)
#define CVA_PTPCFG_PPSMACLATENCY                           (CVA_PTP_STATUS_SRAM + 0x1958u)
#define CVA_PTPCFG_PPSTRIGLOWTHRESHOLD                     (CVA_PTP_STATUS_SRAM + 0x195Cu)
#define CVA_PTPCFG_PPSTRIGHIGHTHRESHOLD                    (CVA_PTP_STATUS_SRAM + 0x1960u)
#define CVA_PTPCFG_ALLOWEDFAULTS                           (CVA_PTP_STATUS_SRAM + 0x1964u)
#define CVA_PTPCFG_DETECTEDFAULTS                          (CVA_PTP_STATUS_SRAM + 0x1973u)
#define CVA_PTPCFG_ASCAPABLEACROSSDOMAINS                  (CVA_PTP_STATUS_SRAM + 0x1982u)
#define CVA_PTPCFG_ASYMMETRYMEASUREMENTMODE                (CVA_PTP_STATUS_SRAM + 0x1991u)
#define CVA_PTPCFG_DELAYMEASUREMECH                        (CVA_PTP_STATUS_SRAM + 0x19A0u)
#define CVA_PTPCFG_DELAYASYMMETRY                          (CVA_PTP_STATUS_SRAM + 0x19A4u)
#define CVA_PTPCFG_RXDELAYASYMMETRY                        (CVA_PTP_STATUS_SRAM + 0x1A58u)
#define CVA_PTPCFG_HWPDELAYDOMAIN                          (CVA_PTP_STATUS_SRAM + 0x1B0Cu)
#define CVA_PTPCFG_PARENTLOGSYNCINTERVAL                   (CVA_PTP_STATUS_SRAM + 0x1B1Cu)
#define CVA_PTPCFG_SYNCLOCKED                              (CVA_PTP_STATUS_SRAM + 0x1B28u)
#define CVA_PTPCFG_SYNCSLOWDOWN                            (CVA_PTP_STATUS_SRAM + 0x1B55u)
#define CVA_PTPCFG_OLDSYNCINTERVAL                         (CVA_PTP_STATUS_SRAM + 0x1B84u)
#define CVA_PTPCFG_USEMGTSETTABLELOGSYNCINTERVAL           (CVA_PTP_STATUS_SRAM + 0x1C38u)
#define CVA_PTPCFG_MGTSETTABLELOGSYNCINTERVAL              (CVA_PTP_STATUS_SRAM + 0x1C3Bu)
#define CVA_PTPCFG_USEMGTSETTABLELOGPDELAYREQINTERVAL      (CVA_PTP_STATUS_SRAM + 0x1C3Eu)
#define CVA_PTPCFG_MGTSETTABLELOGPDELAYREQINTERVAL         (CVA_PTP_STATUS_SRAM + 0x1C4Du)
#define CVA_PTPCFG_USEMGTSETTABLECOMPUTENEIGHBORRATERATIO  (CVA_PTP_STATUS_SRAM + 0x1C5Cu)
#define CVA_PTPCFG_MGTSETTABLECOMPUTENEIGHBORRATERATIO     (CVA_PTP_STATUS_SRAM + 0x1C6Bu)
#define CVA_PTPCFG_USEMGTSETTABLECOMPUTEMEANLINKDELAY      (CVA_PTP_STATUS_SRAM + 0x1C7Au)
#define CVA_PTPCFG_MGTSETTABLECOMPUTEMEANLINKDELAY         (CVA_PTP_STATUS_SRAM + 0x1C89u)
#define CVA_PTPCFG_USEMGTSETTABLEONESTEPTXOPER             (CVA_PTP_STATUS_SRAM + 0x1C98u)
#define CVA_PTPCFG_MGTSETTABLEONESTEPTXOPER                (CVA_PTP_STATUS_SRAM + 0x1CA7u)
#define CVA_PTPCFG_USEMGTSETTABLELOGGPTPCAPABLEMSGINTERVAL (CVA_PTP_STATUS_SRAM + 0x1CD4u)
#define CVA_PTPCFG_MGTSETTABLELOGGPTPCAPABLEMSGINTERVAL    (CVA_PTP_STATUS_SRAM + 0x1CE3u)
#define CVA_PTPCFG_INITLOGGPTPCAPABLEMSGINTVAL             (CVA_PTP_STATUS_SRAM + 0x1D10u)
#define CVA_PTPCFG_GPTPCAPABLEMSGINTERVAL                  (CVA_PTP_STATUS_SRAM + 0x1D40u)
#define CVA_PTPCFG_OLDGPTPCAPABLEMSGINTERVAL               (CVA_PTP_STATUS_SRAM + 0x1DF4u)
#define CVA_PTPCFG_GPTPSLOWDOWN                            (CVA_PTP_STATUS_SRAM + 0x1EA8u)
#define CVA_PTPCFG_NEIGHBORGPTPCAPABLE                     (CVA_PTP_STATUS_SRAM + 0x1ED5u)
#define CVA_PTPCFG_NEIGHBORGPTPRXTIMER                     (CVA_PTP_STATUS_SRAM + 0x1F04u)
#define CVA_PTPCFG_NEIGHBORGPTPRXTIMERTARGET               (CVA_PTP_STATUS_SRAM + 0x1FB8u)
#define CVA_PTPCFG_NEIGHBORGPTPRXTIMEOUT                   (CVA_PTP_STATUS_SRAM + 0x206Cu)
#define CVA_PTPCFG_LATESTNEIGHGPTPRXINTERVALLOG            (CVA_PTP_STATUS_SRAM + 0x207Bu)
#define CVA_PTPCFG_RXCMLDSPDELAYREQ                        (CVA_PTP_STATUS_SRAM + 0x20A8u)
#define CVA_PTPCFG_RXPDELAYREQDOMAINNUM                    (CVA_PTP_STATUS_SRAM + 0x20B7u)
#define CVA_PTPCFG_AS2020                                  (CVA_PTP_STATUS_SRAM + 0x20C6u)
#define CVA_PTPCFG_GPTPCAPABLETIMER                        (CVA_PTP_STATUS_SRAM + 0x20C8u)
#define CVA_PTPCFG_POLLINGERRORRECOVERCNT                  (CVA_PTP_STATUS_SRAM + 0x217Cu)

/*Black magic address. It may changes*/
#define CVA_PTP_ISREADY_ADDRESS (0x001AFF4C)

/* CVA_CHK_FUN_INVALID_RET */
#define CVA_CHK_FUN_INVALID_RET(r, f)                                     \
    {                                                                     \
        if((int32)(r) != (f))                                             \
        {                                                                 \
            rtlglue_printf("(%d)Invalid input %d failed\n", __LINE__, r); \
            return FAILED;                                                \
        }                                                                 \
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
} cva_taicfg_t;

/* DomainPortMask:
 * D0:bit0 ~ bit15, D1:bit16 ~ bit31, D2:bit32 ~ bit47 */
typedef struct
{
    uint32 remainTxTime;         /**< Remain tx time of next ptp packet. (0 to 0xFFFFFFFF) */
    uint32 remainTxTimeBuffer;   /**< Remain Tx time buffer (0 to 0xFFFFFFFF) */
    uint64 urgentDomainPortMask; /**< Tx Port mask of next ptp packet. */
} cva_ptpUrgentTxInfo_t;

extern uint32 cva_g_ptpTimeConfigTimeout;
extern uint32 cva_g_ptpControlClkConfigTimeout;
extern uint32 cva_g_ptpConfigMultipleTimeout;
extern uint32 cva_g_ptpConfigTimeout;
extern uint32 cva_g_ptpPortTXRXTSTimeout;
extern uint32 cva_g_ptpTaiConfigTimeout;

EXTERN_API RtkApiRet cva_drv_ptp_time_direct_get(uint32 unit, uint8 timesel, PtpTs_t *ts);
EXTERN_API RtkApiRet cva_drv_ptp_time_direct_set(uint32 unit, uint8 timesel, PtpTs_t ts);
EXTERN_API RtkApiRet cva_drv_ptp_time_get(uint32 unit, uint8 timesel, PtpTs_t *ts);
EXTERN_API RtkApiRet cva_drv_ptp_time_set(uint32 unit, uint8 timesel, uint8 timeoper, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cva_drv_ptp_tai_cfg_set(uint32 unit, uint8 ppsidx, cva_taicfg_t cfg);
EXTERN_API RtkApiRet cva_drv_ptp_tai_cfg_get(uint32 unit, uint8 ppsidx, cva_taicfg_t *cfg);
EXTERN_API RtkApiRet cva_drv_ptp_tai_run(uint32 unit, uint8 ppsidx);
EXTERN_API RtkApiRet cva_drv_ptp_tai_done(uint32 unit, uint8 ppsidx, uint8 *ishwRunning);
EXTERN_API RtkApiRet cva_drv_ptp_tai_get_time(uint32 unit, uint8 ppsidx, PtpTs_t *pts);
EXTERN_API RtkApiRet cva_drv_ptp_tai_notified(uint32 unit, uint8 ppsidx, uint8 *isnotified);
EXTERN_API RtkApiRet cva_drv_ptp_get_port_tx_rx(uint32 unit, RtkPort port, uint8 domain, uint8 msgtype, uint8 txrxsel, PtpTs_t *ts);
EXTERN_API RtkApiRet cva_drv_ptp_get_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 *ptrxts_overwr_en);
EXTERN_API RtkApiRet cva_drv_ptp_set_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 trxts_overwr_en);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_rx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_rx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_rx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_tx_presonse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_rx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_tx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_get_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_tx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_tx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_tx_presponse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_tx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_sync_receipt_timeout_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_rx_ptp_packet_discard_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_mib_set_pdelay_allowed_lost_response_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cva_drv_ptp_expLog_loss_of_sync_get(uint32 unit, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet cva_drv_ptp_set_explog_loss_of_sync(uint32 unit, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cva_drv_ptp_expLog_link_dn_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet cva_drv_ptp_set_explog_link_dn_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cva_drv_ptp_expLog_link_up_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet cva_drv_ptp_set_explog_link_up_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cva_drv_ptp_get_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet cva_drv_ptp_set_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cva_drv_ptp_get_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 *u8data);
EXTERN_API RtkApiRet cva_drv_ptp_set_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 u8data);
EXTERN_API RtkApiRet cva_drv_ptp_allowlostrespnum_get(uint32 unit, uint8 *u8data);
EXTERN_API RtkApiRet cva_drv_ptp_allowlostrespnum_set(uint32 unit, uint8 u8data);
EXTERN_API RtkApiRet cva_drv_ptp_protocolPortmask_get(uint32 unit, uint32 domainid, uint16 *portmask);
EXTERN_API RtkApiRet cva_drv_ptp_onesteptxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepTxSync);
EXTERN_API int8      cva_drv_ptp_ts_cmp(const PtpTs_t *a, const PtpTs_t *b);
EXTERN_API void      cva_drv_ptp_ts_trim(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime);
EXTERN_API void      cva_drv_ptp_ts_trim_sub(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime);
EXTERN_API uint8     cva_drv_ptp_invalid_ptpdomainid(uint32 domainid);
EXTERN_API RtkApiRet cva_drv_ptp_acl_redirect_set(uint32 unit, uint8 isEnabled, uint16 ethertype, uint8 const *const DA, uint16 spmask, uint8 priority, uint32 redirectPortMsk);
EXTERN_API RtkApiRet cva_drv_ptp_acl_redirect_get(uint32 unit, uint8 *isEnabled, uint16 *ethertype, uint8 *DA, uint16 *spmask, uint8 *priority, uint32 *redirectPortMsk);
EXTERN_API RtkApiRet cva_drv_ptp_onesteprxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepRxSync);
EXTERN_API RtkApiRet cva_drv_ptp_urgentTXInfo_get(uint32 unit, cva_ptpUrgentTxInfo_t *urgentTXInfo);
EXTERN_API RtkApiRet cva_drv_ptp_urgentTXTimeBuffer_set(uint32 unit, uint32 txTimeBuffer);
EXTERN_API RtkApiRet cva_drv_ptp_set_txtimesup_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet cva_drv_ptp_get_txtimesup_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet cva_drv_ptp_tx_times_up_cnt_set(uint32 unit, uint32 count);
EXTERN_API RtkApiRet cva_drv_ptp_tx_times_up_cnt_get(uint32 unit, uint32 *count);
EXTERN_API RtkApiRet cva_drv_ptp_txLatency_set(uint32 unit, RtkPort port, uint32 latency);
EXTERN_API RtkApiRet cva_drv_ptp_txLatency_get(uint32 unit, RtkPort port, uint32 *latency);
EXTERN_API RtkApiRet cva_drv_ptp_rxLatency_set(uint32 unit, RtkPort port, uint32 latency);
EXTERN_API RtkApiRet cva_drv_ptp_rxLatency_get(uint32 unit, RtkPort port, uint32 *latency);
EXTERN_API RtkApiRet cva_drv_ptp_ppsmaclatency_set(uint32 unit, uint32 latency);
EXTERN_API RtkApiRet cva_drv_ptp_ppsmaclatency_get(uint32 unit, uint32 *latency);
EXTERN_API RtkApiRet cva_drv_ptp_get_ppstrig_low_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet cva_drv_ptp_set_ppstrig_low_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet cva_drv_ptp_get_ppstrig_high_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet cva_drv_ptp_set_ppstrig_high_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet cva_drv_ptp_detectedfaults_get(uint32 unit, RtkPort port, uint8 *detectfaults);
EXTERN_API RtkApiRet cva_drv_ptp_set_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 ascapablecrosdomain);
EXTERN_API RtkApiRet cva_drv_ptp_get_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 *ascapablecrosdomain);
EXTERN_API RtkApiRet cva_drv_ptp_get_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 *mode);
EXTERN_API RtkApiRet cva_drv_ptp_set_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 mode);
EXTERN_API RtkApiRet cva_drv_ptp_hw_pdelay_domain_get(uint32 unit, RtkPort port, uint8 *domain);
EXTERN_API RtkApiRet cva_drv_ptp_parentlog_interval_get(uint32 unit, uint32 domain, uint32 *parentlog);
EXTERN_API RtkApiRet cva_drv_ptp_runtime_synclock_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncLockStatus);
EXTERN_API RtkApiRet cva_drv_ptp_runtime_syncslowdown_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncSlowdownStatus);
EXTERN_API RtkApiRet cva_drv_ptp_runtime_gptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval);
EXTERN_API RtkApiRet cva_drv_ptp_runtime_lastgptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval);
EXTERN_API RtkApiRet cva_drv_ptp_runtime_gptpslowdown_get(uint32 unit, RtkPort port, uint32 domain, uint8 *gptpSlowdown);
EXTERN_API RtkApiRet cva_drv_ptp_runtime_neigptpcapable_get(uint32 unit, RtkPort port, uint32 domain, uint8 *neiGptpCapable);
EXTERN_API RtkApiRet cva_drv_ptp_neigptp_rxtimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt);
EXTERN_API RtkApiRet cva_drv_ptp_neigptp_rxtimertarget_get(uint32 unit, RtkPort port, uint32 domain, uint32 *targetTime);
EXTERN_API RtkApiRet cva_drv_ptp_last_neigptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, int8 *logIntval);
EXTERN_API RtkApiRet cva_drv_ptp_rx_pdelayreq_cmds_get(uint32 unit, RtkPort port, uint8 *cmldsStatus);
EXTERN_API RtkApiRet cva_drv_ptp_rx_pdelay_domainnum_get(uint32 unit, RtkPort port, uint8 *pdelayDomainNum);
EXTERN_API RtkApiRet cva_drv_ptp_get_ts_polling_error_recover_count(uint32 unit, uint16 *recoverCnt);
EXTERN_API RtkApiRet cva_drv_ptp_set_ts_polling_error_recover_count(uint32 unit, uint16 recoverCnt);
EXTERN_API RtkApiRet cva_drv_ptp_gptpcapabletimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt);
EXTERN_API RtkApiRet cva_drv_ptp_check_reinitprotocol_done(uint32 unit, uint32 regval);
EXTERN_API RtkApiRet cva_drv_ptp_customize_get_in_glenable(uint32 unit, uint16 *ethertype, uint8 *da);
EXTERN_API RtkApiRet cva_drv_ptp_invalid_logintval(int8 logintval);
EXTERN_API RtkApiRet cva_drv_check_ptp_time_set_done(uint32 unit, uint32 regval);
RtkApiRet            cva_drv_ptp_if_rxlatency_get(PtpPortType_e interface, uint16 *latency);
RtkApiRet            cva_drv_ptp_if_txlatency_get(PtpPortType_e interface, uint16 *latency);
#endif
