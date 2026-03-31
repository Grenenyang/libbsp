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

#ifndef CVB_PTP_H
#define CVB_PTP_H
#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtl907xc_vb_api_ptp.h"

#include "rtkas_types.h"
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_table_struct.h"
#include "rtkas_debug.h"
#include "rtl907xc_vb_api_acl.h"
#include "rtl907xc_vb_asicdrv_acl.h"

#define CVB_PTP_POLLING_INTERVAL (1000u)
#define CVB_PTP_CFG_TYPE_NORMAL  (0u)
#define CVB_PTP_CFG_TYPE_MIB     (1u)
#define CVB_PTP_CFG_TYPE_LOG     (2u)
#define CVB_PTP_CFG_TYPE_TIME    (3u)
#define CVB_PTP_CFG_REINIT       (4u)
#define CVB_PTP_CFG_DEFAULT      (5u)

#define CVB_PTP_CFG_TIM_INC    (4u)
#define CVB_PTP_CFG_TIM_STABLE (2u)
#define CVB_PTP_CFG_TIM_DEC    (1u)

#define CVB_PTP_CFG_TIM_GET (3u)
#define CVB_PTP_CFG_REINIT  (4u)

#define CVB_PTP_STAMP_MAX_NS (1000000000u)
#define CVB_PTP_STAMP_SUB    (0u)
#define CVB_PTP_STAMP_ADD    (1u)
#define CVB_PTP_STAMP_COPY   (3u)
#define CVB_PTP_STAMP_RESET  (4u)
#define CVB_PPS_MAC_LATENCY  (58u)

#define CVB_ptppatch_PTP_CFG_ENABLETIMER (4u)

#define CVB_PTP_CTRL_CLK_TIME_CFG_SEL_MASTER (0u)
#define CVB_PTP_CTRL_CLK_TIME_CFG_SEL_LOCAL  (1u)
#define CVB_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE1 (2u)
#define CVB_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE2 (3u)
#define CVB_PTP_CTRL_CLK_TIME_CFG_SEL_SLAVE4 (4u)

/* PTP ACL Index */
#define CVB_PTP_ACL_IDX (545u)

/* PTP SDK TODO: Unnecessary define. Remove it when refine CLE. */
#define CVB_PTP_PATCH_SRAM (0x0u)
#define CVB_PTP_SRAM       (0x0u)
/****************************************************************/
/* Dummy Reg */
#define CVB_PTP_CFG_001 CVB_HW_MISC_DMY_RESIGSER_8
#define CVB_PTP_CFG_002 CVB_HW_MISC_DMY_RESIGSER_9
#define CVB_PTP_CFG_003 CVB_HW_MISC_DMY_RESIGSER_10
#define CVB_PTP_CFG_004 CVB_HW_MISC_DMY_RESIGSER_11

/* PtpStatus_t */
#define CVB_PTP_STATUS_SRAM (0x19FB60u)

