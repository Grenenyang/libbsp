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

#include "rtl907xd_va_api_ext.h"
#include "rtl907xd_va_api_reset.h"
#include "rtkas_types.h"

/* Declaration */
RtkApiRet sys_reg_interface_init(void);

/* ********************************************************** */
/*  platform-depent code                                      */
/* ********************************************************** */
/**
 * @brief Platform I2C read function.
 *
 * @section Purpose
 * This function reads a value from the specified register address using the I2C interface.
 *
 * @section Description
 * This function performs an I2C read operation to retrieve the value stored at the given
 * register address. The register address and the value are both in little-endian format.
 * Please refer to the I2C protocol described in the datasheet.
 *
 * @param[in]  regAddr The register address to read from.
 * @param[out] value   Pointer to store the read value.
 *
 * @return RtkApiRet   RT_ERR_OK on success, error code otherwise.
 */
static RtkApiRet platform_i2c_read(uint32 regAddr, uint32 *value)
{
    /* implement i2c driver here */
    (void)regAddr;
    (void)value;

    return RT_ERR_OK;
}

/**
 * @brief Platform I2C write function.
 *
 * @section Purpose
 * This function writes a value to the specified register address using the I2C interface.
 *
 * @section Description
 * This function performs an I2C write operation to store the given value at the specified
 * register address. Both the register address and the value are in little-endian format.
 * Please refer to the I2C protocol described in the datasheet.
 *
 * @param[in] regAddr The register address to write to.
 * @param[in] value   The value to write.
 *
 * @return RtkApiRet  RT_ERR_OK on success, error code otherwise.
 */
static RtkApiRet platform_i2c_write(uint32 regAddr, uint32 value)
{
    /* implement i2c driver here */
    (void)regAddr;
    (void)value;

    return RT_ERR_OK;
}

/**
 * @brief Platform I2C burst read function.
 *
 * @section Purpose
 * This function reads multiple values starting from the specified register address using the I2C interface.
 *
 * @section Description
 * This function performs an I2C burst read operation to retrieve multiple values starting from the given
 * register address. The register address, values, and length are all in little-endian format.
 * Please refer to the I2C protocol described in the datasheet.
 *
 * @param[in]  regAddr The starting register address to read from.
 * @param[out] value   Pointer to store the read values.
 * @param[in]  length  The number of values to read.
 *
 * @return RtkApiRet   RT_ERR_OK on success, error code otherwise.
 */
static RtkApiRet platform_i2c_read_burst(uint32 regAddr, uint32 *value, uint32 length)
{
    /* implement i2c driver here */
    (void)regAddr;
    (void)value;
    (void)length;

    return RT_ERR_OK;
}

/**
 * @brief Platform I2C burst write function.
 *
 * @section Purpose
 * This function writes multiple values starting from the specified register address using the I2C interface.
 *
 * @section Description
 * This function performs an I2C burst write operation to store multiple values starting from the given
 * register address. The register address, values, and length are all in little-endian format.
 * Please refer to the I2C protocol described in the datasheet.
 *
 * @param[in] regAddr The starting register address to write to.
 * @param[in] value   Pointer to the values to write.
 * @param[in] length  The number of values to write.
 *
 * @return RtkApiRet RT_ERR_OK on success, error code otherwise.
 */
static RtkApiRet platform_i2c_write_burst(uint32 regAddr, const uint32 *value, uint32 length)
{
    /* implement i2c driver here */
    (void)regAddr;
    (void)value;
    (void)length;

    return RT_ERR_OK;
}

/**
 * @addtogroup EXAMPLE
 * @{
 */

/**
 * @addtogroup SYS_PORTING_EX System Porting Example
 * @{
 */

/**
 * @addtogroup REGIF_INIT Initialize Register Interface Example
 *
 * @brief <b> Initialize the register interface for the SDK. </b> \n
 *        Initialize the I2C register interface
 *
 * @section Purpose
 *          This example demonstrates how to register I2C read/write callback functions.
 *
 * @section Description
 *           After calling this API, the SDK can access the switch via the I2C interface on your platform.
 *           It sets up the necessary callback functions for I2C read/write operations.
 *
 * @{
 */
RtkApiRet sys_reg_interface_init(void)
{
    RtkApiRet ret = RT_ERR_OK;

    RegifCallback_t regifCallback = {
        .readFunc       = &platform_i2c_read,
        .writeFunc      = &platform_i2c_write,
        .readBurstFunc  = &platform_i2c_read_burst,
        .writeBurstFunc = &platform_i2c_write_burst,
    };

    ret = rtk_regif_init(REGIF_I2C, &regifCallback);

    /* Expected Behaviour:
     *    After calling this API, SDK can access switch by I2C interface on your platform
     */

    return ret;
}
/**@}*/ /* REGIF_INIT */
/**@}*/ /* SYS_PORTING_EX */
/**@}*/ /* EXAMPLE */