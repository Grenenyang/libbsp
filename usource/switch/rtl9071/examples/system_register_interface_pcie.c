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

#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "rtl907xd_va_api_ext.h"

/*******************************************************************************
 *   Defines
 *******************************************************************************/
/* RTCORE Command ID */
#define SWC_CMD_REG_GET 70
#define SWC_CMD_REG_SET 71
#define SWC_CMD_TYPE_8  0
#define SWC_CMD_TYPE_16 1
#define SWC_CMD_TYPE_32 2
/* RTCORE device */
#ifndef RTASE_SWC_NAME
#define RTASE_SWC_NAME "/dev/rtcore"
#endif

#define PAGE_SWITCH_REG (0xFFFF01u)

/*******************************************************************************
 *   Structure
 *******************************************************************************/
typedef struct
{
    uint8  type;
    uint32 regAddr;
    uint32 regValue;
} rtase_swc_cmd_t;

/*******************************************************************************
 *  Global Variables
 ******************************************************************************/
static int32  g_pcieFd   = 0;
static uint32 g_pciePage = 0x4A000000;

/* Declaration */
RtkApiRet sys_reg_interface_pcie_init(void);
RtkApiRet platform_pcie_write_byte(uint32 regAddr, uint32 value);
void      sys_reg_interface_pcie_close(void);
void      sys_reg_interface_pcie_open(void);

/*******************************************************************************
 *  Functions
 ******************************************************************************/
/**
 * @brief Open the PCIe file descriptor for the SDK.
 *
 * @section Purpose
 * This function opens the PCIe file descriptor required for register access operations.
 *
 * @section Description
 * This function checks if the PCIe file descriptor is already open. If not, it opens the
 * file descriptor for the PCIe device.
 */
static void pcie_open(void)
{
    if(0 == g_pcieFd)
    {
        g_pcieFd = open(RTASE_SWC_NAME, O_RDWR);
        if(0 > g_pcieFd)
        {
#ifdef DEBUG_PCIE_INTERFACE
            printf("PCIe: Can't open device.\n");
#endif
            g_pcieFd = 0;
        }
    }
    else
    {
#ifdef DEBUG_PCIE_INTERFACE
        printf("PCIe: device has been open.\n");
#endif
    }
}

/**
 * @brief Close the PCIe file descriptor for the SDK.
 *
 * @section Purpose
 * This function closes the PCIe file descriptor after all register access operations are done.
 *
 * @section Description
 * This function checks if the PCIe file descriptor is open. If so, it closes the file descriptor
 * for the PCIe device.
 */
static void pcie_close(void)
{
    if(0 < g_pcieFd)
    {
        (void)close(g_pcieFd);
        g_pcieFd = 0;
    }
}

/* ********************************************************** */
/*  platform-depent code                                      */
/* ********************************************************** */
/**
 * @brief Platform PCIe read function.
 *
 * @section Purpose
 * This function reads a value from the specified register address using the PCIe interface.
 *
 * @section Description
 * This function performs a PCIe read operation to retrieve the value stored at the given
 * register address. The register address and the value are both in little-endian format.
 *
 * @param[in]  regAddr The register address to read from.
 * @param[out] value   Pointer to store the read value.
 *
 * @return RtkApiRet   RT_ERR_OK on success, error code otherwise.
 */
static RtkApiRet platform_pcie_read(uint32 regAddr, uint32 *value)
{
    RtkApiRet       ret = SUCCESS;
    rtase_swc_cmd_t cmd;

    (void)drv_memset(&cmd, 0, sizeof(rtase_swc_cmd_t));
    if(0 == g_pcieFd)
    {
#ifdef DEBUG_PCIE_INTERFACE
        printf("PCIe: device has not been open.\n");
#endif
        ret = -RT_ERR_PCIE_FILE_NOT_OPEN;
    }
    else
    {
        if(regAddr == PAGE_SWITCH_REG)
        {
            *value = (g_pciePage >> 24u) & 0xFFu;
        }
        else
        {
            cmd.type    = SWC_CMD_TYPE_32;
            cmd.regAddr = regAddr | g_pciePage;
            if(ioctl(g_pcieFd, SWC_CMD_REG_GET, &cmd) != 0)
            {
#ifdef DEBUG_PCIE_INTERFACE
                printf("PCIe: ioctl failed.\n");
#endif
                ret = -RT_ERR_PCIE_IOCTL_FAIL;
            }
            *value = cmd.regValue;
        }
    }

    return ret;
}

/**
 * @brief Platform PCIe write function.
 *
 * @section Purpose
 * This function writes a value to the specified register address using the PCIe interface.
 *
 * @section Description
 * This function performs a PCIe write operation to store the given value at the specified
 * register address. Both the register address and the value are in little-endian format.
 *
 * @param[in] regAddr The register address to write to.
 * @param[in] value   The value to write.
 *
 * @return RtkApiRet  RT_ERR_OK on success, error code otherwise.
 */
