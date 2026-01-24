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

#ifndef RTKAS_ENDIANS_H_
#define RTKAS_ENDIANS_H_

#if(defined(__BYTE_ORDER__) && ((__BYTE_ORDER__) == __ORDER_BIG_ENDIAN__)) || (defined(__BYTE_ORDER) && ((__BYTE_ORDER) == __BIG_ENDIAN)) || (defined(_BYTE_ORDER) && ((_BYTE_ORDER) == _BIG_ENDIAN)) || (defined(BYTE_ORDER) && ((BYTE_ORDER) == BIG_ENDIAN)) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__) || defined(_M_PPC) || defined(__C51__) || defined(__CX51__)
// #  warning "RTK_BIG_ENDIAN"
#define RTK_BIG_ENDIAN
#endif

#if(defined(__BYTE_ORDER__) && ((__BYTE_ORDER__) == __ORDER_LITTLE_ENDIAN__)) || (defined(__BYTE_ORDER) && ((__BYTE_ORDER) == __LITTLE_ENDIAN)) || (defined(_BYTE_ORDER) && ((_BYTE_ORDER) == _LITTLE_ENDIAN)) || (defined(BYTE_ORDER) && ((BYTE_ORDER) == LITTLE_ENDIAN)) || defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || defined(_M_ARM) /* msvc code on arm executes in little endian mode */
// #  warning "RTK_LITTLE_ENDIAN"
#define RTK_LITTLE_ENDIAN
#endif

#if defined(RTK_LITTLE_ENDIAN) && defined(RTK_BIG_ENDIAN)
#error Both RTK_LITTLE_ENDIAN and RTK_BIG_ENDIAN have been defined!!!
#endif

#if !defined(RTK_LITTLE_ENDIAN) && !defined(RTK_BIG_ENDIAN)
#error Cannot detect endian type, please include system header or assign correct value manually.
#endif

#define SWAPL32_OP(op, x) \
    ((((x)op 0xff000000U) >> 24u) | (((x)&0x00ff0000U) >> 8u) | (((x)&0x0000ff00U) << 8u) | (((x)&0x000000ffU) << 24u))

#define SWAPS16_OP(op, x) \
    ((((x)op 0xff00u) << 8u) | (((x)&0x00ffu) << 8u))

#ifdef RTK_LITTLE_ENDIAN
#define ntohs(x) (swaps16(x))
#define ntohl(x) (swapl32(x))
#define htons(x) (swaps16(x))
#define htonl(x) (swapl32(x))

#define ntohs_op(op, x)   (SWAPS16_OP(op, x))
#define ntohl_op(op, x)   (SWAPL32_OP(op, x))
#define htons_op(op, x)   (SWAPS16_OP(op, x))
#define htonl_op(op, x)   (SWAPL32_OP(op, x))
#define htobe16_op(op, x) (SWAPS16_op(op, x))
#define htole16_op(op, x) ((0xFFFFu)op(x))
#define be16toh_op(op, x) (SWAPS16_op(op, x))
#define le16toh_op(op, x) ((0xFFFFu)op(x))
#define htobe32_op(op, x) (SWAPL32_OP(op, x))
#define htole32_op(op, x) ((0xFFFFFFFFu)op(x))
#define be32toh_op(op, x) (SWAPL32_OP(op, x))
#define le32toh_op(op, x) ((0xFFFFFFFFu)op(x))

#ifndef htobe16
#define htobe16(x) (swaps16(x))
#endif

#ifndef htole16
#define htole16(x) (x)
#endif

#ifndef be16toh
#define be16toh(x) (swaps16(x))
#endif

#ifndef le16toh
#define le16toh(x) (x)
#endif

#ifndef htobe32
#define htobe32(x) (swapl32(x))
#endif

#ifndef htole32
#define htole32(x) (x)
#endif

#ifndef be32toh
#define be32toh(x) (swapl32(x))
#endif

#ifndef le32toh
#define le32toh(x) (x)
#endif
#else

#define ntohs(x) (x)
#define ntohl(x) (x)
#define htons(x) (x)
#define htonl(x) (x)

#define ntohs_op(op, x)   ((0xFFFFu)op(x))
#define ntohl_op(op, x)   ((0xFFFFFFFFu)op(x))
#define htons_op(op, x)   ((0xFFFFu)op(x))
#define htonl_op(op, x)   ((0xFFFFFFFFu)op(x))
#define htobe16_op(op, x) ((0xFFFFu)op(x))
#define htole16_op(op, x) (SWAPS16_op(op, x))
#define be16toh_op(op, x) ((0xFFFFu)op(x))
#define le16toh_op(op, x) (SWAPS16_op(op, x))
#define htobe32_op(op, x) ((0xFFFFFFFFu)op(x))
#define htole32_op(op, x) (SWAPL32_OP(op, x))
#define be32toh_op(op, x) ((0xFFFFFFFFu)op(x))
#define le32toh_op(op, x) (SWAPL32_OP(op, x))

