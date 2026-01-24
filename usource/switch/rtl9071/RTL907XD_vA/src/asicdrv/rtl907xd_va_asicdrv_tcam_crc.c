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

/******************************************************************************/
/* Include                                                                    */
/******************************************************************************/
#include "rtkas_api_ext.h"
#include "rtl907xd_va_api_acl.h"
#include "rtl907xd_va_asicdrv_acl.h"
#include "rtl907xd_va_asicdrv_tcam.h"
#include "rtl907xd_va_asicdrv_tcam_crc.h"

/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
static uint32 dva_drv_tcam_crc_32_calculate(const uint8 *data, uint32 len)
{
    /* crcCalc = 0xFFFFFFFFu */
    /* *ptr = data */
    /* i = 0u */
    uint32       crcCalc = 0xFFFFFFFFu;
    const uint8 *ptr     = data;
    uint32       i       = 0u;

    /* clang-format off */ 

    /* Declare crcTable[256] with pre-calculated result */
    const uint32 crcTable[256] = {
        0x00000000u, 0x77073096u, 0xEE0E612Cu, 0x990951BAu,
        0x076DC419u, 0x706AF48Fu, 0xE963A535u, 0x9E6495A3u,
        0x0EDB8832u, 0x79DCB8A4u, 0xE0D5E91Eu, 0x97D2D988u,
        0x09B64C2Bu, 0x7EB17CBDu, 0xE7B82D07u, 0x90BF1D91u,
        0x1DB71064u, 0x6AB020F2u, 0xF3B97148u, 0x84BE41DEu,
        0x1ADAD47Du, 0x6DDDE4EBu, 0xF4D4B551u, 0x83D385C7u,
        0x136C9856u, 0x646BA8C0u, 0xFD62F97Au, 0x8A65C9ECu,
        0x14015C4Fu, 0x63066CD9u, 0xFA0F3D63u, 0x8D080DF5u,
        0x3B6E20C8u, 0x4C69105Eu, 0xD56041E4u, 0xA2677172u,
        0x3C03E4D1u, 0x4B04D447u, 0xD20D85FDu, 0xA50AB56Bu,
        0x35B5A8FAu, 0x42B2986Cu, 0xDBBBC9D6u, 0xACBCF940u,
        0x32D86CE3u, 0x45DF5C75u, 0xDCD60DCFu, 0xABD13D59u,
        0x26D930ACu, 0x51DE003Au, 0xC8D75180u, 0xBFD06116u,
        0x21B4F4B5u, 0x56B3C423u, 0xCFBA9599u, 0xB8BDA50Fu,
        0x2802B89Eu, 0x5F058808u, 0xC60CD9B2u, 0xB10BE924u,
        0x2F6F7C87u, 0x58684C11u, 0xC1611DABu, 0xB6662D3Du,
        0x76DC4190u, 0x01DB7106u, 0x98D220BCu, 0xEFD5102Au,
        0x71B18589u, 0x06B6B51Fu, 0x9FBFE4A5u, 0xE8B8D433u,
        0x7807C9A2u, 0x0F00F934u, 0x9609A88Eu, 0xE10E9818u,
        0x7F6A0DBBu, 0x086D3D2Du, 0x91646C97u, 0xE6635C01u,
        0x6B6B51F4u, 0x1C6C6162u, 0x856530D8u, 0xF262004Eu,
        0x6C0695EDu, 0x1B01A57Bu, 0x8208F4C1u, 0xF50FC457u,
        0x65B0D9C6u, 0x12B7E950u, 0x8BBEB8EAu, 0xFCB9887Cu,
        0x62DD1DDFu, 0x15DA2D49u, 0x8CD37CF3u, 0xFBD44C65u,
        0x4DB26158u, 0x3AB551CEu, 0xA3BC0074u, 0xD4BB30E2u,
        0x4ADFA541u, 0x3DD895D7u, 0xA4D1C46Du, 0xD3D6F4FBu,
        0x4369E96Au, 0x346ED9FCu, 0xAD678846u, 0xDA60B8D0u,
        0x44042D73u, 0x33031DE5u, 0xAA0A4C5Fu, 0xDD0D7CC9u,
        0x5005713Cu, 0x270241AAu, 0xBE0B1010u, 0xC90C2086u,
        0x5768B525u, 0x206F85B3u, 0xB966D409u, 0xCE61E49Fu,
        0x5EDEF90Eu, 0x29D9C998u, 0xB0D09822u, 0xC7D7A8B4u,
        0x59B33D17u, 0x2EB40D81u, 0xB7BD5C3Bu, 0xC0BA6CADu,
        0xEDB88320u, 0x9ABFB3B6u, 0x03B6E20Cu, 0x74B1D29Au,
        0xEAD54739u, 0x9DD277AFu, 0x04DB2615u, 0x73DC1683u,
        0xE3630B12u, 0x94643B84u, 0x0D6D6A3Eu, 0x7A6A5AA8u,
        0xE40ECF0Bu, 0x9309FF9Du, 0x0A00AE27u, 0x7D079EB1u,
        0xF00F9344u, 0x8708A3D2u, 0x1E01F268u, 0x6906C2FEu,
        0xF762575Du, 0x806567CBu, 0x196C3671u, 0x6E6B06E7u,
        0xFED41B76u, 0x89D32BE0u, 0x10DA7A5Au, 0x67DD4ACCu,
        0xF9B9DF6Fu, 0x8EBEEFF9u, 0x17B7BE43u, 0x60B08ED5u,
        0xD6D6A3E8u, 0xA1D1937Eu, 0x38D8C2C4u, 0x4FDFF252u,
        0xD1BB67F1u, 0xA6BC5767u, 0x3FB506DDu, 0x48B2364Bu,
        0xD80D2BDAu, 0xAF0A1B4Cu, 0x36034AF6u, 0x41047A60u,
        0xDF60EFC3u, 0xA867DF55u, 0x316E8EEFu, 0x4669BE79u,
        0xCB61B38Cu, 0xBC66831Au, 0x256FD2A0u, 0x5268E236u,
        0xCC0C7795u, 0xBB0B4703u, 0x220216B9u, 0x5505262Fu,
        0xC5BA3BBEu, 0xB2BD0B28u, 0x2BB45A92u, 0x5CB36A04u,
        0xC2D7FFA7u, 0xB5D0CF31u, 0x2CD99E8Bu, 0x5BDEAE1Du,
        0x9B64C2B0u, 0xEC63F226u, 0x756AA39Cu, 0x026D930Au,
        0x9C0906A9u, 0xEB0E363Fu, 0x72076785u, 0x05005713u,
        0x95BF4A82u, 0xE2B87A14u, 0x7BB12BAEu, 0x0CB61B38u,
        0x92D28E9Bu, 0xE5D5BE0Du, 0x7CDCEFB7u, 0x0BDBDF21u,
        0x86D3D2D4u, 0xF1D4E242u, 0x68DDB3F8u, 0x1FDA836Eu,
        0x81BE16CDu, 0xF6B9265Bu, 0x6FB077E1u, 0x18B74777u,
        0x88085AE6u, 0xFF0F6A70u, 0x66063BCAu, 0x11010B5Cu,
        0x8F659EFFu, 0xF862AE69u, 0x616BFFD3u, 0x166CCF45u,
        0xA00AE278u, 0xD70DD2EEu, 0x4E048354u, 0x3903B3C2u,
        0xA7672661u, 0xD06016F7u, 0x4969474Du, 0x3E6E77DBu,
        0xAED16A4Au, 0xD9D65ADCu, 0x40DF0B66u, 0x37D83BF0u,
        0xA9BCAE53u, 0xDEBB9EC5u, 0x47B2CF7Fu, 0x30B5FFE9u,
        0xBDBDF21Cu, 0xCABAC28Au, 0x53B39330u, 0x24B4A3A6u,
        0xBAD03605u, 0xCDD70693u, 0x54DE5729u, 0x23D967BFu,
        0xB3667A2Eu, 0xC4614AB8u, 0x5D681B02u, 0x2A6F2B94u,
        0xB40BBE37u, 0xC30C8EA1u, 0x5A05DF1Bu, 0x2D02EF8Du,
    };

    /* clang-format on */

    /* For loop : i = 0u; i < len; i++ */
    for(i = 0u; i < len; i++)
    {
        /* Calculate CRC32 value of each byte */
        crcCalc = (crcCalc >> 8u) ^ crcTable[(crcCalc ^ ptr[i]) & 0xFFu];
    }

    /* Return CRC32 value */
    return ~crcCalc;
}

