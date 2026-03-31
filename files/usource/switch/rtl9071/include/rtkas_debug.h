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

#ifndef RTKAS_DEBUG_H
#define RTKAS_DEBUG_H

#include "rtkas_system_porting.h"

#define DBG_MSG /* DEFINED: enable debugging message */

/*=========== global debug message ==========*/
/* global debug level */
#define DBG_MASK_NULL 0x0u
#define DBG_VERIF     0x1u
#define DBG_INFO      0x2u
#define DBG_WARN      0x4u
#define DBG_ERR       0x8u
#define DBG_MEMDUMP   0x10u

/* Individual module debug mask */
#define DBG_NULL          0x0u
#define DBG_VLAN          0x1u     /* vlan debug mask                     */
#define DBG_PRI           0x2u     /* priority debug mask                 */
#define DBG_802DOT1X      0x4u     /*802.1X network access control        */
#define DBG_RANGECHECK    0x8u     /*TCP/UDP port range checking          */
#define DBG_PKTTRANSLATOR 0x10u    /*packet translator debug              */
#define DBG_TRAFFIC       0x20u    /*traffic isolation filter debug       */
#define DBG_PIE           0x40u    /*PIE look up                          */
#define DBG_FLOWCLF       0x80u    /*flow classification                  */
#define DBG_ACL           0x100u   /*ACL debug                            */
#define DBG_PARSER        0x200u   /*packet parser debug                  */
#define DBG_LEARNING      0x400u   /*source mac learning debug            */
#define DBG_LUT           0x800u   /*L2 table and foward table lookup     */
#define DBG_RMA           0x1000u  /*RMA                                  */
#define DBG_OAM           0x2000u  /*OAM Parser decision& oam multiplexer */
#define DBG_LINKAGGR      0x4000u  /*Link Aggregation                     */
#define DBG_LOOPDETECTION 0x8000u  /*Loop detection                       */
#define DBG_PKTGEN        0x10000u /*packet generator                     */
#define DBG_TEMPLGEN      0x20000u
#define DBG_MIRROR        0x40000u    /*Mirror, sFlow and RSPAN              */
#define DBG_ATTACKPREV    0x80000u    /*Attack prevention                    */
#define DBG_ICMODEL       0x100000u   /*IC Model                             */
#define DBG_SPT           0x200000u   /*Spanning tree protocol               */
#define DBG_LOCK          0x400000u   /*lock mechanism                       */
#define DBG_MEMCTRL       0x800000u   /*memory controller                    */
#define DBG_PHY           0x1000000u  /*phy access                           */
#define DBG_TESTCASE      0x2000000u  /*for testing cases                    */
#define DBG_REGVERIFY     0x4000000u  /*for testing cases                    */
#define DBG_TBLACCESS     0x8000000u  /*for testing cases                    */
#define DBG_REG           0x10000000u /*register access                      */
#define DBG_PORT          0x20000000u /*port                                 */
#define DBG_ROUTING       0x40000000u /*for testing cases */

/*to add your module debug mask here.....*/
#define DBG_MISC 0x80000000u /*for all trivial things*/

/* define debug level                   */
/* skip the Misra check #define DBG_MASK           DBG_MASK_NULL*/
#define DBG_MASK        (DBG_ERR | DBG_VERIF | DBG_INFO | DBG_REG)
#define DBG_MODULE_MASK (DBG_PKTTRANSLATOR | DBG_PRI | DBG_MIRROR | DBG_MEMCTRL | DBG_LINKAGGR | DBG_TESTCASE | DBG_REG)

/* global debug message define */
#if(0 != (DBG_MASK & DBG_VERIF))
#if __GNUC__ < 5
#define DEBUG_VERIF(type, ...)                                            \
    if(0u != ((type)&DBG_MODULE_MASK))                                    \
    {                                                                     \
        (void)rtlglue_printf("[%s-%d]-verif-: ", __FUNCTION__, __LINE__); \
        (void)rtlglue_printf(__VA_ARGS__);                                \
        (void)rtlglue_printf("\n");                                       \
    }
#else
#define DEBUG_VERIF(type, ...)                                        \
    if(0u != ((type)&DBG_MODULE_MASK))                                \
    {                                                                 \
        (void)rtlglue_printf("[%s-%d]-verif-: ", __func__, __LINE__); \
        (void)rtlglue_printf(__VA_ARGS__);                            \
        (void)rtlglue_printf("\n");                                   \
    }
#endif
#else
#define DEBUG_VERIF(type, ...) \
    do                         \
    {                          \
    } while(0)
#endif