#define CVB_PTPCFG_GLENABLE                                (CVB_PTP_STATUS_SRAM + 0x0u)
#define CVB_PTPCFG_CFGGM                                   (CVB_PTP_STATUS_SRAM + 0x1u)
#define CVB_PTPCFG_AUTOASENABLE                            (CVB_PTP_STATUS_SRAM + 0x2u)
#define CVB_PTPCFG_PPSENABLE                               (CVB_PTP_STATUS_SRAM + 0x11u)
#define CVB_PTPCFG_SIGENABLE                               (CVB_PTP_STATUS_SRAM + 0x13u)
#define CVB_PTPCFG_SLAVEPORTID                             (CVB_PTP_STATUS_SRAM + 0x14u)
#define CVB_PTPCFG_SLAVEPORTMASK                           (CVB_PTP_STATUS_SRAM + 0x18u)
#define CVB_PTPCFG_PRIORITY1                               (CVB_PTP_STATUS_SRAM + 0x1Au)
#define CVB_PTPCFG_SYNCRXTIMOUTTXSYNCEN                    (CVB_PTP_STATUS_SRAM + 0x1Du)
#define CVB_PTPCFG_ALLOWLOSTRESPNUM                        (CVB_PTP_STATUS_SRAM + 0x2Cu)
#define CVB_PTPCFG_DELAYTHRESH                             (CVB_PTP_STATUS_SRAM + 0x30u)
#define CVB_PTPCFG_INITLOGSYNCINTVAL                       (CVB_PTP_STATUS_SRAM + 0x3Cu)
#define CVB_PTPCFG_INITLOGPDELAYINTVAL                     (CVB_PTP_STATUS_SRAM + 0x69u)
#define CVB_PTPCFG_OPERLOGSYNCINTVAL                       (CVB_PTP_STATUS_SRAM + 0x78u)
#define CVB_PTPCFG_OPERLOGPDELAYINTVAL                     (CVB_PTP_STATUS_SRAM + 0x7Bu)
#define CVB_PTPCFG_SYNCRXTIMERNUMINTVAL                    (CVB_PTP_STATUS_SRAM + 0x8Au)
#define CVB_PTPCFG_SIGNALTIMERTARGET                       (CVB_PTP_STATUS_SRAM + 0x9Au)
#define CVB_PTPCFG_FISTBOOTUPTIMTARGET                     (CVB_PTP_STATUS_SRAM + 0xA0u)
#define CVB_PTPCFG_PORTENABLEMASK                          (CVB_PTP_STATUS_SRAM + 0xA2u)
#define CVB_PTPCFG_ASCAPABLEMASK                           (CVB_PTP_STATUS_SRAM + 0xA4u)
#define CVB_PTPCFG_CALDELAYENMASK                          (CVB_PTP_STATUS_SRAM + 0xAAu)
#define CVB_PTPCFG_CLKSTABLE                               (CVB_PTP_STATUS_SRAM + 0xACu)
#define CVB_PTPCFG_PID                                     (CVB_PTP_STATUS_SRAM + 0xB0u)
#define CVB_PTPCFG_DELAYCOMBGATE                           (CVB_PTP_STATUS_SRAM + 0xE0u)
#define CVB_PTPCFG_DOMAINMAP                               (CVB_PTP_STATUS_SRAM + 0xE2u)
#define CVB_PTPCFG_FLASHERASE                              (CVB_PTP_STATUS_SRAM + 0xE8u)
#define CVB_PTPCFG_ONESTEPTXSYNC                           (CVB_PTP_STATUS_SRAM + 0xE9u)
#define CVB_PTPCFG_ONESTEPRXSYNC                           (CVB_PTP_STATUS_SRAM + 0x116u)
#define CVB_PTPCFG_INITONESTEPTXOPER                       (CVB_PTP_STATUS_SRAM + 0x143u)
#define CVB_PTPCFG_TAG                                     (CVB_PTP_STATUS_SRAM + 0x170u)
#define CVB_PTPCFG_EXCHANGEINFO                            (CVB_PTP_STATUS_SRAM + 0x194u)
#define CVB_PTPCFG_CHECKAVNUIOL                            (CVB_PTP_STATUS_SRAM + 0x195u)
#define CVB_PTPCFG_SKIPLOADCFG                             (CVB_PTP_STATUS_SRAM + 0x196u)
#define CVB_PTPCFG_SEND2DOWNPORTS                          (CVB_PTP_STATUS_SRAM + 0x197u)
#define CVB_PTPCFG_CHECKRXTS                               (CVB_PTP_STATUS_SRAM + 0x198u)
#define CVB_PTPCFG_DASEL                                   (CVB_PTP_STATUS_SRAM + 0x1A7u)
#define CVB_PTPCFG_OPTIONDA                                (CVB_PTP_STATUS_SRAM + 0x1AAu)
#define CVB_PTPCFG_OPTIONETHERHEADER                       (CVB_PTP_STATUS_SRAM + 0x1B0u)
#define CVB_PTPCFG_ISOPTIONETHERHEADER                     (CVB_PTP_STATUS_SRAM + 0x1B2u)
#define CVB_PTPCFG_CUSTOMRECOVERYENABLE                    (CVB_PTP_STATUS_SRAM + 0x1B5u)
#define CVB_PTPCFG_TIMEOUTFACTOR                           (CVB_PTP_STATUS_SRAM + 0x1B8u)
#define CVB_PTPCFG_CALRATIOENMASK                          (CVB_PTP_STATUS_SRAM + 0x1BCu)
#define CVB_PTPCFG_PORTMODE                                (CVB_PTP_STATUS_SRAM + 0x1BEu)
#define CVB_PTPCFG_EMPTYDOMAINNOPDELAY                     (CVB_PTP_STATUS_SRAM + 0x1EBu)
#define CVB_PTPCFG_ADJ_DIFF_NS                             (CVB_PTP_STATUS_SRAM + 0x1ECu)
#define CVB_PTPCFG_ADJ_LOCK_TARGET_NS                      (CVB_PTP_STATUS_SRAM + 0x1F0u)
#define CVB_PTPCFG_ADJ_LOCK_STABLE_TIMES                   (CVB_PTP_STATUS_SRAM + 0x1F4u)
#define CVB_PTPCFG_REMAPPINGPORTNUM                        (CVB_PTP_STATUS_SRAM + 0x1F5u)
#define CVB_PTPCFG_ENABLE_PORTREMAPPING                    (CVB_PTP_STATUS_SRAM + 0x204u)
#define CVB_PTPCFG_SIP                                     (CVB_PTP_STATUS_SRAM + 0x205u)
#define CVB_PTPCFG_DIP                                     (CVB_PTP_STATUS_SRAM + 0x20Du)
#define CVB_PTPCFG_PDELAYDIP                               (CVB_PTP_STATUS_SRAM + 0x225u)
#define CVB_PTPCFG_SELL2L3                                 (CVB_PTP_STATUS_SRAM + 0x22Du)
#define CVB_PTPCFG_RATIO                                   (CVB_PTP_STATUS_SRAM + 0x230u)
#define CVB_PTPCFG_NEIRATIO                                (CVB_PTP_STATUS_SRAM + 0x2A8u)
#define CVB_PTPCFG_SYNCRCVD                                (CVB_PTP_STATUS_SRAM + 0x320u)
#define CVB_PTPCFG_ONESTEPSYNCRCVD                         (CVB_PTP_STATUS_SRAM + 0x32Fu)
#define CVB_PTPCFG_SYNCROUND                               (CVB_PTP_STATUS_SRAM + 0x332u)
#define CVB_PTPCFG_FOLLOWUPRCVD                            (CVB_PTP_STATUS_SRAM + 0x341u)
#define CVB_PTPCFG_RCVDPSSYNC                              (CVB_PTP_STATUS_SRAM + 0x350u)
#define CVB_PTPCFG_NEIWASUPDATED                           (CVB_PTP_STATUS_SRAM + 0x35Fu)
#define CVB_PTPCFG_FIRSTBOOTUP                             (CVB_PTP_STATUS_SRAM + 0x38Cu)
#define CVB_PTPCFG_LOSSOFSYNCFLAG                          (CVB_PTP_STATUS_SRAM + 0x39Bu)
#define CVB_PTPCFG_LASTSYNCSEQID                           (CVB_PTP_STATUS_SRAM + 0x3AAu)
#define CVB_PTPCFG_LASTFOLOUPCFADDDELAY                    (CVB_PTP_STATUS_SRAM + 0x3B0u)
#define CVB_PTPCFG_SYNCRESNOTGOOD                          (CVB_PTP_STATUS_SRAM + 0x464u)
#define CVB_PTPCFG_LASTSEQIDFROMHW                         (CVB_PTP_STATUS_SRAM + 0x474u)
#define CVB_PTPCFG_LASTSYNCINTS                            (CVB_PTP_STATUS_SRAM + 0x47Cu)
#define CVB_PTPCFG_LASTSYNCSLAVEIN                         (CVB_PTP_STATUS_SRAM + 0x4A0u)
#define CVB_PTPCFG_RELAYLASTSYNCINTS                       (CVB_PTP_STATUS_SRAM + 0x4C4u)
#define CVB_PTPCFG_RELAYLASTSYNCSLAVEINTS                  (CVB_PTP_STATUS_SRAM + 0x578u)
#define CVB_PTPCFG_RELAYLASTFOLOUPCFADDDELAY               (CVB_PTP_STATUS_SRAM + 0x59Cu)
#define CVB_PTPCFG_DOMAINNUM                               (CVB_PTP_STATUS_SRAM + 0x650u)
#define CVB_PTPCFG_OFFSETSIGN                              (CVB_PTP_STATUS_SRAM + 0x67Du)
#define CVB_PTPCFG_OFFSETFROMMASTER                        (CVB_PTP_STATUS_SRAM + 0x680u)
#define CVB_PTPCFG_MINMASTERSYNCINTVALPORTID               (CVB_PTP_STATUS_SRAM + 0x6A4u)
#define CVB_PTPCFG_CURNTSYNCINTVAL                         (CVB_PTP_STATUS_SRAM + 0x6A8u)
#define CVB_PTPCFG_CURNTPDELAYREQINTVAL                    (CVB_PTP_STATUS_SRAM + 0x75Cu)
#define CVB_PTPCFG_PPSTRIGTIMER                            (CVB_PTP_STATUS_SRAM + 0x798u)
#define CVB_PTPCFG_PPSTRIGTIMERTARGET                      (CVB_PTP_STATUS_SRAM + 0x7A0u)
#define CVB_PTPCFG_PPSTRIGADJ                              (CVB_PTP_STATUS_SRAM + 0x7A8u)
#define CVB_PTPCFG_SYNCRXTIMER                             (CVB_PTP_STATUS_SRAM + 0x7ACu)
#define CVB_PTPCFG_SYNCRXTIMERTARGET                       (CVB_PTP_STATUS_SRAM + 0x7B8u)
#define CVB_PTPCFG_FOLLOWUPTIMER                           (CVB_PTP_STATUS_SRAM + 0x7C4u)
#define CVB_PTPCFG_SIGNALTIMER                             (CVB_PTP_STATUS_SRAM + 0x7D0u)
#define CVB_PTPCFG_SYNCTIMER                               (CVB_PTP_STATUS_SRAM + 0x7DCu)
#define CVB_PTPCFG_PDELAYREQTIMER                          (CVB_PTP_STATUS_SRAM + 0x890u)
#define CVB_PTPCFG_PORTACTIVEMASK                          (CVB_PTP_STATUS_SRAM + 0x8CCu)
#define CVB_PTPCFG_PORTMACORPHYMASK                        (CVB_PTP_STATUS_SRAM + 0x8CEu)
#define CVB_PTPCFG_LINKDNANDUPMASK                         (CVB_PTP_STATUS_SRAM + 0x8D0u)
#define CVB_PTPCFG_RCVDPSSYNCPERPORTMASK                   (CVB_PTP_STATUS_SRAM + 0x8D2u)
#define CVB_PTPCFG_INITPDELAYMASK                          (CVB_PTP_STATUS_SRAM + 0x8D8u)
#define CVB_PTPCFG_PDELAYSTABLEMASK                        (CVB_PTP_STATUS_SRAM + 0x8DAu)
#define CVB_PTPCFG_PDELAYRESPRCVDMASK                      (CVB_PTP_STATUS_SRAM + 0x8DCu)
#define CVB_PTPCFG_PDELAYREQROUNDMASK                      (CVB_PTP_STATUS_SRAM + 0x8DEu)
#define CVB_PTPCFG_PDELAYTIMOUTLOGFLAGMASK                 (CVB_PTP_STATUS_SRAM + 0x8E0u)
#define CVB_PTPCFG_SAVEDFLASHMASK                          (CVB_PTP_STATUS_SRAM + 0x8E2u)
#define CVB_PTPCFG_LASTSIGNALSEQID                         (CVB_PTP_STATUS_SRAM + 0x8E4u)
#define CVB_PTPCFG_LASTRCVDPDELAYSEQID                     (CVB_PTP_STATUS_SRAM + 0x902u)
#define CVB_PTPCFG_LOSTRESPADDER                           (CVB_PTP_STATUS_SRAM + 0x920u)
#define CVB_PTPCFG_SYNCSEQIDADDER                          (CVB_PTP_STATUS_SRAM + 0x930u)
#define CVB_PTPCFG_PDELAYSEQIDADDER                        (CVB_PTP_STATUS_SRAM + 0x98Au)
#define CVB_PTPCFG_SIGNALSEQID                             (CVB_PTP_STATUS_SRAM + 0x9A8u)
#define CVB_PTPCFG_T1DELAY                                 (CVB_PTP_STATUS_SRAM + 0xA04u)
#define CVB_PTPCFG_T2DELAY                                 (CVB_PTP_STATUS_SRAM + 0xAB8u)
#define CVB_PTPCFG_T3DELAY                                 (CVB_PTP_STATUS_SRAM + 0xB6Cu)
#define CVB_PTPCFG_T4DELAY                                 (CVB_PTP_STATUS_SRAM + 0xC20u)
#define CVB_PTPCFG_NEIDELAY                                (CVB_PTP_STATUS_SRAM + 0xCD4u)
#define CVB_PTPCFG_ACCESSFLASH                             (CVB_PTP_STATUS_SRAM + 0xD88u)
#define CVB_PTPCFG_PDELAYTHRESHOLD                         (CVB_PTP_STATUS_SRAM + 0xD8Cu)
#define CVB_PTPCFG_LOCKFLASHSTATUS                         (CVB_PTP_STATUS_SRAM + 0xD90u)
#define CVB_PTPCFG_LASTPDELAYRESPT3                        (CVB_PTP_STATUS_SRAM + 0xD94u)
#define CVB_PTPCFG_LASTPDELAYRESPT4                        (CVB_PTP_STATUS_SRAM + 0xE48u)
#define CVB_PTPCFG_SLDELAYFISTCTRL                         (CVB_PTP_STATUS_SRAM + 0xEFCu)
#define CVB_PTPCFG_SLDELAYFISTSET                          (CVB_PTP_STATUS_SRAM + 0xEFFu)
#define CVB_PTPCFG_FINETUADJ                               (CVB_PTP_STATUS_SRAM + 0xF02u)
#define CVB_PTPCFG_RATEVAL                                 (CVB_PTP_STATUS_SRAM + 0xF08u)
#define CVB_PTPCFG_DELAYSTABLETIMES                        (CVB_PTP_STATUS_SRAM + 0xF14u)
#define CVB_PTPCFG_DELAYFILADDER                           (CVB_PTP_STATUS_SRAM + 0xF24u)
#define CVB_PTPCFG_LASTDELAY                               (CVB_PTP_STATUS_SRAM + 0xF44u)
#define CVB_PTPCFG_DELAYFILTERVAL                          (CVB_PTP_STATUS_SRAM + 0xFF8u)
#define CVB_PTPCFG_LASTSUCCESSFULSYN                       (CVB_PTP_STATUS_SRAM + 0x1034u)
#define CVB_PTPCFG_LASTSUCCESSFULPATHDELY                  (CVB_PTP_STATUS_SRAM + 0x1064u)
#define CVB_PTPCFG_NEEDRECOVERYSYNC                        (CVB_PTP_STATUS_SRAM + 0x1154u)
#define CVB_PTPCFG_SEAMLESS_ORIGINALTS                     (CVB_PTP_STATUS_SRAM + 0x1158u)
#define CVB_PTPCFG_SEAMLESS_LASTFOLOUPCFADDDELAY           (CVB_PTP_STATUS_SRAM + 0x11A0u)
#define CVB_PTPCFG_SEAMLESS_LASTSYNCSLAVEIN                (CVB_PTP_STATUS_SRAM + 0x11E8u)
#define CVB_PTPCFG_EXPLOG                                  (CVB_PTP_STATUS_SRAM + 0x1230u)
#define CVB_PTPCFG_URGETTXINFO                             (CVB_PTP_STATUS_SRAM + 0x18A8u)
#define CVB_PTPCFG_TXTIMESUPTHRESHOLD                      (CVB_PTP_STATUS_SRAM + 0x18B8u)
#define CVB_PTPCFG_TXTIMESUPCNT                            (CVB_PTP_STATUS_SRAM + 0x18BCu)
#define CVB_SIGENABLE_D1D2                                 (CVB_PTP_STATUS_SRAM + 0x18C0u)
#define CVB_ASSIGNTXPRI_ENABLE                             (CVB_PTP_STATUS_SRAM + 0x18C2u)
#define CVB_PTP_PRIORITY                                   (CVB_PTP_STATUS_SRAM + 0x18C4u)
#define CVB_PTPCFG_NORECOVERYFIRSTBOOT                     (CVB_PTP_STATUS_SRAM + 0x18C5u)
#define CVB_PTPCFG_SILENCEIFNEVERSYNCED                    (CVB_PTP_STATUS_SRAM + 0x18C6u)
#define CVB_PTPCFG_USEMYCLOCKID                            (CVB_PTP_STATUS_SRAM + 0x18C9u)
#define CVB_PTPCFG_PDELAYTOMASTERPORT                      (CVB_PTP_STATUS_SRAM + 0x18CAu)
#define CVB_GOTSYNC_D0                                     (CVB_PTP_STATUS_SRAM + 0x18CBu)
#define CVB_PTPCFG_SYNCLOCKSWITCH                          (CVB_PTP_STATUS_SRAM + 0x18CEu)
#define CVB_PTPCFG_SYNCSLOWDOWNSWITCH                      (CVB_PTP_STATUS_SRAM + 0x18CFu)
#define CVB_PTPCFG_TXLATENCY                               (CVB_PTP_STATUS_SRAM + 0x18D0u)
#define CVB_PTPCFG_RXLATENCY                               (CVB_PTP_STATUS_SRAM + 0x190Cu)
#define CVB_PTPCFG_SEAMLESS1EN                             (CVB_PTP_STATUS_SRAM + 0x1948u)
#define CVB_PTPCFG_SEAMLESS1CHKPORTDOMAIN                  (CVB_PTP_STATUS_SRAM + 0x194Cu)
#define CVB_PTPCFG_SEAMLESS2EN                             (CVB_PTP_STATUS_SRAM + 0x1950u)
#define CVB_PTPCFG_SEAMLESS2CHKPORTDOMAIN                  (CVB_PTP_STATUS_SRAM + 0x1954u)
#define CVB_PTPCFG_PPSMACLATENCY                           (CVB_PTP_STATUS_SRAM + 0x1958u)
#define CVB_PTPCFG_PPSTRIGLOWTHRESHOLD                     (CVB_PTP_STATUS_SRAM + 0x195Cu)
#define CVB_PTPCFG_PPSTRIGHIGHTHRESHOLD                    (CVB_PTP_STATUS_SRAM + 0x1960u)
#define CVB_PTPCFG_ALLOWEDFAULTS                           (CVB_PTP_STATUS_SRAM + 0x1964u)
#define CVB_PTPCFG_DETECTEDFAULTS                          (CVB_PTP_STATUS_SRAM + 0x1973u)
#define CVB_PTPCFG_ASCAPABLEACROSSDOMAINS                  (CVB_PTP_STATUS_SRAM + 0x1982u)
#define CVB_PTPCFG_ASYMMETRYMEASUREMENTMODE                (CVB_PTP_STATUS_SRAM + 0x1991u)
#define CVB_PTPCFG_DELAYMEASUREMECH                        (CVB_PTP_STATUS_SRAM + 0x19A0u)
#define CVB_PTPCFG_DELAYASYMMETRY                          (CVB_PTP_STATUS_SRAM + 0x19A4u)
#define CVB_PTPCFG_RXDELAYASYMMETRY                        (CVB_PTP_STATUS_SRAM + 0x1A58u)
#define CVB_PTPCFG_HWPDELAYDOMAIN                          (CVB_PTP_STATUS_SRAM + 0x1B0Cu)
#define CVB_PTPCFG_PARENTLOGSYNCINTERVAL                   (CVB_PTP_STATUS_SRAM + 0x1B1Cu)
#define CVB_PTPCFG_SYNCLOCKED                              (CVB_PTP_STATUS_SRAM + 0x1B28u)
#define CVB_PTPCFG_SYNCSLOWDOWN                            (CVB_PTP_STATUS_SRAM + 0x1B55u)
#define CVB_PTPCFG_OLDSYNCINTERVAL                         (CVB_PTP_STATUS_SRAM + 0x1B84u)
#define CVB_PTPCFG_USEMGTSETTABLELOGSYNCINTERVAL           (CVB_PTP_STATUS_SRAM + 0x1C38u)
#define CVB_PTPCFG_MGTSETTABLELOGSYNCINTERVAL              (CVB_PTP_STATUS_SRAM + 0x1C3Bu)
#define CVB_PTPCFG_USEMGTSETTABLELOGPDELAYREQINTERVAL      (CVB_PTP_STATUS_SRAM + 0x1C3Eu)
#define CVB_PTPCFG_MGTSETTABLELOGPDELAYREQINTERVAL         (CVB_PTP_STATUS_SRAM + 0x1C4Du)
#define CVB_PTPCFG_USEMGTSETTABLECOMPUTENEIGHBORRATERATIO  (CVB_PTP_STATUS_SRAM + 0x1C5Cu)
#define CVB_PTPCFG_MGTSETTABLECOMPUTENEIGHBORRATERATIO     (CVB_PTP_STATUS_SRAM + 0x1C6Bu)
#define CVB_PTPCFG_USEMGTSETTABLECOMPUTEMEANLINKDELAY      (CVB_PTP_STATUS_SRAM + 0x1C7Au)
#define CVB_PTPCFG_MGTSETTABLECOMPUTEMEANLINKDELAY         (CVB_PTP_STATUS_SRAM + 0x1C89u)
#define CVB_PTPCFG_USEMGTSETTABLEONESTEPTXOPER             (CVB_PTP_STATUS_SRAM + 0x1C98u)
#define CVB_PTPCFG_MGTSETTABLEONESTEPTXOPER                (CVB_PTP_STATUS_SRAM + 0x1CA7u)
#define CVB_PTPCFG_USEMGTSETTABLELOGGPTPCAPABLEMSGINTERVAL (CVB_PTP_STATUS_SRAM + 0x1CD4u)
#define CVB_PTPCFG_MGTSETTABLELOGGPTPCAPABLEMSGINTERVAL    (CVB_PTP_STATUS_SRAM + 0x1CE3u)
#define CVB_PTPCFG_INITLOGGPTPCAPABLEMSGINTVAL             (CVB_PTP_STATUS_SRAM + 0x1D10u)
#define CVB_PTPCFG_GPTPCAPABLEMSGINTERVAL                  (CVB_PTP_STATUS_SRAM + 0x1D40u)
#define CVB_PTPCFG_OLDGPTPCAPABLEMSGINTERVAL               (CVB_PTP_STATUS_SRAM + 0x1DF4u)
#define CVB_PTPCFG_GPTPSLOWDOWN                            (CVB_PTP_STATUS_SRAM + 0x1EA8u)
#define CVB_PTPCFG_NEIGHBORGPTPCAPABLE                     (CVB_PTP_STATUS_SRAM + 0x1ED5u)
#define CVB_PTPCFG_NEIGHBORGPTPRXTIMER                     (CVB_PTP_STATUS_SRAM + 0x1F04u)
#define CVB_PTPCFG_NEIGHBORGPTPRXTIMERTARGET               (CVB_PTP_STATUS_SRAM + 0x1FB8u)
#define CVB_PTPCFG_NEIGHBORGPTPRXTIMEOUT                   (CVB_PTP_STATUS_SRAM + 0x206Cu)
#define CVB_PTPCFG_LATESTNEIGHGPTPRXINTERVALLOG            (CVB_PTP_STATUS_SRAM + 0x207Bu)
#define CVB_PTPCFG_RXCMLDSPDELAYREQ                        (CVB_PTP_STATUS_SRAM + 0x20A8u)
#define CVB_PTPCFG_RXPDELAYREQDOMAINNUM                    (CVB_PTP_STATUS_SRAM + 0x20B7u)
#define CVB_PTPCFG_AS2020                                  (CVB_PTP_STATUS_SRAM + 0x20C6u)
#define CVB_PTPCFG_GPTPCAPABLETIMER                        (CVB_PTP_STATUS_SRAM + 0x20C8u)
#define CVB_PTPCFG_POLLINGERRORRECOVERCNT                  (CVB_PTP_STATUS_SRAM + 0x217Cu)