/******************* PIE *******************/
static RtkApiRet dva_drv_pie_tcam0_data_handler(const uint32 *data, uint32 offset, uint8 entryValid, uint32 *tcam0CurCrc)
{
    /* tcam0: crc(32) + valid(7'b0 + 1) + pie_data[95:64](32) + pie_data[63:32](32) + pie_data[31:0](32) = 136 bits (8bits * 17) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam0[17] */
        uint8  tcam0[17]  = {0};
        uint32 tcam0Index = 0u;
        uint32 temp32     = 0u;

        /* Init TCAM0 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam0, 0, sizeof(tcam0)));
        tcam0Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)tcam0CurCrc, sizeof(*tcam0CurCrc)));
        tcam0Index += (uint32)sizeof(*tcam0CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &entryValid, sizeof(entryValid)));
            tcam0Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + pie_data[95:64](32) */
            /* pie_data[95:64](data reg 2), pie_care[95:64](data reg 9) */
            temp32 = swapl32(data[offset + 2u] & data[offset + 9u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + pie_data[95:64](32) + pie_data[63:32](32) */
            /* pie_data[63:32](data reg 1), pie_care[63:32](data reg 8) */
            temp32 = swapl32(data[offset + 1u] & data[offset + 8u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + pie_data[95:64](32) + pie_data[63:32](32) + pie_data[31:0](32) */
            /* pie_data[31:0](data reg 0), pie_care[31:0](data reg 7) */
            temp32 = swapl32(data[offset + 0u] & data[offset + 7u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM0 data CRC32 value */
        *tcam0CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam0, (uint32)sizeof(tcam0)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_pie_tcam0_mask_handler(const uint32 *data, uint32 offset, uint8 entryValid, uint32 *tcam0CurCrc)
{
    /* tcam0: crc(32) + valid(7'b0 + 1) + pie_data[95:64](32) + pie_data[63:32](32) + pie_data[31:0](32) = 136 bits (8bits * 17) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam0[17] */
        uint8  tcam0[17]  = {0};
        uint32 tcam0Index = 0u;
        uint32 temp32     = 0u;

        /* Init TCAM0 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam0, 0, sizeof(tcam0)));
        tcam0Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)tcam0CurCrc, sizeof(*tcam0CurCrc)));
        tcam0Index += (uint32)sizeof(*tcam0CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &entryValid, sizeof(entryValid)));
            tcam0Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + pie_care[95:64](32) */
            /* pie_care[95:64](data reg 9) */
            temp32 = swapl32(data[offset + 9u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + pie_care[95:64](32) + pie_care[63:32] */
            /* pie_care[63:32](data reg 8) */
            temp32 = swapl32(data[offset + 8u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + pie_care[95:64](32) + pie_care[63:32](32) + pie_care[31:0](32) */
            /* pie_care[31:0](data reg 7) */
            temp32 = swapl32(data[offset + 7u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM0 data & mask CRC32 value */
        *tcam0CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam0, (uint32)sizeof(tcam0)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_pie_tcam1_data_handler(const uint32 *data, uint32 offset, uint8 entryValid, uint32 *tcam1CurCrc)
{
    /* tcam1: crc(32) + valid(7'b0 + 1) + pie_data[191:160](32) + pie_data[159:128](32) + pie_data[127:96](32) = 136 bits (8bits * 17) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam1[17] */
        uint8  tcam1[17]  = {0};
        uint32 tcam1Index = 0u;
        uint32 temp32     = 0u;

        /* Init TCAM1 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam1, 0, sizeof(tcam1)));
        tcam1Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)tcam1CurCrc, sizeof(*tcam1CurCrc)));
        tcam1Index += (uint32)sizeof(*tcam1CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], &entryValid, sizeof(entryValid)));
            tcam1Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + pie_data[191:160](32) */
            /* pie_data[191:160](data reg 5), pie_care[191:160](data reg 12) */
            temp32 = swapl32(data[offset + 5u] & data[offset + 12u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + pie_data[191:160](32) + pie_data[158:128](32) */
            /* pie_data[158:128](data reg 4), pie_care[158:128](data reg 11) */
            temp32 = swapl32(data[offset + 4u] & data[offset + 11u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + pie_data[191:160](32) + pie_data[158:128](32) + pie_data[127:96](32) */
            /* pie_data[127:96](data reg 3), pie_care[127:96](data reg 10) */
            temp32 = swapl32(data[offset + 3u] & data[offset + 10u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM1 data CRC32 value */
        *tcam1CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam1, (uint32)sizeof(tcam1)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_pie_tcam1_mask_handler(const uint32 *data, uint32 offset, uint8 entryValid, uint32 *tcam1CurCrc)
{
    /* tcam1: crc(32) + valid(7'b0 + 1) + pie_data[191:160](32) + pie_data[159:128](32) + pie_data[127:96](32) = 136 bits (8bits * 17) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam1[17] */
        uint8  tcam1[17]  = {0};
        uint32 tcam1Index = 0u;
        uint32 temp32     = 0u;

        /* Init TCAM1 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam1, 0, sizeof(tcam1)));
        tcam1Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)tcam1CurCrc, sizeof(*tcam1CurCrc)));
        tcam1Index += (uint32)sizeof(*tcam1CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], &entryValid, sizeof(entryValid)));
            tcam1Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + pie_care[191:160](32) */
            /* pie_care[191:160](data reg 12) */
            temp32 = swapl32(data[offset + 12u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + pie_care[191:160](32) + pie_care[158:128] */
            /* pie_care[158:128](data reg 11) */
            temp32 = swapl32(data[offset + 11u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + pie_care[191:160](32) + pie_care[158:128](32) + pie_care[127:96](32) */
            /* pie_care[127:96](data reg 10) */
            temp32 = swapl32(data[offset + 10u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM1 data & mask CRC32 value */
        *tcam1CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam1, (uint32)sizeof(tcam1)));
    } while(0u == 1u);
    return ret;
}

RtkApiRet dva_drv_tcam_pie_crc_calculate(const uint32 *data, uint32 entryNum, const dva_drv_TcamPieCrcVal_t *curCrc, dva_drv_TcamPieCrcVal_t *goldenCrc)
{
    /* Initialize the return value to RT_ERR_OK */
    RtkApiRet ret = RT_ERR_OK;

    /* Initialize current CRC values for TCAM0 and TCAM1 */
    uint32 tcam0CurCrc = swapl32(curCrc->val[0]);
    uint32 tcam1CurCrc = swapl32(curCrc->val[1]);

    /* Initialize counter, offset, and entry valid flag */
    uint32 counter    = 0u;
    uint32 offset     = 0u;
    uint8  entryValid = 0u;

    /* Get the number of data registers */
    uint32 dataReg = dva_table_list[DVA_PIE_TABLE].datareg_num;

    /* Loop while counter is less than entryNum */
    while(counter < entryNum)
    {
        /* Get the entry valid flag */
        entryValid = (uint8)(data[offset + 6u] & 0x1u);

        /* Handle TCAM0 data */
        ret = dva_drv_pie_tcam0_data_handler(data, offset, entryValid, &tcam0CurCrc);

        /* Handle TCAM0 mask */
        CHK_FUN_RET(ret, dva_drv_pie_tcam0_mask_handler(data, offset, entryValid, &tcam0CurCrc));

        /* Handle TCAM1 data */
        CHK_FUN_RET(ret, dva_drv_pie_tcam1_data_handler(data, offset, entryValid, &tcam1CurCrc));

        /* Handle TCAM1 mask */
        CHK_FUN_RET(ret, dva_drv_pie_tcam1_mask_handler(data, offset, entryValid, &tcam1CurCrc));

        if(RT_ERR_OK != ret)
        {
            break;
        }

        /* Increment the counter */
        counter++;

        /* Update the offset */
        offset += dataReg;
    }

    /* If no errors, assign the calculated CRC values to goldenCrc */
    if(RT_ERR_OK == ret)
    {
        goldenCrc->val[0] = swapl32(tcam0CurCrc);
        goldenCrc->val[1] = swapl32(tcam1CurCrc);
    }

    /* Return the result */
    return ret;
}

RtkApiRet dva_drv_tcam_get_pie_table_crc(uint32 unit, uint32 block, dva_drv_TcamEntryInfo *updateEntry, dva_drv_TcamPieCrcVal_t *crc)
{
    dva_table_list_t        tableType                  = DVA_PIE_TABLE;
    uint32                  val[DVA_TCAM_PIE_DATA_REG] = {0};
    uint32                  lowerBoundAddr             = 0u;
    uint32                  upperBoundAddr             = 0u;
    dva_drv_TcamPieCrcVal_t curCrc                     = {.val = {~0u, ~0u}};
    dva_drv_TcamPieCrcVal_t goldenCrc                  = {.val = {0u, 0u}};
    uint32                  i                          = 0u;
    RtkApiRet               ret                        = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(block > DVA_TCAM_PIE_BLOCK_BOUND)
        {
            ret = -(RT_ERR_TCAM_PIE_BLOCK);
            continue;
        }

        if(NULL == updateEntry)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(NULL == crc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        lowerBoundAddr = block * DVA_TCAM_PIE_BLOCK_ENTRY_NUM;
        upperBoundAddr = lowerBoundAddr + DVA_TCAM_PIE_BLOCK_ENTRY_NUM - 1u;

        for(i = lowerBoundAddr; i <= upperBoundAddr; i++)
        {
            if(RT_ERR_OK != dva_drv_acl_internal_rule_id_check(unit, i))
            {
                continue;
            }

            if((i == updateEntry->addr) && (1u == updateEntry->valid))
            {
                ret = dva_drv_memcpy(&val[0], updateEntry->data, sizeof(val));
            }
            else
            {
                ret = dva_table_read(unit, tableType, i, &val[0]);
            }

            CHK_FUN_RET(ret, dva_drv_tcam_pie_crc_calculate(&val[0], 1u, &curCrc, &goldenCrc));
            CHK_FUN_RET(ret, dva_drv_memcpy(&curCrc, &goldenCrc, sizeof(goldenCrc)));
            if(RT_ERR_OK != ret)
            {
                break;
            }
        }
        if(RT_ERR_OK == ret)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(crc, &goldenCrc, sizeof(goldenCrc)));
        }
    } while(0u == 1u);

    return ret;
}

/******************* Netroute *******************/
static RtkApiRet dva_drv_netroute_tcam0_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam0CurCrc)
{
    /* tcam0: crc(32) + valid(7'b0 + 1) + sip[31:0](32) + sip[63:32](32) + sip[95:64](32) + sip[127:96](32) = 168 bits (8bits * 21) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam0[21] */
        uint8  tcam0[21]  = {0};
        uint32 tcam0Index = 0u;
        uint32 temp32     = 0u;

        /* Init TCAM0 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam0, 0, sizeof(tcam0)));
        tcam0Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)tcam0CurCrc, sizeof(*tcam0CurCrc)));
        tcam0Index += (uint32)sizeof(*tcam0CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &entryValid, sizeof(entryValid)));
            tcam0Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + sip[127:96](32) */
            /* sip[127:96](data reg 3), sip[127:96]_care(data reg 15) */
            temp32 = swapl32(data[offset + 3u] & data[offset + 15u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + sip[127:96](32) + sip[95:64](32) */
            /* sip[95:64](data reg 2), sip[95:64]_care(data reg 14) */
            temp32 = swapl32(data[offset + 2u] & data[offset + 14u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + sip[127:96](32) + sip[95:64](32) + sip[63:32](32) */
            /* sip[63:32](data reg 1), sip[63:32]_care(data reg 13) */
            temp32 = swapl32(data[offset + 1u] & data[offset + 13u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + sip[127:96](32) + sip[95:64](32) + sip[63:32](32) + sip[31:0](32) */
            /* sip[31:0](data reg 0), sip[31:0]_care(data reg 12) */
            temp32 = swapl32(data[offset + 0u] & data[offset + 12u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM0 data CRC32 value */
        *tcam0CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam0, (uint32)sizeof(tcam0)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam0_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam0CurCrc)
{
    /* tcam0: crc(32) + valid(7'b0 + 1) + sip[31:0](32) + sip[63:32](32) + sip[95:64](32) + sip[127:96](32) = 168 bits (8bits * 21) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam0[21] */
        uint8  tcam0[21]  = {0};
        uint32 tcam0Index = 0u;
        uint32 temp32     = 0u;

        /* Init TCAM0 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam0, 0, sizeof(tcam0)));
        tcam0Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)tcam0CurCrc, sizeof(*tcam0CurCrc)));
        tcam0Index += (uint32)sizeof(*tcam0CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &entryValid, sizeof(entryValid)));
            tcam0Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + sip[127:96]_care(32) */
            /* sip[127:96]_care(data reg 15) */
            temp32 = swapl32(data[offset + 15u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + sip[127:96]_care(32) + sip[95:64]_care(32) */
            /* sip[95:64]_care(data reg 14) */
            temp32 = swapl32(data[offset + 14u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + sip[127:96]_care(32) + sip[95:64]_care(32) + sip[63:32]_care(32) */
            /* sip[63:32]_care(data reg 13) */
            temp32 = swapl32(data[offset + 13u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + sip[127:96]_care(32) + sip[95:64]_care(32) + sip[63:32]_care(32) + sip[31:0]_care(32) */
            /* sip[31:0]_care(data reg 12) */
            temp32 = swapl32(data[offset + 12u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM0 data & mask CRC32 value */
        *tcam0CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam0, (uint32)sizeof(tcam0)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam1_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam1CurCrc)
{
    /* tcam1: crc(32) + valid(7'b0 + 1) + dip[31:0](32) + dip[63:32](32) + dip[95:64](32) + dip[127:96](32) = 168 bits (8bits * 21) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam1[21] */
        uint8  tcam1[21]  = {0};
        uint32 tcam1Index = 0u;
        uint32 temp32     = 0u;

        /* Init TCAM1 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam1, 0, sizeof(tcam1)));
        tcam1Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)tcam1CurCrc, sizeof(*tcam1CurCrc)));
        tcam1Index += (uint32)sizeof(*tcam1CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], &entryValid, sizeof(entryValid)));
            tcam1Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + dip[127:96](32) */
            /* dip[127:96](data reg 8), dip[127:96]_care(data reg 19) */
            temp32 = swapl32(data[offset + 8u] & data[offset + 19u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + dip[127:96](32) + dip[95:64](32) */
            /* dip[95:64](data reg 6), dip[95:64]_care(data reg 18) */
            temp32 = swapl32(data[offset + 6u] & data[offset + 18u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + dip[127:96](32) + dip[95:64](32) + dip[63:32](32) */
            /* dip[63:32](data reg 5), dip[63:32]_care(data reg 17) */
            temp32 = swapl32(data[offset + 5u] & data[offset + 17u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + dip[127:96](32) + dip[95:64](32) + dip[63:32](32) + dip[31:0](32) */
            /* dip[31:0](data reg 4), dip[31:0]_care(data reg 16) */
            temp32 = swapl32(data[offset + 4u] & data[offset + 16u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM1 data CRC32 value */
        *tcam1CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam1, (uint32)sizeof(tcam1)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam1_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam1CurCrc)
{
    /* tcam1: crc(32) + valid(7'b0 + 1) + dip[31:0](32) + dip[63:32](32) + dip[95:64](32) + dip[127:96](32) = 168 bits (8bits * 21) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam1[21] */
        uint8  tcam1[21]  = {0};
        uint32 tcam1Index = 0u;
        uint32 temp32     = 0u;

        /* Init TCAM1 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam1, 0, sizeof(tcam1)));
        tcam1Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)tcam1CurCrc, sizeof(*tcam1CurCrc)));
        tcam1Index += (uint32)sizeof(*tcam1CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], &entryValid, sizeof(entryValid)));
            tcam1Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + dip[127:96]_care(32) */
            /* dip[127:96]_care(data reg 19) */
            temp32 = swapl32(data[offset + 19u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + dip[127:96]_care(32) + dip[95:64]_care(32) */
            /* dip[95:64]_care(data reg 18) */
            temp32 = swapl32(data[offset + 18u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + dip[127:96]_care(32) + dip[95:64]_care(32) + dip[63:32]_care(32) */
            /* dip[63:32]_care(data reg 17) */
            temp32 = swapl32(data[offset + 17u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + dip[127:96]_care(32) + dip[95:64]_care(32) + dip[63:32]_care(32) + dip[31:0]_care(32) */
            /* dip[31:0]_care(data reg 16) */
            temp32 = swapl32(data[offset + 16u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM1 data & mask CRC32 value */
        *tcam1CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam1, (uint32)sizeof(tcam1)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam2_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam2CurCrc)
{
    /* tcam2: crc(32) + valid(7'b0 + 1) + sport(16) = 56 bits (8bits * 7) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam2[7] */
        uint8  tcam2[7]   = {0};
        uint32 tcam2Index = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM2 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam2, 0, sizeof(tcam2)));
        tcam2Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)tcam2CurCrc, sizeof(*tcam2CurCrc)));
        tcam2Index += (uint32)sizeof(*tcam2CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], &entryValid, sizeof(entryValid)));
            tcam2Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + sport(16) */
            /* sport(data reg 9, bit[15:0]), sport_care(data reg 20, bit[15:0]) */
            temp16 = swaps16((uint16)((data[offset + 9u] & data[offset + 20u]) & 0xFFFFu));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM2 data CRC32 value */
        *tcam2CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam2, (uint32)sizeof(tcam2)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam2_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam2CurCrc)
{
    /* tcam2: crc(32) + valid(7'b0 + 1) + sport(16) = 56 bits (8bits * 7) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam2[7] */
        uint8  tcam2[7]   = {0};
        uint32 tcam2Index = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM2 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam2, 0, sizeof(tcam2)));
        tcam2Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)tcam2CurCrc, sizeof(*tcam2CurCrc)));
        tcam2Index += (uint32)sizeof(*tcam2CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], &entryValid, sizeof(entryValid)));
            tcam2Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + sport_care(16) */
            /* sport_care(data reg 20, bit[15:0]) */
            temp16 = swaps16((uint16)(data[offset + 20u] & 0xFFFFu));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM2 data & mask CRC32 value */
        *tcam2CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam2, (uint32)sizeof(tcam2)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam3_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam3CurCrc)
{
    /* tcam3: crc(32) + valid(7'b0 + 1) + dport(16) = 56 bits (8bits * 7) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam3[7] */
        uint8  tcam3[7]   = {0};
        uint32 tcam3Index = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM3 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam3, 0, sizeof(tcam3)));
        tcam3Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)tcam3CurCrc, sizeof(*tcam3CurCrc)));
        tcam3Index += (uint32)sizeof(*tcam3CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], &entryValid, sizeof(entryValid)));
            tcam3Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + dport(16) */
            /* dport(data reg 9, bit[31:16]), dport_care(data reg 20, bit[31:16]) */
            temp16 = swaps16((uint16)(((data[offset + 9u] & data[offset + 20u]) & 0xFFFF0000u) >> 16u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM3 data CRC32 value */
        *tcam3CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam3, (uint32)sizeof(tcam3)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam3_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam3CurCrc)
{
    /* tcam3: crc(32) + valid(7'b0 + 1) + dport(16) = 56 bits (8bits * 7) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam3[7] */
        uint8  tcam3[7]   = {0};
        uint32 tcam3Index = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM3 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam3, 0, sizeof(tcam3)));
        tcam3Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)tcam3CurCrc, sizeof(*tcam3CurCrc)));
        tcam3Index += (uint32)sizeof(*tcam3CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], &entryValid, sizeof(entryValid)));
            tcam3Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + dport_care(16) */
            /* dport_care(data reg 20, bit[31:16]) */
            temp16 = swaps16((uint16)((data[offset + 20u] & 0xFFFF0000u) >> 16u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM3 data & mask CRC32 value */
        *tcam3CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam3, (uint32)sizeof(tcam3)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam4_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam4CurCrc)
{
    /* tcam4: crc(32) + valid(7'b0 + 1) + reserved(4) + ivid(12) = 56 bits (8bits * 7) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam4[7] */
        uint8  tcam4[7]   = {0};
        uint32 tcam4Index = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM4 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam4, 0, sizeof(tcam4)));
        tcam4Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam4[tcam4Index], (uint8 *)tcam4CurCrc, sizeof(*tcam4CurCrc)));
        tcam4Index += (uint32)sizeof(*tcam4CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam4[tcam4Index], &entryValid, sizeof(entryValid)));
            tcam4Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + reserved(4) + ivid(12) */
            /* ivid(data reg 11, bit[20:9]), ivid_care(data reg 22, bit[20:9]) */
            temp16 = swaps16((uint16)(((data[offset + 11u] & data[offset + 22u]) & 0x1FFE00u) >> 9u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam4[tcam4Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM4 data CRC32 value */
        *tcam4CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam4, (uint32)sizeof(tcam4)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam4_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam4CurCrc)
{
    /* tcam4: crc(32) + valid(7'b0 + 1) + reserved(4) + ivid(12) = 56 bits (8bits * 7) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam4[7] */
        uint8  tcam4[7]   = {0};
        uint32 tcam4Index = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM4 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam4, 0, sizeof(tcam4)));
        tcam4Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam4[tcam4Index], (uint8 *)tcam4CurCrc, sizeof(*tcam4CurCrc)));
        tcam4Index += (uint32)sizeof(*tcam4CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam4[tcam4Index], &entryValid, sizeof(entryValid)));
            tcam4Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + reserved(4) + ivid_care(12) */
            /* ivid_care(data reg 22, bit[20:9]) */
            temp16 = swaps16((uint16)((data[offset + 22u] & 0x1FFE00u) >> 9u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam4[tcam4Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM4 data & mask CRC32 value */
        *tcam4CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam4, (uint32)sizeof(tcam4)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam5_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam5CurCrc)
{
    /* tcam5: crc(32) + valid(7'b0 + 1) + reserved(1) + dport_fmt(2) + sport_fmt(2) + type(3) = 48 bits (8bits * 6) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam5[6] */
        uint8  tcam5[6]   = {0};
        uint32 tcam5Index = 0u;
        uint8  temp8      = 0u;

        /* Init TCAM5 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam5, 0, sizeof(tcam5)));
        tcam5Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam5[tcam5Index], (uint8 *)tcam5CurCrc, sizeof(*tcam5CurCrc)));
        tcam5Index += (uint32)sizeof(*tcam5CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam5[tcam5Index], &entryValid, sizeof(entryValid)));
            tcam5Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + reserved(1) + dport_fmt(2) + sport_fmt(2) + type(3) */

            /* dport_fmt(data reg 10, bit[23:22]), dport_fmt_care(data reg 21, bit[23:22]) */
            temp8 = (uint8)((((data[offset + 10u] & data[offset + 21u]) & 0xC00000u) >> 22u) << 5u);

            /* sport_fmt(data reg 10, bit[21:20]), sport_fmt_care(data reg 21, bit[21:20]) */
            temp8 |= (uint8)((((data[offset + 10u] & data[offset + 21u]) & 0x300000u) >> 20u) << 3u);

            /* type(data reg 10, bit[3:1]), type_care(data reg 21, bit[3:1]) */
            temp8 |= (uint8)(((data[offset + 10u] & data[offset + 21u]) & 0xEu) >> 1u);

            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam5[tcam5Index], &temp8, sizeof(temp8)));
        }

        /* Calculate TCAM5 data CRC32 value */
        *tcam5CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam5, (uint32)sizeof(tcam5)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam5_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam5CurCrc)
{
    /* tcam5: crc(32) + valid(7'b0 + 1) + reserved(1) + dport_fmt(2) + sport_fmt(2) + type(3) = 48 bits (8bits * 6) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam5[6] */
        uint8  tcam5[6]   = {0};
        uint32 tcam5Index = 0u;
        uint8  temp8      = 0u;

        /* Init TCAM5 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam5, 0, sizeof(tcam5)));
        tcam5Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam5[tcam5Index], (uint8 *)tcam5CurCrc, sizeof(*tcam5CurCrc)));
        tcam5Index += (uint32)sizeof(*tcam5CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam5[tcam5Index], &entryValid, sizeof(entryValid)));
            tcam5Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + reserved(1) + dport_fmt_care(2) + sport_fmt_care(2) + type_care(3) */

            /* dport_fmt_care(data reg 21, bit[23:22]) */
            temp8 = (uint8)(((data[offset + 21u] & 0xC00000u) >> 22u) << 5u);

            /* sport_fmt_care(data reg 21, bit[21:20]) */
            temp8 |= (uint8)(((data[offset + 21u] & 0x300000u) >> 20u) << 3u);

            /* type_care(data reg 21, bit[3:1]) */
            temp8 |= (uint8)((data[offset + 21u] & 0xEu) >> 1u);

            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam5[tcam5Index], &temp8, sizeof(temp8)));
        }

        /* Calculate TCAM5 data & mask CRC32 value */
        *tcam5CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam5, (uint32)sizeof(tcam5)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_netroute_tcam0_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam0CurCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Handle TCAM0 data */
    ret = dva_drv_netroute_tcam0_data_handler(entryValid, data, offset, tcam0CurCrc);
    /* Handle TCAM0 mask */
    CHK_FUN_RET(ret, dva_drv_netroute_tcam0_mask_handler(entryValid, data, offset, tcam0CurCrc));

    return ret;
}

static RtkApiRet dva_drv_netroute_tcam1_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam1CurCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Handle TCAM1 data */
    ret = dva_drv_netroute_tcam1_data_handler(entryValid, data, offset, tcam1CurCrc);
    /* Handle TCAM1 mask */
    CHK_FUN_RET(ret, dva_drv_netroute_tcam1_mask_handler(entryValid, data, offset, tcam1CurCrc));

    return ret;
}

static RtkApiRet dva_drv_netroute_tcam2_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam2CurCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Handle TCAM2 data */
    ret = dva_drv_netroute_tcam2_data_handler(entryValid, data, offset, tcam2CurCrc);
    /* Handle TCAM2 mask */
    CHK_FUN_RET(ret, dva_drv_netroute_tcam2_mask_handler(entryValid, data, offset, tcam2CurCrc));

    return ret;
}

static RtkApiRet dva_drv_netroute_tcam3_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam3CurCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Handle TCAM3 data */
    ret = dva_drv_netroute_tcam3_data_handler(entryValid, data, offset, tcam3CurCrc);
    /* Handle TCAM3 mask */
    CHK_FUN_RET(ret, dva_drv_netroute_tcam3_mask_handler(entryValid, data, offset, tcam3CurCrc));

    return ret;
}

static RtkApiRet dva_drv_netroute_tcam4_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam4CurCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Handle TCAM4 data */
    ret = dva_drv_netroute_tcam4_data_handler(entryValid, data, offset, tcam4CurCrc);
    /* Handle TCAM4 mask */
    CHK_FUN_RET(ret, dva_drv_netroute_tcam4_mask_handler(entryValid, data, offset, tcam4CurCrc));

    return ret;
}

static RtkApiRet dva_drv_netroute_tcam5_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam5CurCrc)
{
    RtkApiRet ret = RT_ERR_OK;

    /* Handle TCAM5 data */
    ret = dva_drv_netroute_tcam5_data_handler(entryValid, data, offset, tcam5CurCrc);
    /* Handle TCAM5 mask */
    CHK_FUN_RET(ret, dva_drv_netroute_tcam5_mask_handler(entryValid, data, offset, tcam5CurCrc));

    return ret;
}

RtkApiRet dva_drv_tcam_netroute_crc_calculate(const uint32 *data, uint32 entryNum, const dva_drv_TcamNetRouteCrcVal_t *curCrc, dva_drv_TcamNetRouteCrcVal_t *goldenCrc)
{
    /* Initialize the return value to RT_ERR_OK */
    RtkApiRet ret = RT_ERR_OK;

    /* Initialize current CRC values for TCAM0 to TCAM5 */
    uint32 tcam0CurCrc = swapl32(curCrc->val[0]);
    uint32 tcam1CurCrc = swapl32(curCrc->val[1]);
    uint32 tcam2CurCrc = swapl32(curCrc->val[2]);
    uint32 tcam3CurCrc = swapl32(curCrc->val[3]);
    uint32 tcam4CurCrc = swapl32(curCrc->val[4]);
    uint32 tcam5CurCrc = swapl32(curCrc->val[5]);

    /* Initialize counter, offset, and entry valid flag */
    uint32 counter    = 0u;
    uint32 offset     = 0u;
    uint8  entryValid = 0u;

    /* Get the number of data registers */
    uint32 dataReg = dva_table_list[DVA_NETWORK_ROUTE_TABLE].datareg_num;

    /* Loop while counter is less than entryNum */
    while(counter < entryNum)
    {
        /* Get valid(data reg 7) */
        entryValid = (uint8)(data[offset + 7u] & 0x1u);

        ret = dva_drv_netroute_tcam0_handler(entryValid, data, offset, &tcam0CurCrc);

        /* Handle TCAM1 */
        CHK_FUN_RET(ret, dva_drv_netroute_tcam1_handler(entryValid, data, offset, &tcam1CurCrc));

        /* Handle TCAM2 */
        CHK_FUN_RET(ret, dva_drv_netroute_tcam2_handler(entryValid, data, offset, &tcam2CurCrc));

        /* Handle TCAM3 */
        CHK_FUN_RET(ret, dva_drv_netroute_tcam3_handler(entryValid, data, offset, &tcam3CurCrc));

        /* Handle TCAM4 */
        CHK_FUN_RET(ret, dva_drv_netroute_tcam4_handler(entryValid, data, offset, &tcam4CurCrc));

        /* Handle TCAM5 */
        CHK_FUN_RET(ret, dva_drv_netroute_tcam5_handler(entryValid, data, offset, &tcam5CurCrc));

        if(RT_ERR_OK != ret)
        {
            break;
        }

        /* Increment the counter */
        counter++;

        /* Update the offset */
        offset += dataReg;
    }

    /* If no errors, assign the calculated CRC values to goldenCrc */
    if(RT_ERR_OK == ret)
    {
        goldenCrc->val[0] = swapl32(tcam0CurCrc);
        goldenCrc->val[1] = swapl32(tcam1CurCrc);
        goldenCrc->val[2] = swapl32(tcam2CurCrc);
        goldenCrc->val[3] = swapl32(tcam3CurCrc);
        goldenCrc->val[4] = swapl32(tcam4CurCrc);
        goldenCrc->val[5] = swapl32(tcam5CurCrc);
    }

    /* Return the result */
    return ret;
}

RtkApiRet dva_drv_tcam_get_netroute_table_crc(uint32 unit, dva_drv_TcamEntryInfo *updateEntry, dva_drv_TcamNetRouteCrcVal_t *crc)
{
    dva_table_list_t             tableType                       = DVA_NETWORK_ROUTE_TABLE;
    uint32                       tableSize                       = dva_table_list[tableType].size;
    uint32                       val[DVA_TCAM_NETROUTE_DATA_REG] = {0};
    dva_drv_TcamNetRouteCrcVal_t curCrc                          = {.val = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u}};
    dva_drv_TcamNetRouteCrcVal_t goldenCrc                       = {.val = {0u, 0u, 0u, 0u, 0u, 0u}};
    uint32                       i                               = 0u;
    RtkApiRet                    ret                             = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == updateEntry)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(NULL == crc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        for(i = 0u; i < tableSize; i++)
        {
            if((i == updateEntry->addr) && (1u == updateEntry->valid))
            {
                ret = dva_drv_memcpy(&val[0], updateEntry->data, sizeof(val));
            }
            else
            {
                ret = dva_table_read(unit, tableType, i, &val[0]);
            }

            CHK_FUN_RET(ret, dva_drv_tcam_netroute_crc_calculate(&val[0], 1u, &curCrc, &goldenCrc));
            CHK_FUN_RET(ret, dva_drv_memcpy(&curCrc, &goldenCrc, sizeof(goldenCrc)));

            if(RT_ERR_OK != ret)
            {
                break;
            }
        }

        if(RT_ERR_OK == ret)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(crc, &goldenCrc, sizeof(goldenCrc)));
        }
    } while(0u == 1u);

    return ret;
}

/******************* Ingress Interface *******************/
static RtkApiRet dva_drv_ingif_tcam0_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam0CurCrc)
{
    /* tcam0: crc(32) + valid(7'b0 + 1) + mac47_16(32) + mac15_0(16) = 88 bits (8bits * 11) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam0[11] */
        uint8  tcam0[11]  = {0};
        uint32 tcam0Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM0 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam0, 0, sizeof(tcam0)));
        tcam0Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)tcam0CurCrc, sizeof(*tcam0CurCrc)));
        tcam0Index += (uint32)sizeof(*tcam0CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &entryValid, sizeof(entryValid)));
            tcam0Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + mac47_16(32) */
            /* mac47_16(data reg 1), mac47_16_care(data reg 7) */
            temp32 = swapl32(data[offset + 1u] & data[offset + 7u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + mac47_16(32) + mac15_0(16) */
            /* mac15_0(data reg 0, bit[31:16]), mac15_0_care(data reg 5, bit[31:16]) */
            temp16 = swaps16((uint16)(((data[offset + 0u] & data[offset + 5u]) & 0xFFFF0000u) >> 16u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM0 data CRC32 value */
        *tcam0CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam0, (uint32)sizeof(tcam0)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_ingif_tcam0_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam0CurCrc)
{
    /* tcam0: crc(32) + valid(7'b0 + 1) + mac47_16(32) + mac15_0(16) = 88 bits (8bits * 11) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam0[11] */
        uint8  tcam0[11]  = {0};
        uint32 tcam0Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM0 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam0, 0, sizeof(tcam0)));
        tcam0Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)tcam0CurCrc, sizeof(*tcam0CurCrc)));
        tcam0Index += (uint32)sizeof(*tcam0CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &entryValid, sizeof(entryValid)));
            tcam0Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + mac47_16_care(32) */
            /* mac47_16_care(data reg 7) */
            temp32 = swapl32(data[offset + 7u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + mac47_16_care(32) + mac15_0_care(16) */
            /* mac15_0_care(data reg 5, bit[31:16]) */
            temp16 = swaps16((uint16)((data[offset + 5u] & 0xFFFF0000u) >> 16u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM0 data & mask CRC32 value */
        *tcam0CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam0, (uint32)sizeof(tcam0)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_ingif_tcam1_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam1CurCrc)
{
    /* tcam1: crc(32) + valid(7'b0 + 1) + dport(16) + sport(16) + ivid(12) + portid(4) = 88 bits (8bits * 11) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam1[11] */
        uint8  tcam1[11]  = {0};
        uint32 tcam1Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM1 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam1, 0, sizeof(tcam1)));
        tcam1Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)tcam1CurCrc, sizeof(*tcam1CurCrc)));
        tcam1Index += (uint32)sizeof(*tcam1CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], &entryValid, sizeof(entryValid)));
            tcam1Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + dport(16) + sport(16) */
            /* dport_sport(data reg 2), dport_sport_care(data reg 8) */
            temp32 = swapl32(data[offset + 2u] & data[offset + 8u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + dport(16) + sport(16) + ivid(12) + portid(4) */
            /* ivid_portid(data reg 0, bit[15:0]), ivid_portid_care(data reg 5, bit[15:0]) */
            temp16 = swaps16((uint16)((data[offset + 0u] & data[offset + 5u]) & 0xFFFFu));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM1 data CRC32 value */
        *tcam1CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam1, (uint32)sizeof(tcam1)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_ingif_tcam1_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam1CurCrc)
{
    /* tcam1: crc(32) + valid(7'b0 + 1) + dport(16) + sport(16) + ivid(12) + portid(4) = 88 bits (8bits * 11) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam1[11] */
        uint8  tcam1[11]  = {0};
        uint32 tcam1Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;

        /* Init TCAM1 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam1, 0, sizeof(tcam1)));
        tcam1Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)tcam1CurCrc, sizeof(*tcam1CurCrc)));
        tcam1Index += (uint32)sizeof(*tcam1CurCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], &entryValid, sizeof(entryValid)));
            tcam1Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + dport_care(16) + sport_care(16) */
            /* dport_sport_care(data reg 8) */
            temp32 = swapl32(data[offset + 8u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + dport_care(16) + sport_care(16) + ivid_care(12) + portid_care(4) */
            /* ivid_portid_care(data reg 5, bit[15:0]) */
            temp16 = swaps16((uint16)(data[offset + 5u] & 0xFFFFu));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM1 data & mask CRC32 value */
        *tcam1CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam1, (uint32)sizeof(tcam1)));
    } while(0u == 1u);
    return ret;
}