#ifndef htobe16
#define htobe16(x) (x)
#endif

#ifndef htole16
#define htole16(x) (swaps16(x))
#endif

#ifndef be16toh
#define be16toh(x) (x)
#endif

#ifndef le16toh
#define le16toh(x) (swaps16(x))
#endif

#ifndef htobe32
#define htobe32(x) (x)
#endif

#ifndef htole32
#define htole32(x) (swapl32(x))
#endif

#ifndef be32toh
#define be32toh(x) (x)
#endif

#ifndef le32toh
#define le32toh(x) (swapl32(x))
#endif

#endif

#include "rtkas_types.h"

static inline uint16 swaps16(uint16 x)
{
    return ((x & 0xFF00u) >> 8) | ((x & 0x00FFu) << 8);
}

static inline uint32 swapl32(uint32 x)
{
    return ((x & 0xFF000000u) >> 24) | ((x & 0x00FF0000u) >> 8) | ((x & 0x0000FF00u) << 8) | ((x & 0x000000FFu) << 24);
}

static inline uint16 rtl_swaps16(uint16 x)
{
    return ((((x)&0xff00u) >> 8u) | (((x)&0x00ffu) << 8u));
}
static inline uint32 rtl_swapl32(uint32 x)
{
    return ((((x)&0xff000000U) >> 24u) | (((x)&0x00ff0000U) >> 8u) | (((x)&0x0000ff00U) << 8u) | (((x)&0x000000ffU) << 24u));
}

static inline uint16 rtl_ntohs(uint16 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return ((((x)&0xff00u) >> 8u) | (((x)&0x00ffu) << 8u));
#else
    return x;
#endif
}
static inline uint32 rtl_ntohl(uint32 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return ((((x)&0xff000000U) >> 24u) | (((x)&0x00ff0000U) >> 8u) | (((x)&0x0000ff00U) << 8u) | (((x)&0x000000ffU) << 24u));
#else
    return x;
#endif
}

static inline uint16 rtl_htons(uint16 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return ((((x)&0xff00u) >> 8u) | (((x)&0x00ffu) << 8u));
#else
    return x;
#endif
}
static inline uint32 rtl_htonl(uint32 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return ((((x)&0xff000000U) >> 24u) | (((x)&0x00ff0000U) >> 8u) | (((x)&0x0000ff00U) << 8u) | (((x)&0x000000ffU) << 24u));
#else
    return x;
#endif
}
static inline uint16 rtl_htobe16(uint16 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return ((((x)&0xff00u) >> 8u) | (((x)&0x00ffu) << 8u));
#else
    return x;
#endif
}
static inline uint16 rtl_htole16(uint16 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return x;
#else
    return ((((x)&0xff00u) >> 8u) | (((x)&0x00ffu) << 8u));
#endif
}
static inline uint16 rtl_be16toh(uint16 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return ((((x)&0xff00u) >> 8u) | (((x)&0x00ffu) << 8u));
#else
    return x;
#endif
}
static inline uint16 rtl_le16toh(uint16 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return x;
#else
    return ((((x)&0xff00u) >> 8u) | (((x)&0x00ffu) << 8u));
#endif
}
static inline uint32 rtl_htobe32(uint32 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return ((((x)&0xff000000U) >> 24u) | (((x)&0x00ff0000U) >> 8u) | (((x)&0x0000ff00U) << 8u) | (((x)&0x000000ffU) << 24u));
#else
    return x;
#endif
}
static inline uint32 rtl_htole32(uint32 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return x;
#else
    return ((((x)&0xff000000U) >> 24u) | (((x)&0x00ff0000U) >> 8u) | (((x)&0x0000ff00U) << 8u) | (((x)&0x000000ffU) << 24u));
#endif
}
static inline uint32 rtl_be32toh(uint32 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return ((((x)&0xff000000U) >> 24u) | (((x)&0x00ff0000U) >> 8u) | (((x)&0x0000ff00U) << 8u) | (((x)&0x000000ffU) << 24u));
#else
    return x;
#endif
}
static inline uint32 rtl_le32toh(uint32 x)
{
#ifdef RTK_LITTLE_ENDIAN
    return x;
#else
    return ((((x)&0xff000000U) >> 24u) | (((x)&0x00ff0000U) >> 8u) | (((x)&0x0000ff00U) << 8u) | (((x)&0x000000ffU) << 24u));
#endif
}

#endif
