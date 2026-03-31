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

#include "rtkas_common_types.h"
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtkas_system_porting.h"

static int32 file_open(void **fp, uint8 mode, const char *path);
static int32 file_read(void *fp, uint8 *buf, uint32 offset, size_t size);
static void  file_close(void **fp);

static timer_get_cur_func g_fp_timer_get_cur = NULL;

usecond_delay_func g_fp_udelay    = NULL;
printf_func_ptr    rtlglue_printf = NULL;
fopen_func_ptr     rtl_fopen      = NULL;
fclose_func_ptr    rtl_fclose     = NULL;
fread_func_ptr     rtl_fread      = NULL;
fseek_func_ptr     rtl_fseek      = NULL;
memcmp_func_ptr    rtl_memcmp     = NULL;
memset_func_ptr    rtl_memset     = NULL;
memcpy_func_ptr    rtl_memcpy     = NULL;
strcmp_func_ptr    rtl_strcmp     = NULL;
strlen_func_ptr    rtl_strlen     = NULL;
random_func_ptr    rtl_random     = NULL;

fileOpen_func_ptr  rtl_file_open  = &file_open;
fileClose_func_ptr rtl_file_close = &file_close;
fileRead_func_ptr  rtl_file_read  = &file_read;

etherAccess_tx_ptr   rtk_etherAccess_tx_packet = NULL;
etherAccess_rx_ptr   rtk_etherAccess_rx_packet = NULL;
etherAccess_smac_ptr rtk_etherAccess_smac_get  = NULL;
etherAccess_dmac_ptr rtk_etherAccess_dmac_get  = NULL;

/* This global data pointer is remained for communicating to SDK APIs  */
uint8 *g_glbDataPtr = NULL;

/**
 * @addtogroup RTK_MODULE_SYS_PORT System Porting
 * @brief System Porting APIs. This component contains various APIs for user to register platform-depended callback functions.
 * @{
 */

/**
 * @addtogroup SYSPORT_GLOBAL_DATA Customer Data Access
 * @brief APIs to initialize and manipulate specific SDK data address
 * @{
 */

/**
 * @brief The global data pointer serves as an indication for the memory space that is designated for storing data that is read from or written to the SDK's data address.
 *
 * @param[in] dst                      Pointer to the destination data.
 *
 * @retval    RT_ERR_OK                Successfully initialized global data pointer.
 * @retval    -RT_ERR_NULL_POINTER     dst is NULL.
 */
RtkApiRet rtk_global_data_init(uint8 *dst)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == dst)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        g_glbDataPtr = dst;
    }

    return retVal;
}

/**
 * @brief To read the data from SDK to the address that global data pointer point to
 *
 * @param[in] dst                      Pointer to the destination buffer.
 * @param[in] size                     Size of the data to copy.
 *
 * @retval    RT_ERR_OK                Successfully got global data.
 * @retval    -RT_ERR_NULL_POINTER     g_glbDataPtr or dst is NULL.
 */
RtkApiRet rtk_global_data_get(uint8 *dst, uint32 size)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == g_glbDataPtr) || (NULL == dst))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        (void)rtl_memcpy(dst, g_glbDataPtr, size);
    }

    return retVal;
}

/**
 * @brief To write the data to specific SDK data address from the address that global data pointer point to
 *
 * @param[in] src                      Pointer to the source data.
 * @param[in] size                     Size of the data to copy.
 *
 * @retval    RT_ERR_OK                Successfully set global data.
 * @retval    -RT_ERR_NULL_POINTER     g_glbDataPtr or src is NULL.
 */
RtkApiRet rtk_global_data_set(const uint8 *src, uint32 size)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((NULL == g_glbDataPtr) || (NULL == src))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        (void)rtl_memcpy(g_glbDataPtr, src, size);
    }

    return retVal;
}

/**@}*/ /* SYSPORT_GLOBAL_DATA */

/**
 * @addtogroup SYSPORT_TIMER Timer Function Initialization
 * @brief APIs to initialize and manipulate time-related functions
 * @{
 */

/**
 * @brief Initialize microsecond delay function.
 *
 * @param[in] delayFunc                Pointer to the delay function.
 *
 * @retval    RT_ERR_OK                Successfully initialized microsecond delay function.
 * @retval    -RT_ERR_NULL_POINTER     delayFunc is NULL.
 */
RtkApiRet rtk_usecond_delay_init(usecond_delay_func delayFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == delayFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        g_fp_udelay = delayFunc;
    }

    return retVal;
}