RtkApiRet dva_drv_tcam_ingif_crc_calculate(const uint32 *data, uint32 entryNum, const dva_drv_TcamIngifCrcVal_t *curCrc, dva_drv_TcamIngifCrcVal_t *goldenCrc)
{
    /* Initialize the return value to RT_ERR_OK */
    RtkApiRet ret = RT_ERR_OK;

    /* Initialize current CRC values for TCAM0 and TCAM1 */
    uint32 tcam0CurCrc = swapl32(curCrc->val[0]);
    uint32 tcam1CurCrc = swapl32(curCrc->val[1]);

    /* Initialize counter, offset, and entry valid flag */
    uint32 counter    = 0u;
    uint32 offset     = 0u;
    uint8  entryValid = 0u;

    /* Get the number of data registers */
    uint32 dataReg = dva_table_list[DVA_INGIF_TABLE].datareg_num;

    /* Loop while counter is less than entryNum */
    while(counter < entryNum)
    {
        /* Get valid(data reg 6) */
        entryValid = (uint8)(data[offset + 6u] & 0x1u);

        /* Handle TCAM0 data */
        ret = dva_drv_ingif_tcam0_data_handler(entryValid, data, offset, &tcam0CurCrc);

        /* Handle TCAM0 mask */
        CHK_FUN_RET(ret, dva_drv_ingif_tcam0_mask_handler(entryValid, data, offset, &tcam0CurCrc));

        /* Handle TCAM1 data */
        CHK_FUN_RET(ret, dva_drv_ingif_tcam1_data_handler(entryValid, data, offset, &tcam1CurCrc));

        /* Handle TCAM1 mask */
        CHK_FUN_RET(ret, dva_drv_ingif_tcam1_mask_handler(entryValid, data, offset, &tcam1CurCrc));

        if(RT_ERR_OK != ret)
        {
            break;
        }

        /* Increment the counter */
        counter++;

        /* Update the offset */
        offset += dataReg;
    }

    /* If no errors, assign the calculated CRC values to goldenCrc */
    if(RT_ERR_OK == ret)
    {
        goldenCrc->val[0] = swapl32(tcam0CurCrc);
        goldenCrc->val[1] = swapl32(tcam1CurCrc);
    }

    /* Return the result */
    return ret;
}

