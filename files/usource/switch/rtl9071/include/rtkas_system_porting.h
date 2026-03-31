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

#ifndef RTKAS_SYSTEM_PORTING_H
#define RTKAS_SYSTEM_PORTING_H

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/

/******************************************************************************/
/* Defines                                                                    */
/******************************************************************************/

#define REGIF_I2C      (0u)
#define REGIF_MDCMDIO  (1u)
#define REGIF_SPI      (2u)
#define REGIF_PCIE     (3u)
#define REGIF_ETHERNET (4u)
#define REGIF_END      (5u)
#define REGIF_EXT_CFG  (100u)

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/

/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/

/**
 * @brief Function pointer type for microsecond delay function.
 */
typedef int32 (*usecond_delay_func)(uint32 delayTime);

/**
 * @brief Function pointer type for getting the current timer value.
 */
typedef int32 (*timer_get_cur_func)(uint32 *us);

/**
 * @brief Function pointer type for printf function.
 */
typedef int32 (*printf_func_ptr)(char const *format, ...);

/**
 * @brief Function pointer type for opening a file.
 */
typedef int32 (*fileOpen_func_ptr)(void **fp, uint8 mode, const char *path);

/**
 * @brief Function pointer type for closing a file.
 */
typedef void (*fileClose_func_ptr)(void **fp);

/**
 * @brief Function pointer type for reading from a file.
 */
typedef int32 (*fileRead_func_ptr)(void *fp, uint8 *buf, uint32 offset, size_t size);

/**
 * @brief Function pointer type for opening a file with mode.
 */
typedef void *(*fopen_func_ptr)(const char *path, const char *mode);

/**
 * @brief Function pointer type for closing a file.
 */
typedef int32 (*fclose_func_ptr)(void *fp);

/**
 * @brief Function pointer type for reading from a file.
 */
typedef int32 (*fread_func_ptr)(void *ptr, size_t size, size_t nmemb, void *fp);

/**
 * @brief Function pointer type for seeking within a file.
 */
typedef int32 (*fseek_func_ptr)(void *fp, uint32 offset, int32 origin);

/**
 * @brief Function pointer type for comparing memory blocks.
 */
typedef int32 (*memcmp_func_ptr)(const void *prt1, const void *ptr2, size_t num);

/**
 * @brief Function pointer type for setting memory.
 */
typedef void *(*memset_func_ptr)(void *ptr, int value, size_t num);

/**
 * @brief Function pointer type for copying memory.
 */
typedef void *(*memcpy_func_ptr)(void *destination, const void *source, size_t num);

/**
 * @brief Function pointer type for comparing strings.
 */
typedef int32 (*strcmp_func_ptr)(const char *str1, const char *str2);

/**
 * @brief Function pointer type for getting the length of a string.
 */
typedef size_t (*strlen_func_ptr)(const char *str);

/**
 * @brief Function pointer type for generating a random number.
 */
typedef int32 (*random_func_ptr)(void);

/**
 * @brief Function pointer type for transmitting an Ethernet packet.
 */
typedef RtkApiRet (*etherAccess_tx_ptr)(const uint8 *txPkt, const uint32 len);

/**
 * @brief Function pointer type for receiving an Ethernet packet.
 */
typedef RtkApiRet (*etherAccess_rx_ptr)(uint8 *rxPkt, uint32 *len);

/**
 * @brief Function pointer type for getting the source MAC address.
 */
typedef RtkApiRet (*etherAccess_smac_ptr)(uint8 *macAddr);

/**
 * @brief Function pointer type for getting the destination MAC address.
 */
typedef RtkApiRet (*etherAccess_dmac_ptr)(uint8 *macAddr);

/**
 * @brief Function pointer type for reading a 32-bit register via the register interface.
 */
typedef RtkApiRet (*regif_read_ptr)(uint32 regAddr, uint32 *value);

/**
 * @brief Function pointer type for writing a 32-bit value to a register via the register interface.
 */
typedef RtkApiRet (*regif_write_ptr)(uint32 regAddr, uint32 value);

/**
 * @brief Function pointer type for burst reading from registers via the register interface.
 */
typedef RtkApiRet (*regif_read_burst_ptr)(uint32 regAddr, uint32 *value, uint32 length);

/**
 * @brief Function pointer type for burst writing to registers via the register interface.
 */
typedef RtkApiRet (*regif_write_burst_ptr)(uint32 regAddr, const uint32 *value, uint32 length);

/**
 * @brief Function pointer type for writing bytes to a register via the register interface.
 */
typedef RtkApiRet (*regif_write_bytes_ptr)(uint32 regAddr, uint32 value);

/**
 * @brief Function pointer type for indirect partial writing a 32-bit value to a register via the register interface.
 */
typedef RtkApiRet (*regif_write_partial_indir_ptr)(uint32 regAddr, uint32 mask, uint32 value);

/**
 * @brief Structure to hold callback functions for register interface operations.
 */
