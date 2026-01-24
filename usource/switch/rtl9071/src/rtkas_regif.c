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
#include "rtkas_common.h"
#include "rtkas_error.h"
#include "rtkas_system_porting.h"
#include "rtkas_types.h"
#include "rtkas_debug.h"
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_reg_struct.h"
#include "rtl907xd_v1_asicdrv_mem_ctrl.h"
#endif

#if defined(RTL_907XD_VA)
#include "rtl907xd_va_reg_struct.h"
#include "rtl907xd_va_asicdrv_mem_ctrl.h"
#include "rtl907xd_va_asicdrv_timer.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_reg_struct.h"
#include "rtl907xc_vb_asicdrv_mem_ctrl.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_reg_struct.h"
#include "rtl907xc_va_asicdrv_mem_ctrl.h"
#endif

uint8 g_RegIF;

static RtkApiRet regif_i2c_read(uint32 regAddr, uint32 *value);
static RtkApiRet regif_i2c_write(uint32 regAddr, uint32 value);
static RtkApiRet regif_i2c_read_burst(uint32 regAddr, uint32 *value, uint32 length);
static RtkApiRet regif_i2c_write_burst(uint32 regAddr, uint32 *value, uint32 length);

static RtkApiRet regif_mdcmdio_read(uint32 regAddr, uint32 *value);
static RtkApiRet regif_mdcmdio_write(uint32 regAddr, uint32 value);

static RtkApiRet regif_spi_read(uint32 regAddr, uint32 *value);
static RtkApiRet regif_spi_write(uint32 regAddr, uint32 value);

static RtkApiRet regif_pcie_read(uint32 regAddr, uint32 *value);
static RtkApiRet regif_pcie_write(uint32 regAddr, uint32 value);
static RtkApiRet regif_pcie_write_bytes(uint32 regAddr, uint32 value);

static RtkApiRet regif_eth_read(uint32 regAddr, uint32 *value);
static RtkApiRet regif_eth_write(uint32 regAddr, uint32 value);
static RtkApiRet regif_eth_read_burst(uint32 regAddr, uint32 *value, uint32 length);
static RtkApiRet regif_eth_write_burst(uint32 regAddr, uint32 *value, uint32 length);
static RtkApiRet regif_eth_write_bytes(uint32 regAddr, uint32 value);

static regif_read_ptr                regif_i2c_read_impl                = NULL;
static regif_write_ptr               regif_i2c_write_impl               = NULL;
static regif_read_burst_ptr          regif_i2c_read_burst_impl          = NULL;
static regif_write_burst_ptr         regif_i2c_write_burst_impl         = NULL;
static regif_read_ptr                regif_mdcmdio_read_impl            = NULL;
static regif_write_ptr               regif_mdcmdio_write_impl           = NULL;
static regif_read_ptr                regif_spi_read_impl                = NULL;
static regif_write_ptr               regif_spi_write_impl               = NULL;
static regif_read_ptr                regif_pcie_read_impl               = NULL;
static regif_write_ptr               regif_pcie_write_impl              = NULL;
static regif_write_bytes_ptr         regif_pcie_write_bytes_impl        = NULL;
static regif_read_ptr                regif_eth_read_impl                = NULL;
static regif_write_ptr               regif_eth_write_impl               = NULL;
static regif_write_partial_indir_ptr regif_eth_write_partial_indir_impl = NULL;
static regif_read_burst_ptr          regif_eth_read_burst_impl          = NULL;
static regif_write_burst_ptr         regif_eth_write_burst_impl         = NULL;
static regif_write_bytes_ptr         regif_eth_write_bytes_impl         = NULL;