RtkApiRet dva_drv_tcam_get_ingif_table_crc(uint32 unit, dva_drv_TcamEntryInfo *updateEntry, dva_drv_TcamIngifCrcVal_t *crc)
{
    dva_table_list_t          tableType                    = DVA_INGIF_TABLE;
    uint32                    tableSize                    = dva_table_list[tableType].size;
    uint32                    val[DVA_TCAM_INGIF_DATA_REG] = {0};
    dva_drv_TcamIngifCrcVal_t curCrc                       = {.val = {~0u, ~0u}};
    dva_drv_TcamIngifCrcVal_t goldenCrc                    = {.val = {0u, 0u}};
    uint32                    i                            = 0u;
    RtkApiRet                 ret                          = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == updateEntry)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(NULL == crc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        for(i = 0u; i < tableSize; i++)
        {
            if((i == updateEntry->addr) && (1u == updateEntry->valid))
            {
                ret = dva_drv_memcpy(&val[0], updateEntry->data, sizeof(val));
            }
            else
            {
                ret = dva_table_read(unit, tableType, i, &val[0]);
            }

            CHK_FUN_RET(ret, dva_drv_tcam_ingif_crc_calculate(&val[0], 1u, &curCrc, &goldenCrc));
            CHK_FUN_RET(ret, dva_drv_memcpy(&curCrc, &goldenCrc, sizeof(goldenCrc)));

            if(RT_ERR_OK != ret)
            {
                break;
            }
        }

        if(RT_ERR_OK == ret)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(crc, &goldenCrc, sizeof(goldenCrc)));
        }
    } while(0u == 1u);

    return ret;
}

