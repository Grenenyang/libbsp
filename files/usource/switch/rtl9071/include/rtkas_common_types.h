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

#ifndef RTKAS_COMMON_TYPES_H_
#define RTKAS_COMMON_TYPES_H_

#include "rtkas_std.h"
/* For Internal tool usage */
#ifdef EXTRAL_CFG_HOOK
#ifdef RTL906X_USER_LINUX_LIBUSB
#define EXTERN_API extern
#else
#define EXTERN_API __declspec(dllexport)
#endif
#else
#define EXTERN_API extern
#endif

/* For Internal tool usage */
#ifdef RTL_907X_ASICDRV_FOR_INTERNAL
#define DYNAMIC_SCOPE
#else
#define DYNAMIC_SCOPE static
#endif

typedef unsigned long long uint64;
typedef long long          int64;
typedef unsigned int       uint32;
typedef int                int32;
typedef unsigned short     uint16;
/* skip the misra check typedef short                 int16;*/
typedef unsigned char uint8;
typedef char          int8;
typedef double        float64;
typedef uint32        ipaddr_t; /*IPv4 address type*/

#ifndef UINT32_MAX
#define UINT32_MAX (0xFFFFFFFFu)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX (0xFFFFFFFFFFFFFFFFu)
#endif

typedef uint32 ipaddr_t; /*IPv4 address type*/
/* skip the misra check typedef uint32               memaddr;   */

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6
#endif

typedef int32 RtkApiRet;
/* skip the misra check typedef uint64                  rtk_u_long_t;*/

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef ENABLE
#define ENABLE ((uint32)1)
#endif

#ifndef DISABLE
#define DISABLE ((uint32)0)
#endif

#ifndef SUCCESS
#define SUCCESS (0)
#endif

#ifndef FAILED
#define FAILED (-1)
#endif

#ifndef TSN_VALID
#define TSN_VALID (1)
#endif

#ifndef TSN_INVALID
#define TSN_INVALID (0)
#endif

#ifndef MAC_ADDR_LEN
#define MAC_ADDR_LEN (6u)
#endif

#ifndef IPV6_ADDR_LEN
#define IPV6_ADDR_LEN (16u)
#endif

#ifndef RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST
#define RTK_TOTAL_NUM_OF_WORD_FOR_1BIT_PORT_LIST (1u)
#endif

#ifndef IPV4_ADDR_LEN
#define IPV4_ADDR_LEN (4u)
#endif

/* ACL Type Defines */
typedef uint32 AclEnable;
typedef uint32 AclDefAct;
typedef uint32 AclPartition;
typedef uint8  AclMode;
typedef uint32 AclTemplateId;
typedef uint32 AclTemplateFieldId;
typedef uint32 AclFilterFieldType;
typedef uint32 AclRuleId;
typedef uint32 AclPieBlockId;
typedef uint32 AclPriVal;
typedef uint32 AclReverse;
typedef uint32 AclAggOp;
typedef uint32 AclOffsetIndex;
typedef uint32 AclOffsetType;
typedef uint16 AclOffsetPosition;
typedef uint32 AclFilterIndex;
typedef uint32 AclRangeType;
typedef uint32 AclFilterBound;
typedef uint32 AclIpv4;
typedef uint8  AclIpv6;
typedef uint8  AclMac;
typedef uint32 AclMemberType;
typedef uint32 AclMember;
typedef uint32 AclThreshold;
typedef uint8  AclBoundType;
typedef uint32 AclHeaderDumpId;

#endif /* RTKAS_COMMON_TYPES_H_ */