#if(0 != (DBG_MASK & DBG_INFO))
#if __GNUC__ < 5
#define DEBUG_INFO(type, ...)                                            \
    if(0u != ((type)&DBG_MODULE_MASK))                                   \
    {                                                                    \
        (void)rtlglue_printf("[%s-%d]-info-: ", __FUNCTION__, __LINE__); \
        (void)rtlglue_printf(__VA_ARGS__);                               \
        (void)rtlglue_printf("\n");                                      \
    }
#else
#define DEBUG_INFO(type, ...)                                        \
    if(0u != ((type)&DBG_MODULE_MASK))                               \
    {                                                                \
        (void)rtlglue_printf("[%s-%d]-info-: ", __func__, __LINE__); \
        (void)rtlglue_printf(__VA_ARGS__);                           \
        (void)rtlglue_printf("\n");                                  \
    }
#endif
#else
#define DEBUG_INFO(type, ...) \
    do                        \
    {                         \
    } while(0)
#endif

#if(0 != ((DBG_MASK)&DBG_ERR))
#if __GNUC__ < 5
#define DEBUG_ERR(type, ...)                                            \
    do                                                                  \
    {                                                                   \
        if(type & (DBG_MODULE_MASK))                                    \
        {                                                               \
            rtlglue_printf("[%s-%d]-error-: ", __FUNCTION__, __LINE__); \
            rtlglue_printf(__VA_ARGS__);                                \
            rtlglue_printf("\n");                                       \
        }                                                               \
    } while(0);
#else
#define DEBUG_ERR(type, ...)                                        \
    do                                                              \
    {                                                               \
        if(type & (DBG_MODULE_MASK))                                \
        {                                                           \
            rtlglue_printf("[%s-%d]-error-: ", __func__, __LINE__); \
            rtlglue_printf(__VA_ARGS__);                            \
            rtlglue_printf("\n");                                   \
        }                                                           \
    } while(0);
#endif
#else
#define DEBUG_ERR(type, ...) \
    do                       \
    {                        \
    } while(0)
#endif

#if(0 != ((DBG_MASK)&DBG_MEMDUMP))
#if __GNUC__ < 5
#define DEBUG_MEMDUMP(type, ...)                                          \
    do                                                                    \
    {                                                                     \
        if(type & (DBG_MODULE_MASK))                                      \
        {                                                                 \
            rtlglue_printf("[%s-%d]-memdump-: ", __FUNCTION__, __LINE__); \
            rtlglue_printf(__VA_ARGS__);                                  \
            rtlglue_printf("\n");                                         \
        }                                                                 \
    } while(0);
#else
#define DEBUG_MEMDUMP(type, ...)                                      \
    do                                                                \
    {                                                                 \
        if(type & (DBG_MODULE_MASK))                                  \
        {                                                             \
            rtlglue_printf("[%s-%d]-memdump-: ", __func__, __LINE__); \
            rtlglue_printf(__VA_ARGS__);                              \
            rtlglue_printf("\n");                                     \
        }                                                             \
    } while(0);
#endif
#else
#define DEBUG_MEMDUMP(type, ...) \
    do                           \
    {                            \
    } while(0)
#endif

#ifndef RTL906X_DEBUG
#define ASSERT(expr) \
    do               \
    {                \
        if(expr)     \
            ;        \
    } while(0)
#else
#define ASSERT(expr)                                           \
    if((expr) == 0)                                            \
    {                                                          \
        rtlglue_printf("\033[33;41m%s:%d: assert(%s)\033[m\n", \
                       __FILE__, __LINE__, #expr);             \
    }
#endif
#ifndef RTL906X_DEBUG
#define PRINT_ERROR(fmt, ...) \
    do                        \
    {                         \
    } while(0)
#else
#if __GNUC__ < 5
#if 0
#define PRINT_ERROR(...)                                            \
    do                                                              \
    {                                                               \
        rtlglue_printf("[%s-%d]-ERROR-: ", __FUNCTION__, __LINE__); \
        rtlglue_printf(__VA_ARGS__);                                \
        rtlglue_printf("\n");                                       \
    } while(0);
#endif
#define PRINT_ERROR(fmt, ...)                                                               \
    do                                                                                      \
    {                                                                                       \
        rtlglue_printf("[%s-%d]-ERROR-: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while(0)
#else
#define PRINT_ERROR(...)                                        \
    do                                                          \
    {                                                           \
        rtlglue_printf("[%s-%d]-ERROR-: ", __func__, __LINE__); \
        rtlglue_printf(__VA_ARGS__);                            \
        rtlglue_printf("\n");                                   \
    } while(0);
#endif
#endif

#endif /*header file*/