/******************* Stream Identification *******************/
static RtkApiRet dva_drv_sid_tcam0_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam0CurCrc)
{
    /* tcam0: crc(32) + valid(7'b0 + 1) + VLANID[6:0](7) + ETYPEinvalid(1) + ETYPE[15:0](16) + SRCMAC[47:16](32) + SRCMAC[15:0](16) + DSTMAC[47:32](16) + DSTMAC[31:0](32) = 160 bits (8bits * 20) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam0[20] */
        uint8  tcam0[20]  = {0};
        uint32 tcam0Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;
        uint8  temp8      = 0u;
        uint32 curCrc     = *tcam0CurCrc;

        /* Init TCAM0 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam0, 0, sizeof(tcam0)));
        tcam0Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&curCrc, sizeof(curCrc)));
        tcam0Index += (uint32)sizeof(curCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &entryValid, sizeof(entryValid)));
            tcam0Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0](7) + ETYPEinvalid(1) */
            /* VLANID[6:0]_ETYPEinvalid(data reg 3, bit[23:16]), VLANID[6:0]_ETYPEinvalid_care(data reg 19, bit[23:16]) */
            temp8 = (uint8)(((data[offset + 3u] & data[offset + 19u]) & 0xFF0000u) >> 16u);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &temp8, sizeof(temp8)));
            tcam0Index += (uint32)sizeof(temp8);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0](7) + ETYPEinvalid(1) + ETYPE[15:0](16) */
            /* ETYPE[15:0](data reg 3, bit[15:0]), ETYPE[15:0]_care(data reg 19, bit[15:0]) */
            temp16 = swaps16((uint16)((data[offset + 3u] & data[offset + 19u]) & 0xFFFFu));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp16, sizeof(temp16)));
            tcam0Index += (uint32)sizeof(temp16);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0](7) + ETYPEinvalid(1) + ETYPE[15:0](16) + SRCMAC[47:16](32) */
            /* SRCMAC[47:16](data reg 2), SRCMAC[47:16]_care(data reg 18) */
            temp32 = swapl32(data[offset + 2u] & data[offset + 18u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0](7) + ETYPEinvalid(1) + ETYPE[15:0](16) + SRCMAC[47:16](32) + SRCMAC[15:0](16) + DSTMAC[47:32](16) */
            /* SRCMAC[15:0]_DSTMAC[47:32](data reg 1), SRCMAC[15:0]_DSTMAC[47:32]_care(data reg 17) */
            temp32 = swapl32(data[offset + 1u] & data[offset + 17u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0](7) + ETYPEinvalid(1) + ETYPE[15:0](16) + SRCMAC[47:16](32) + SRCMAC[15:0](16) + DSTMAC[47:32](16) + DSTMAC[31:0](32) */
            /* DSTMAC[31:0](data reg 0), DSTMAC[31:0]_care(data reg 16) */
            temp32 = swapl32(data[offset + 0u] & data[offset + 16u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM0 data CRC32 value */
        *tcam0CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam0, (uint32)sizeof(tcam0)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sid_tcam0_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam0CurCrc)
{
    /* tcam0: crc(32) + valid(7'b0 + 1) + VLANID[6:0](7) + ETYPEinvalid(1) + ETYPE[15:0](16) + SRCMAC[47:16](32) + SRCMAC[15:0](16) + DSTMAC[47:32](16) + DSTMAC[31:0](32) = 160 bits (8bits * 20) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam0[20] */
        uint8  tcam0[20]  = {0};
        uint32 tcam0Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;
        uint8  temp8      = 0u;
        uint32 curCrc     = *tcam0CurCrc;

        /* Init TCAM0 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam0, 0, sizeof(tcam0)));
        tcam0Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&curCrc, sizeof(curCrc)));
        tcam0Index += (uint32)sizeof(curCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &entryValid, sizeof(entryValid)));
            tcam0Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0]_care(7) + ETYPEinvalid_care(1) */
            /* VLANID[6:0]_ETYPEinvalid_care(data reg 19, bit[23:16]) */
            temp8 = (uint8)((data[offset + 19u] & 0xFF0000u) >> 16u);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], &temp8, sizeof(temp8)));
            tcam0Index += (uint32)sizeof(temp8);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0]_care(7) + ETYPEinvalid_care(1) + ETYPE[15:0]_care(16) */
            /* ETYPE[15:0]_care(data reg 19, bit[15:0]) */
            temp16 = swaps16((uint16)(data[offset + 19u] & 0xFFFFu));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp16, sizeof(temp16)));
            tcam0Index += (uint32)sizeof(temp16);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0]_care(7) + ETYPEinvalid_care(1) + ETYPE[15:0]_care(16) + SRCMAC[47:16]_care(32) */
            /* SRCMAC[47:16](data reg 2), SRCMAC[47:16]_care(data reg 18) */
            temp32 = swapl32(data[offset + 18u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0]_care(7) + ETYPEinvalid_care(1) + ETYPE[15:0]_care(16) + SRCMAC[47:16]_care(32) + SRCMAC[15:0]_care(16) + DSTMAC[47:32]_care(16) */
            /* SRCMAC[15:0]_DSTMAC[47:32]_care(data reg 17) */
            temp32 = swapl32(data[offset + 17u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam0Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + VLANID[6:0]_care(7) + ETYPEinvalid_care(1) + ETYPE[15:0]_care(16) + SRCMAC[47:16]_care(32) + SRCMAC[15:0]_care(16) + DSTMAC[47:32]_care(16) + DSTMAC[31:0]_care(32) */
            /* DSTMAC[31:0]_care(data reg 16) */
            temp32 = swapl32(data[offset + 16u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam0[tcam0Index], (uint8 *)&temp32, sizeof(temp32)));
        }

        /* Calculate TCAM0 data & mask CRC32 value */
        *tcam0CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam0, (uint32)sizeof(tcam0)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sid_tcam1_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam1CurCrc)
{
    /* tcam1: crc(32) + valid(7'b0 + 1) + SRCIP[113:98](16) + SRCIP[97:66](32) + SRCIP[65:34](32) + SRCIP[33:2](32) + SRCIP[1:0](2) + VLANIDinvalid(1) + VLANID[11:7](5) = 160 bits (8bits * 20) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam1[20] */
        uint8  tcam1[20]  = {0};
        uint32 tcam1Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;
        uint8  temp8      = 0u;
        uint32 curCrc     = *tcam1CurCrc;

        /* Init TCAM1 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam1, 0, sizeof(tcam1)));
        tcam1Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&curCrc, sizeof(curCrc)));
        tcam1Index += (uint32)sizeof(curCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], &entryValid, sizeof(entryValid)));
            tcam1Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98](16) */
            /* SRCIP[113:98](data reg 8), SRCIP[113:98]_care(data reg 23) */
            temp16 = swaps16((uint16)((data[offset + 8u] & data[offset + 23u]) & 0xFFFFu));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp16, sizeof(temp16)));
            tcam1Index += (uint32)sizeof(temp16);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98](16) + SRCIP[97:66](32) */
            /* SRCIP[97:66](data reg 7), SRCIP[97:66]_care(data reg 22) */
            temp32 = swapl32(data[offset + 7u] & data[offset + 22u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98](16) + SRCIP[97:66](32) + SRCIP[65:34](32) */
            /*  SRCIP[65:34](data reg 5), SRCIP[65:34]_care(data reg 21) */
            temp32 = swapl32(data[offset + 5u] & data[offset + 21u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98](16) + SRCIP[97:66](32) + SRCIP[65:34](32) + SRCIP[33:2](32) */
            /* SRCIP[33:2](data reg 4), SRCIP[33:2]_care(data reg 20) */
            temp32 = swapl32(data[offset + 4u] & data[offset + 20u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98](16) + SRCIP[97:66](32) + SRCIP[65:34](32) + SRCIP[33:2](32) + SRCIP[1:0](2) + VLANIDinvalid(1) + VLANID[11:7](5) */
            /* SRCIP[1:0]_VLANIDinvalid_VLANID[11:7](data reg 3, bit[31:24]), SRCIP[1:0]_VLANIDinvalid_VLANID[11:7]_care(data reg 19, bit[31:24]) */
            temp8 = (uint8)(((data[offset + 3u] & data[offset + 19u]) & 0xFF000000u) >> 24u);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp8, sizeof(temp8)));
        }

        /* Calculate TCAM1 data CRC32 value */
        *tcam1CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam1, (uint32)sizeof(tcam1)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sid_tcam1_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam1CurCrc)
{
    /* tcam1: crc(32) + valid(7'b0 + 1) + SRCIP[113:98](16) + SRCIP[97:66](32) + SRCIP[65:34](32) + SRCIP[33:2](32) + SRCIP[1:0](2) + VLANIDinvalid(1) + VLANID[11:7](5) = 160 bits (8bits * 20) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam1[20] */
        uint8  tcam1[20]  = {0};
        uint32 tcam1Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;
        uint8  temp8      = 0u;
        uint32 curCrc     = *tcam1CurCrc;

        /* Init TCAM1 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam1, 0, sizeof(tcam1)));
        tcam1Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&curCrc, sizeof(curCrc)));
        tcam1Index += (uint32)sizeof(curCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], &entryValid, sizeof(entryValid)));
            tcam1Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98]_care(16) */
            /* SRCIP[113:98]_care(data reg 23) */
            temp16 = swaps16((uint16)(data[offset + 23u] & 0xFFFFu));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp16, sizeof(temp16)));
            tcam1Index += (uint32)sizeof(temp16);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98]_care(16) + SRCIP[97:66]_care(32) */
            /* SRCIP[97:66]_care(data reg 22) */
            temp32 = swapl32(data[offset + 22u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98]_care(16) + SRCIP[97:66]_care(32) + SRCIP[65:34]_care(32) */
            /* SRCIP[65:34]_care(data reg 21) */
            temp32 = swapl32(data[offset + 21u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98]_care(16) + SRCIP[97:66]_care(32) + SRCIP[65:34]_care(32) + SRCIP[33:2]_care(32) */
            /* SRCIP[33:2]_care(data reg 20) */
            temp32 = swapl32(data[offset + 20u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam1Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + SRCIP[113:98]_care(16) + SRCIP[97:66]_care(32) + SRCIP[65:34]_care(32) + SRCIP[33:2]_care(32) + SRCIP[1:0]_care(2) + VLANIDinvalid_care(1) + VLANID[11:7]_care(5) */
            /* SRCIP[1:0]_VLANIDinvalid_VLANID[11:7]_care(data reg 19, bit[31:24]) */
            temp8 = (uint8)((data[offset + 19u] & 0xFF000000u) >> 24u);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam1[tcam1Index], (uint8 *)&temp8, sizeof(temp8)));
        }

        /* Calculate TCAM1 data & mask CRC32 value */
        *tcam1CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam1, (uint32)sizeof(tcam1)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sid_tcam2_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam2CurCrc)
{
    /* tcam2: crc(32) + valid(7'b0 + 1) + DSTIP[104:97](8) + DSTIP[96:65](32) + DSTIP[64:33](32) + DSTIP[32:1](32) + DSTIP[0:0](1) + SRCIPinvalid(1) + SRCIP[127:114](14) = 160 bits (8bits * 20) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam2[20] */
        uint8  tcam2[20]  = {0};
        uint32 tcam2Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;
        uint8  temp8      = 0u;
        uint32 curCrc     = *tcam2CurCrc;

        /* Init TCAM2 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam2, 0, sizeof(tcam2)));
        tcam2Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&curCrc, sizeof(curCrc)));
        tcam2Index += (uint32)sizeof(curCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], &entryValid, sizeof(entryValid)));
            tcam2Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97](8) */
            /* DSTIP[104:97](data reg 12, bit[7:0]), DSTIP[104:97]_care(data reg 27, bit[7:0]) */
            temp8 = (uint8)((data[offset + 12u] & data[offset + 27u]) & 0xFFu);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp8, sizeof(temp8)));
            tcam2Index += (uint32)sizeof(temp8);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97](8) + DSTIP[96:65](32) */
            /* DSTIP[96:65](data reg 11), DSTIP[96:65]_care(data reg 26) */
            temp32 = swapl32(data[offset + 11u] & data[offset + 26u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam2Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97](8) + DSTIP[96:65](32) + DSTIP[64:33](32) */
            /* DSTIP[64:33](data reg 10), DSTIP[64:33]_care(data reg 25) */
            temp32 = swapl32(data[offset + 10u] & data[offset + 25u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam2Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97](8) + DSTIP[96:65](32) + DSTIP[64:33](32) + DSTIP[32:1](32) */
            /* DSTIP[32:1](data reg 9), DSTIP[32:1]_care(data reg 24) */
            temp32 = swapl32(data[offset + 9u] & data[offset + 24u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam2Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97](8) + DSTIP[96:65](32) + DSTIP[64:33](32) + DSTIP[32:1](32) + DSTIP[0:0](1) + SRCIPinvalid(1) + SRCIP[127:114](14) */
            /* DSTIP[0:0]_SRCIPinvalid_SRCIP[127:114](data reg 8, bit[31:16]), DSTIP[0:0]_SRCIPinvalid_SRCIP[127:114]_care(data reg 23, bit[31:16]) */
            temp16 = swaps16((uint16)(((data[offset + 8u] & data[offset + 23u]) & 0xFFFF0000u) >> 16u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM2 data CRC32 value */
        *tcam2CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam2, (uint32)sizeof(tcam2)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sid_tcam2_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam2CurCrc)
{
    /* tcam2: crc(32) + valid(7'b0 + 1) + DSTIP[104:97](8) + DSTIP[96:65](32) + DSTIP[64:33](32) + DSTIP[32:1](32) + DSTIP[0:0](1) + SRCIPinvalid(1) + SRCIP[127:114](14) = 160 bits (8bits * 20) */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam2[20] */
        uint8  tcam2[20]  = {0};
        uint32 tcam2Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;
        uint8  temp8      = 0u;
        uint32 curCrc     = *tcam2CurCrc;

        /* Init TCAM2 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam2, 0, sizeof(tcam2)));
        tcam2Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&curCrc, sizeof(curCrc)));
        tcam2Index += (uint32)sizeof(curCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], &entryValid, sizeof(entryValid)));
            tcam2Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97]_care(8) */
            /* DSTIP[104:97]_care(data reg 27, bit[7:0]) */
            temp8 = (uint8)(data[offset + 27u] & 0xFFu);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp8, sizeof(temp8)));
            tcam2Index += (uint32)sizeof(temp8);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97]_care(8) + DSTIP[96:65]_care(32) */
            /* DSTIP[96:65]_care(data reg 26) */
            temp32 = swapl32(data[offset + 26u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam2Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97]_care(8) + DSTIP[96:65]_care(32) + DSTIP[64:33]_care(32) */
            /* DSTIP[64:33]_care(data reg 25) */
            temp32 = swapl32(data[offset + 25u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam2Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97]_care(8) + DSTIP[96:65]_care(32) + DSTIP[64:33]_care(32) + DSTIP[32:1]_care(32) */
            /* DSTIP[32:1]_care(data reg 24) */
            temp32 = swapl32(data[offset + 24u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam2Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + DSTIP[104:97](8) + DSTIP[96:65](32) + DSTIP[64:33](32) + DSTIP[32:1](32) + DSTIP[0:0](1) + SRCIPinvalid(1) + SRCIP[127:114](14) */
            /* DSTIP[0:0]_SRCIPinvalid_SRCIP[127:114]_care(data reg 23, bit[31:16]) */
            temp16 = swaps16((uint16)((data[offset + 23u] & 0xFFFF0000u) >> 16u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam2[tcam2Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM2 data & mask CRC32 value */
        *tcam2CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam2, (uint32)sizeof(tcam2)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sid_tcam3_data_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam3CurCrc)
{
    /*
        tcam3:
        crc(32) + valid(7'b0 + 1) + reserved(32) +
        reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) +
        SRCPORT[15:0](16) + NEXTPROCinvalid(1) + NEXTPROC[7:0](8) + DSCPinvalid(1) + DSCP[5:0](6) +
        DSTIPinvalid(1) + DSTIP[127:121](7) +
        DSTIP[120:105](16)
        = 160 bits (8bits * 20)
    */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam3[20] */
        uint8  tcam3[20]  = {0};
        uint32 tcam3Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;
        uint8  temp8      = 0u;
        uint32 curCrc     = *tcam3CurCrc;

        /* Init TCAM3 data and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam3, 0, sizeof(tcam3)));
        tcam3Index = 0u;

        /* EntryX Data CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&curCrc, sizeof(curCrc)));
        tcam3Index += (uint32)sizeof(curCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], &entryValid, sizeof(entryValid)));
            tcam3Index += (uint32)sizeof(entryValid);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + reserved(32) */
            temp32 = swapl32(data[offset + 15u] & data[offset + 30u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam3Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + reserved(32) + reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) */
            /* data(data reg 14), care(data reg 29) */
            temp32 = swapl32(data[offset + 14u] & data[offset + 29u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam3Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + reserved(32) + reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) + SRCPORT[15:0](16) + NEXTPROCinvalid(1) + NEXTPROC[7:0](8) + DSCPinvalid(1) + DSCP[5:0](6) */
            /* data(data reg 13), care(data reg 28) */
            temp32 = swapl32(data[offset + 13u] & data[offset + 28u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam3Index += (uint32)sizeof(temp32);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + reserved(32) + reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) + SRCPORT[15:0](16) + NEXTPROCinvalid(1) + NEXTPROC[7:0](8) + DSCPinvalid(1) + DSCP[5:0](6) + DSTIPinvalid(1) + DSTIP[127:121](7) */
            /* data(data reg 12), care(data reg 27) */
            temp8 = (uint8)(((data[offset + 12u] & data[offset + 27u]) & 0xFF000000u) >> 24u);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp8, sizeof(temp8)));
            tcam3Index += (uint32)sizeof(temp8);

            /* EntryX Data CRC: crc(32) + valid(7'b0 + 1) + reserved(32) + reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) + SRCPORT[15:0](16) + NEXTPROCinvalid(1) + NEXTPROC[7:0](8) + DSCPinvalid(1) + DSCP[5:0](6) + DSTIPinvalid(1) + DSTIP[127:121](7) + DSTIP[120:105](16) */
            /* data(data reg 12), care(data reg 27) */
            temp16 = swaps16((uint16)(((data[offset + 12u] & data[offset + 27u]) & 0xFFFF00u) >> 8u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM3 data CRC32 value */
        *tcam3CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam3, (uint32)sizeof(tcam3)));
    } while(0u == 1u);
    return ret;
}

static RtkApiRet dva_drv_sid_tcam3_mask_handler(uint8 entryValid, const uint32 *data, uint32 offset, uint32 *tcam3CurCrc)
{
    /*
        tcam3:
        crc(32) + valid(7'b0 + 1) + reserved(32) +
        reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) +
        SRCPORT[15:0](16) + NEXTPROCinvalid(1) + NEXTPROC[7:0](8) + DSCPinvalid(1) + DSCP[5:0](6) +
        DSTIPinvalid(1) + DSTIP[127:121](7) +
        DSTIP[120:105](16)
        = 160 bits (8bits * 20)
    */

    RtkApiRet ret = RT_ERR_OK;
    do
    {
        /* Declare tcam3[20] */
        uint8  tcam3[20]  = {0};
        uint32 tcam3Index = 0u;
        uint32 temp32     = 0u;
        uint16 temp16     = 0u;
        uint8  temp8      = 0u;
        uint32 curCrc     = *tcam3CurCrc;

        /* Init TCAM3 care and index */
        CHK_FUN_CONTINUE(ret, dva_drv_memset(tcam3, 0, sizeof(tcam3)));
        tcam3Index = 0u;

        /* EntryX Care CRC: crc(32) */
        CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&curCrc, sizeof(curCrc)));
        tcam3Index += (uint32)sizeof(curCrc);

        /* Check entryValid value: check if entryValid is equal to 1 */
        if(1u == entryValid)
        {
            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) */
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], &entryValid, sizeof(entryValid)));
            tcam3Index += (uint32)sizeof(entryValid);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + reserved(32) */
            temp32 = swapl32(data[offset + 30u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam3Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + reserved(32) + reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) */
            /* care(data reg 29) */
            temp32 = swapl32(data[offset + 29u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam3Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + reserved(32) + reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) + SRCPORT[15:0](16) + NEXTPROCinvalid(1) + NEXTPROC[7:0](8) + DSCPinvalid(1) + DSCP[5:0](6) */
            /* care(data reg 28) */
            temp32 = swapl32(data[offset + 28u]);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp32, sizeof(temp32)));
            tcam3Index += (uint32)sizeof(temp32);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + reserved(32) + reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) + SRCPORT[15:0](16) + NEXTPROCinvalid(1) + NEXTPROC[7:0](8) + DSCPinvalid(1) + DSCP[5:0](6) + DSTIPinvalid(1) + DSTIP[127:121](7) */
            /* care(data reg 27) */
            temp8 = (uint8)((data[offset + 27u] & 0xFF000000u) >> 24u);
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp8, sizeof(temp8)));
            tcam3Index += (uint32)sizeof(temp8);

            /* EntryX Care CRC: crc(32) + valid(7'b0 + 1) + reserved(32) + reserved(7) + L4PORT_RANGE(4) + VLANPRI(3) + DSTPORTinvalid(1) + DSTPORT[15:0](16) + SRCPORTinvalid(1) + SRCPORT[15:0](16) + NEXTPROCinvalid(1) + NEXTPROC[7:0](8) + DSCPinvalid(1) + DSCP[5:0](6) + DSTIPinvalid(1) + DSTIP[127:121](7) + DSTIP[120:105](16) */
            /* care(data reg 27) */
            temp16 = swaps16((uint16)((data[offset + 27u] & 0xFFFF00u) >> 8u));
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(&tcam3[tcam3Index], (uint8 *)&temp16, sizeof(temp16)));
        }

        /* Calculate TCAM3 data & mask CRC32 value */
        *tcam3CurCrc = swapl32(dva_drv_tcam_crc_32_calculate(tcam3, (uint32)sizeof(tcam3)));
    } while(0u == 1u);
    return ret;
}