static RtkApiRet regif_i2c_read(uint32 regAddr, uint32 *value)
{
    return (regif_i2c_read_impl != NULL) ? regif_i2c_read_impl(regAddr, value) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_i2c_write(uint32 regAddr, uint32 value)
{
    return (regif_i2c_write_impl != NULL) ? regif_i2c_write_impl(regAddr, value) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_i2c_read_burst(uint32 regAddr, uint32 *value, uint32 length)
{
    return (regif_i2c_read_burst_impl != NULL) ? regif_i2c_read_burst_impl(regAddr, value, length) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_i2c_write_burst(uint32 regAddr, uint32 *value, uint32 length)
{
    return (regif_i2c_write_burst_impl != NULL) ? regif_i2c_write_burst_impl(regAddr, value, length) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_mdcmdio_read(uint32 regAddr, uint32 *value)
{
    return (regif_mdcmdio_read_impl != NULL) ? regif_mdcmdio_read_impl(regAddr, value) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_mdcmdio_write(uint32 regAddr, uint32 value)
{
    return (regif_mdcmdio_write_impl != NULL) ? regif_mdcmdio_write_impl(regAddr, value) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_spi_read(uint32 regAddr, uint32 *value)
{
    return (regif_spi_read_impl != NULL) ? regif_spi_read_impl(regAddr, value) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_spi_write(uint32 regAddr, uint32 value)
{
    return (regif_spi_write_impl != NULL) ? regif_spi_write_impl(regAddr, value) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_pcie_read(uint32 regAddr, uint32 *value)
{
    return (regif_pcie_read_impl != NULL) ? regif_pcie_read_impl(regAddr, value) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_pcie_write(uint32 regAddr, uint32 value)
{
    return (regif_pcie_write_impl != NULL) ? regif_pcie_write_impl(regAddr, value) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_pcie_write_bytes(uint32 regAddr, uint32 value)
{
    return (regif_pcie_write_bytes_impl != NULL) ? regif_pcie_write_bytes_impl(regAddr, value) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_eth_read(uint32 regAddr, uint32 *value)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(regif_eth_read_impl != NULL)
    {
        retVal = regif_eth_read_impl(regAddr, value);
    }
    else if(regif_eth_read_burst_impl != NULL)
    {
        retVal = regif_eth_read_burst_impl(regAddr, value, 1);
    }
    else
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}

static RtkApiRet regif_eth_write(uint32 regAddr, uint32 value)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(regif_eth_write_impl != NULL)
    {
        retVal = regif_eth_write_impl(regAddr, value);
    }
    else if(regif_eth_write_burst_impl != NULL)
    {
        retVal = regif_eth_write_burst_impl(regAddr, &value, 1);
    }
    else
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}

static RtkApiRet regif_eth_write_bytes(uint32 regAddr, uint32 value)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(regif_eth_write_bytes_impl != NULL)
    {
        retVal = regif_eth_write_bytes_impl(regAddr, value);
    }
    else if(regif_eth_write_burst_impl != NULL)
    {
#if 0
        uint32 regAddrTemp[2];
        regAddrTemp[0] = 0xFFFF05u;
        regAddrTemp[1] = regAddr;
        CHK_FUN_CONTINUE(retVal, regif_eth_write_burst_impl(regAddrTemp, &value, 1));
#endif
    }
    else
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}

static RtkApiRet regif_eth_read_burst(uint32 regAddr, uint32 *value, uint32 length)
{
    return (regif_eth_read_burst_impl != NULL) ? regif_eth_read_burst_impl(regAddr, value, length) : -RT_ERR_NULL_POINTER;
}

static RtkApiRet regif_eth_write_burst(uint32 regAddr, uint32 *value, uint32 length)
{
    return (regif_eth_write_burst_impl != NULL) ? regif_eth_write_burst_impl(regAddr, value, length) : -RT_ERR_NULL_POINTER;
}

RtkApiRet regif_eth_partial_indir_write(uint32 regAddr, uint32 mask, uint32 value)
{
    RtkApiRet retVal = RT_ERR_OK;

    if(regif_eth_write_partial_indir_impl != NULL)
    {
        retVal = regif_eth_write_partial_indir_impl(regAddr, mask, value);
    }
    else
    {
        retVal = -RT_ERR_NULL_POINTER;
    }

    return retVal;
}

static RtkApiRet regif_init_i2c(const RegifCallback_t *regifCallback)
{
    RtkApiRet retCode = RT_ERR_OK;

    if(NULL != regifCallback)
    {
        g_RegIF                    = REGIF_I2C;
        regif_i2c_read_impl        = regifCallback->readFunc;
        regif_i2c_write_impl       = regifCallback->writeFunc;
        regif_i2c_read_burst_impl  = regifCallback->readBurstFunc;
        regif_i2c_write_burst_impl = regifCallback->writeBurstFunc;
    }
    else
    {
        retCode = -RT_ERR_NULL_POINTER;
    }

    return retCode;
}

static RtkApiRet regif_init_mdcmdio(const RegifCallback_t *regifCallback)
{
    RtkApiRet retCode = RT_ERR_OK;

    if(NULL != regifCallback)
    {
        g_RegIF                  = REGIF_MDCMDIO;
        regif_mdcmdio_read_impl  = regifCallback->readFunc;
        regif_mdcmdio_write_impl = regifCallback->writeFunc;
    }
    else
    {
        retCode = -RT_ERR_NULL_POINTER;
    }

    return retCode;
}

static RtkApiRet regif_init_spi(const RegifCallback_t *regifCallback)
{
    RtkApiRet retCode = RT_ERR_OK;

    if(NULL != regifCallback)
    {
        g_RegIF              = REGIF_SPI;
        regif_spi_read_impl  = regifCallback->readFunc;
        regif_spi_write_impl = regifCallback->writeFunc;
    }
    else
    {
        retCode = -RT_ERR_NULL_POINTER;
    }

    return retCode;
}

static RtkApiRet regif_init_pcie(const RegifCallback_t *regifCallback)
{
    RtkApiRet retCode = RT_ERR_OK;

    if(NULL != regifCallback)
    {
        g_RegIF                     = REGIF_PCIE;
        regif_pcie_read_impl        = regifCallback->readFunc;
        regif_pcie_write_impl       = regifCallback->writeFunc;
        regif_pcie_write_bytes_impl = regifCallback->writeBytesFunc;
    }
    else
    {
        retCode = -RT_ERR_NULL_POINTER;
    }

    return retCode;
}

static RtkApiRet regif_init_eth(const RegifCallback_t *regifCallback)
{
    RtkApiRet retCode = RT_ERR_OK;

    if(NULL != regifCallback)
    {
        g_RegIF                            = REGIF_ETHERNET;
        regif_eth_read_impl                = regifCallback->readFunc;
        regif_eth_write_impl               = regifCallback->writeFunc;
        regif_eth_write_partial_indir_impl = regifCallback->writePartialIndirFunc;
        regif_eth_write_bytes_impl         = regifCallback->writeBytesFunc;
        regif_eth_read_burst_impl          = regifCallback->readBurstFunc;
        regif_eth_write_burst_impl         = regifCallback->writeBurstFunc;
    }
    else
    {
        retCode = -RT_ERR_NULL_POINTER;
    }

    return retCode;
}

/**
 * @addtogroup RTK_MODULE_SYS_PORT System Porting
 * @brief System Porting APIs. This component contains various APIs for user to register platform-depended callback functions.
 * @{
 */

/**
 * @addtogroup SYSPORT_REGIF Register Interface Initialization
 * @brief APIs to initialize register interface
 * @{
 */

/**
 * @brief This API is used to initialize register callback functions
 *
 * @param[in] regifType             Register Interface Type (REGIF_I2C to REGIF_ETHERNET)\n
 *                                  The register interface type used to communicate with the switch
 * @param[in] regifCallback         Register Interface Callback Function
 *                                  The platform-depend callback functions for read/write register via the specific register interface
 * @param[out] None
 * @retval RT_ERR_OK                Initialize register callback functions successfully
 * @retval -RT_ERR_INPUT            The regifType is out of range
 * @retval Others Please refer to regif_init_i2c(), regif_init_mdcmdio(), regif_init_spi(), regif_init_pcie(), regif_init_eth()
 *
 * Different register interface types have different function requirements.
 * Please register the necessary function callbacks for the register interfaces used, as outlined in the table below.
 *
 *  regif type    | Read | Write | Read Burst | Write Burst | Write Byte |
 * -------------- | -----| ----- | ---------- | ----------- | ---------- |
 * REGIF_I2C      |   V  |   V   |      V     |       V     |     -      |
 * REGIF_MDCMDIO  |   V  |   V   |      -     |       -     |     -      |
 * REGIF_SPI      |   V  |   V   |      -     |       -     |     -      |
 * REGIF_PCIE     |   V  |   V   |      -     |       -     |     V      |
 * REGIF_ETHERNET |   V  |   V   |      V     |       V     |     V      |
 *
 */

RtkApiRet rtk_regif_init(uint32 regifType, const RegifCallback_t *regifCallback)
{
    RtkApiRet retCode = RT_ERR_OK;

    switch(regifType)
    {
        case REGIF_I2C:
            retCode = regif_init_i2c(regifCallback);
            break;
        case REGIF_MDCMDIO:
            retCode = regif_init_mdcmdio(regifCallback);
            break;
        case REGIF_SPI:
            retCode = regif_init_spi(regifCallback);
            break;
        case REGIF_PCIE:
            retCode = regif_init_pcie(regifCallback);
            break;
        case REGIF_ETHERNET:
            retCode = regif_init_eth(regifCallback);
            break;
        default:
            retCode = -RT_ERR_INPUT;
            break;
    }

    return retCode;
}
/**@}*/ /* SYSPORT_REGIF */
/**@}*/ /* RTK_MODULE_SYS_PORT */

/**
 * @addtogroup RTK_MODULE_SYSTEM System
 * @{
 */

/**
 * @addtogroup SYSTEM_REG Register Access
 * @brief Switch register accesses APIs.
 * @{
 */

/**
 * @brief This API is used to read a value of a specific register
 *
 * @param[in] regAddr               Register Address (0x0 to 0xFFFFFF)
 *                                  The address of a specific register
 * @param[out] value                Register value (N/A)
                                    The value of the register
 * @retval RT_ERR_OK                read value of a specific register successfully
 * @retval -RT_ERR_FAILED           The register inteface type is unknown or unsupported
 * @retval Others Please refer to regif_i2c_read(), regif_mdcmdio_read(), regif_spi_read(), regif_pcie_read(), regif_eth_read()
 */
RtkApiRet reg_direct_read(uint32 regAddr, uint32 *value)
{
    RtkApiRet retVal = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tRead regAddr: 0x%08x", regAddr);
#endif

    if(NULL == value)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(REGIF_I2C == g_RegIF)
    {
        retVal = regif_i2c_read(regAddr, value);
    }
    else if(REGIF_MDCMDIO == g_RegIF)
    {
        retVal = regif_mdcmdio_read(regAddr, value);
    }
    else if(REGIF_SPI == g_RegIF)
    {
        retVal = regif_spi_read(regAddr, value);
    }
    else if(REGIF_PCIE == g_RegIF)
    {
        retVal = regif_pcie_read(regAddr, value);
    }
    else if(g_RegIF == (uint8)REGIF_ETHERNET)
    {
        retVal = regif_eth_read(regAddr, value);
    }
    else
    {
        retVal = -RT_ERR_FAILED;
    }

#ifdef EN_DBG_REG
    if(RT_ERR_OK == retVal)
    {
        DEBUG_INFO(g_dbgMsk, "\t> return: 0x%08x", *value);
    }
#endif
    return retVal;
}

/**
 * @brief This API is used to write a value of a specific register
 *
 * @param[in] regAddr               Register Address (0x0 to 0xFFFFFF)
 *                                  The address of a specific register
 * @param[in] value                 Register value (0x0 to 0xFFFFFFFF)
 *                                  The value of the register
 * @param[out] None
 * @retval RT_ERR_OK                write value of a specific register successfully
 * @retval -RT_ERR_FAILED           The register inteface type is unknown or unsupported
 * @retval Others Please refer to regif_i2c_write(), regif_mdcmdio_write(), regif_spi_write(), regif_pcie_write(), regif_eth_write()
 */
RtkApiRet reg_direct_write(uint32 regAddr, uint32 value)
{
    RtkApiRet retVal = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tWrite regAddr: 0x%08x value: 0x%08x", regAddr, value);
#endif

    if(g_RegIF == REGIF_I2C)
    {
        retVal = regif_i2c_write(regAddr, value);
    }
    else if(g_RegIF == REGIF_MDCMDIO)
    {
        retVal = regif_mdcmdio_write(regAddr, value);
    }
    else if(g_RegIF == REGIF_SPI)
    {
        retVal = regif_spi_write(regAddr, value);
    }
    else if(g_RegIF == REGIF_PCIE)
    {
        retVal = regif_pcie_write(regAddr, value);
    }
    else if(g_RegIF == (uint8)REGIF_ETHERNET)
    {
        retVal = regif_eth_write(regAddr, value);
    }
    else
    {
        retVal = -RT_ERR_FAILED;
    }

    return retVal;
}

/**
 * @brief This API is used to write a single byte of a specific register
 *
 * @param[in] regAddr               Register Address (0x0 to 0xFFFFFF)
 *                                  The address of a specific register
 * @param[in] value                 Register Value (0x0 to 0xFF)
 *                                  The byte value of the register
 * @param[out] None
 * @retval RT_ERR_OK                write value of a specific register successfully
 * @retval -RT_ERR_FAILED           The register inteface type is unknown or unsupported
 * @retval Others Please refer to reg_direct_write(), regif_eth_write_bytes(), regif_pcie_write_bytes()
 */
RtkApiRet reg_direct_write_bytes(uint32 regAddr, uint32 value)
{
    RtkApiRet retVal = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tWrite regAddr: 0x%08x value: 0x%08x", regAddr, value);
#endif

    /* REGIF_I2C, REGIF_MDCMDIO, REGIF_SPI */
    if(g_RegIF <= (uint8)REGIF_SPI)
    {
        /* Configure 1 bytes data to access for external interface */
        CHK_FUN_RET(retVal, reg_direct_write((uint32)0xffff05, (uint32)0x1));
        CHK_FUN_RET(retVal, reg_direct_write((uint32)regAddr, (uint32)value));
        /* Configure 4 bytes data to access for external interface */
        CHK_FUN_RET(retVal, reg_direct_write((uint32)0xffff05, (uint32)0x0));
    }
    else if(g_RegIF == REGIF_ETHERNET)
    {
        CHK_FUN_RET(retVal, regif_eth_write_bytes(regAddr, value));
    }
    else if(g_RegIF == REGIF_PCIE)
    {
        CHK_FUN_RET(retVal, regif_pcie_write_bytes(regAddr, value));
    }
    else
    {
        retVal = -RT_ERR_FAILED;
    }

    return retVal;
}

/**
 * @brief This API is used to read values of some registers in a row\n
 *
 * @param[in] regAddr               Register Address (0x0 to 0xFFFFFF)
 *                                  The address of a specific register
 * @param[in] length                Length (0x0 to 0xFFFFFFFF)
 *                                  The number of regiseter to read in a single burst read operation
 * @param[out] value                Register Values (N/A)
                                    The value of all registers.\n
                                    Caller MUST ensure that the memory space that "value" point to is large enough when calling this API
 * @retval RT_ERR_OK                read value of a specific register successfully
 * @retval -RT_ERR_NULL_POINTER     The "value" is NULL
 * @retval -RT_ERR_FAILED           The register inteface type is unknown or unsupported. \n
 *                                  Currently only support burst read for I2c interface
 * @retval Others Please refer to regif_i2c_read_burst()
 */
RtkApiRet reg_direct_read_burst(uint32 regAddr, uint32 *value, uint32 length)
{
    RtkApiRet retVal = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tRead regAddr: 0x%08x length:0x%x", regAddr, length);
#endif

    if(NULL == value)
    {
        retVal = -RT_ERR_NULL_POINTER;
    }
    else if(REGIF_I2C == g_RegIF)
    {
        retVal = regif_i2c_read_burst(regAddr, value, length);
    }
    else
    {
        retVal = -RT_ERR_FAILED;
    }

#ifdef EN_DBG_REG
    if(RT_ERR_OK == retVal)
    {
        DEBUG_INFO(g_dbgMsk, "\t> return: 0x%08x", *value);
    }
#endif
    return retVal;
}

RtkApiRet reg_read(UnitChip_t unitChip, uint32 regAddr, uint32 *value)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tRead regAddr: 0x%08x", regAddr);
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_read(unitChip.unit, regAddr, value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_read(unitChip.unit, regAddr, value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_read(unitChip.unit, regAddr, value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_read(unitChip.unit, regAddr, value);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

#ifdef EN_DBG_REG
    if(RT_ERR_OK == ret)
    {
        DEBUG_INFO(g_dbgMsk, "\t> return: 0x%08x", *value);
    }
#endif
    return ret;
}

RtkApiRet reg_write(UnitChip_t unitChip, uint32 regAddr, uint32 value)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tregAddr: 0x%08x value: 0x%08x", regAddr, value);
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_write(unitChip.unit, regAddr, value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_write(unitChip.unit, regAddr, value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_write(unitChip.unit, regAddr, value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_write(unitChip.unit, regAddr, value);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

RtkApiRet reg_field_get(UnitChip_t unitChip, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value, uint32 data)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_field_get(regAddr, lsp, len, value, data);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_field_get(regAddr, lsp, len, value, data);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_field_get(regAddr, lsp, len, value, data);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_field_get(regAddr, lsp, len, value, data);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

RtkApiRet reg_field_set(UnitChip_t unitChip, uint32 regAddr, uint32 lsp, uint32 len, uint32 value, uint32 *data)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_field_set(regAddr, lsp, len, value, data);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_field_set(regAddr, lsp, len, value, data);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_field_set(regAddr, lsp, len, value, data);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_field_set(regAddr, lsp, len, value, data);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

RtkApiRet reg_field_read(UnitChip_t unitChip, uint32 regAddr, uint32 lsp, uint32 len, uint32 *value)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tRead regAddr: 0x%08x lsp:%2d len:%2d", regAddr, lsp, len);
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_field_read(unitChip.unit, regAddr, lsp, len, value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_field_read(unitChip.unit, regAddr, lsp, len, value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_field_read(unitChip.unit, regAddr, lsp, len, value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_field_read(unitChip.unit, regAddr, lsp, len, value);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

#ifdef EN_DBG_REG
    if(RT_ERR_OK == ret)
    {
        DEBUG_INFO(g_dbgMsk, "\t> return: 0x%08x", *value);
    }
#endif
    return ret;
}

RtkApiRet reg_field_write(UnitChip_t unitChip, uint32 regAddr, uint32 lsp, uint32 len, uint32 value)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tWrite regAddr: 0x%08x lsp:%2d len:%2d value: 0x%08x", regAddr, lsp, len, value);
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_field_write(unitChip.unit, regAddr, lsp, len, value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_field_write(unitChip.unit, regAddr, lsp, len, value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_field_write(unitChip.unit, regAddr, lsp, len, value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_field_write(unitChip.unit, regAddr, lsp, len, value);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

RtkApiRet reg_bit_get(UnitChip_t unitChip, uint32 reg, uint32 bit, uint32 *value)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tWrite regAddr: 0x%08x bit:%2d", reg, bit);
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_bit_get(unitChip.unit, reg, bit, value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_bit_get(unitChip.unit, reg, bit, value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_bit_get(unitChip.unit, reg, bit, value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_bit_get(unitChip.unit, reg, bit, value);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

#ifdef EN_DBG_REG
    if(RT_ERR_OK == ret)
    {
        DEBUG_INFO(g_dbgMsk, "\t> return: 0x%08x\n", *value);
    }
#endif

    return ret;
}

RtkApiRet reg_bit_set(UnitChip_t unitChip, uint32 reg, uint32 bit, uint32 value)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tWrite regAddr: 0x%08x bit:%2d value:%08x", reg, bit, value);
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_bit_set(unitChip.unit, reg, bit, value);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_bit_set(unitChip.unit, reg, bit, value);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_bit_set(unitChip.unit, reg, bit, value);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_bit_set(unitChip.unit, reg, bit, value);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

RtkApiRet reg_page_read(UnitChip_t unitChip, uint32 *page)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tRead page");
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_page_read(unitChip.unit, page);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_page_read(unitChip.unit, page);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_page_read(unitChip.unit, page);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_page_read(unitChip.unit, page);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

#ifdef EN_DBG_REG
    if(RT_ERR_OK == ret)
    {
        DEBUG_INFO(g_dbgMsk, "\t> return: 0x%08x", *page);
    }
#endif

    return ret;
}

RtkApiRet reg_page_write(UnitChip_t unitChip, uint32 page)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tWrite page: 0x%08x", page);
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_reg_page_write(unitChip.unit, page);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_reg_page_write(unitChip.unit, page);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_reg_page_write(unitChip.unit, page);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_reg_page_write(unitChip.unit, page);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

RtkApiRet asicdrv_hw_indirect_get(UnitChip_t unitChip, uint32 regAddr, uint32 *regVal)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tGet regAddr: 0x%08x", regAddr);
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_drv_hw_indirect_get(unitChip.unit, regAddr, regVal);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_drv_hw_indirect_get(unitChip.unit, regAddr, regVal);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_drv_hw_indirect_get(unitChip.unit, regAddr, regVal);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_drv_hw_indirect_get(unitChip.unit, regAddr, regVal);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

#ifdef EN_DBG_REG
    if(RT_ERR_OK == ret)
    {
        DEBUG_INFO(g_dbgMsk, "\t> return: 0x%08x", *regVal);
    }
#endif

    return ret;
}

RtkApiRet asicdrv_hw_indirect_set(UnitChip_t unitChip, uint32 regAddr, uint32 regVal)
{
    RtkApiRet ret = RT_ERR_OK;
#ifdef EN_DBG_REG
    DEBUG_INFO(g_dbgMsk, "\tSet regAddr: 0x%08x, regVal:0x%08x", regAddr, regVal);
#endif

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_drv_hw_indirect_set(unitChip.unit, regAddr, regVal);
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_drv_hw_indirect_set(unitChip.unit, regAddr, regVal);
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_drv_hw_indirect_set(unitChip.unit, regAddr, regVal);
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_drv_hw_indirect_set(unitChip.unit, regAddr, regVal);
            break;
#endif

        default:
            ret = -RT_ERR_NOT_SUPPORT;
            break;
    }

    return ret;
}

/**@}*/ /* SYSTEM_REG */
/**@}*/ /* RTK_MODULE_SYSTEM */
