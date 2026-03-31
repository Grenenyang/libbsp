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

#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_system_porting.h"

uint32 g_dbgMsk = 0;

/**
 * @brief Memory set
 *
 * @param[out] str Pointer to the block of memory to fill (N/A)
 * @param[in] val Value to be set (0x0 to 0xFFFFFFFF)
 * @param[in] size Number of bytes to be set to the value (N/A)
 * @retval RT_ERR_OK Memory set successfully
 * @retval -RT_ERR_MEM_ACCESS Memory set failed
 */
RtkApiRet drv_memset(void *str, int32 val, size_t size)
{
    RtkApiRet ret = RT_ERR_OK;

    if(str != rtl_memset(str, val, size))
    {
        ret = -RT_ERR_MEM_ACCESS;
    }

    return ret;
}

/**
 * @brief Memory copy
 *
 * @param[out] dst Pointer to the destination array where the content is to be copied (N/A)
 * @param[in] src Pointer to the source of data to be copied (N/A)
 * @param[in] size Number of bytes to copy (N/A)
 * @retval RT_ERR_OK Memory copy successfully
 * @retval -RT_ERR_MEM_ACCESS Memory copy failed
 */
RtkApiRet drv_memcpy(void *dst, const void *src, size_t size)
{
    RtkApiRet ret = RT_ERR_OK;

    if(dst != rtl_memcpy(dst, src, size))
    {
        ret = -RT_ERR_MEM_ACCESS;
    }

    return ret;
}

/**
 * @brief Endian convert
 *
 * @param[in,out] src Pointer to the source of data, the data will be swapped if dst is NULL (N/A)
 * @param[out] dst Pointer to the destination array where the content is to be set (N/A)
 * @param[in] len Number of bytes to swap (N/A)
 * @retval RT_ERR_OK Endian convert successfully
 * @retval -RT_ERR_MEM_ACCESS Pointer to the source of data is point to NULL address
 */
RtkApiRet drv_endian_change(uint8 *src, uint8 *dst, uint32 len)
{
    RtkApiRet ret   = RT_ERR_OK;
    int32     left  = 0;
    int32     right = 0;
    uint8     tmp;

    if(0u == len)
    {
        ret = -RT_ERR_INPUT;
    }
    else if(NULL != src)
    {
        right = len - 1u;

        if(NULL == dst)
        {
            while(left < right)
            {
                tmp        = src[left];
                src[left]  = src[right];
                src[right] = tmp;

                right--;
                left++;
            }
        }
        else
        {
            while(0 <= right)
            {
                dst[right] = src[left];

                right--;
                left++;
            }
        }
    }
    else
    {
        ret = -RT_ERR_NULL_POINTER;
    }

    return ret;
}