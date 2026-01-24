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
#if defined(RTL_907XD_V1)
#include "rtl907xd_v1_api_ethernet_access.h"
#endif
#if defined(RTL_907XD_VA)
#include "rtl907xd_va_api_ethernet_access.h"
#endif
#if defined(RTL_907XC_VB)
#include "rtl907xc_vb_api_ethernet_access.h"
#endif
#if defined(RTL_907XC_VA)
#include "rtl907xc_va_api_ethernet_access.h"
#endif

#include "rtkas_api_ethernet_access.h"

/**
 * @defgroup RTK_MODULE_ETHERNET_ACCESS Ethernet Access
 * @brief Ethernet Access APIs \n
 * For the full Ethernet access flow, including generating packets, transmitting packets, receiving reply packets, and checking reply packets, APIs with rtk_eth_acc_flow_XXX are used. \n
 * For generating Ethernet access packets, APIs with rtk_eth_acc_gen_XXX are used. \n
 * For checking replied packets, APIs with rtk_eth_acc_check_XXX are used. \n
 * If users call APIs with rtk_eth_acc_gen_XXX and rtk_eth_acc_check_XXX, they should follow the flow of calling rtk_eth_acc_gen_XXX, transmitting the packet, receiving the packet, and then calling rtk_eth_acc_check_XXX.
 * @{
 */
/**
 * @defgroup ETH_FLASH Ethernet Access Flash
 * @brief The APIs are used for access flash via Ethernet Access.
 * @{
 */
/**
 * @brief Flash write directly via Ethernet Access. \n
 *        This API includes the "Flash Write" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashWrite Ethernet Access flow flash write information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash write was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_flow_flash_write(), dva_eth_acc_flow_flash_write(), cvb_eth_acc_flow_flash_write(), cva_eth_acc_flow_flash_write().
 */
RtkApiRet rtk_eth_acc_flow_flash_write(UnitChip_t unitChip, EthAccFlowFlashWrite_t *ethAccFlowFlashWrite)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashWrite)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_flow_flash_write(ethAccFlowFlashWrite->addr, ethAccFlowFlashWrite->len, ethAccFlowFlashWrite->data);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_write(ethAccFlowFlashWrite->addr, ethAccFlowFlashWrite->len, ethAccFlowFlashWrite->data);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_flow_flash_write(ethAccFlowFlashWrite->addr, ethAccFlowFlashWrite->len, ethAccFlowFlashWrite->data);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_flow_flash_write(ethAccFlowFlashWrite->addr, ethAccFlowFlashWrite->len, ethAccFlowFlashWrite->data);
                break;
#endif

            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash read directly via Ethernet Access. \n
 *        This API includes the "Flash Read" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashRead Ethernet Access flow flash read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash read was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_flow_flash_read(), dva_eth_acc_flow_flash_read(), cvb_eth_acc_flow_flash_read(), cva_eth_acc_flow_flash_read().
 */
RtkApiRet rtk_eth_acc_flow_flash_read(UnitChip_t unitChip, EthAccFlowFlashRead_t *ethAccFlowFlashRead)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashRead)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_flow_flash_read(ethAccFlowFlashRead->addr, ethAccFlowFlashRead->len, ethAccFlowFlashRead->data);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_read(ethAccFlowFlashRead->addr, ethAccFlowFlashRead->len, ethAccFlowFlashRead->data);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_flow_flash_read(ethAccFlowFlashRead->addr, ethAccFlowFlashRead->len, ethAccFlowFlashRead->data);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_flow_flash_read(ethAccFlowFlashRead->addr, ethAccFlowFlashRead->len, ethAccFlowFlashRead->data);
                break;
#endif

            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash sector erase directly via Ethernet Access. \n
 *        This API includes the "Flash Erase" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashSecErase Ethernet Access flow flash sec erase information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_flow_flash_sec_erase(), dva_eth_acc_flow_flash_sec_erase(), cvb_eth_acc_flow_flash_sec_erase(), cva_eth_acc_flow_flash_sec_erase().
 */
RtkApiRet rtk_eth_acc_flow_flash_sec_erase(UnitChip_t unitChip, EthAccFlowFlashSecErase_t *ethAccFlowFlashSecErase)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashSecErase)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_flow_flash_sec_erase(ethAccFlowFlashSecErase->addr, ethAccFlowFlashSecErase->num);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_sec_erase(ethAccFlowFlashSecErase->addr, ethAccFlowFlashSecErase->num);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_flow_flash_sec_erase(ethAccFlowFlashSecErase->addr, ethAccFlowFlashSecErase->num);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_flow_flash_sec_erase(ethAccFlowFlashSecErase->addr, ethAccFlowFlashSecErase->num);
                break;
#endif

            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash block erase directly via Ethernet Access. \n
 *        This API includes the "Flash Erase" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashBlkErase Ethernet Access flow flash blk erase information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_flow_flash_blk_erase(), dva_eth_acc_flow_flash_blk_erase(), cvb_eth_acc_flow_flash_blk_erase(), cva_eth_acc_flow_flash_blk_erase().
 */
RtkApiRet rtk_eth_acc_flow_flash_blk_erase(UnitChip_t unitChip, EthAccFlowFlashBlkErase_t *ethAccFlowFlashBlkErase)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashBlkErase)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_flow_flash_blk_erase(ethAccFlowFlashBlkErase->addr, ethAccFlowFlashBlkErase->num);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_blk_erase(ethAccFlowFlashBlkErase->addr, ethAccFlowFlashBlkErase->num);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_flow_flash_blk_erase(ethAccFlowFlashBlkErase->addr, ethAccFlowFlashBlkErase->num);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_flow_flash_blk_erase(ethAccFlowFlashBlkErase->addr, ethAccFlowFlashBlkErase->num);
                break;
#endif

            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash erase all directly via Ethernet Access. \n
 *        This API includes the "Flash Erase" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_eth_acc_flow_flash_all_erase(), dva_eth_acc_flow_flash_all_erase(), cvb_eth_acc_flow_flash_all_erase(), cva_eth_acc_flow_flash_all_erase().
 */