static RtkApiRet platform_pcie_write(uint32 regAddr, uint32 value)
{
    RtkApiRet       ret = RT_ERR_OK;
    rtase_swc_cmd_t cmd;

    if(0 == g_pcieFd)
    {
#ifdef DEBUG_PCIE_INTERFACE
        printf("PCIe: device has not been open.");
#endif
        ret = -RT_ERR_PCIE_FILE_NOT_OPEN;
    }
    else
    {
        if(regAddr == PAGE_SWITCH_REG)
        {
            g_pciePage = ((value & 0xFFu) << 24u);
        }
        else
        {
            cmd.type     = SWC_CMD_TYPE_32;
            cmd.regAddr  = regAddr | g_pciePage;
            cmd.regValue = value;
            if(ioctl(g_pcieFd, SWC_CMD_REG_SET, &cmd) != 0)
            {
#ifdef DEBUG_PCIE_INTERFACE
                printf("PCIe: ioctl failed.\n");
#endif
                ret = -RT_ERR_PCIE_IOCTL_FAIL;
            }
        }
    }

    return ret;
}

/**
 * @brief Platform PCIe write byte function.
 *
 * @section Purpose
 * This function writes a byte value to the specified register address using the PCIe interface.
 *
 * @section Description
 * This function performs a PCIe write operation to store the given byte value at the specified
 * register address. Both the register address and the value are in little-endian format.
 *
 * @param[in] regAddr The register address to write to.
 * @param[in] value   The byte value to write.
 *
 * @return RtkApiRet  RT_ERR_OK on success, error code otherwise.
 */
RtkApiRet platform_pcie_write_byte(uint32 regAddr, uint32 value)
{
    RtkApiRet       ret = RT_ERR_OK;
    rtase_swc_cmd_t cmd;

    if(0 == g_pcieFd)
    {
#ifdef DEBUG_PCIE_INTERFACE
        printf("PCIe: device has not been open.");
#endif
        ret = -RT_ERR_PCIE_FILE_NOT_OPEN;
    }
    else
    {
        if(regAddr == PAGE_SWITCH_REG)
        {
            g_pciePage = ((value & 0xFFu) << 24u);
        }
        else
        {
            cmd.type     = SWC_CMD_TYPE_8;
            cmd.regAddr  = regAddr | g_pciePage;
            cmd.regValue = value;
            if(ioctl(g_pcieFd, SWC_CMD_REG_SET, &cmd) != 0)
            {
#ifdef DEBUG_PCIE_INTERFACE
                printf("PCIe: ioctl failed.\n");
#endif
                ret = -RT_ERR_PCIE_IOCTL_FAIL;
            }
        }
    }

    return ret;
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
 * @addtogroup REGIF_PCIE_INIT Initialize PCIe Register Interface Example
 * @{
 */

/**
 * @brief Open the PCIe file descriptor for the SDK.
 *
 * @section Purpose
 * The PCIe register interface needs to open the driver file descriptor before any register access operation.
 *
 * @section Description
 * This function opens the PCIe file descriptor required for register access operations.
 */
void sys_reg_interface_pcie_open(void)
{
    pcie_open();
}

/**
 * @brief Close the PCIe file descriptor for the SDK.
 *
 * @section Purpose
 * The PCIe register interface needs to close the driver file descriptor after all register access operations are done.
 *
 * @section Description
 * This function closes the PCIe file descriptor after all register access operations are done.
 */
void sys_reg_interface_pcie_close(void)
{
    pcie_close();
}

/**
 * @brief Initialize the PCIe register interface for the SDK.
 *
 * @section Purpose
 * This example demonstrates how to register PCIe read/write/writebyte callbacks.
 *
 * @section Description
 * After calling this API, the SDK can access the switch via the PCIe interface on your platform.
 * It sets up the necessary callback functions for PCIe read/write operations.
 *
 * @return RtkApiRet RT_ERR_OK on success, error code otherwise.
 */
RtkApiRet sys_reg_interface_pcie_init(void)
{
    RtkApiRet ret = RT_ERR_OK;

    RegifCallback_t regifCallback = {
        .readFunc       = &platform_pcie_read,
        .writeFunc      = &platform_pcie_write,
        .writeBytesFunc = &platform_pcie_write_byte};

    ret = rtk_regif_init(REGIF_PCIE, (const RegifCallback_t *)&regifCallback);

    /* Expected Behaviour:
     *    After calling this API, SDK can access switch by PCIe interface on your platform
     */

    return ret;
}
#endif /* __linux__ */

/**@}*/ /* REGIF_PCIE_INIT */
/**@}*/ /* SYS_PORTING_EX */
/**@}*/ /* EXAMPLE */