/**
 * @brief Delay for a specified number of microseconds.
 *
 * @param[in] delayTime                Number of microseconds to delay.
 *
 * @retval    RT_ERR_OK                Successfully delayed.
 * @retval    -RT_ERR_SEC_DLY          Delay function failed.
 */
RtkApiRet rtk_usecond_delay(uint32 delayTime)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(RT_ERR_OK != g_fp_udelay(delayTime))
    {
        retVal = -RT_ERR_SEC_DLY;
    }

    return retVal;
}

/**
 * @brief Initialize timer get current function.
 *
 * @param[in] timerGetCurFunc          Pointer to the timer get current function.
 *
 * @retval    RT_ERR_OK                Successfully initialized timer get current function.
 * @retval    -RT_ERR_NULL_POINTER     timerGetCurFunc is NULL.
 */
RtkApiRet rtk_timer_get_current_init(timer_get_cur_func timerGetCurFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == timerGetCurFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        g_fp_timer_get_cur = timerGetCurFunc;
    }

    return retVal;
}

/**
 * @brief Get the current timer value in microseconds.
 *
 * @param[in] us                       Pointer to store the current timer value.
 *
 * @retval    RT_ERR_OK                Successfully got current timer value.
 * @retval    -RT_ERR_TIMER_GET_CUR_ERROR Timer get current function failed.
 */
RtkApiRet rtk_timer_get_current(uint32 *us)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(RT_ERR_OK != g_fp_timer_get_cur(us))
    {
        retVal = -RT_ERR_TIMER_GET_CUR_ERROR;
    }

    return retVal;
}

/**@}*/ /* SYSPORT_TIMER */

/**
 * @addtogroup SYSPORT_STDLIB Common Stanand Library Initialization
 * @brief APIs to register essential functions of standard library for the SDK
 * @{
 */

/**
 * @brief Initialize printf function.
 *
 * @param[in] printfFunc               Pointer to the printf function.
 *
 * @retval    RT_ERR_OK                Successfully initialized printf function.
 * @retval    -RT_ERR_NULL_POINTER     printfFunc is NULL.
 */
RtkApiRet rtk_printf_init(printf_func_ptr printfFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == printfFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtlglue_printf = printfFunc;
    }

    return retVal;
}

/**
 * @brief Initialize file open function.
 *
 * @param[in] fileOpenFunc             Pointer to the file open function.
 *
 * @retval    RT_ERR_OK                Successfully initialized file open function.
 * @retval    -RT_ERR_NULL_POINTER     fileOpenFunc is NULL.
 */
RtkApiRet rtk_file_open_init(fileOpen_func_ptr fileOpenFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == fileOpenFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_file_open = fileOpenFunc;
    }

    return retVal;
}

/**
 * @brief Initialize file close function.
 *
 * @param[in] fileCloseFunc            Pointer to the file close function.
 *
 * @retval    RT_ERR_OK                Successfully initialized file close function.
 * @retval    -RT_ERR_NULL_POINTER     fileCloseFunc is NULL.
 */
RtkApiRet rtk_file_close_init(fileClose_func_ptr fileCloseFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == fileCloseFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_file_close = fileCloseFunc;
    }

    return retVal;
}

/**
 * @brief Initialize file read function.
 *
 * @param[in] fileReadFunc             Pointer to the file read function.
 *
 * @retval    RT_ERR_OK                Successfully initialized file read function.
 * @retval    -RT_ERR_NULL_POINTER     fileReadFunc is NULL.
 */
RtkApiRet rtk_file_read_init(fileRead_func_ptr fileReadFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == fileReadFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_file_read = fileReadFunc;
    }

    return retVal;
}

/**
 * @brief Initialize fopen function.
 *
 * @param[in] fopenFunc                Pointer to the fopen function.
 *
 * @retval    RT_ERR_OK                Successfully initialized fopen function.
 * @retval    -RT_ERR_NULL_POINTER     fopenFunc is NULL.
 */
RtkApiRet rtk_fopen_init(fopen_func_ptr fopenFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == fopenFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_fopen = fopenFunc;
    }

    return retVal;
}

/**
 * @brief Initialize fclose function.
 *
 * @param[in] fcloseFunc               Pointer to the fclose function.
 *
 * @retval    RT_ERR_OK                Successfully initialized fclose function.
 * @retval    -RT_ERR_NULL_POINTER     fcloseFunc is NULL.
 */
RtkApiRet rtk_fclose_init(fclose_func_ptr fcloseFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == fcloseFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_fclose = fcloseFunc;
    }

    return retVal;
}

/**
 * @brief Initialize fread function.
 *
 * @param[in] freadFunc                Pointer to the fread function.
 *
 * @retval    RT_ERR_OK                Successfully initialized fread function.
 * @retval    -RT_ERR_NULL_POINTER     freadFunc is NULL.
 */