RtkApiRet rtk_eth_acc_flow_flash_all_erase(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_eth_acc_flow_flash_all_erase();
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_eth_acc_flow_flash_all_erase();
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_eth_acc_flow_flash_all_erase();
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_eth_acc_flow_flash_all_erase();
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Disable all protocols via Ethernet Access. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The portocol disable was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_eth_acc_flow_flash_disable_protocol(), dva_eth_acc_flow_flash_disable_protocol(), cvb_eth_acc_flow_flash_disable_protocol(), cva_eth_acc_flow_flash_disable_protocol().
 */
RtkApiRet rtk_eth_acc_flow_flash_disable_protocol(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_eth_acc_flow_flash_disable_protocol();
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_eth_acc_flow_flash_disable_protocol();
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_eth_acc_flow_flash_disable_protocol();
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_eth_acc_flow_flash_disable_protocol();
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Generate a packet for flash erase all directly command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashEraseAll Ethernet Access gen flash erase all information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_flash_erase_all(), dva_eth_acc_gen_flash_erase_all(), cvb_eth_acc_gen_flash_erase_all(), cva_eth_acc_gen_flash_erase_all().
 */
RtkApiRet rtk_eth_acc_gen_flash_erase_all(UnitChip_t unitChip, EthAccGenFlashEraseAll_t *ethAccGenFlashEraseAll)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashEraseAll)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_flash_erase_all(ethAccGenFlashEraseAll->sMac, ethAccGenFlashEraseAll->dMac, ethAccGenFlashEraseAll->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_erase_all(ethAccGenFlashEraseAll->sMac, ethAccGenFlashEraseAll->dMac, ethAccGenFlashEraseAll->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_flash_erase_all(ethAccGenFlashEraseAll->sMac, ethAccGenFlashEraseAll->dMac, ethAccGenFlashEraseAll->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_flash_erase_all(ethAccGenFlashEraseAll->sMac, ethAccGenFlashEraseAll->dMac, ethAccGenFlashEraseAll->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for flash start command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashStart Ethernet Access gen flash start information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash start packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_flash_start(), dva_eth_acc_gen_flash_start(), cvb_eth_acc_gen_flash_start(), cva_eth_acc_gen_flash_start().
 */
RtkApiRet rtk_eth_acc_gen_flash_start(UnitChip_t unitChip, EthAccGenFlashStart_t *ethAccGenFlashStart)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashStart)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_flash_start(ethAccGenFlashStart->mac, ethAccGenFlashStart->addr, ethAccGenFlashStart->size, ethAccGenFlashStart->checksum, ethAccGenFlashStart->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_start(ethAccGenFlashStart->mac, ethAccGenFlashStart->addr, ethAccGenFlashStart->size, ethAccGenFlashStart->perLen, ethAccGenFlashStart->checksum, ethAccGenFlashStart->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_flash_start(ethAccGenFlashStart->mac, ethAccGenFlashStart->addr, ethAccGenFlashStart->size, ethAccGenFlashStart->checksum, ethAccGenFlashStart->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_flash_start(ethAccGenFlashStart->mac, ethAccGenFlashStart->addr, ethAccGenFlashStart->size, ethAccGenFlashStart->checksum, ethAccGenFlashStart->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for flash write directly command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashWrite Ethernet Access gen flash write information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash write packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_flash_write(), dva_eth_acc_gen_flash_write(), cvb_eth_acc_gen_flash_write(), cva_eth_acc_gen_flash_write().
 */
RtkApiRet rtk_eth_acc_gen_flash_write(UnitChip_t unitChip, EthAccGenFlashWrite_t *ethAccGenFlashWrite)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashWrite)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_flash_write(ethAccGenFlashWrite->mac, ethAccGenFlashWrite->addr, ethAccGenFlashWrite->flash, ethAccGenFlashWrite->len, ethAccGenFlashWrite->seqId, ethAccGenFlashWrite->checksum, ethAccGenFlashWrite->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_write(ethAccGenFlashWrite->mac, ethAccGenFlashWrite->addr, ethAccGenFlashWrite->flash, ethAccGenFlashWrite->len, ethAccGenFlashWrite->seqId, ethAccGenFlashWrite->checksum, ethAccGenFlashWrite->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_flash_write(ethAccGenFlashWrite->mac, ethAccGenFlashWrite->addr, ethAccGenFlashWrite->flash, ethAccGenFlashWrite->len, ethAccGenFlashWrite->seqId, ethAccGenFlashWrite->checksum, ethAccGenFlashWrite->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_flash_write(ethAccGenFlashWrite->mac, ethAccGenFlashWrite->addr, ethAccGenFlashWrite->flash, ethAccGenFlashWrite->len, ethAccGenFlashWrite->seqId, ethAccGenFlashWrite->checksum, ethAccGenFlashWrite->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash erase all directly command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckFlashEraseAll Ethernet Access check flash erase all information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase reply packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_flash_erase_all(), dva_eth_acc_check_flash_erase_all(), cvb_eth_acc_check_flash_erase_all(), cva_eth_acc_check_flash_erase_all().
 */
RtkApiRet rtk_eth_acc_check_flash_erase_all(UnitChip_t unitChip, EthAccCheckFlashEraseAll_t *ethAccCheckFlashEraseAll)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckFlashEraseAll)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_flash_erase_all(ethAccCheckFlashEraseAll->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_flash_erase_all(ethAccCheckFlashEraseAll->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_flash_erase_all(ethAccCheckFlashEraseAll->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_check_flash_erase_all(ethAccCheckFlashEraseAll->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, sequence ID, and ack ID of the packet are correct for flash/patch write command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckRepWrite Ethernet Access check rep write information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash/patch write packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_rep_write(), dva_eth_acc_check_rep_write(), cvb_eth_acc_check_rep_write(), cva_eth_acc_check_rep_write().
 */
RtkApiRet rtk_eth_acc_check_rep_write(UnitChip_t unitChip, EthAccCheckRepWrite_t *ethAccCheckRepWrite)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckRepWrite)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_rep_write(ethAccCheckRepWrite->packet, ethAccCheckRepWrite->seqId, &ethAccCheckRepWrite->ackId, &ethAccCheckRepWrite->checksum);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_rep_write(ethAccCheckRepWrite->packet, ethAccCheckRepWrite->seqId, &ethAccCheckRepWrite->ackId, &ethAccCheckRepWrite->checksum);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_rep_write(ethAccCheckRepWrite->packet, ethAccCheckRepWrite->seqId, &ethAccCheckRepWrite->ackId, &ethAccCheckRepWrite->checksum);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_check_rep_write(ethAccCheckRepWrite->packet, ethAccCheckRepWrite->seqId, &ethAccCheckRepWrite->ackId, &ethAccCheckRepWrite->checksum);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for sector erase directly command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashEraseSector Ethernet Access gen flash erase sector information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase sector packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_flash_erase_sector(), dva_eth_acc_gen_flash_erase_sector(), cvb_eth_acc_gen_flash_erase_sector(), cva_eth_acc_gen_flash_erase_sector().
 */
RtkApiRet rtk_eth_acc_gen_flash_erase_sector(UnitChip_t unitChip, EthAccGenFlashEraseSector_t *ethAccGenFlashEraseSector)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashEraseSector)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_flash_erase_sector(ethAccGenFlashEraseSector->sMac, ethAccGenFlashEraseSector->dMac, ethAccGenFlashEraseSector->addr, ethAccGenFlashEraseSector->num, ethAccGenFlashEraseSector->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_erase_sector(ethAccGenFlashEraseSector->sMac, ethAccGenFlashEraseSector->dMac, ethAccGenFlashEraseSector->addr, ethAccGenFlashEraseSector->num, ethAccGenFlashEraseSector->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_flash_erase_sector(ethAccGenFlashEraseSector->sMac, ethAccGenFlashEraseSector->dMac, ethAccGenFlashEraseSector->addr, ethAccGenFlashEraseSector->num, ethAccGenFlashEraseSector->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_flash_erase_sector(ethAccGenFlashEraseSector->sMac, ethAccGenFlashEraseSector->dMac, ethAccGenFlashEraseSector->addr, ethAccGenFlashEraseSector->num, ethAccGenFlashEraseSector->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash sector erase directly command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckFlashEraseSector Ethernet Access check flash erase sector information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase sector packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_flash_erase_sector(), dva_eth_acc_check_flash_erase_sector(), cvb_eth_acc_check_flash_erase_sector(), cva_eth_acc_check_flash_erase_sector().
 */
RtkApiRet rtk_eth_acc_check_flash_erase_sector(UnitChip_t unitChip, EthAccCheckFlashEraseSector_t *ethAccCheckFlashEraseSector)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckFlashEraseSector)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_flash_erase_sector(ethAccCheckFlashEraseSector->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_flash_erase_sector(ethAccCheckFlashEraseSector->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_flash_erase_sector(ethAccCheckFlashEraseSector->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_check_flash_erase_sector(ethAccCheckFlashEraseSector->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for block erase directly command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashEraseBlock Ethernet Access gen flash erase block information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_flash_erase_block(), dva_eth_acc_gen_flash_erase_block(), cvb_eth_acc_gen_flash_erase_block(), cva_eth_acc_gen_flash_erase_block().
 */
RtkApiRet rtk_eth_acc_gen_flash_erase_block(UnitChip_t unitChip, EthAccGenFlashEraseBlock_t *ethAccGenFlashEraseBlock)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashEraseBlock)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_flash_erase_block(ethAccGenFlashEraseBlock->sMac, ethAccGenFlashEraseBlock->dMac, ethAccGenFlashEraseBlock->addr, ethAccGenFlashEraseBlock->num, ethAccGenFlashEraseBlock->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_erase_block(ethAccGenFlashEraseBlock->sMac, ethAccGenFlashEraseBlock->dMac, ethAccGenFlashEraseBlock->addr, ethAccGenFlashEraseBlock->num, ethAccGenFlashEraseBlock->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_flash_erase_block(ethAccGenFlashEraseBlock->sMac, ethAccGenFlashEraseBlock->dMac, ethAccGenFlashEraseBlock->addr, ethAccGenFlashEraseBlock->num, ethAccGenFlashEraseBlock->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_flash_erase_block(ethAccGenFlashEraseBlock->sMac, ethAccGenFlashEraseBlock->dMac, ethAccGenFlashEraseBlock->addr, ethAccGenFlashEraseBlock->num, ethAccGenFlashEraseBlock->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash block erase directly command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckFlashEraseBlock Ethernet Access check flash erase block information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_flash_erase_block(), dva_eth_acc_check_flash_erase_block(), cvb_eth_acc_check_flash_erase_block(), cva_eth_acc_check_flash_erase_block().
 */
RtkApiRet rtk_eth_acc_check_flash_erase_block(UnitChip_t unitChip, EthAccCheckFlashEraseBlock_t *ethAccCheckFlashEraseBlock)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckFlashEraseBlock)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_flash_erase_block(ethAccCheckFlashEraseBlock->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_flash_erase_block(ethAccCheckFlashEraseBlock->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_flash_erase_block(ethAccCheckFlashEraseBlock->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_check_flash_erase_block(ethAccCheckFlashEraseBlock->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for flash read directly command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashRead Ethernet Access gen flash read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash read packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_flash_read(), dva_eth_acc_gen_flash_read(), cvb_eth_acc_gen_flash_read(), cva_eth_acc_gen_flash_read().
 */
RtkApiRet rtk_eth_acc_gen_flash_read(UnitChip_t unitChip, EthAccGenFlashRead_t *ethAccGenFlashRead)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashRead)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_flash_read(ethAccGenFlashRead->sMac, ethAccGenFlashRead->dMac, ethAccGenFlashRead->addr, ethAccGenFlashRead->len, ethAccGenFlashRead->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_read(ethAccGenFlashRead->sMac, ethAccGenFlashRead->dMac, ethAccGenFlashRead->addr, ethAccGenFlashRead->len, ethAccGenFlashRead->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_flash_read(ethAccGenFlashRead->sMac, ethAccGenFlashRead->dMac, ethAccGenFlashRead->addr, ethAccGenFlashRead->len, ethAccGenFlashRead->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_flash_read(ethAccGenFlashRead->sMac, ethAccGenFlashRead->dMac, ethAccGenFlashRead->addr, ethAccGenFlashRead->len, ethAccGenFlashRead->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash read directly command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckRepRead Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash read packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_rep_read(), dva_eth_acc_check_rep_read(), cvb_eth_acc_check_rep_read(), cva_eth_acc_check_rep_read().
 */
RtkApiRet rtk_eth_acc_check_rep_read(UnitChip_t unitChip, EthAccCheckRepRead_t *ethAccCheckRepRead)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckRepRead)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_rep_read(ethAccCheckRepRead->packetStruct, ethAccCheckRepRead->data, ethAccCheckRepRead->len);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_rep_read(ethAccCheckRepRead->packetArr, ethAccCheckRepRead->data, ethAccCheckRepRead->len);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_rep_read(ethAccCheckRepRead->packetArr, ethAccCheckRepRead->data, ethAccCheckRepRead->len);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_check_rep_read(ethAccCheckRepRead->packetStruct, ethAccCheckRepRead->data, ethAccCheckRepRead->len);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for erase request command. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashEraseReq Ethernet Access gen flash erase req information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_flash_erase_req(), cvb_eth_acc_gen_flash_erase_req().
 */
RtkApiRet rtk_eth_acc_gen_flash_erase_req(UnitChip_t unitChip, EthAccGenFlashEraseReq_t *ethAccGenFlashEraseReq)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashEraseReq)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_erase_req(ethAccGenFlashEraseReq->sMac, ethAccGenFlashEraseReq->dMac, ethAccGenFlashEraseReq->addr, ethAccGenFlashEraseReq->eraseCmd, ethAccGenFlashEraseReq->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_flash_erase_req(ethAccGenFlashEraseReq->sMac, ethAccGenFlashEraseReq->dMac, ethAccGenFlashEraseReq->addr, ethAccGenFlashEraseReq->eraseCmd, ethAccGenFlashEraseReq->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash erase request command. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckFlashEraseReq Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_check_flash_erase_req(), cvb_eth_acc_check_flash_erase_req().
 */
RtkApiRet rtk_eth_acc_check_flash_erase_req(UnitChip_t unitChip, EthAccCheckFlashEraseReq_t *ethAccCheckFlashEraseReq)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckFlashEraseReq)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_flash_erase_req(ethAccCheckFlashEraseReq->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_flash_erase_req(ethAccCheckFlashEraseReq->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for flash status check command. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashStatusCheck Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash status check packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_flash_status_check(), cvb_eth_acc_gen_flash_status_check().
 */
RtkApiRet rtk_eth_acc_gen_flash_status_check(UnitChip_t unitChip, EthAccGenFlashStatusCheck_t *ethAccGenFlashStatusCheck)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashStatusCheck)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_status_check(ethAccGenFlashStatusCheck->sMac, ethAccGenFlashStatusCheck->dMac, ethAccGenFlashStatusCheck->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_flash_status_check(ethAccGenFlashStatusCheck->sMac, ethAccGenFlashStatusCheck->dMac, ethAccGenFlashStatusCheck->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash status check command. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckFlashStatusCheck Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash flash status packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_check_flash_status_check(), cvb_eth_acc_check_flash_status_check().
 */
RtkApiRet rtk_eth_acc_check_flash_status_check(UnitChip_t unitChip, EthAccCheckFlashStatusCheck_t *ethAccCheckFlashStatusCheck)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckFlashStatusCheck)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_flash_status_check(ethAccCheckFlashStatusCheck->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_flash_status_check(ethAccCheckFlashStatusCheck->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash erase sector with requirement via Ethernet Access without cpu busy wait. \n
 *        This API includes the "Flash Requirement Erase" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashSecEraseReq Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_flow_flash_req_sec_erase(), cvb_eth_acc_flow_flash_req_sec_erase().
 */
RtkApiRet rtk_eth_acc_flow_flash_req_sec_erase(UnitChip_t unitChip, EthAccFlowFlashSecEraseReq_t *ethAccFlowFlashSecEraseReq)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashSecEraseReq)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_req_sec_erase(ethAccFlowFlashSecEraseReq->addr, ethAccFlowFlashSecEraseReq->num);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_flow_flash_req_sec_erase(ethAccFlowFlashSecEraseReq->addr, ethAccFlowFlashSecEraseReq->num);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash erase block with requirement via Ethernet Access without cpu busy wait. \n
 *        This API includes the "Flash Requirement Erase" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashBlkEraseReq Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_flow_flash_req_blk_erase(), cvb_eth_acc_flow_flash_req_blk_erase().
 */
RtkApiRet rtk_eth_acc_flow_flash_req_blk_erase(UnitChip_t unitChip, EthAccFlowFlashBlkEraseReq_t *ethAccFlowFlashBlkEraseReq)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashBlkEraseReq)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_req_blk_erase(ethAccFlowFlashBlkEraseReq->addr, ethAccFlowFlashBlkEraseReq->num);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_flow_flash_req_blk_erase(ethAccFlowFlashBlkEraseReq->addr, ethAccFlowFlashBlkEraseReq->num);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash erase all with requirement via Ethernet Access without cpu busy wait. \n
 *        This API includes the "Flash Requirement Erase" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eth_acc_flow_flash_req_all_erase(), cvb_eth_acc_flow_flash_req_all_erase().
 */
RtkApiRet rtk_eth_acc_flow_flash_req_all_erase(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_eth_acc_flow_flash_req_all_erase();
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_eth_acc_flow_flash_req_all_erase();
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Generate a packet for flash require write require command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashWriteReq Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash write packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_flash_write_req().
 */
RtkApiRet rtk_eth_acc_gen_flash_write_req(UnitChip_t unitChip, EthAccGenFlashWriteReq_t *ethAccGenFlashWriteReq)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashWriteReq)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_write_req(ethAccGenFlashWriteReq->mac, ethAccGenFlashWriteReq->addr, ethAccGenFlashWriteReq->flash, ethAccGenFlashWriteReq->len, ethAccGenFlashWriteReq->seqId, ethAccGenFlashWriteReq->checksum, ethAccGenFlashWriteReq->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash write with requirement via Ethernet Access without cpu busy wait. \n
 *        This API includes the "Flash Requirement Write" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashWriteReq Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash write was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_flow_flash_write_req().
 */
RtkApiRet rtk_eth_acc_flow_flash_write_req(UnitChip_t unitChip, EthAccFlowFlashWriteReq_t *ethAccFlowFlashWriteReq)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashWriteReq)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_write_req(ethAccFlowFlashWriteReq->addr, ethAccFlowFlashWriteReq->len, ethAccFlowFlashWriteReq->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for erase event command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashEraseEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_flash_erase_event().
 */
RtkApiRet rtk_eth_acc_gen_flash_erase_event(UnitChip_t unitChip, EthAccGenFlashEraseEvent_t *ethAccGenFlashEraseEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashEraseEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_erase_event(ethAccGenFlashEraseEvent->sMac, ethAccGenFlashEraseEvent->dMac, ethAccGenFlashEraseEvent->addr, ethAccGenFlashEraseEvent->eraseCmd, ethAccGenFlashEraseEvent->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash erase event command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckFlashEraseEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_check_flash_erase_event().
 */
RtkApiRet rtk_eth_acc_check_flash_erase_event(UnitChip_t unitChip, EthAccCheckFlashEraseEvent_t *ethAccCheckFlashEraseEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckFlashEraseEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_flash_erase_event(ethAccCheckFlashEraseEvent->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for flash write event command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashWriteEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash write packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_flash_write_event().
 */
RtkApiRet rtk_eth_acc_gen_flash_write_event(UnitChip_t unitChip, EthAccGenFlashWriteEvent_t *ethAccGenFlashWriteEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashWriteEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_write_event(ethAccGenFlashWriteEvent->mac, ethAccGenFlashWriteEvent->addr, ethAccGenFlashWriteEvent->flash, ethAccGenFlashWriteEvent->len, ethAccGenFlashWriteEvent->seqId, ethAccGenFlashWriteEvent->checksum, ethAccGenFlashWriteEvent->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for flash event check command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash event check packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_flash_event().
 */
RtkApiRet rtk_eth_acc_gen_flash_event(UnitChip_t unitChip, EthAccGenFlashEvent_t *ethAccGenFlashEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_event(ethAccGenFlashEvent->sMac, ethAccGenFlashEvent->dMac, ethAccGenFlashEvent->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash event check command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckFlashEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash flash event packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_check_flash_event().
 */
RtkApiRet rtk_eth_acc_check_flash_event(UnitChip_t unitChip, EthAccCheckFlashEvent_t *ethAccCheckFlashEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckFlashEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_flash_event(ethAccCheckFlashEvent->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash erase sector with event command via Ethernet Access without cpu busy wait. \n
 *        This API includes the "Flash Erase" in "Flash Access Event Handshaking Flow", as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashSecEraseEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_flow_flash_event_sec_erase().
 */
RtkApiRet rtk_eth_acc_flow_flash_event_sec_erase(UnitChip_t unitChip, EthAccFlowFlashSecEraseEvent_t *ethAccFlowFlashSecEraseEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashSecEraseEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_event_sec_erase(ethAccFlowFlashSecEraseEvent->addr, ethAccFlowFlashSecEraseEvent->num);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash erase block with event command via Ethernet Access without cpu busy wait. \n
 *        This API includes the "Flash Erase" in "Flash Access Event Handshaking Flow", as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashBlkEraseEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_flow_flash_event_blk_erase().
 */
RtkApiRet rtk_eth_acc_flow_flash_event_blk_erase(UnitChip_t unitChip, EthAccFlowFlashBlkEraseEvent_t *ethAccFlowFlashBlkEraseEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashBlkEraseEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_event_blk_erase(ethAccFlowFlashBlkEraseEvent->addr, ethAccFlowFlashBlkEraseEvent->num);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash erase all with event command via Ethernet Access without cpu busy wait. \n
 *        This API includes the "Flash Erase" in "Flash Access Event Handshaking Flow", as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The flash erase was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eth_acc_flow_flash_event_all_erase().
 */
RtkApiRet rtk_eth_acc_flow_flash_event_all_erase(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_eth_acc_flow_flash_event_all_erase();
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Flash write with event command via Ethernet Access. \n
 *        This API includes the "Flash Write" in "Flash Access Event Handshaking Flow", as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashWriteEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash write was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_flow_flash_event_write().
 */
RtkApiRet rtk_eth_acc_flow_flash_event_write(UnitChip_t unitChip, EthAccFlowFlashWriteEvent_t *ethAccFlowFlashWriteEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashWriteEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_event_write(ethAccFlowFlashWriteEvent->addr, ethAccFlowFlashWriteEvent->len, ethAccFlowFlashWriteEvent->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for flash read event command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashReadEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash read packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_flash_read_event().
 */
RtkApiRet rtk_eth_acc_gen_flash_read_event(UnitChip_t unitChip, EthAccGenFlashReadEvent_t *ethAccGenFlashReadEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashReadEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_read_event(ethAccGenFlashReadEvent->sMac, ethAccGenFlashReadEvent->dMac, ethAccGenFlashReadEvent->addr, ethAccGenFlashReadEvent->len, ethAccGenFlashReadEvent->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash read event event command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckRepReadEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash read packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_check_rep_read_event().
 */
RtkApiRet rtk_eth_acc_check_rep_read_event(UnitChip_t unitChip, EthAccCheckRepReadEvent_t *ethAccCheckRepReadEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckRepReadEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_rep_read_event(ethAccCheckRepReadEvent->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for flash status check event command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashReadCheck Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash status check packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_flash_read_check().
 */
RtkApiRet rtk_eth_acc_gen_flash_read_check(UnitChip_t unitChip, EthAccGenFlashReadCheck_t *ethAccGenFlashReadCheck)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashReadCheck)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_read_check(ethAccGenFlashReadCheck->sMac, ethAccGenFlashReadCheck->dMac, ethAccGenFlashReadCheck->offset, ethAccGenFlashReadCheck->len, ethAccGenFlashReadCheck->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash read check event command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckFlashReadCheck Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash flash status packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_check_flash_read_check().
 */
RtkApiRet rtk_eth_acc_check_flash_read_check(UnitChip_t unitChip, EthAccCheckFlashReadCheck_t *ethAccCheckFlashReadCheck)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckFlashReadCheck)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_flash_read_check(ethAccCheckFlashReadCheck->packet, ethAccCheckFlashReadCheck->offset, ethAccCheckFlashReadCheck->len, ethAccCheckFlashReadCheck->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Flash read with event command with background via Ethernet Access. \n
 *        This API includes the "Flash Read" in "Flash Access Event Handshaking Flow", as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowFlashReadEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash read was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_flow_flash_read_event().
 */
RtkApiRet rtk_eth_acc_flow_flash_read_event(UnitChip_t unitChip, EthAccFlowFlashReadEvent_t *ethAccFlowFlashReadEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowFlashReadEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_flash_read_event(ethAccFlowFlashReadEvent->addr, ethAccFlowFlashReadEvent->len, ethAccFlowFlashReadEvent->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for clear event command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenFlashClearEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_flash_clear_event().
 */
RtkApiRet rtk_eth_acc_gen_flash_clear_event(UnitChip_t unitChip, EthAccGenFlashClearEvent_t *ethAccGenFlashClearEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenFlashClearEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_flash_clear_event(ethAccGenFlashClearEvent->sMac, ethAccGenFlashClearEvent->dMac, ethAccGenFlashClearEvent->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for flash clear event command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenCheckFlashClearEvent Ethernet Access check rep read information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The flash erase block packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_check_flash_clear_event().
 */
RtkApiRet rtk_eth_acc_check_flash_clear_event(UnitChip_t unitChip, EthAccCheckFlashClearEvent_t *ethAccGenCheckFlashClearEvent)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenCheckFlashClearEvent)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_flash_clear_event(ethAccGenCheckFlashClearEvent->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Clear flash state command via Ethernet Access. \n
 *        Packet flow of flash clear state directly includs the following call sequence: \n
 *        rtk_eth_acc_gen_flash_clear_event \n
 *        rtk_etherAccess_tx_packet \n
 *        rtk_etherAccess_rx_packet \n
 *        rtk_eth_acc_check_flash_clear_event \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The state clear was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dva_eth_acc_flow_flash_clear_event().
 */
RtkApiRet rtk_eth_acc_flow_flash_clear_event(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_eth_acc_flow_flash_clear_event();
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Reset system with ethernet access. \n
 *        Packet flow of reset system includs the following call sequence: \n
 *        This API includes the "Reset System" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[out] None
 * @retval RT_ERR_OK The system reset was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval Others Please refer to dv1_eth_acc_flow_reset_system(), dva_eth_acc_flow_reset_system(), cvb_eth_acc_flow_reset_system(), cva_eth_acc_flow_reset_system().
 */
RtkApiRet rtk_eth_acc_flow_reset_system(UnitChip_t unitChip)
{
    RtkApiRet ret = RT_ERR_OK;

    switch(unitChip.chip)
    {
#if defined(RTL_907XD_V1)
        case CHIP_907XD_V1:
            ret = dv1_eth_acc_flow_reset_system();
            break;
#endif
#if defined(RTL_907XD_VA)
        case CHIP_907XD_VA:
            ret = dva_eth_acc_flow_reset_system();
            break;
#endif
#if defined(RTL_907XC_VB)
        case CHIP_907XC_VB:
            ret = cvb_eth_acc_flow_reset_system();
            break;
#endif
#if defined(RTL_907XC_VA)
        case CHIP_907XC_VA:
            ret = cva_eth_acc_flow_reset_system();
            break;
#endif
        default:
            ret = -(RT_ERR_NOT_SUPPORT);
            break;
    }
    return ret;
}

/**
 * @brief Generate a packet for reset system command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenResetSystem Ethernet Access gen reset system information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The system reset packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_reset_system(), dva_eth_acc_gen_reset_system(), cvb_eth_acc_gen_reset_system(), cva_eth_acc_gen_reset_system().
 */
RtkApiRet rtk_eth_acc_gen_reset_system(UnitChip_t unitChip, EthAccGenResetSystem_t *ethAccGenResetSystem)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenResetSystem)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_reset_system(ethAccGenResetSystem->sMac, ethAccGenResetSystem->dMac, ethAccGenResetSystem->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_reset_system(ethAccGenResetSystem->sMac, ethAccGenResetSystem->dMac, ethAccGenResetSystem->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_reset_system(ethAccGenResetSystem->sMac, ethAccGenResetSystem->dMac, ethAccGenResetSystem->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_reset_system(ethAccGenResetSystem->sMac, ethAccGenResetSystem->dMac, ethAccGenResetSystem->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for system reset command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckResetSystem Ethernet Access check reset system information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The system reset packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_reset_system(), dva_eth_acc_check_reset_system(), cvb_eth_acc_check_reset_system(), cva_eth_acc_check_reset_system().
 */
RtkApiRet rtk_eth_acc_check_reset_system(UnitChip_t unitChip, EthAccCheckResetSystem_t *ethAccCheckResetSystem)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckResetSystem)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_reset_system(ethAccCheckResetSystem->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_reset_system(ethAccCheckResetSystem->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_reset_system(ethAccCheckResetSystem->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_check_reset_system(ethAccCheckResetSystem->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* ETH_FLASH */

/**
 * @defgroup ETH_PATCH Ethernet Access Patch
 * @brief The APIs are used for access patch (sram) via Ethernet Access.
 * @{
 */
/**
 * @brief Patch write (write sram) via Ethernet Access. \n
 *        This API includes the "Patch Write" flow, as detailed in the Ethernet Access section of the RTL907XD programmer’s reference guide. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowPatchWrite Ethernet Access gen patch write information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The patch write packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_flow_patch_write(), dva_eth_acc_flow_patch_write(), cvb_eth_acc_flow_patch_write(), cva_eth_acc_flow_patch_write().
 */
RtkApiRet rtk_eth_acc_flow_patch_write(UnitChip_t unitChip, EthAccFlowPatchWrite_t *ethAccFlowPatchWrite)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowPatchWrite)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_flow_patch_write(ethAccFlowPatchWrite->addr, ethAccFlowPatchWrite->len, ethAccFlowPatchWrite->data);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_patch_write(ethAccFlowPatchWrite->addr, ethAccFlowPatchWrite->len, ethAccFlowPatchWrite->data);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_flow_patch_write(ethAccFlowPatchWrite->addr, ethAccFlowPatchWrite->len, ethAccFlowPatchWrite->data);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_flow_patch_write(ethAccFlowPatchWrite->addr, ethAccFlowPatchWrite->len, ethAccFlowPatchWrite->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for patch start command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenPatchStart Ethernet Access gen patch start information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The patch start packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_patch_start(), dva_eth_acc_gen_patch_start(), cvb_eth_acc_gen_patch_start(), cva_eth_acc_gen_patch_start().
 */
RtkApiRet rtk_eth_acc_gen_patch_start(UnitChip_t unitChip, EthAccGenPatchStart_t *ethAccGenPatchStart)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenPatchStart)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_patch_start(ethAccGenPatchStart->sMac, ethAccGenPatchStart->dMac, ethAccGenPatchStart->addr, ethAccGenPatchStart->size, ethAccGenPatchStart->checksum, ethAccGenPatchStart->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_patch_start(ethAccGenPatchStart->sMac, ethAccGenPatchStart->dMac, ethAccGenPatchStart->addr, ethAccGenPatchStart->size, ethAccGenPatchStart->checksum, ethAccGenPatchStart->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_patch_start(ethAccGenPatchStart->sMac, ethAccGenPatchStart->dMac, ethAccGenPatchStart->addr, ethAccGenPatchStart->size, ethAccGenPatchStart->checksum, ethAccGenPatchStart->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_patch_start(ethAccGenPatchStart->sMac, ethAccGenPatchStart->dMac, ethAccGenPatchStart->addr, ethAccGenPatchStart->size, ethAccGenPatchStart->checksum, ethAccGenPatchStart->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for patch write command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenPatchWrite Ethernet Access check burst rw information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The reg access reply packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_patch_write(), dva_eth_acc_gen_patch_write(), cvb_eth_acc_gen_patch_write(), cva_eth_acc_gen_patch_write().
 */
RtkApiRet rtk_eth_acc_gen_patch_write(UnitChip_t unitChip, EthAccGenPatchWrite_t *ethAccGenPatchWrite)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenPatchWrite)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_patch_write(ethAccGenPatchWrite->mac, ethAccGenPatchWrite->addr, ethAccGenPatchWrite->patch, ethAccGenPatchWrite->len, ethAccGenPatchWrite->seqId, ethAccGenPatchWrite->checksum, ethAccGenPatchWrite->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_patch_write(ethAccGenPatchWrite->mac, ethAccGenPatchWrite->addr, ethAccGenPatchWrite->patch, ethAccGenPatchWrite->len, ethAccGenPatchWrite->seqId, ethAccGenPatchWrite->checksum, ethAccGenPatchWrite->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_patch_write(ethAccGenPatchWrite->mac, ethAccGenPatchWrite->addr, ethAccGenPatchWrite->patch, ethAccGenPatchWrite->len, ethAccGenPatchWrite->seqId, ethAccGenPatchWrite->checksum, ethAccGenPatchWrite->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_patch_write(ethAccGenPatchWrite->mac, ethAccGenPatchWrite->addr, ethAccGenPatchWrite->patch, ethAccGenPatchWrite->len, ethAccGenPatchWrite->seqId, ethAccGenPatchWrite->checksum, ethAccGenPatchWrite->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* ETH_PATCH */

/**
 * @defgroup ETH_REG Ethernet Access Register
 * @brief The APIs are used for access register via Ethernet Access.
 * @{
 */

/**
 * @brief Generate a packet for register read/write burst command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethGenBurst Ethernet Access gen burst rw information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The reg access packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_burst_rw(), dva_eth_acc_gen_burst_rw(), cvb_eth_acc_gen_burst_rw(), cva_eth_acc_gen_burst_rw().
 */
RtkApiRet rtk_eth_acc_gen_burst_rw(UnitChip_t unitChip, EthGenBurst_t *ethGenBurst)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethGenBurst)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_burst_rw(ethGenBurst->mac, ethGenBurst->number, ethGenBurst->regCmd, &ethGenBurst->regAddr[0], ethGenBurst->regValue, ethGenBurst->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_burst_rw(ethGenBurst->mac, ethGenBurst->number, ethGenBurst->regCmd, &ethGenBurst->regAddr[0], ethGenBurst->regValue, ethGenBurst->portMask, ethGenBurst->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_burst_rw(ethGenBurst->mac, ethGenBurst->number, ethGenBurst->regCmd, &ethGenBurst->regAddr[0], ethGenBurst->regValue, ethGenBurst->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_burst_rw(ethGenBurst->mac, ethGenBurst->number, ethGenBurst->regCmd, &ethGenBurst->regAddr[0], ethGenBurst->regValue, ethGenBurst->packet);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the ethertype, RTL9040_Identity, and ack ID of the packet are correct for register read/write burst command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethChkBurst Ethernet Access gen burst rw information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The burst rw packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_burst_rw(), dva_eth_acc_check_burst_rw(), cvb_eth_acc_check_burst_rw(), cva_eth_acc_check_burst_rw().
 */
RtkApiRet rtk_eth_acc_check_burst_rw(UnitChip_t unitChip, EthChkBurst_t *ethChkBurst)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethChkBurst)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_burst_rw(ethChkBurst->number, ethChkBurst->regValue, ethChkBurst->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_burst_rw(ethChkBurst->number, ethChkBurst->regValue, ethChkBurst->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_burst_rw(ethChkBurst->number, ethChkBurst->regValue, ethChkBurst->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_check_burst_rw(ethChkBurst->number, ethChkBurst->regValue, ethChkBurst->packet);
                break;
#endif

            default:
                ret = -RT_ERR_NOT_SUPPORT;
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for table dump. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ethAccFlowTableEntryGet Ethernet Access flow table entry get information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The table get was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_flow_table_entry_get(), dva_eth_acc_flow_table_entry_get(), cvb_eth_acc_flow_table_entry_get(), cva_eth_acc_flow_table_entry_get().
 */
RtkApiRet rtk_eth_acc_flow_table_entry_get(UnitChip_t unitChip, EthAccFlowTableEntryGet_t *ethAccFlowTableEntryGet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowTableEntryGet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_flow_table_entry_get(ethAccFlowTableEntryGet->tableDV1, ethAccFlowTableEntryGet->startIndex, ethAccFlowTableEntryGet->entryNum, ethAccFlowTableEntryGet->data);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_table_entry_get(ethAccFlowTableEntryGet->tableDVa, ethAccFlowTableEntryGet->startIndex, ethAccFlowTableEntryGet->entryNum, ethAccFlowTableEntryGet->data);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_flow_table_entry_get(ethAccFlowTableEntryGet->tableCVb, ethAccFlowTableEntryGet->startIndex, ethAccFlowTableEntryGet->entryNum, ethAccFlowTableEntryGet->data);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_flow_table_entry_get(ethAccFlowTableEntryGet->tableCVa, ethAccFlowTableEntryGet->startIndex, ethAccFlowTableEntryGet->entryNum, ethAccFlowTableEntryGet->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for table set. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccFlowTableEntrySet Ethernet Access flow table entry set information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The table set was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_flow_table_entry_set(), dva_eth_acc_flow_table_entry_set(), cvb_eth_acc_flow_table_entry_set(), cva_eth_acc_flow_table_entry_set().
 */
RtkApiRet rtk_eth_acc_flow_table_entry_set(UnitChip_t unitChip, EthAccFlowTableEntrySet_t *ethAccFlowTableEntrySet)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccFlowTableEntrySet)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_flow_table_entry_set(ethAccFlowTableEntrySet->tableDV1, ethAccFlowTableEntrySet->startIndex, ethAccFlowTableEntrySet->entryNum, ethAccFlowTableEntrySet->data);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_flow_table_entry_set(ethAccFlowTableEntrySet->tableDVa, ethAccFlowTableEntrySet->startIndex, ethAccFlowTableEntrySet->entryNum, ethAccFlowTableEntrySet->data);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_flow_table_entry_set(ethAccFlowTableEntrySet->tableCVb, ethAccFlowTableEntrySet->startIndex, ethAccFlowTableEntrySet->entryNum, ethAccFlowTableEntrySet->data);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_flow_table_entry_set(ethAccFlowTableEntrySet->tableCVa, ethAccFlowTableEntrySet->startIndex, ethAccFlowTableEntrySet->entryNum, ethAccFlowTableEntrySet->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* ETH_REG */

/**
 * @defgroup ETH_L3_OTP Ethernet Access L3 OTP
 * @brief The APIs are used for access OTP via Ethernet Access with L3 packets.
 * @{
 */
/**
 * @brief Generate a packet for L3 otp ask command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenL3OtpAsk Ethernet Access gen otp ask information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp ask packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_l3_otp_ask(), dva_eth_acc_gen_l3_otp_ask(), cvb_eth_acc_gen_l3_otp_ask(), cva_eth_acc_gen_l3_otp_ask().
 */
RtkApiRet rtk_eth_acc_gen_l3_otp_ask(UnitChip_t unitChip, EthAccGenL3OtpAsk_t *ethAccGenL3OtpAsk)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenL3OtpAsk)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_l3_otp_ask(ethAccGenL3OtpAsk->macAddr, ethAccGenL3OtpAsk->sip, ethAccGenL3OtpAsk->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_l3_otp_ask(ethAccGenL3OtpAsk->macAddr, ethAccGenL3OtpAsk->sip, ethAccGenL3OtpAsk->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_l3_otp_ask(ethAccGenL3OtpAsk->macAddr, ethAccGenL3OtpAsk->sip, ethAccGenL3OtpAsk->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_l3_otp_ask(ethAccGenL3OtpAsk->macAddr, ethAccGenL3OtpAsk->sip, ethAccGenL3OtpAsk->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for L3 otp check command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenL3OtpCheck Ethernet Access gen otp check information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp check packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_l3_otp_check(), dva_eth_acc_gen_l3_otp_check(), cvb_eth_acc_gen_l3_otp_check(), cva_eth_acc_gen_l3_otp_check().
 */
RtkApiRet rtk_eth_acc_gen_l3_otp_check(UnitChip_t unitChip, EthAccGenL3OtpCheck_t *ethAccGenL3OtpCheck)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenL3OtpCheck)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_l3_otp_check(ethAccGenL3OtpCheck->macAddr, ethAccGenL3OtpCheck->sip, ethAccGenL3OtpCheck->uuid, ethAccGenL3OtpCheck->type, ethAccGenL3OtpCheck->typeId, ethAccGenL3OtpCheck->len, ethAccGenL3OtpCheck->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_l3_otp_check(ethAccGenL3OtpCheck->macAddr, ethAccGenL3OtpCheck->sip, ethAccGenL3OtpCheck->uuid, ethAccGenL3OtpCheck->type, ethAccGenL3OtpCheck->typeId, ethAccGenL3OtpCheck->len, ethAccGenL3OtpCheck->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_l3_otp_check(ethAccGenL3OtpCheck->macAddr, ethAccGenL3OtpCheck->sip, ethAccGenL3OtpCheck->uuid, ethAccGenL3OtpCheck->type, ethAccGenL3OtpCheck->typeId, ethAccGenL3OtpCheck->len, ethAccGenL3OtpCheck->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_l3_otp_check(ethAccGenL3OtpCheck->macAddr, ethAccGenL3OtpCheck->sip, ethAccGenL3OtpCheck->uuid, ethAccGenL3OtpCheck->type, ethAccGenL3OtpCheck->typeId, ethAccGenL3OtpCheck->len, ethAccGenL3OtpCheck->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for L3 otp write command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenL3OtpWrite Ethernet Access gen otp write information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp write packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_l3_otp_write(), dva_eth_acc_gen_l3_otp_write(), cvb_eth_acc_gen_l3_otp_write(), cva_eth_acc_gen_l3_otp_write().
 */
RtkApiRet rtk_eth_acc_gen_l3_otp_write(UnitChip_t unitChip, EthAccGenL3OtpWrite_t *ethAccGenL3OtpWrite)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenL3OtpWrite)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_l3_otp_write(ethAccGenL3OtpWrite->macAddr, ethAccGenL3OtpWrite->sip, ethAccGenL3OtpWrite->uuid, ethAccGenL3OtpWrite->otpInfo, ethAccGenL3OtpWrite->data, ethAccGenL3OtpWrite->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_l3_otp_write(ethAccGenL3OtpWrite->macAddr, ethAccGenL3OtpWrite->sip, ethAccGenL3OtpWrite->uuid, ethAccGenL3OtpWrite->otpInfo, ethAccGenL3OtpWrite->data, ethAccGenL3OtpWrite->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_l3_otp_write(ethAccGenL3OtpWrite->macAddr, ethAccGenL3OtpWrite->sip, ethAccGenL3OtpWrite->uuid, ethAccGenL3OtpWrite->otpInfo, ethAccGenL3OtpWrite->data, ethAccGenL3OtpWrite->packet);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_gen_l3_otp_write(ethAccGenL3OtpWrite->macAddr, ethAccGenL3OtpWrite->sip, ethAccGenL3OtpWrite->uuid, ethAccGenL3OtpWrite->otpInfo, ethAccGenL3OtpWrite->data, ethAccGenL3OtpWrite->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Get ttl in otp packet. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in,out] ethAccGetL3OtpTtl Ethernet Access get otp ttl information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The TTL get was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_get_l3_otp_ttl(), dva_eth_acc_get_l3_otp_ttl(), cvb_eth_acc_get_l3_otp_ttl(), cva_eth_acc_get_l3_otp_ttl().
 */
RtkApiRet rtk_eth_acc_get_l3_otp_ttl(UnitChip_t unitChip, EthAccGetL3OtpTtl_t *ethAccGetL3OtpTtl)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGetL3OtpTtl)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_get_l3_otp_ttl(ethAccGetL3OtpTtl->packet, &ethAccGetL3OtpTtl->ttl);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_get_l3_otp_ttl(ethAccGetL3OtpTtl->packet, &ethAccGetL3OtpTtl->ttl);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_get_l3_otp_ttl(ethAccGetL3OtpTtl->packet, &ethAccGetL3OtpTtl->ttl);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_get_l3_otp_ttl(ethAccGetL3OtpTtl->packet, &ethAccGetL3OtpTtl->ttl);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the command ID and ack ID of the packet are correct, and get UUID for L3 OTP join. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckL3OtpJoin Ethernet Access check otp join information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp join packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_l3_otp_join(), dva_eth_acc_check_l3_otp_join(), cvb_eth_acc_check_l3_otp_join(), cva_eth_acc_check_l3_otp_join().
 */
RtkApiRet rtk_eth_acc_check_l3_otp_join(UnitChip_t unitChip, EthAccCheckL3OtpJoin_t *ethAccCheckL3OtpJoin)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckL3OtpJoin)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_l3_otp_join(ethAccCheckL3OtpJoin->packet, &ethAccCheckL3OtpJoin->ackId, ethAccCheckL3OtpJoin->uuid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_l3_otp_join(ethAccCheckL3OtpJoin->packet, &ethAccCheckL3OtpJoin->ackId, ethAccCheckL3OtpJoin->uuid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_l3_otp_join(ethAccCheckL3OtpJoin->packet, &ethAccCheckL3OtpJoin->ackId, ethAccCheckL3OtpJoin->uuid);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_check_l3_otp_join(ethAccCheckL3OtpJoin->packet, &ethAccCheckL3OtpJoin->ackId, ethAccCheckL3OtpJoin->uuid);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the command ID, UUID, type, type ID, and ack ID of the packet are correct for L3 OTP reply. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB/RTL907XC_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckL3OtpRep Ethernet Access check otp rep information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp check/write was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_l3_otp_rep(), dva_eth_acc_check_l3_otp_rep(), cvb_eth_acc_check_l3_otp_rep(), cva_eth_acc_check_l3_otp_rep().
 */
RtkApiRet rtk_eth_acc_check_l3_otp_rep(UnitChip_t unitChip, EthAccCheckL3OtpRep_t *ethAccCheckL3OtpRep)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckL3OtpRep)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_l3_otp_rep(ethAccCheckL3OtpRep->packet, ethAccCheckL3OtpRep->uuid, ethAccCheckL3OtpRep->type, ethAccCheckL3OtpRep->typeId, ethAccCheckL3OtpRep->len, &ethAccCheckL3OtpRep->ackId, ethAccCheckL3OtpRep->data);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_l3_otp_rep(ethAccCheckL3OtpRep->packet, ethAccCheckL3OtpRep->uuid, ethAccCheckL3OtpRep->type, ethAccCheckL3OtpRep->typeId, ethAccCheckL3OtpRep->len, &ethAccCheckL3OtpRep->ackId, ethAccCheckL3OtpRep->data);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_l3_otp_rep(ethAccCheckL3OtpRep->packet, ethAccCheckL3OtpRep->uuid, ethAccCheckL3OtpRep->type, ethAccCheckL3OtpRep->typeId, ethAccCheckL3OtpRep->len, &ethAccCheckL3OtpRep->ackId, ethAccCheckL3OtpRep->data);
                break;
#endif
#if defined(RTL_907XC_VA)
            case CHIP_907XC_VA:
                ret = cva_eth_acc_check_l3_otp_rep(ethAccCheckL3OtpRep->packet, ethAccCheckL3OtpRep->uuid, ethAccCheckL3OtpRep->type, ethAccCheckL3OtpRep->typeId, ethAccCheckL3OtpRep->len, &ethAccCheckL3OtpRep->ackId, ethAccCheckL3OtpRep->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* ETH_L3_OTP */

/**
 * @defgroup ETH_L2_OTP Ethernet Access L2 OTP
 * @brief The APIs are used for access OTP via Ethernet Access with L2 packets.
 * @{
 */
/**
 * @brief Generate a packet for L2 otp ask command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenL2OtpAsk Ethernet Access gen L2 otp ask information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The L2 otp ask packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_l2_otp_ask(), dva_eth_acc_gen_l2_otp_ask(), cvb_eth_acc_gen_l2_otp_ask().
 */
RtkApiRet rtk_eth_acc_gen_l2_otp_ask(UnitChip_t unitChip, EthAccGenL2OtpAsk_t *ethAccGenL2OtpAsk)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenL2OtpAsk)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_l2_otp_ask(ethAccGenL2OtpAsk->sMac, ethAccGenL2OtpAsk->dMac, ethAccGenL2OtpAsk->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_l2_otp_ask(ethAccGenL2OtpAsk->sMac, ethAccGenL2OtpAsk->dMac, ethAccGenL2OtpAsk->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_l2_otp_ask(ethAccGenL2OtpAsk->sMac, ethAccGenL2OtpAsk->dMac, ethAccGenL2OtpAsk->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for L2 otp check command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenL2OtpCheck Ethernet Access gen L2 otp check information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp check packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_l2_otp_check(), dva_eth_acc_gen_l2_otp_check(), cvb_eth_acc_gen_l2_otp_check().
 */
RtkApiRet rtk_eth_acc_gen_l2_otp_check(UnitChip_t unitChip, EthAccGenL2OtpCheck_t *ethAccGenL2OtpCheck)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenL2OtpCheck)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_l2_otp_check(ethAccGenL2OtpCheck->sMac, ethAccGenL2OtpCheck->dMac, ethAccGenL2OtpCheck->uuid, ethAccGenL2OtpCheck->type, ethAccGenL2OtpCheck->typeId, ethAccGenL2OtpCheck->len, ethAccGenL2OtpCheck->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_l2_otp_check(ethAccGenL2OtpCheck->sMac, ethAccGenL2OtpCheck->dMac, ethAccGenL2OtpCheck->uuid, ethAccGenL2OtpCheck->type, ethAccGenL2OtpCheck->typeId, ethAccGenL2OtpCheck->len, ethAccGenL2OtpCheck->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_l2_otp_check(ethAccGenL2OtpCheck->sMac, ethAccGenL2OtpCheck->dMac, ethAccGenL2OtpCheck->uuid, ethAccGenL2OtpCheck->type, ethAccGenL2OtpCheck->typeId, ethAccGenL2OtpCheck->len, ethAccGenL2OtpCheck->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for L2 otp write command. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenL2OtpWrite Ethernet Access gen L2 otp write information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp write packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_gen_l2_otp_write(), dva_eth_acc_gen_l2_otp_write(), cvb_eth_acc_gen_l2_otp_write().
 */
RtkApiRet rtk_eth_acc_gen_l2_otp_write(UnitChip_t unitChip, EthAccGenL2OtpWrite_t *ethAccGenL2OtpWrite)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenL2OtpWrite)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_gen_l2_otp_write(ethAccGenL2OtpWrite->sMac, ethAccGenL2OtpWrite->dMac, ethAccGenL2OtpWrite->uuid, ethAccGenL2OtpWrite->otpInfo, ethAccGenL2OtpWrite->data, ethAccGenL2OtpWrite->packet);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_l2_otp_write(ethAccGenL2OtpWrite->sMac, ethAccGenL2OtpWrite->dMac, ethAccGenL2OtpWrite->uuid, ethAccGenL2OtpWrite->otpInfo, ethAccGenL2OtpWrite->data, ethAccGenL2OtpWrite->packet);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_gen_l2_otp_write(ethAccGenL2OtpWrite->sMac, ethAccGenL2OtpWrite->dMac, ethAccGenL2OtpWrite->uuid, ethAccGenL2OtpWrite->otpInfo, ethAccGenL2OtpWrite->data, ethAccGenL2OtpWrite->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the command ID and ack ID of the packet are correct, and get UUID for L2 OTP join. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckL2OtpJoin Ethernet Access check L2 otp join information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp join packet check was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_l2_otp_join(), dva_eth_acc_check_l2_otp_join(), cvb_eth_acc_check_l2_otp_join().
 */
RtkApiRet rtk_eth_acc_check_l2_otp_join(UnitChip_t unitChip, EthAccCheckL2OtpJoin_t *ethAccCheckL2OtpJoin)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckL2OtpJoin)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_l2_otp_join(ethAccCheckL2OtpJoin->packet, &ethAccCheckL2OtpJoin->ackId, ethAccCheckL2OtpJoin->uuid);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_l2_otp_join(ethAccCheckL2OtpJoin->packet, &ethAccCheckL2OtpJoin->ackId, ethAccCheckL2OtpJoin->uuid);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_l2_otp_join(ethAccCheckL2OtpJoin->packet, &ethAccCheckL2OtpJoin->ackId, ethAccCheckL2OtpJoin->uuid);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the command ID, UUID, type, type ID, and ack ID of the packet are correct for L2 OTP reply. \n
 *        Supported chips: RTL907XD_VA/RTL907XD_V1/RTL907XC_VB
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckL2OtpRep Ethernet Access check L2 otp rep information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp check/write was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dv1_eth_acc_check_l2_otp_rep(), dva_eth_acc_check_l2_otp_rep(), cvb_eth_acc_check_l2_otp_rep().
 */
RtkApiRet rtk_eth_acc_check_l2_otp_rep(UnitChip_t unitChip, EthAccCheckL2OtpRep_t *ethAccCheckL2OtpRep)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckL2OtpRep)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_V1)
            case CHIP_907XD_V1:
                ret = dv1_eth_acc_check_l2_otp_rep(ethAccCheckL2OtpRep->packet, ethAccCheckL2OtpRep->uuid, ethAccCheckL2OtpRep->type, ethAccCheckL2OtpRep->typeId, ethAccCheckL2OtpRep->len, &ethAccCheckL2OtpRep->ackId, ethAccCheckL2OtpRep->data);
                break;
#endif
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_l2_otp_rep(ethAccCheckL2OtpRep->packet, ethAccCheckL2OtpRep->uuid, ethAccCheckL2OtpRep->type, ethAccCheckL2OtpRep->typeId, ethAccCheckL2OtpRep->len, &ethAccCheckL2OtpRep->ackId, ethAccCheckL2OtpRep->data);
                break;
#endif
#if defined(RTL_907XC_VB)
            case CHIP_907XC_VB:
                ret = cvb_eth_acc_check_l2_otp_rep(ethAccCheckL2OtpRep->packet, ethAccCheckL2OtpRep->uuid, ethAccCheckL2OtpRep->type, ethAccCheckL2OtpRep->typeId, ethAccCheckL2OtpRep->len, &ethAccCheckL2OtpRep->ackId, ethAccCheckL2OtpRep->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for L2 adapt otp check command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenL2AdaptOtpCheck Ethernet Access gen L2 adapt otp check information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp check packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_l2_adapt_otp_check().
 */
RtkApiRet rtk_eth_acc_gen_l2_adapt_otp_check(UnitChip_t unitChip, EthAccGenL2AdaptOtpCheck_t *ethAccGenL2AdaptOtpCheck)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenL2AdaptOtpCheck)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_l2_adapt_otp_check(ethAccGenL2AdaptOtpCheck->sMac, ethAccGenL2AdaptOtpCheck->dMac, ethAccGenL2AdaptOtpCheck->uuid, ethAccGenL2AdaptOtpCheck->addr, ethAccGenL2AdaptOtpCheck->len, ethAccGenL2AdaptOtpCheck->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Generate a packet for L2 adapt otp write command. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccGenL2AdaptOtpWrite Ethernet Access gen L2 adapt otp write information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp write packet generation was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_gen_l2_adapt_otp_write().
 */
RtkApiRet rtk_eth_acc_gen_l2_adapt_otp_write(UnitChip_t unitChip, EthAccGenL2AdaptOtpWrite_t *ethAccGenL2AdaptOtpWrite)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccGenL2AdaptOtpWrite)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_gen_l2_adapt_otp_write(ethAccGenL2AdaptOtpWrite->sMac, ethAccGenL2AdaptOtpWrite->dMac, ethAccGenL2AdaptOtpWrite->uuid, ethAccGenL2AdaptOtpWrite->otpInfo, ethAccGenL2AdaptOtpWrite->data, ethAccGenL2AdaptOtpWrite->packet);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**
 * @brief Check if the command ID, UUID, and ack ID of the packet are correct for L2 adapt OTP reply. \n
 *        Supported chips: RTL907XD_VA
 *
 * @param[in]  unitChip Stacking unit and chip ID (N/A) \n
 *             Stacking unit ID is used to indicate which switch is to be accessed.
 *             Chip ID is used to indicate which chip model is to be accessed.
 * @param[in]  ethAccCheckL2AdaptOtpRep Ethernet Access check L2 adapt otp rep information (N/A) \n
 *             Please refer to the struct field introductions in the .h files for detailed content
 * @param[out] None
 * @retval RT_ERR_OK The otp check/write was successful.
 * @retval -RT_ERR_NOT_SUPPORT Not supported.
 * @retval -RT_ERR_NULL_POINTER The input parameter is NULL.
 * @retval Others Please refer to dva_eth_acc_check_l2_adapt_otp_rep().
 */
RtkApiRet rtk_eth_acc_check_l2_adapt_otp_rep(UnitChip_t unitChip, EthAccCheckL2AdaptOtpRep_t *ethAccCheckL2AdaptOtpRep)
{
    RtkApiRet ret = RT_ERR_OK;

    if(NULL == ethAccCheckL2AdaptOtpRep)
    {
        ret = -(RT_ERR_NULL_POINTER);
    }
    else
    {
        switch(unitChip.chip)
        {
#if defined(RTL_907XD_VA)
            case CHIP_907XD_VA:
                ret = dva_eth_acc_check_l2_adapt_otp_rep(ethAccCheckL2AdaptOtpRep->packet, ethAccCheckL2AdaptOtpRep->uuid, ethAccCheckL2AdaptOtpRep->len, &ethAccCheckL2AdaptOtpRep->ackId, ethAccCheckL2AdaptOtpRep->data);
                break;
#endif
            default:
                ret = -(RT_ERR_NOT_SUPPORT);
                break;
        }
    }
    return ret;
}

/**@}*/ /* ETH_L2_OTP */

/**@}*/  // Ethernet Access