#define CVB_PTP_ISREADY_ADDRESS (0x001B2650u)

/*Black magic address. It may changes*/
#define CVB_BM_PTP_PID0_kP            (0x48234u)
#define CVB_BM_PTP_PID1_kP            (0x48244u)
#define CVB_BM_PTP_PID2_kP            (0x48254u)
#define CVB_BM_PTP_DELAYTHRESH_NS     (0x481C0u)
#define CVB_BM_PTP_AS_ENABLE_PORTMASK (0x4822Au)
#define CVB_BM_PTP_START              (0x48188u)

/* CVB_CHK_FUN_INVALID_RET */
#define CVB_CHK_FUN_INVALID_RET(r, f)                                     \
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
} cvb_taicfg_t;

/* DomainPortMask:
 * D0:bit0 ~ bit15, D1:bit16 ~ bit31, D2:bit32 ~ bit47 */
typedef struct
{
    uint32 remainTxTime;         /**< Remain tx time of next ptp packet. (0 to 0xFFFFFFFF) */
    uint32 remainTxTimeBuffer;   /**< Remain Tx time buffer (0 to 0xFFFFFFFF) */
    uint64 urgentDomainPortMask; /**< Tx Port mask of next ptp packet. */
} cvb_ptpUrgentTxInfo_t;