RtkApiRet rtk_fread_init(fread_func_ptr freadFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == freadFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_fread = freadFunc;
    }

    return retVal;
}

/**
 * @brief Initialize fseek function.
 *
 * @param[in] fseekFunc                Pointer to the fseek function.
 *
 * @retval    RT_ERR_OK                Successfully initialized fseek function.
 * @retval    -RT_ERR_NULL_POINTER     fseekFunc is NULL.
 */
RtkApiRet rtk_fseek_init(fseek_func_ptr fseekFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == fseekFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_fseek = fseekFunc;
    }

    return retVal;
}

/**
 * @brief Initialize memset function.
 *
 * @param[in] memsetFunc               Pointer to the memset function.
 *
 * @retval    RT_ERR_OK                Successfully initialized memset function.
 * @retval    -RT_ERR_NULL_POINTER     memsetFunc is NULL.
 */
RtkApiRet rtk_memset_init(memset_func_ptr memsetFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == memsetFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_memset = memsetFunc;
    }

    return retVal;
}

/**
 * @brief Initialize memcpy function.
 *
 * @param[in] memcpyFunc               Pointer to the memcpy function.
 *
 * @retval    RT_ERR_OK                Successfully initialized memcpy function.
 * @retval    -RT_ERR_NULL_POINTER     memcpyFunc is NULL.
 */
RtkApiRet rtk_memcpy_init(memcpy_func_ptr memcpyFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == memcpyFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_memcpy = memcpyFunc;
    }

    return retVal;
}

/**
 * @brief Initialize memcmp function.
 *
 * @param[in] memcmpFunc               Pointer to the memcmp function.
 *
 * @retval    RT_ERR_OK                Successfully initialized memcmp function.
 * @retval    -RT_ERR_NULL_POINTER     memcmpFunc is NULL.
 */
RtkApiRet rtk_memcmp_init(memcmp_func_ptr memcmpFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == memcmpFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_memcmp = memcmpFunc;
    }

    return retVal;
}

/**
 * @brief Initialize strcmp function.
 *
 * @param[in] strcmpFunc               Pointer to the strcmp function.
 *
 * @retval    RT_ERR_OK                Successfully initialized strcmp function.
 * @retval    -RT_ERR_NULL_POINTER     strcmpFunc is NULL.
 */
RtkApiRet rtk_strcmp_init(strcmp_func_ptr strcmpFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == strcmpFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_strcmp = strcmpFunc;
    }

    return retVal;
}

/**
 * @brief Initialize strlen function.
 *
 * @param[in] strlenFunc               Pointer to the strlen function.
 *
 * @retval    RT_ERR_OK                Successfully initialized strlen function.
 * @retval    -RT_ERR_NULL_POINTER     strlenFunc is NULL.
 */
RtkApiRet rtk_strlen_init(strlen_func_ptr strlenFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == strlenFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_strlen = strlenFunc;
    }

    return retVal;
}

/**
 * @brief Open a file.
 *
 * @param[in] fp                       Pointer to the file pointer.
 * @param[in] mode                     Mode to open the file (0 for read, 1 for write).
 * @param[in] path                     Path to the file.
 *
 * @retval    RT_ERR_OK                Successfully opened the file.
 * @retval    -RT_ERR_NULL_POINTER     fp or path is NULL.
 * @retval    -RT_ERR_FAILED           Failed to open the file.
 */
static int32 file_open(void **fp, uint8 mode, const char *path)
{
    RtkApiRet retVal = RT_ERR_OK;

    if((fp == NULL) || (path == NULL))
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        if((uint8)0u == mode)
        {
            *fp = rtl_fopen(path, "rb");
        }
        else if((uint8)1u == mode)
        {
            *fp = rtl_fopen(path, "w");
        }
        else
        {
            retVal = -RT_ERR_FAILED;
        }

        if(*fp == NULL)
        {
            (void)rtlglue_printf("FILE %s open fail!\n", path);
            retVal = -RT_ERR_FAILED;
        }
    }

    return (int32)retVal;
}

/**
 * @brief Read from a file.
 *
 * @param[in] fp                       Pointer to the file.
 * @param[in] buf                      Buffer to store the read data.
 * @param[in] offset                   Offset to start reading from.
 * @param[in] size                     Number of bytes to read.
 *
 * @retval    RT_ERR_OK                Successfully read from the file.
 * @retval    -RT_ERR_NULL_POINTER     fp or buf is NULL.
 * @retval    -RT_ERR_FAILED           Failed to read from the file.
 */
