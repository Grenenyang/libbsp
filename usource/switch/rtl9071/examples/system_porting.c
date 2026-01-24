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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rtkas_error.h"
#include "rtkas_types.h"
#include "rtkas_system_porting.h"
#ifdef __linux__
#include <unistd.h>
#endif
/* Declaration */
/**
 * @brief Register standard library API.
 *
 * @section Purpose
 * This example demonstrates how to register C standard library APIs to SDK.
 *
 * @section Description
 * This function registers a set of standard C library functions (such as memcmp, memset, memcpy, and strcmp)
 * and custom library functions (such as printf, fopen, fclose, fread, fseek, usleep, and timer_get_cur) to the SDK.
 * It ensures that these functions are available for use within the SDK.
 *
 * @warning It may cause unexpected behavior while calling SDK APIs without registering following APIs.
 *
 * @return RtkApiRet RT_ERR_OK on success, error code otherwise.
 */
RtkApiRet sys_porting_stdlib(void);

/**
 * @brief Register and access external data API.
 *
 * @section Purpose
 * This example demonstrates how to register and access the data of customer size to SDK.
 *
 * @section Description
 * This example demonstrates how to register an external data address, set a value to it, and then get the value.
 *
 * @return RtkApiRet RT_ERR_OK on success, error code otherwise.
 */
RtkApiRet sys_custom_data_access(void);

static int32 my_printf_wrapper(char const *format, ...);
static void *my_fopen_wrapper(const char *path, const char *mode);
static int32 my_fclose_wrapper(void *fp);
static int32 my_fread_wrapper(void *ptr, size_t size, size_t nmemb, void *fp);
static int32 my_fseek_wrapper(void *fp, uint32 offset, int32 origin);
static int32 my_usecond_delay_wrapper(uint32 delayTime);
static int32 my_timer_get_cur_wrapper(uint32 *us);

/**
 * @brief Custom printf wrapper function.
 *
 * @section Purpose
 * This function is a wrapper for the standard printf function.
 *
 * @section Description
 * Users should implement this function according to their development environment
 * and return an appropriate value.
 *
 * @param[in] format The format string.
 *
 * @return The number of characters printed, or a negative value if an error occurs.
 */
static int32 my_printf_wrapper(char const *format, ...)
{
    /* implement printf here */
    (void)format;
    return 0;
}

/**
 * @brief Custom fopen wrapper function.
 *
 * @section Purpose
 * This function is a wrapper for the standard fopen function.
 *
 * @section Description
 * Users should implement this function according to their development environment
 * and return an appropriate file pointer.
 *
 * @param[in] path The file path.
 * @param[in] mode The file mode.
 *
 * @return A file pointer on success, or NULL if an error occurs.
 */
static void *my_fopen_wrapper(const char *path, const char *mode)
{
    /* implement fopen here */
    (void)path;
    (void)mode;
    return NULL;
}

/**
 * @brief Custom fclose wrapper function.
 *
 * @section Purpose
 * This function is a wrapper for the standard fclose function.
 *
 * @section Description
 * Users should implement this function according to their development environment
 * and return an appropriate value.
 *
 * @param[in] fp The file pointer.
 *
 * @return 0 on success, or EOF if an error occurs.
 */
static int32 my_fclose_wrapper(void *fp)
{
    /* implement fclose here */
    (void)fp;
    return 0;
}

/**
 * @brief Custom fread wrapper function.
 *
 * @section Purpose
 * This function is a wrapper for the standard fread function.
 *
 * @section Description
 * Users should implement this function according to their development environment
 * and return the number of elements successfully read.
 *
 * @param[in] ptr   The buffer to read into.
 * @param[in] size  The size of each element to read.
 * @param[in] nmemb The number of elements to read.
 * @param[in] fp    The file pointer.
 *
 * @return The number of elements successfully read, or 0 if an error occurs.
 */
static int32 my_fread_wrapper(void *ptr, size_t size, size_t nmemb, void *fp)
{
    /* implement fread here */
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)fp;

    return 0;
}

/**
 * @brief Custom fseek wrapper function.
 *
 * @section Purpose
 * This function is a wrapper for the standard fseek function.
 *
 * @section Description
 * Users should implement this function according to their development environment
 * and return an appropriate value.
 *
 * @param[in] fp     The file pointer.
 * @param[in] offset The offset to seek to.
 * @param[in] origin The origin from where to seek.
 *
 * @return 0 on success, or a non-zero value if an error occurs.
 */
static int32 my_fseek_wrapper(void *fp, uint32 offset, int32 origin)
{
    /* implement fseek here */
    (void)fp;
    (void)offset;
    (void)origin;

    return 0;
}