typedef struct
{
    regif_read_ptr                readFunc;              /**< [in] Function pointer for reading a register. */
    regif_write_ptr               writeFunc;             /**< [in] Function pointer for writing to a register. */
    regif_read_burst_ptr          readBurstFunc;         /**< [in] Function pointer for burst reading from registers. */
    regif_write_burst_ptr         writeBurstFunc;        /**< [in] Function pointer for burst writing to registers. */
    regif_write_bytes_ptr         writeBytesFunc;        /**< [in] Function pointer for writing bytes to a register. */
    regif_write_partial_indir_ptr writePartialIndirFunc; /**< [in] Function pointer for indirect partial writing to a register. (Ethernet Access only) */
} RegifCallback_t;

/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
extern printf_func_ptr      rtlglue_printf;
extern fileOpen_func_ptr    rtl_file_open;
extern fileClose_func_ptr   rtl_file_close;
extern fileRead_func_ptr    rtl_file_read;
extern fopen_func_ptr       rtl_fopen;
extern fclose_func_ptr      rtl_fclose;
extern fread_func_ptr       rtl_fread;
extern fseek_func_ptr       rtl_fseek;
extern memcmp_func_ptr      rtl_memcmp;
extern memset_func_ptr      rtl_memset;
extern memcpy_func_ptr      rtl_memcpy;
extern strcmp_func_ptr      rtl_strcmp;
extern strlen_func_ptr      rtl_strlen;
extern random_func_ptr      rtl_random;
extern etherAccess_tx_ptr   rtk_etherAccess_tx_packet;
extern etherAccess_rx_ptr   rtk_etherAccess_rx_packet;
extern etherAccess_smac_ptr rtk_etherAccess_smac_get;
extern etherAccess_dmac_ptr rtk_etherAccess_dmac_get;
extern usecond_delay_func   g_fp_udelay;

/* This global data pointer is remained for communicating to SDK APIs  */
extern uint8 *g_glbDataPtr;

extern uint8 g_RegIF;
/*******************************************************************************
 *   Function Prototype
 *******************************************************************************/
EXTERN_API RtkApiRet rtk_regif_init(uint32 regifType, const RegifCallback_t *regifCallback);

EXTERN_API RtkApiRet rtk_printf_init(printf_func_ptr printfFunc);
EXTERN_API RtkApiRet rtk_fopen_init(fopen_func_ptr fopenFunc);
EXTERN_API RtkApiRet rtk_fclose_init(fclose_func_ptr fcloseFunc);
EXTERN_API RtkApiRet rtk_fread_init(fread_func_ptr freadFunc);
EXTERN_API RtkApiRet rtk_fseek_init(fseek_func_ptr fseekFunc);
EXTERN_API RtkApiRet rtk_memset_init(memset_func_ptr memsetFunc);
EXTERN_API RtkApiRet rtk_memcpy_init(memcpy_func_ptr memcpyFunc);
EXTERN_API RtkApiRet rtk_memcmp_init(memcmp_func_ptr memcmpFunc);
EXTERN_API RtkApiRet rtk_strcmp_init(strcmp_func_ptr strcmpFunc);
EXTERN_API RtkApiRet rtk_strlen_init(strlen_func_ptr strlenFunc);
EXTERN_API RtkApiRet rtk_random_init(random_func_ptr randomFunc);

EXTERN_API RtkApiRet rtk_file_open_init(fileOpen_func_ptr fileOpenFunc);
EXTERN_API RtkApiRet rtk_file_close_init(fileClose_func_ptr fileCloseFunc);
EXTERN_API RtkApiRet rtk_file_read_init(fileRead_func_ptr fileReadFunc);

EXTERN_API RtkApiRet rtk_eth_acc_tx_packet_init(etherAccess_tx_ptr etherAccess_tx_func);
EXTERN_API RtkApiRet rtk_eth_acc_rx_packet_init(etherAccess_rx_ptr etherAccess_rx_func);
EXTERN_API RtkApiRet rtk_eth_acc_smac_init(etherAccess_smac_ptr etherAccess_smac_func);
EXTERN_API RtkApiRet rtk_eth_acc_dmac_init(etherAccess_dmac_ptr etherAccess_dmac_func);

EXTERN_API RtkApiRet rtk_global_data_init(uint8 *dst);
EXTERN_API RtkApiRet rtk_usecond_delay_init(usecond_delay_func delayFunc);
EXTERN_API RtkApiRet rtk_timer_get_current_init(timer_get_cur_func timerGetCurFunc);

EXTERN_API RtkApiRet rtk_usecond_delay(uint32 delayTime);
EXTERN_API RtkApiRet rtk_timer_get_current(uint32 *us);
EXTERN_API RtkApiRet rtk_global_data_get(uint8 *dst, uint32 size);
EXTERN_API RtkApiRet rtk_global_data_set(const uint8 *src, uint32 size);

#endif /* RTKAS_SYSTEM_PORTING_H */