static int32 file_read(void *fp, uint8 *buf, uint32 offset, size_t size)
{
    int32 retVal;
    do
    {
        if((fp == NULL) || (buf == NULL))
        {
            (void)rtlglue_printf("NULL pointer!\n");
            retVal = 0;
            continue;
        }

        RtkApiRet ret = rtl_fseek(fp, offset, SEEK_SET_RTK);
        if(ret != RT_ERR_OK)
        {
            (void)rtlglue_printf("FILE read fail!\n");
            retVal = 0;
            continue;
        }
        retVal = rtl_fread(buf, sizeof(char), size, (void *)fp);
    } while(0u == 1u);

    return retVal;
}

/**
 * @brief Close a file.
 *
 * @param[in] fp                       Pointer to the file pointer.
 */
static void file_close(void **fp)
{
    if((fp != NULL) && (*fp != NULL))
    {
        (void)rtl_fclose((void *)*fp);
        *fp = NULL;
    }
}

/**
 * @brief Initialize random function.
 *
 * @param[in] randomFunc               Pointer to the random function.
 *
 * @retval    RT_ERR_OK                Successfully initialized random function.
 * @retval    -RT_ERR_NULL_POINTER     randomFunc is NULL.
 */
RtkApiRet rtk_random_init(random_func_ptr randomFunc)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == randomFunc)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtl_random = randomFunc;
    }

    return retVal;
}

/**@}*/ /* SYSPORT_STDLIB */

/**
 * @addtogroup SYSPORT_ETHACC Ethernet Access Function Initialization
 * @brief APIs to initialize the necessary functions of Ethernet Access
 * @{
 */

#if defined(RTK_MODULE_ETHERNET_ACCESS) || defined(RTK_MODULE_PHY_TEST_MODE) || defined(RTK_MODULE_SYSTEM) || defined(RTK_MODULE_INTERFACE)
/**
 * @brief Initialize Ethernet access TX packet function.
 *
 * @param[in] etherAccess_tx_func      Pointer to the Ethernet access TX packet function.
 *
 * @retval    RT_ERR_OK                Successfully initialized Ethernet access TX packet function.
 * @retval    -RT_ERR_NULL_POINTER     etherAccess_tx_func is NULL.
 */
RtkApiRet rtk_eth_acc_tx_packet_init(etherAccess_tx_ptr etherAccess_tx_func)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == etherAccess_tx_func)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtk_etherAccess_tx_packet = etherAccess_tx_func;
    }

    return retVal;
}

/**
 * @brief Initialize Ethernet access RX packet function.
 *
 * @param[in] etherAccess_rx_func      Pointer to the Ethernet access RX packet function.
 *
 * @retval    RT_ERR_OK                Successfully initialized Ethernet access RX packet function.
 * @retval    -RT_ERR_NULL_POINTER     etherAccess_rx_func is NULL.
 */
RtkApiRet rtk_eth_acc_rx_packet_init(etherAccess_rx_ptr etherAccess_rx_func)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == etherAccess_rx_func)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtk_etherAccess_rx_packet = etherAccess_rx_func;
    }

    return retVal;
}
#endif

#if defined(RTK_MODULE_ETHERNET_ACCESS)
/**
 * @brief Initialize Ethernet access SMAC function.
 *
 * @param[in] etherAccess_smac_func    Pointer to the Ethernet access SMAC function.
 *
 * @retval    RT_ERR_OK                Successfully initialized Ethernet access SMAC function.
 * @retval    -RT_ERR_NULL_POINTER     etherAccess_smac_func is NULL.
 */
RtkApiRet rtk_eth_acc_smac_init(etherAccess_smac_ptr etherAccess_smac_func)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == etherAccess_smac_func)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtk_etherAccess_smac_get = etherAccess_smac_func;
    }

    return retVal;
}

/**
 * @brief Initialize Ethernet access DMAC function.
 *
 * @param[in] etherAccess_dmac_func    Pointer to the Ethernet access DMAC function.
 *
 * @retval    RT_ERR_OK                Successfully initialized Ethernet access DMAC function.
 * @retval    -RT_ERR_NULL_POINTER     etherAccess_dmac_func is NULL.
 */
RtkApiRet rtk_eth_acc_dmac_init(etherAccess_dmac_ptr etherAccess_dmac_func)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(NULL == etherAccess_dmac_func)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        rtk_etherAccess_dmac_get = etherAccess_dmac_func;
    }

    return retVal;
}
#endif

/**@}*/ /* SYSPORT_ETHACC */

/**@}*/ /* RTK_MODULE_SYS_PORT */