/**
 * @brief Custom microsecond delay wrapper function.
 *
 * @section Purpose
 * This function is a wrapper for the usleep function.
 *
 * @section Description
 * Users should implement this function according to their development environment
 * and return an appropriate value.
 *
 * @param[in] delayTime The delay time in microseconds.
 *
 * @return 0 on success, or a non-zero value if an error occurs.
 */
static int32 my_usecond_delay_wrapper(uint32 delayTime)
{
#ifdef __linux__
    return usleep(delayTime);
#else
    return 0;
#endif
}

/**
 * @brief Custom timer get current time wrapper function.
 *
 * @section Purpose
 * This function is a wrapper for getting the current time in microseconds.
 *
 * @section Description
 * Users should implement this function according to their development environment
 * and return an appropriate value.
 *
 * @param[out] us The current time in microseconds.
 *
 * @return 0 on success, or a non-zero value if an error occurs.
 */
static int32 my_timer_get_cur_wrapper(uint32 *us)
{
    int32 retVal;

#if 0
    struct timespec curTime;
#endif

    if(NULL == us)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else
    {
        retVal = RT_ERR_OK;
    }

#if 0
    CHK_FUN_RET(retVal, clock_gettime(CLOCK_REALTIME, &curTime));
    if(RT_ERR_OK == retVal)
    {
        *us = (uint32)(curTime.tv_sec * 1000000 + (curTime.tv_nsec / 1000u));
    }
#endif

    return retVal;
}

/* Function */
/**
 * @addtogroup EXAMPLE
 * @{
 */

/**
 * @addtogroup SYS_PORTING_EX System Porting Example
 * @{
 */

/**
 * @addtogroup SYS_PORTING_STDLIB System Porting Standard Library Example
 * @{
 */
/**
 * @brief Register standard library API.
 *
 * @section Purpose
 * This example demonstrates how to register C standard library APIs to SDK.
 *
 * @section Description
 * This function registers a set of standard C library functions (such as memcmp, memset, memcpy, and strcmp)
 * and custom library functions (such as printf, fopen, fclose, fread, fseek, usleep, and timer_get_cur) to the SDK.
 * It ensures that these functions are available for use within the SDK.
 *
 * @warning It may cause unexpected behavior while calling SDK APIs without registering following APIs.
 *
 * @return RtkApiRet RT_ERR_OK on success, error code otherwise.
 */
RtkApiRet sys_porting_stdlib(void)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* register c standard library API */
        CHK_FUN_CONTINUE(ret, rtk_memcmp_init(&memcmp));
        CHK_FUN_CONTINUE(ret, rtk_memset_init(&memset));
        CHK_FUN_CONTINUE(ret, rtk_memcpy_init(&memcpy));
        CHK_FUN_CONTINUE(ret, rtk_strcmp_init(&strcmp));

        /* register customer own library API */
        CHK_FUN_CONTINUE(ret, rtk_printf_init(&my_printf_wrapper));
        CHK_FUN_CONTINUE(ret, rtk_fopen_init(&my_fopen_wrapper));
        CHK_FUN_CONTINUE(ret, rtk_fclose_init(&my_fclose_wrapper));
        CHK_FUN_CONTINUE(ret, rtk_fread_init(&my_fread_wrapper));
        CHK_FUN_CONTINUE(ret, rtk_fseek_init(&my_fseek_wrapper));
        CHK_FUN_CONTINUE(ret, rtk_usecond_delay_init(&my_usecond_delay_wrapper));
        CHK_FUN_CONTINUE(ret, rtk_timer_get_current_init(&my_timer_get_cur_wrapper));

    } while(false);

    return ret;
}
/**@}*/ /* SYS_PORTING_STDLIB */

/**
 * @defgroup CUST_DATA_EX Custom Data Access
 * @{
 */

/**
 * @brief Register and access external data API.
 *
 * @section Purpose
 * This example demonstrates how to register and access the data of customer size to SDK.
 *
 * @section Description
 * This example demonstrates how to register an external data address, set a value to it, and then get the value.
 *
 * @return RtkApiRet RT_ERR_OK on success, error code otherwise.
 */
RtkApiRet sys_custom_data_access(void)
{
    RtkApiRet ret = RT_ERR_OK;

    static uint8 extData  = 0;
    const uint32 dataSize = 1u;
    uint8        setVal   = 1u;
    uint8        getVal   = 0u;

    do
    {
        /* register external data address */
        CHK_FUN_CONTINUE(ret, rtk_global_data_init(&extData));
        /* Set external data address */
        CHK_FUN_CONTINUE(ret, rtk_global_data_set(&setVal, dataSize));
        /* Get external data address */
        CHK_FUN_CONTINUE(ret, rtk_global_data_get(&getVal, dataSize));

    } while(false);

    return ret;
}
/**@}*/ /* CUST_DATA_EX */
/**@}*/ /* SYS_PORTING_EX */
/**@}*/ /* EXAMPLE */
