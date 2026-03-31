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

#ifndef RTKAS_ERROR_H
#define RTKAS_ERROR_H

/*
 * Include Files
 */
#include "rtkas_common_types.h"

EXTERN_API const char *rtk_errMsg_get(int32 errCode);
/*
 * Data Type Declaration
 */
#define RT_ERR_OK                                             (0x0)
#define RT_ERR_FAILED                                         (0x00000001)
#define RT_ERR_INPUT                                          (0x00000002)
#define RT_ERR_UNIT_ID                                        (0x00000003)
#define RT_ERR_PORT_ID                                        (0x00000004)
#define RT_ERR_PORT_MASK                                      (0x00000005)
#define RT_ERR_PORT_LINKDOWN                                  (0x00000006)
#define RT_ERR_ENTRY_INDEX                                    (0x00000007)
#define RT_ERR_NULL_POINTER                                   (0x00000008)
#define RT_ERR_QUEUE_ID                                       (0x00000009)
#define RT_ERR_QUEUE_NUM                                      (0x0000000A)
#define RT_ERR_BUSYWAIT_TIMEOUT                               (0x0000000B)
#define RT_ERR_MAC                                            (0x0000000C)
#define RT_ERR_OUT_OF_RANGE                                   (0x0000000D)
#define RT_ERR_CHIP_NOT_SUPPORTED                             (0x0000000E)
#define RT_ERR_SMI                                            (0x0000000F)
#define RT_ERR_NOT_INIT                                       (0x00000010)
#define RT_ERR_CHIP_NOT_FOUND                                 (0x00000011)
#define RT_ERR_NOT_ALLOWED                                    (0x00000012)
#define RT_ERR_DRIVER_NOT_FOUND                               (0x00000013)
#define RT_ERR_SEM_LOCK_FAILED                                (0x00000014)
#define RT_ERR_SEM_UNLOCK_FAILED                              (0x00000015)
#define RT_ERR_ENABLE                                         (0x00000016)
#define RT_ERR_TBL_FULL                                       (0x00000017)
#define RT_ERR_NO_ACK                                         (0x00000018)
#define RT_ERR_PORT_MODE                                      (0x00000019)
#define RT_ERR_IC_PART_NUM                                    (0x0000001A)
#define RT_ERR_MEM_ACCESS                                     (0x0000001B)
#define RT_ERR_I2C                                            (0x0000001C)
#define RT_ERR_SPI                                            (0x0000001D)
#define RT_ERR_ETH_ACCESS                                     (0x0000001E)
#define RT_ERR_SEC_DLY                                        (0x0000001F)
#define RT_ERR_FILE_READ                                      (0x00000020)
#define RT_ERR_TIMER_GET_CUR_ERROR                            (0x00000022)
#define RT_ERR_LOWER_BOUND_FAILED                             (0x00000023)
#define RT_ERR_NOT_SUPPORT                                    (0x000000EE)
#define RT_ERR_VLAN_VID                                       (0x00010000)
#define RT_ERR_VLAN_PRIORITY                                  (0x00010001)
#define RT_ERR_VLAN_EMPTY_ENTRY                               (0x00010002)
#define RT_ERR_VLAN_ACCEPT_FRAME_TYPE                         (0x00010003)
#define RT_ERR_VLAN_EXIST                                     (0x00010004)
#define RT_ERR_VLAN_ENTRY_NOT_FOUND                           (0x00010005)
#define RT_ERR_VLAN_PORT_MBR_EXIST                            (0x00010006)
#define RT_ERR_VLAN_PROTO_AND_PORT                            (0x00010007)
#define RT_ERR_VLAN_ING_RETAG_IDX                             (0x00010008)
#define RT_ERR_VLAN_EGR_RETAG_IDX                             (0x00010009)
#define RT_ERR_SVLAN_ENTRY_INDEX                              (0x00020000)
#define RT_ERR_SVLAN_ETHER_TYPE                               (0x00020001)
#define RT_ERR_SVLAN_TABLE_FULL                               (0x00020002)
#define RT_ERR_SVLAN_ENTRY_NOT_FOUND                          (0x00020003)
#define RT_ERR_SVLAN_EXIST                                    (0x00020004)
#define RT_ERR_SVLAN_VID                                      (0x00020005)
#define RT_ERR_MSTI                                           (0x00030000)
#define RT_ERR_MSTP_STATE                                     (0x00030001)
#define RT_ERR_MSTI_EXIST                                     (0x00030002)
#define RT_ERR_MSTI_NOT_EXIST                                 (0x00030003)
#define RT_ERR_STP_BRIDGE_PRIORITY                            (0x00030004)
#define RT_ERR_STP_FORWARDDELAY                               (0x00030005)
#define RT_ERR_STP_MAXAGE                                     (0x00030006)
#define RT_ERR_STP_HELLOTIME                                  (0x00030007)
#define RT_ERR_STP_PORT_PRIORITY                              (0x00030008)
#define RT_ERR_STP_PORT_PATH_COST                             (0x00030009)
#define RT_ERR_STP_PARAMETER_CHANGE_INT_CLEAR                 (0x0003000A)
#define RT_ERR_STP_PARAMETER_CHANGE_INT_DONE                  (0x0003000B)
#define RT_ERR_TIMESLOT                                       (0x00040000)
#define RT_ERR_TOKEN                                          (0x00040001)
#define RT_ERR_RATE                                           (0x00040002)
#define RT_ERR_TICK                                           (0x00040003)
#define RT_ERR_RMA_ADDR                                       (0x00050000)
#define RT_ERR_RMA_ACTION                                     (0x00050001)
#define RT_ERR_L2_HASH_KEY                                    (0x00060000)
#define RT_ERR_L2_HASH_INDEX                                  (0x00060001)
#define RT_ERR_L2_CAM_INDEX                                   (0x00060002)
#define RT_ERR_L2_ENRTYSEL                                    (0x00060003)
#define RT_ERR_L2_INDEXTABLE_INDEX                            (0x00060004)
#define RT_ERR_LIMITED_L2ENTRY_NUM                            (0x00060005)
#define RT_ERR_L2_AGGREG_PORT                                 (0x00060006)
#define RT_ERR_L2_FID                                         (0x00060007)
#define RT_ERR_L2_RVID                                        (0x00060008)
#define RT_ERR_L2_NO_EMPTY_ENTRY                              (0x00060009)
#define RT_ERR_L2_ENTRY_NOTFOUND                              (0x0006000A)
#define RT_ERR_L2_INDEXTBL_FULL                               (0x0006000B)
#define RT_ERR_L2_INVALID_FLOWTYPE                            (0x0006000C)
#define RT_ERR_L2_L2UNI_PARAM                                 (0x0006000D)
#define RT_ERR_L2_L2MULTI_PARAM                               (0x0006000E)
#define RT_ERR_L2_IPMULTI_PARAM                               (0x0006000F)
#define RT_ERR_L2_PARTIAL_HASH_KEY                            (0x00060010)
#define RT_ERR_L2_EMPTY_ENTRY                                 (0x00060011)
#define RT_ERR_L2_FLUSH_TYPE                                  (0x00060012)
#define RT_ERR_L2_NO_CPU_PORT                                 (0x00060013)
#define RT_ERR_ACL_BLOCKNUM                                   (0x00070000)
#define RT_ERR_ACL_ENTRYIDX                                   (0x00070001)
#define RT_ERR_ACL_TGIDX                                      (0x00070002)
#define RT_ERR_ACL_INT_TGIDX                                  (0x00070003)
#define RT_ERR_ACL_AGG_OP                                     (0x00070004)
#define RT_ERR_ACL_ANY_OFFSET_IDX                             (0x00070005)
#define RT_ERR_ACL_ANY_OFFSET_TYPE                            (0x00070006)
#define RT_ERR_ACL_ANY_OFFSET_POSITION                        (0x00070007)
#define RT_ERR_ACL_RANGE_CHK_IDX                              (0x00070008)
#define RT_ERR_ACL_RANGE_CHK_TYPE                             (0x00070009)
#define RT_ERR_ACL_RANGE_CHK_BOUND                            (0x0007000A)
#define RT_ERR_ACL_RANGE_ERROR                                (0x0007000B)
#define RT_ERR_ACL_TIME_SLOT_THRESHOLD                        (0x0007000C)
#define RT_ERR_ACL_ENABLE                                     (0x0007000D)
#define RT_ERR_ACL_DEF_ACT                                    (0x0007000E)
#define RT_ERR_ACL_POLICER_RATE                               (0x0007000F)
#define RT_ERR_ACL_MEMBER_CHK_IDX                             (0x00070010)
#define RT_ERR_ACL_MEMBER_CHK_TYPE                            (0x00070011)
#define RT_ERR_ACL_MEMBER_CHK_BOUND                           (0x00070012)
#define RT_ERR_ACL_TG_FIELD_IDX                               (0x00070013)
#define RT_ERR_ACL_TG_FIELD_TYPE                              (0x00070014)
#define RT_ERR_ACL_BOUND_TYPE                                 (0x00070015)
#define RT_ERR_ACL_PRIORITY_ERRPRI                            (0x00070016)
#define RT_ERR_ACL_REVERSE                                    (0x00070017)
#define RT_ERR_ACL_PARTITION                                  (0x00070018)
#define RT_ERR_ACL_MODE                                       (0x00070019)
#define RT_ERR_ACL_IACL_PARTITION                             (0x0007001A)
#define RT_ERR_ACL_EACL_PARTITION                             (0x00070020)
#define RT_ERR_ACL_SRAM_NOT_AVL                               (0x00070021)
#define RT_ERR_ACL_PIE_ENTRY_NOT_FOUND                        (0x00070022)
#define RT_ERR_ACL_SRAM_NO_SPACE                              (0x00070023)
#define RT_ERR_ACL_INDAC_TRIGGER_FAIL                         (0x00070024)
#define RT_ERR_ACL_INDAC_EXE_FAIL                             (0x00070025)
#define RT_ERR_ACL_DUP_ENTRYIDX                               (0x00070026)
#define RT_ERR_TCAM_WRITE                                     (0x00080000)
#define RT_ERR_TCAM_COMPARE                                   (0x00080001)
#define RT_ERR_TCAM_SAFETY_ENABLE                             (0x00080002)
#define RT_ERR_TCAM_PIE_BLOCK                                 (0x00080003)
#define RT_ERR_TCAM_NUM                                       (0x00080004)
#define RT_ERR_TCAM_CHECK_ENABLE                              (0x00080005)
#define RT_ERR_TCAM_ADDR                                      (0x00080006)
#define RT_ERR_TCAM_IRQ_CLEAR                                 (0x00080007)
#define RT_ERR_TCAM_ERROR_EN_MASK                             (0x00080008)
#define RT_ERR_LA_CPUPORT                                     (0x00090000)
#define RT_ERR_LA_TRUNK_ID                                    (0x00090001)
#define RT_ERR_LA_PORTMASK                                    (0x00090002)
#define RT_ERR_LA_HASHMASK                                    (0x00090003)
#define RT_ERR_LA_DUMB                                        (0x00090004)
#define RT_ERR_LA_PORTNUM_DUMB                                (0x00090005)
#define RT_ERR_LA_PORTNUM_NORMAL                              (0x00090006)
#define RT_ERR_LA_MEMBER_OVERLAP                              (0x00090007)
#define RT_ERR_LA_NOT_MEMBER_PORT                             (0x00090008)
#define RT_ERR_LA_TRUNK_NOT_EXIST                             (0x00090009)
#define RT_ERR_SFC_TICK_PERIOD                                (0x000A0000)
#define RT_ERR_SFC_UNKNOWN_GROUP                              (0x000A0001)
#define RT_ERR_PM_MASK                                        (0x000B0000)
#define RT_ERR_PM_LENGTH                                      (0x000B0001)
#define RT_ERR_PM_MODE                                        (0x000B0002)
#define RT_ERR_INBW_TICK_PERIOD                               (0x000C0000)
#define RT_ERR_INBW_TOKEN_AMOUNT                              (0x000C0001)
#define RT_ERR_INBW_FCON_VALUE                                (0x000C0002)
#define RT_ERR_INBW_FCOFF_VALUE                               (0x000C0003)
#define RT_ERR_INBW_FC_ALLOWANCE                              (0x000C0004)
#define RT_ERR_INBW_RATE                                      (0x000C0005)
#define RT_ERR_QOS_1P_PRIORITY                                (0x000D0000)
#define RT_ERR_QOS_DSCP_VALUE                                 (0x000D0001)
#define RT_ERR_QOS_INT_PRIORITY                               (0x000D0002)
#define RT_ERR_QOS_SEL_DSCP_PRI                               (0x000D0003)
#define RT_ERR_QOS_SEL_PORT_PRI                               (0x000D0004)
#define RT_ERR_QOS_SEL_IN_ACL_PRI                             (0x000D0005)
#define RT_ERR_QOS_SEL_CLASS_PRI                              (0x000D0006)
#define RT_ERR_QOS_EBW_RATE                                   (0x000D0007)
#define RT_ERR_QOS_SCHE_TYPE                                  (0x000D0008)
#define RT_ERR_QOS_QUEUE_WEIGHT                               (0x000D0009)
#define RT_ERR_QOS_SEL_PRI_SOURCE                             (0x000D000A)
#define RT_ERR_QOS_SEL_PRI_GROUP                              (0x000D000B)
#define RT_ERR_QOS_1P_DEI                                     (0x000D000C)
#define RT_ERR_QOS_SCHEDULE                                   (0x000D000D)
#define RT_ERR_QOS_INGRESS_LEAKY                              (0x000D000E)
#define RT_ERR_QOS_BW                                         (0x000D000F)
#define RT_ERR_PHY_PAGE_ID                                    (0x000E0000)
#define RT_ERR_PHY_REG_ID                                     (0x000E0001)
#define RT_ERR_PHY_DATAMASK                                   (0x000E0002)
#define RT_ERR_PHY_AUTO_NEGO_MODE                             (0x000E0003)
#define RT_ERR_PHY_SPEED                                      (0x000E0004)
#define RT_ERR_PHY_DUPLEX                                     (0x000E0005)
#define RT_ERR_PHY_FORCE_ABILITY                              (0x000E0006)
#define RT_ERR_PHY_FORCE_1000                                 (0x000E0007)
#define RT_ERR_PHY_TXRX                                       (0x000E0008)
#define RT_ERR_MAC_FORCE_1000                                 (0x000E0009)
#define RT_ERR_PHY_SQI_INVALID                                (0x000E000A)
#define RT_ERR_MIRROR_DIRECTION                               (0x000F0000)
#define RT_ERR_MIRROR_SESSION_FULL                            (0x000F0001)
#define RT_ERR_MIRROR_SESSION_NOEXIST                         (0x000F0002)
#define RT_ERR_MIRROR_PORT_EXIST                              (0x000F0003)
#define RT_ERR_MIRROR_PORT_NOT_EXIST                          (0x000F0004)
#define RT_ERR_MIRROR_PORT_FULL                               (0x000F0005)
#define RT_ERR_STAT_INVALID_GLOBAL_CNTR                       (0x00100000)
#define RT_ERR_STAT_INVALID_PORT_CNTR                         (0x00100001)
#define RT_ERR_STAT_GLOBAL_CNTR_FAIL                          (0x00100002)
#define RT_ERR_STAT_PORT_CNTR_FAIL                            (0x00100003)
#define RT_ERR_DOT1X_INVALID_DIRECTION                        (0x00110000)
#define RT_ERR_DOT1X_PORTBASEDPNEN                            (0x00110001)
#define RT_ERR_DOT1X_PORTBASEDAUTH                            (0x00110002)
#define RT_ERR_DOT1X_PORTBASEDOPDIR                           (0x00110003)
#define RT_ERR_DOT1X_MACBASEDPNEN                             (0x00110004)
#define RT_ERR_DOT1X_MACBASEDOPDIR                            (0x00110005)
#define RT_ERR_DOT1X_PROC                                     (0x00110006)
#define RT_ERR_DOT1X_GVLANIDX                                 (0x00110007)
#define RT_ERR_DOT1X_GVLANTALK                                (0x00110008)
#define RT_ERR_DOT1X_MAC_PORT_MISMATCH                        (0x00110009)
#define RT_ERR_DOT1X_GUEST_VLAN_ENTRY_EXIST                   (0x0011000A)
#define RT_ERR_WOL_INVALID_NODEID                             (0x00120000)
#define RT_ERR_WOL_CPU_PORT_NOT_EN                            (0x00120001)
#define RT_ERR_SPDIF_MULTI_SOURCE                             (0x00130000)
#define RT_ERR_SPDIF_LOOP_DISABLED                            (0x00130001)
#define RT_ERR_SPDIF_LOOP_ENABLED                             (0x00130002)
#define RT_ERR_QAV_INVALID_BW                                 (0x00140000)
#define RT_ERR_QAV_UNKNOWN_ALGO                               (0x00140001)
#define RT_ERR_ST_INVALID_LENGTH                              (0x00150000)
#define RT_ERR_ST_WAIT_PENDING                                (0x00150001)
#define RT_ERR_ST_INVALID_OPERAION                            (0x00150002)
#define RT_ERR_ST_INVALID_TIME_INTERVAL                       (0x00150003)
#define RT_ERR_ST_NONALIGNED_TIME_INTERVAL                    (0x00150004)
#define RT_ERR_ST_INSUFFICIENT_TIME_INTERVAL                  (0x00150005)
#define RT_ERR_ST_INVALID_CYCLE_TIME                          (0x00150006)
#define RT_ERR_ST_MACSEC_CONFIG                               (0x00150007)
#define RT_ERR_ST_PORT_TYPE                                   (0x00150008)
#define RT_ERR_ST_PORT_SPEED                                  (0x00150009)
#define RT_ERR_ST_WAIT_PRECONFIG                              (0x0015000A)
#define RT_ERR_ST_PORT                                        (0x0015000B)
#define RT_ERR_ST_WAIT_ISDONE                                 (0x0015000C)
#define RT_ERR_PSFP_INVALID_ENTRY_INDEX                       (0x00160000)
#define RT_ERR_PSFP_WAIT_PENDING                              (0x00160001)
#define RT_ERR_PSFP_WRITE_ON_ACTIVE_ENTRY                     (0x00160002)
#define RT_ERR_PSFP_POLL_DONE_FAIL                            (0x00160003)
#define RT_ERR_PSFP_REQUEST_BUSY                              (0x00160004)
#define RT_ERR_PSFP_REQUEST_FAIL                              (0x00160005)
#define RT_ERR_PSFP_INVALID_TYPE                              (0x00160006)
#define RT_ERR_PSFP_WAIT_PRECONFIG                            (0x00160007)
#define RT_ERR_PSFP_WAIT_ISDONE                               (0x00160008)
#define RT_ERR_FRER_INVALID_ENTRY_INDEX                       (0x00170000)
#define RT_ERR_FRER_BAD_TABLE                                 (0x00170001)
#define RT_ERR_FRER_XXX_FAIL                                  (0x00170002)
#define RT_ERR_STACK_NOT_INIT                                 (0x00180000)
#define RT_ERR_STACK_ALREADY_INIT                             (0x00180001)
#define RT_ERR_STACK_CONFIGURE_ERROR                          (0x00180002)
#define RT_ERR_STACK_REMOTE_REJECT                            (0x00180003)
#define RT_ERR_STACK_REMOTE_NO_RESP                           (0x00180004)
#define RT_ERR_STACK_LINK_FAIL                                (0x00180005)
#define RT_ERR_STACK_TABLE_FAIL                               (0x00180006)
#define RT_ERR_STACK_LOAD_SLAVE                               (0x00180007)
#define RT_ERR_STACK_IMAGE_DUT_NUM                            (0x00180010)
#define RT_ERR_STACK_IMAGE_ID                                 (0x00180011)
#define RT_ERR_STACK_IMAGE_MAC                                (0x00180012)
#define RT_ERR_STACK_IMAGE_MASTER                             (0x00180013)
#define RT_ERR_STACK_IMAGE_LINK                               (0x00180014)
#define RT_ERR_STACK_IMAGE_NEIGHBOR                           (0x00180015)
#define RT_ERR_STACK_OUTPUT_ERROR                             (0x00180016)
#define RT_ERR_STACK_ACCESS_ERROR                             (0x00180017)
#define RT_ERR_PCIE_FILE_NOT_OPEN                             (0x00190000)
#define RT_ERR_PCIE_IOCTL_FAIL                                (0x00190001)
#define RT_ERR_ROUTE_IPVER                                    (0x001A0000)
#define RT_ERR_ROUTE_LU_MISS                                  (0x001A0001)
#define RT_ERR_ROUTE_INVALID_TBL_TYPE                         (0x001A0002)
#define RT_ERR_OTP_FSM_MAIN_TIMEOUT_XC_VA                     (0x00200000) /* Polling OTP main fsm timeout                                         */
#define RT_ERR_OTP_FSM_IF_TIMEOUT_XC_VA                       (0x00200001) /* Polling OTP IF fsm timeout                                           */
#define RT_ERR_OTP_FSM_READY_TIMEOUT_XC_VA                    (0x00200002) /* Polling OTP Ready timeout                                            */
#define RT_ERR_OTP_INT_XC_VA                                  (0x00200003) /* Polling OTP interrupt failed                                         */
#define RT_ERR_OTP_MISMATCH_XC_VA                             (0x00200004) /* OTP mismatch interrupt                                               */
#define RT_ERR_OTP_LOCK_ROLE_INVALID_XC_VA                    (0x00200005) /* OTP lock role invalid                                                */
#define RT_ERR_OTP_LOCK_XC_VA                                 (0x00200006) /* OTP lock                                                             */
#define RT_ERR_OTP_UNLOCK_ROLE_INVALID_XC_VA                  (0x00200007) /* OTP unlock role invalid                                              */
#define RT_ERR_OTP_UNLOCK_XC_VA                               (0x00200008) /* OTP unlock failed                                                    */
#define RT_ERR_OTP_INIT_MARGIN_READ_CMP_XC_VA                 (0x00200009) /* OTP init margin read cmp failed                                      */
#define RT_ERR_OTP_PGM_MARGIN_READ_CMP_XC_VA                  (0x0020000A) /* OTP PGM margin read cmp failed                                       */
#define RT_ERR_OTP_BASIC_READ_CMP_XC_VA                       (0x0020000B) /* OTP read cmp failed                                                  */
#define RT_ERR_OTP_READ_LOCKDATA_XC_VA                        (0x0020000C) /* OTP Read lockdata                                                    */
#define RT_ERR_OTP_PGM_SAME_DATA_XC_VA                        (0x0020000D) /* OTP PGM same data                                                    */
#define RT_ERR_OTP_PGM_INVALID_DATA_XC_VA                     (0x0020000E) /* OTP PGM invliad data                                                 */
#define RT_ERR_OTP_PGM_LOCKDATA_XC_VA                         (0x0020000F) /* OTP PGM lockdata                                                     */
#define RT_ERR_OTP_DEADLOCK_XC_VA                             (0x00200010) /* OTP channel deadlock                                                 */
#define RT_ERR_OTP_NG_XC_VA                                   (0x00200011) /* OTP NG (# <= 32)                                                     */
#define RT_ERR_OTP_KILLCHIP_XC_VA                             (0x00200012) /* OTP Kill Chip (NG# > 32)                                             */
#define RT_ERR_OTP_NOT_REPAIR_XC_VA                           (0x00200013) /* OTP can't be repaired                                                */
#define RT_ERR_OTP_TESTROW_INIT_MARGIN_READ_XC_VA             (0x00200014) /* OTP test row data by initial margin read aren't all 1                */
#define RT_ERR_OTP_TESTROW_PGM_MARGIN_READ_XC_VA              (0x00200015) /* OTP test row data by pgm margin read aren't all 1                    */
#define RT_ERR_OTP_TESTROW_NORMAL_READ_XC_VA                  (0x00200016) /* OTP test row data by normal read aren't all 1                        */
#define RT_ERR_OTP_TESTCOL_INIT_MARGIN_READ_XC_VA             (0x00200017) /* OTP test column data by initial margin read aren't all 1             */
#define RT_ERR_OTP_TESTCOL_PGM_MARGIN_READ_XC_VA              (0x00200018) /* OTP test column data by pgm margin read aren't all 1                 */
#define RT_ERR_OTP_TESTCOL_NORMAL_READ_XC_VA                  (0x00200019) /* OTP test column data by normal read aren't all 1                     */
#define RT_ERR_OTP_CHKSUM_ERROR_XC_VA                         (0x0020001A) /* OTP checksum error                                                   */
#define RT_ERR_OTP_DONE_INT_XC_VA                             (0x00200100) /* Polling OTP done interrupt failed                                    */
#define RT_ERR_OTP_MISMATCH_INT_XC_VA                         (0x00200101) /* Polling OTP mismatch interrupt failed                                */
#define RT_ERR_OTP_NG_INT_XC_VA                               (0x00200102) /* Polling OTP ng interrupt failed                                      */
#define RT_ERR_OTP_KILLCHIP_INT_XC_VA                         (0x00200103) /* Polling OTP kill interrupt failed                                    */
#define RT_ERR_OTP_LOCK_TIMEOUT_XC_VA                         (0x00200104) /* OTP lock timeout                                                     */
#define RT_ERR_OTP_LOCK_ANOTHER_XC_VA                         (0x00200105) /* OTP lock another role                                                */
#define RT_ERR_OTP_LOCK_N_ROLES_XC_VA                         (0x00200106) /* OTP lock n roles                                                     */
#define RT_ERR_OTP_UNLOCK_RELEASED_XC_VA                      (0x00200107) /* OTP unlock released                                                  */
#define RT_ERR_OTP_UNLOCK_N_ROLES_XC_VA                       (0x00200108) /* OTP unlock n roles                                                   */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_NORMAL_READ_XC_VA      (0x00200109) /* OTP three read mode check normal read failed                         */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_INIT_MARGIN_READ_XC_VA (0x0020010A) /* OTP three read mode check init margin read failed                    */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_PGM_MARGIN_READ_XC_VA  (0x0020010B) /* OTP three read mode check pgm margin read failed                     */
#define RT_ERR_OTP_FSM_MAIN_TIMEOUT_XC_VB                     (0x00200000) /* Polling OTP main fsm timeout                                         */
#define RT_ERR_OTP_FSM_IF_TIMEOUT_XC_VB                       (0x00200001) /* Polling OTP IF fsm timeout                                           */
#define RT_ERR_OTP_FSM_READY_TIMEOUT_XC_VB                    (0x00200002) /* Polling OTP Ready timeout                                            */
#define RT_ERR_OTP_INT_XC_VB                                  (0x00200003) /* Polling OTP interrupt failed                                         */
#define RT_ERR_OTP_MISMATCH_XC_VB                             (0x00200004) /* OTP mismatch interrupt                                               */
#define RT_ERR_OTP_LOCK_ROLE_INVALID_XC_VB                    (0x00200005) /* OTP lock role invalid                                                */
#define RT_ERR_OTP_LOCK_XC_VB                                 (0x00200006) /* OTP lock                                                             */
#define RT_ERR_OTP_UNLOCK_ROLE_INVALID_XC_VB                  (0x00200007) /* OTP unlock role invalid                                              */
#define RT_ERR_OTP_UNLOCK_XC_VB                               (0x00200008) /* OTP unlock failed                                                    */
#define RT_ERR_OTP_INIT_MARGIN_READ_CMP_XC_VB                 (0x00200009) /* OTP init margin read cmp failed                                      */
#define RT_ERR_OTP_PGM_MARGIN_READ_CMP_XC_VB                  (0x0020000A) /* OTP PGM margin read cmp failed                                       */
#define RT_ERR_OTP_BASIC_READ_CMP_XC_VB                       (0x0020000B) /* OTP read cmp failed                                                  */
#define RT_ERR_OTP_READ_LOCKDATA_XC_VB                        (0x0020000C) /* OTP Read lockdata                                                    */
#define RT_ERR_OTP_PGM_SAME_DATA_XC_VB                        (0x0020000D) /* OTP PGM same data                                                    */
#define RT_ERR_OTP_PGM_INVALID_DATA_XC_VB                     (0x0020000E) /* OTP PGM invliad data                                                 */
#define RT_ERR_OTP_PGM_LOCKDATA_XC_VB                         (0x0020000F) /* OTP PGM lockdata                                                     */
#define RT_ERR_OTP_DEADLOCK_XC_VB                             (0x00200010) /* OTP channel deadlock                                                 */
#define RT_ERR_OTP_NG_XC_VB                                   (0x00200011) /* OTP NG (# <= 32)                                                     */
#define RT_ERR_OTP_KILLCHIP_XC_VB                             (0x00200012) /* OTP Kill Chip (NG# > 32)                                             */
#define RT_ERR_OTP_NOT_REPAIR_XC_VB                           (0x00200013) /* OTP can't be repaired                                                */
#define RT_ERR_OTP_TESTROW_INIT_MARGIN_READ_XC_VB             (0x00200014) /* OTP test row data by initial margin read aren't all 1                */
#define RT_ERR_OTP_TESTROW_PGM_MARGIN_READ_XC_VB              (0x00200015) /* OTP test row data by pgm margin read aren't all 1                    */
#define RT_ERR_OTP_TESTROW_NORMAL_READ_XC_VB                  (0x00200016) /* OTP test row data by normal read aren't all 1                        */
#define RT_ERR_OTP_TESTCOL_INIT_MARGIN_READ_XC_VB             (0x00200017) /* OTP test column data by initial margin read aren't all 1             */
#define RT_ERR_OTP_TESTCOL_PGM_MARGIN_READ_XC_VB              (0x00200018) /* OTP test column data by pgm margin read aren't all 1                 */
#define RT_ERR_OTP_TESTCOL_NORMAL_READ_XC_VB                  (0x00200019) /* OTP test column data by normal read aren't all 1                     */
#define RT_ERR_OTP_CHKSUM_ERROR_XC_VB                         (0x0020001A) /* OTP checksum error                                                   */
#define RT_ERR_OTP_OV_TEMP_MII_LOG_UV_OVER_RETRY_XC_VB        (0x0020001B) /* OTP over temperature retry error                                     */
#define RT_ERR_OTP_DONE_INT_XC_VB                             (0x00200100) /* Polling OTP done interrupt failed                                    */
#define RT_ERR_OTP_MISMATCH_INT_XC_VB                         (0x00200101) /* Polling OTP mismatch interrupt failed                                */
#define RT_ERR_OTP_NG_INT_XC_VB                               (0x00200102) /* Polling OTP ng interrupt failed                                      */
#define RT_ERR_OTP_KILLCHIP_INT_XC_VB                         (0x00200103) /* Polling OTP kill interrupt failed                                    */
#define RT_ERR_OTP_LOCK_TIMEOUT_XC_VB                         (0x00200104) /* OTP lock timeout                                                     */
#define RT_ERR_OTP_LOCK_ANOTHER_XC_VB                         (0x00200105) /* OTP lock another role                                                */
#define RT_ERR_OTP_LOCK_N_ROLES_XC_VB                         (0x00200106) /* OTP lock n roles                                                     */
#define RT_ERR_OTP_UNLOCK_RELEASED_XC_VB                      (0x00200107) /* OTP unlock released                                                  */
#define RT_ERR_OTP_UNLOCK_N_ROLES_XC_VB                       (0x00200108) /* OTP unlock n roles                                                   */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_NORMAL_READ_XC_VB      (0x00200109) /* OTP three read mode check normal read failed                         */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_INIT_MARGIN_READ_XC_VB (0x0020010A) /* OTP three read mode check init margin read failed                    */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_PGM_MARGIN_READ_XC_VB  (0x0020010B) /* OTP three read mode check pgm margin read failed                     */
#define RT_ERR_OTP_FSM_MAIN_TIMEOUT_XD_V1                     (0x00200000) /* Polling OTP main fsm timeout                                         */
#define RT_ERR_OTP_FSM_IF_TIMEOUT_XD_V1                       (0x00200001) /* Polling OTP IF fsm timeout                                           */
#define RT_ERR_OTP_FSM_READY_TIMEOUT_XD_V1                    (0x00200002) /* Polling OTP Ready timeout                                            */
#define RT_ERR_OTP_INT_XD_V1                                  (0x00200003) /* Polling OTP interrupt failed                                         */
#define RT_ERR_OTP_MISMATCH_XD_V1                             (0x00200004) /* OTP mismatch interrupt                                               */
#define RT_ERR_OTP_LOCK_ROLE_INVALID_XD_V1                    (0x00200005) /* OTP lock role invalid                                                */
#define RT_ERR_OTP_LOCK_XD_V1                                 (0x00200006) /* OTP lock                                                             */
#define RT_ERR_OTP_UNLOCK_ROLE_INVALID_XD_V1                  (0x00200007) /* OTP unlock role invalid                                              */
#define RT_ERR_OTP_UNLOCK_XD_V1                               (0x00200008) /* OTP unlock failed                                                    */
#define RT_ERR_OTP_INIT_MARGIN_READ_CMP_XD_V1                 (0x00200009) /* OTP init margin read cmp failed                                      */
#define RT_ERR_OTP_PGM_MARGIN_READ_CMP_XD_V1                  (0x0020000A) /* OTP PGM margin read cmp failed                                       */
#define RT_ERR_OTP_BASIC_READ_CMP_XD_V1                       (0x0020000B) /* OTP read cmp failed                                                  */
#define RT_ERR_OTP_READ_LOCKDATA_XD_V1                        (0x0020000C) /* OTP Read lockdata                                                    */
#define RT_ERR_OTP_PGM_SAME_DATA_XD_V1                        (0x0020000D) /* OTP PGM same data                                                    */
#define RT_ERR_OTP_PGM_INVALID_DATA_XD_V1                     (0x0020000E) /* OTP PGM invliad data                                                 */
#define RT_ERR_OTP_PGM_LOCKDATA_XD_V1                         (0x0020000F) /* OTP PGM lockdata                                                     */
#define RT_ERR_OTP_DEADLOCK_XD_V1                             (0x00200010) /* OTP channel deadlock                                                 */
#define RT_ERR_OTP_NG_XD_V1                                   (0x00200011) /* OTP NG (# <= 32)                                                     */
#define RT_ERR_OTP_KILLCHIP_XD_V1                             (0x00200012) /* OTP Kill Chip (NG# > 32)                                             */
#define RT_ERR_OTP_NOT_REPAIR_XD_V1                           (0x00200013) /* OTP can't be repaired                                                */
#define RT_ERR_OTP_TESTROW_INIT_MARGIN_READ_XD_V1             (0x00200014) /* OTP test row data by initial margin read aren't all 1                */
#define RT_ERR_OTP_TESTROW_PGM_MARGIN_READ_XD_V1              (0x00200015) /* OTP test row data by pgm margin read aren't all 1                    */
#define RT_ERR_OTP_TESTROW_NORMAL_READ_XD_V1                  (0x00200016) /* OTP test row data by normal read aren't all 1                        */
#define RT_ERR_OTP_TESTCOL_INIT_MARGIN_READ_XD_V1             (0x00200017) /* OTP test column data by initial margin read aren't all 1             */
#define RT_ERR_OTP_TESTCOL_PGM_MARGIN_READ_XD_V1              (0x00200018) /* OTP test column data by pgm margin read aren't all 1                 */
#define RT_ERR_OTP_TESTCOL_NORMAL_READ_XD_V1                  (0x00200019) /* OTP test column data by normal read aren't all 1                     */
#define RT_ERR_OTP_CHKSUM_ERROR_XD_V1                         (0x0020001A) /* OTP checksum error                                                   */
#define RT_ERR_OTP_DONE_INT_XD_V1                             (0x00200100) /* Polling OTP done interrupt failed                                    */
#define RT_ERR_OTP_MISMATCH_INT_XD_V1                         (0x00200101) /* Polling OTP mismatch interrupt failed                                */
#define RT_ERR_OTP_NG_INT_XD_V1                               (0x00200102) /* Polling OTP ng interrupt failed                                      */
#define RT_ERR_OTP_KILLCHIP_INT_XD_V1                         (0x00200103) /* Polling OTP kill interrupt failed                                    */
#define RT_ERR_OTP_LOCK_TIMEOUT_XD_V1                         (0x00200104) /* OTP lock timeout                                                     */
#define RT_ERR_OTP_LOCK_ANOTHER_XD_V1                         (0x00200105) /* OTP lock another role                                                */
#define RT_ERR_OTP_LOCK_N_ROLES_XD_V1                         (0x00200106) /* OTP lock n roles                                                     */
#define RT_ERR_OTP_UNLOCK_RELEASED_XD_V1                      (0x00200107) /* OTP unlock released                                                  */
#define RT_ERR_OTP_UNLOCK_N_ROLES_XD_V1                       (0x00200108) /* OTP unlock n roles                                                   */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_NORMAL_READ_XD_V1      (0x00200109) /* OTP three read mode check normal read failed                         */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_INIT_MARGIN_READ_XD_V1 (0x0020010A) /* OTP three read mode check init margin read failed                    */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_PGM_MARGIN_READ_XD_V1  (0x0020010B) /* OTP three read mode check pgm margin read failed                     */
#define RT_ERR_OTP_FSM_MAIN_TIMEOUT_XD_VA                     (0x00200000) /* Polling OTP main fsm timeout                                         */
#define RT_ERR_OTP_FSM_IF_TIMEOUT_XD_VA                       (0x00200001) /* Polling OTP IF fsm timeout                                           */
#define RT_ERR_OTP_FSM_READY_TIMEOUT_XD_VA                    (0x00200002) /* Polling OTP Ready timeout                                            */
#define RT_ERR_OTP_DONE_INT_XD_VA                             (0x00200003) /* Polling OTP done interrupt failed                                    */
#define RT_ERR_OTP_MISMATCH_INT_XD_VA                         (0x00200004) /* Polling OTP mismatch interrupt failed                                */
#define RT_ERR_OTP_NG_INT_XD_VA                               (0x00200005) /* Polling OTP ng interrupt failed                                      */
#define RT_ERR_OTP_KILLCHIP_INT_XD_VA                         (0x00200006) /* Polling OTP kill interrupt failed                                    */
#define RT_ERR_OTP_MISMATCH_XD_VA                             (0x00200007) /* OTP mismatch interrupt                                               */
#define RT_ERR_OTP_LOCK_ROLE_INVALID_XD_VA                    (0x00200008) /* OTP lock role invalid                                                */
#define RT_ERR_OTP_LOCK_XD_VA                                 (0x00200009) /* OTP lock                                                             */
#define RT_ERR_OTP_LOCK_TIMEOUT_XD_VA                         (0x0020000A) /* OTP lock timeout                                                     */
#define RT_ERR_OTP_LOCK_ANOTHER_XD_VA                         (0x0020000B) /* OTP lock another role                                                */
#define RT_ERR_OTP_LOCK_N_ROLES_XD_VA                         (0x0020000C) /* OTP lock n roles                                                     */
#define RT_ERR_OTP_UNLOCK_ROLE_INVALID_XD_VA                  (0x0020000D) /* OTP unlock role invalid                                              */
#define RT_ERR_OTP_UNLOCK_RELEASED_XD_VA                      (0x0020000E) /* OTP unlock released                                                  */
#define RT_ERR_OTP_UNLOCK_XD_VA                               (0x0020000F) /* OTP unlock failed                                                    */
#define RT_ERR_OTP_UNLOCK_N_ROLES_XD_VA                       (0x00200010) /* OTP unlock n roles                                                   */
#define RT_ERR_OTP_INIT_MARGIN_READ_CMP_XD_VA                 (0x00200011) /* OTP init margin read cmp failed                                      */
#define RT_ERR_OTP_PGM_MARGIN_READ_CMP_XD_VA                  (0x00200012) /* OTP PGM margin read cmp failed                                       */
#define RT_ERR_OTP_BASIC_READ_CMP_XD_VA                       (0x00200013) /* OTP read cmp failed                                                  */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_NORMAL_READ_XD_VA      (0x00200014) /* OTP three read mode check normal read failed                         */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_INIT_MARGIN_READ_XD_VA (0x00200015) /* OTP three read mode check init margin read failed                    */
#define RT_ERR_OTP_THREE_READ_MODE_CHK_PGM_MARGIN_READ_XD_VA  (0x00200016) /* OTP three read mode check pgm margin read failed                     */
#define RT_ERR_OTP_READ_LOCKDATA_XD_VA                        (0x00200017) /* OTP Read lockdata                                                    */
#define RT_ERR_OTP_ACCESS_MCU_ONLY_XD_VA                      (0x00200018) /* OTP Read MUC only block                                              */
#define RT_ERR_OTP_PGM_SAME_DATA_XD_VA                        (0x00200019) /* OTP PGM same data                                                    */
#define RT_ERR_OTP_PGM_INVALID_DATA_XD_VA                     (0x0020001A) /* OTP PGM invliad data                                                 */
#define RT_ERR_OTP_PGM_LOCKDATA_XD_VA                         (0x0020001B) /* OTP PGM lockdata                                                     */
#define RT_ERR_OTP_DEADLOCK_XD_VA                             (0x0020001C) /* OTP channel deadlock                                                 */
#define RT_ERR_OTP_NG_XD_VA                                   (0x0020001D) /* OTP NG (# <= 32)                                                     */
#define RT_ERR_OTP_KILLCHIP_XD_VA                             (0x0020001E) /* OTP Kill Chip (NG# > 32)                                             */
#define RT_ERR_OTP_NOT_REPAIR_XD_VA                           (0x0020001F) /* OTP can't be repaired                                                */
#define RT_ERR_OTP_REPAIRED_XD_VA                             (0x00200020) /* OTP has already been repaired                                        */
#define RT_ERR_OTP_TESTROW_INIT_MARGIN_READ_XD_VA             (0x00200021) /* OTP test row data by initial margin read aren't all 1                */
#define RT_ERR_OTP_TESTROW_PGM_MARGIN_READ_XD_VA              (0x00200022) /* OTP test row data by pgm margin read aren't all 1                    */
#define RT_ERR_OTP_TESTROW_NORMAL_READ_XD_VA                  (0x00200023) /* OTP test row data by normal read aren't all 1                        */
#define RT_ERR_OTP_TESTCOL_INIT_MARGIN_READ_XD_VA             (0x00200024) /* OTP test column data by initial margin read aren't all 1             */
#define RT_ERR_OTP_TESTCOL_PGM_MARGIN_READ_XD_VA              (0x00200025) /* OTP test column data by pgm margin read aren't all 1                 */
#define RT_ERR_OTP_TESTCOL_NORMAL_READ_XD_VA                  (0x00200026) /* OTP test column data by normal read aren't all 1                     */
#define RT_ERR_OTP_CHKSUM_ERROR_XD_VA                         (0x00200027) /* OTP checksum error                                                   */
#define RT_ERR_OTP_OV_TEMP_MII_LOG_UV_OVER_RETRY_XD_VA        (0x00200028) /* OTP over temperature retry error                                     */
#define RT_ERR_OTP_FAULT_INJECTION_POSITIVE_A3_XD_VA          (0x00200029) /* OTP wrong A3 in the fault injection positive testing                 */
#define RT_ERR_OTP_FAULT_INJECTION_POSITIVE_MISMATCH_XD_VA    (0x0020002A) /* OTP mismatch interrupt in the fault injection positive testing       */
#define RT_ERR_OTP_FAULT_INJECTION_NEGATIVE_A3_XD_VA          (0x0020002B) /* OTP wrong A3 in the fault injection negative testing                 */
#define RT_ERR_OTP_FAULT_INJECTION_NEGATIVE_MISMATCH_XD_VA    (0x0020002C) /* OTP mismatch interrupt in the fault injection negative testing       */
#define RT_ERR_OTP_INDIR_ACCESS                               (0x00201000) /* OTP indirect access                                                  */
#define RT_ERR_SDS_NULL_VALUE                                 (0x00210000)
#define RT_ERR_SDS_START_BIT                                  (0x00210001)
#define RT_ERR_SDS_END_BIT                                    (0x00210002)
#define RT_ERR_SDS_STARTBIT_MORE_THAN_ENDBIT                  (0x00210003)
#define RT_ERR_SDS_INPUT_OUT_OF_RANGE                         (0x00210004)
#define RT_ERR_FLOWCTRL_PAGE_CNT                              (0x00220000)
#define RT_ERR_QAT_CLS_EN_MSK                                 (0x00230000)
#define RT_ERR_QAT_SR_PVID                                    (0x00230001)
#define RT_ERR_QAT_BW                                         (0x00230002)
#define RT_ERR_QAT_DYNAMIC_VLAN_SUPPORT_NUM                   (0x00230003)
#define RT_ERR_CB_ENTRY_INDEX                                 (0x00240000)
#define RT_ERR_CB_PORT                                        (0x00240001)
#define RT_ERR_CB_INTERNAL_CMD                                (0x00240002)
#define RT_ERR_CB_RSTTIMER                                    (0x00240003)
#define RT_ERR_CB_TIMER                                       (0x00240004)
#define RT_ERR_CB_READ                                        (0x00240005)
#define RT_ERR_CB_READ_TIMEOUT                                (0x00240006)
#define RT_ERR_CB_POW                                         (0x00240007)
#define RT_ERR_PTP_CALL_API_FAILED                            (0x00250000)
#define RT_ERR_PTP_INVALID_LOGINTVAL                          (0x00250001)
#define RT_ERR_PTP_EXCEED_DA_LIMITATION                       (0x00250002)
#define RT_ERR_SDS_PAGE_ID                                    (0x00260005)
#define RT_ERR_SDS_REG_ID                                     (0x00260006)
#define RT_ERR_SDS_APHY_REG_ADDR                              (0x00260007)
#define RT_ERR_SDS_MODE                                       (0x00260008)
#define RT_ERR_SDS_SPEED                                      (0x00260009)
#define RT_ERR_SDS_SPEED_MISMACH                              (0x0026000A)
#define RT_ERR_SDS_PORT_ROLE                                  (0x0026000B)
#define RT_ERR_RESET_2NDRST_OTP_MUTEX                         (0x00270000)
#define RT_ERR_FLOWCTRL_RA_BYPASS                             (0x00280000) /* Flow Control RA bypass is invalid */
#define RT_ERR_ETH_POLLING                                    (0x00290000)
#define RT_ERR_ETH_FLASH_STATE                                (0x00290001)
#define RT_ERR_ETH_ONGOING                                    (0x00290002)
#define RT_ERR_ETH_CHECKID                                    (0x00290003)
#define RT_ERR_ROUTE_ENTRY_FOUND                              (0x002A0000)
#define RT_ERR_ROUTE_ENTRY_NOT_FOUND                          (0x002A0001)
#define RT_ERR_ROUTE_NOT_SUPPORT                              (0x002A0002)
#define RT_ERR_ROUTE_TBL_TYPE                                 (0x002A0003)
#define RT_ERR_REGLIST_PORT_CLK                               (0x002B0000)
#define RT_ERR_ATS_SCHEDULER_ID                               (0x00300000)
#define RT_ERR_ATS_SCHEDULER_GROUP_ID                         (0x00300001)
#define RT_ERR_ATS_CBS                                        (0x00300002)
#define RT_ERR_ATS_CIR                                        (0x00300003)
#define RT_ERR_ATS_MAX_RESIDENCE_TIME                         (0x00300004)
#define RT_ERR_ATS_EMPTY_TO_FULL_TIME                         (0x00300005)
#define RT_ERR_ESM_EVENT_ID                                   (0x00310000)
#define RT_ERR_ESM_EVENT_STATUS                               (0x00310001)

#endif /* RTKAS_ERROR_H */