extern uint32 cvb_g_ptpTimeConfigTimeout;
extern uint32 cvb_g_ptpControlClkConfigTimeout;
extern uint32 cvb_g_ptpConfigMultipleTimeout;
extern uint32 cvb_g_ptpConfigTimeout;
extern uint32 cvb_g_ptpPortTXRXTSTimeout;
extern uint32 cvb_g_ptpTaiConfigTimeout;

EXTERN_API RtkApiRet cvb_drv_ptp_time_direct_set(uint32 unit, uint8 timesel, PtpTs_t ts);
EXTERN_API RtkApiRet cvb_drv_ptp_time_get(uint32 unit, uint8 timesel, PtpTs_t *ts);
EXTERN_API RtkApiRet cvb_drv_ptp_time_set(uint32 unit, uint8 timesel, uint8 timeoper, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cvb_drv_ptp_tai_cfg_set(uint32 unit, uint8 ppsidx, cvb_taicfg_t cfg);
EXTERN_API RtkApiRet cvb_drv_ptp_tai_cfg_get(uint32 unit, uint8 ppsidx, cvb_taicfg_t *cfg);
EXTERN_API RtkApiRet cvb_drv_ptp_tai_run(uint32 unit, uint8 ppsidx);
EXTERN_API RtkApiRet cvb_drv_ptp_tai_done(uint32 unit, uint8 ppsidx, uint8 *ishwRunning);
EXTERN_API RtkApiRet cvb_drv_ptp_tai_get_time(uint32 unit, uint8 ppsidx, PtpTs_t *pts);
EXTERN_API RtkApiRet cvb_drv_ptp_tai_notified(uint32 unit, uint8 ppsidx, uint8 *isnotified);
EXTERN_API RtkApiRet cvb_drv_ptp_get_port_tx_rx(uint32 unit, RtkPort port, uint8 domain, uint8 msgtype, uint8 txrxsel, PtpTs_t *ts);
EXTERN_API RtkApiRet cvb_drv_ptp_get_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 *ptrxts_overwr_en);
EXTERN_API RtkApiRet cvb_drv_ptp_set_port_trxts_overwr_en(uint32 unit, RtkPort port, uint8 trxts_overwr_en);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_rx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_rx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_rx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_tx_presonse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_rx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_tx_sync_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_get_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 *mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_one_step_sync_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_tx_follow_up_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_tx_prequest_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_tx_presponse_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_tx_presp_follow_up(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_sync_receipt_timeout_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_rx_ptp_packet_discard_cont(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_mib_set_pdelay_allowed_lost_response_count(uint32 unit, RtkPort port, uint32 domainid, uint32 mib);
EXTERN_API RtkApiRet cvb_drv_ptp_expLog_loss_of_sync_get(uint32 unit, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet cvb_drv_ptp_set_explog_loss_of_sync(uint32 unit, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cvb_drv_ptp_expLog_link_dn_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet cvb_drv_ptp_set_explog_link_dn_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cvb_drv_ptp_expLog_link_up_Log_get(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet cvb_drv_ptp_set_explog_link_up_log(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cvb_drv_ptp_get_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t *ts);
EXTERN_API RtkApiRet cvb_drv_ptp_set_explog_pdelay_resp_timeout(uint32 unit, RtkPort port, uint32 domainid, PtpTs_t const *const ts);
EXTERN_API RtkApiRet cvb_drv_ptp_get_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 *u8data);
EXTERN_API RtkApiRet cvb_drv_ptp_set_syncrxtimouttxsyncen(uint32 unit, RtkPort port, uint8 u8data);
EXTERN_API RtkApiRet cvb_drv_ptp_allowlostrespnum_get(uint32 unit, uint8 *u8data);
EXTERN_API RtkApiRet cvb_drv_ptp_allowlostrespnum_set(uint32 unit, uint8 u8data);
EXTERN_API RtkApiRet cvb_drv_ptp_protocolPortmask_get(uint32 unit, uint32 domainid, uint16 *portmask);
EXTERN_API RtkApiRet cvb_drv_ptp_onesteptxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepTxSync);
EXTERN_API int8      cvb_drv_ptp_ts_cmp(const PtpTs_t *a, const PtpTs_t *b);
EXTERN_API void      cvb_drv_ptp_ts_trim(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime);
EXTERN_API void      cvb_drv_ptp_ts_trim_sub(uint8 const modiType, PtpTs_t *oriTime, PtpTs_t *subTime);
EXTERN_API uint8     cvb_drv_ptp_invalid_ptpdomainid(uint32 domainid);
EXTERN_API RtkApiRet cvb_drv_ptp_acl_redirect_set(uint32 unit, uint8 isEnabled, uint16 ethertype, uint8 const *const DA, uint16 spmask, uint8 priority, uint32 redirectPortMsk);
EXTERN_API RtkApiRet cvb_drv_ptp_acl_redirect_get(uint32 unit, uint8 *isEnabled, uint16 *ethertype, uint8 *DA, uint16 *spmask, uint8 *priority, uint32 *redirectPortMsk);
EXTERN_API RtkApiRet cvb_drv_ptp_onesteprxen_sw_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *oneStepRxSync);
EXTERN_API RtkApiRet cvb_drv_ptp_urgentTXInfo_get(uint32 unit, cvb_ptpUrgentTxInfo_t *urgentTXInfo);
EXTERN_API RtkApiRet cvb_drv_ptp_urgentTXTimeBuffer_set(uint32 unit, uint32 txTimeBuffer);
EXTERN_API RtkApiRet cvb_drv_ptp_set_txtimesup_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet cvb_drv_ptp_get_txtimesup_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet cvb_drv_ptp_tx_times_up_cnt_set(uint32 unit, uint32 count);
EXTERN_API RtkApiRet cvb_drv_ptp_tx_times_up_cnt_get(uint32 unit, uint32 *count);
EXTERN_API RtkApiRet cvb_drv_ptp_txLatency_set(uint32 unit, RtkPort port, uint32 latency);
EXTERN_API RtkApiRet cvb_drv_ptp_txLatency_get(uint32 unit, RtkPort port, uint32 *latency);
EXTERN_API RtkApiRet cvb_drv_ptp_rxLatency_set(uint32 unit, RtkPort port, uint32 latency);
EXTERN_API RtkApiRet cvb_drv_ptp_rxLatency_get(uint32 unit, RtkPort port, uint32 *latency);
EXTERN_API RtkApiRet cvb_drv_ptp_ppsmaclatency_set(uint32 unit, uint32 latency);
EXTERN_API RtkApiRet cvb_drv_ptp_ppsmaclatency_get(uint32 unit, uint32 *latency);
EXTERN_API RtkApiRet cvb_drv_ptp_get_ppstrig_low_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet cvb_drv_ptp_set_ppstrig_low_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet cvb_drv_ptp_get_ppstrig_high_threshold(uint32 unit, uint32 *threshold);
EXTERN_API RtkApiRet cvb_drv_ptp_set_ppstrig_high_threshold(uint32 unit, uint32 threshold);
EXTERN_API RtkApiRet cvb_drv_ptp_detectedfaults_get(uint32 unit, RtkPort port, uint8 *detectfaults);
EXTERN_API RtkApiRet cvb_drv_ptp_set_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 ascapablecrosdomain);
EXTERN_API RtkApiRet cvb_drv_ptp_get_ascapablecrossdomains(uint32 unit, RtkPort port, uint8 *ascapablecrosdomain);
EXTERN_API RtkApiRet cvb_drv_ptp_get_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 *mode);
EXTERN_API RtkApiRet cvb_drv_ptp_set_asymmetry_measure_mode(uint32 unit, RtkPort port, uint8 mode);
EXTERN_API RtkApiRet cvb_drv_ptp_hw_pdelay_domain_get(uint32 unit, RtkPort port, uint8 *domain);
EXTERN_API RtkApiRet cvb_drv_ptp_parentlog_interval_get(uint32 unit, uint32 domain, uint32 *parentlog);
EXTERN_API RtkApiRet cvb_drv_ptp_runtime_synclock_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncLockStatus);
EXTERN_API RtkApiRet cvb_drv_ptp_runtime_syncslowdown_get(uint32 unit, RtkPort port, uint32 domainid, uint8 *rtSyncSlowdownStatus);
EXTERN_API RtkApiRet cvb_drv_ptp_runtime_gptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval);
EXTERN_API RtkApiRet cvb_drv_ptp_runtime_lastgptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, uint32 *intval);
EXTERN_API RtkApiRet cvb_drv_ptp_runtime_gptpslowdown_get(uint32 unit, RtkPort port, uint32 domain, uint8 *gptpSlowdown);
EXTERN_API RtkApiRet cvb_drv_ptp_runtime_neigptpcapable_get(uint32 unit, RtkPort port, uint32 domain, uint8 *neiGptpCapable);
EXTERN_API RtkApiRet cvb_drv_ptp_neigptp_rxtimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt);
EXTERN_API RtkApiRet cvb_drv_ptp_neigptp_rxtimertarget_get(uint32 unit, RtkPort port, uint32 domain, uint32 *targetTime);
EXTERN_API RtkApiRet cvb_drv_ptp_last_neigptpmsgintval_get(uint32 unit, RtkPort port, uint32 domain, int8 *logIntval);
EXTERN_API RtkApiRet cvb_drv_ptp_rx_pdelayreq_cmds_get(uint32 unit, RtkPort port, uint8 *cmldsStatus);
EXTERN_API RtkApiRet cvb_drv_ptp_rx_pdelay_domainnum_get(uint32 unit, RtkPort port, uint8 *pdelayDomainNum);
EXTERN_API RtkApiRet cvb_drv_ptp_get_ts_polling_error_recover_count(uint32 unit, uint16 *recoverCnt);
EXTERN_API RtkApiRet cvb_drv_ptp_set_ts_polling_error_recover_count(uint32 unit, uint16 recoverCnt);
EXTERN_API RtkApiRet cvb_drv_ptp_gptpcapabletimer_get(uint32 unit, RtkPort port, uint32 domain, uint32 *timerCnt);
EXTERN_API RtkApiRet cvb_drv_ptp_check_reinitprotocol_done(uint32 unit, uint32 regval);
EXTERN_API RtkApiRet cvb_drv_ptp_customize_get_in_glenable(uint32 unit, uint16 *ethertype, uint8 *da);
EXTERN_API RtkApiRet cvb_drv_ptp_invalid_logintval(int8 logintval);
EXTERN_API RtkApiRet cvb_drv_check_ptp_time_set_done(uint32 unit, uint32 regval);
RtkApiRet            cvb_drv_ptp_if_rxlatency_get(PtpPortType_e interface, uint16 *latency);
RtkApiRet            cvb_drv_ptp_if_txlatency_get(PtpPortType_e interface, uint16 *latency);
#endif