RtkApiRet dva_drv_tcam_sid_crc_calculate(const uint32 *data, uint32 entryNum, const dva_drv_TcamSidCrcVal_t *curCrc, dva_drv_TcamSidCrcVal_t *goldenCrc)
{
    /* Initialize the return value to RT_ERR_OK */
    RtkApiRet ret = RT_ERR_OK;

    /* Initialize current CRC values for TCAM0 to TCAM3 */
    uint32 tcam0CurCrc = swapl32(curCrc->val[0]);
    uint32 tcam1CurCrc = swapl32(curCrc->val[1]);
    uint32 tcam2CurCrc = swapl32(curCrc->val[2]);
    uint32 tcam3CurCrc = swapl32(curCrc->val[3]);

    /* Initialize counter, offset, and entry valid flag */
    uint32 counter    = 0u;
    uint32 offset     = 0u;
    uint8  entryValid = 0u;

    /* Get the number of data registers */
    uint32 dataReg = dva_table_list[DVA_STREAM_IDENTIFICATION_TABLE].datareg_num;

    /* Loop while counter is less than entryNum */
    while(counter < entryNum)
    {
        /* Get valid(data reg 6) */
        entryValid = (uint8)(data[offset + 6u] & 0x1u);

        /* Handle TCAM0 data */
        ret = dva_drv_sid_tcam0_data_handler(entryValid, data, offset, &tcam0CurCrc);

        /* Handle TCAM0 mask */
        CHK_FUN_RET(ret, dva_drv_sid_tcam0_mask_handler(entryValid, data, offset, &tcam0CurCrc));

        /* Handle TCAM1 data */
        CHK_FUN_RET(ret, dva_drv_sid_tcam1_data_handler(entryValid, data, offset, &tcam1CurCrc));

        /* Handle TCAM1 mask */
        CHK_FUN_RET(ret, dva_drv_sid_tcam1_mask_handler(entryValid, data, offset, &tcam1CurCrc));

        /* Handle TCAM2 data */
        CHK_FUN_RET(ret, dva_drv_sid_tcam2_data_handler(entryValid, data, offset, &tcam2CurCrc));

        /* Handle TCAM2 mask */
        CHK_FUN_RET(ret, dva_drv_sid_tcam2_mask_handler(entryValid, data, offset, &tcam2CurCrc));

        /* Handle TCAM3 data */
        CHK_FUN_RET(ret, dva_drv_sid_tcam3_data_handler(entryValid, data, offset, &tcam3CurCrc));

        /* Handle TCAM3 mask */
        CHK_FUN_RET(ret, dva_drv_sid_tcam3_mask_handler(entryValid, data, offset, &tcam3CurCrc));

        if(RT_ERR_OK != ret)
        {
            break;
        }

        /* Increment the counter */
        counter++;

        /* Update the offset */
        offset += dataReg;
    }

    /* If no errors, assign the calculated CRC values to goldenCrc */
    if(RT_ERR_OK == ret)
    {
        goldenCrc->val[0] = swapl32(tcam0CurCrc);
        goldenCrc->val[1] = swapl32(tcam1CurCrc);
        goldenCrc->val[2] = swapl32(tcam2CurCrc);
        goldenCrc->val[3] = swapl32(tcam3CurCrc);
    }

    /* Return the result */
    return ret;
}

RtkApiRet dva_drv_tcam_get_sid_table_crc(uint32 unit, dva_drv_TcamEntryInfo *updateEntry, dva_drv_TcamSidCrcVal_t *crc)
{
    dva_table_list_t        tableType                  = DVA_STREAM_IDENTIFICATION_TABLE;
    uint32                  tableSize                  = dva_table_list[tableType].size;
    uint32                  val[DVA_TCAM_SID_DATA_REG] = {0};
    dva_drv_TcamSidCrcVal_t curCrc                     = {.val = {~0u, ~0u, ~0u, ~0u}};
    dva_drv_TcamSidCrcVal_t goldenCrc                  = {.val = {0u, 0u, 0u, 0u}};
    uint32                  i                          = 0u;
    RtkApiRet               ret                        = RT_ERR_OK;

    do
    {
        /* Check if the input parameters are valid */
        if(NULL == updateEntry)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        if(NULL == crc)
        {
            ret = -(RT_ERR_NULL_POINTER);
            continue;
        }

        for(i = 0u; i < tableSize; i++)
        {
            if((i == updateEntry->addr) && (1u == updateEntry->valid))
            {
                ret = dva_drv_memcpy(&val[0], updateEntry->data, sizeof(val));
            }
            else
            {
                ret = dva_table_read(unit, tableType, i, &val[0]);
            }

            CHK_FUN_RET(ret, dva_drv_tcam_sid_crc_calculate(&val[0], 1u, &curCrc, &goldenCrc));
            CHK_FUN_RET(ret, dva_drv_memcpy(&curCrc, &goldenCrc, sizeof(goldenCrc)));

            if(RT_ERR_OK != ret)
            {
                break;
            }
        }

        if(RT_ERR_OK == ret)
        {
            CHK_FUN_CONTINUE(ret, dva_drv_memcpy(crc, &goldenCrc, sizeof(goldenCrc)));
        }
    } while(0u == 1u);

    return ret;
}