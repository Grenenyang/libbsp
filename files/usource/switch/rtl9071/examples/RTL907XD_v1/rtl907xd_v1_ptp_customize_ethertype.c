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

#include "rtkas_api_ptp_ext.h"
/**
 * @addtogroup EXAMPLE Example
 * @{
 */

/**
 * @defgroup PTP PTP Example
 * @{
 */
/**
 * @defgroup RTL907XDV1_PTP_EXAMPLE RTL907XD_V1 PTP Example
 * @{
 */

/**
 *  @defgroup RTL907XDV1_PTP_CUSTOMIZE_ETHERTYPE_EXAMPLE PTP customize ethertype Example
 *  @brief <b>Customized Ethertype</b> \n
 *  @section Purpose
 *           This feature allows customization of the PTP ethertype, enabling the switch to support specific requirements. \n\n
 *  @section Description
 *           This example demonstrates how to customize the PTP ethertype to 0x9600. \n
 *           Note that this ethertype customization must be applied to all PTP devices (GM, Bridge, End point) in the environment to ensure successful time synchronization. \n
 *           The default ethertype for PTP messages is 0x88f7. \n
 * @{
 */

RtkApiRet dv1_ptp_customize_ethertype(void);

/**
 *  @brief <b>Customized Ethertype</b> \n
 *  @section Purpose
 *               This feature allows customization of the PTP ethertype, enabling the switch to support specific requirements. \n\n
 *  @section Description
 *               This example demonstrates how to customize the PTP ethertype to 0x9600. \n
 *               Note that this ethertype customization must be applied to all PTP devices (GM, Bridge, End point) in the environment to ensure successful time synchronization. \n
 *               The default ethertype for PTP messages is 0x88f7. \n
 */
RtkApiRet dv1_ptp_customize_ethertype(void)
{
    uint8      port             = 0u;
    uint16     portmask         = 0u;
    uint16     etherType        = 0x9600u;
    RtkApiRet  ret              = RT_ERR_OK;
    RtkApiRet  apiresult        = RT_ERR_OK;
    uint32     callAPIerrorFlag = 0u;
    UnitChip_t unitChip;

    unitChip.chip = CHIP_907XD_V1;
    unitChip.unit = DV1_UNIT;

    PtpGlenable_t              ptpGlenable;
    PtpDomainmap_t             ptpDomainmap;
    PtpCustomEthertype_t       ptpCustomEthertype;
    PtpCustomEthertypeEnable_t PtpCustomEthertypeEnable;

    do
    {
        /* Step 1. Disable PTP to configure. */
        ptpGlenable.glenable = 0u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_glenable(unitChip, &ptpGlenable));

        /* Step 2. Get domain 0,1,2 portmask. */
        ptpDomainmap.domainid = 0u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_get_domainmap(unitChip, &ptpDomainmap));
        portmask |= ptpDomainmap.domainmap;

        ptpDomainmap.domainid = 1u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_get_domainmap(unitChip, &ptpDomainmap));
        portmask |= ptpDomainmap.domainmap;

        ptpDomainmap.domainid = 2u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_get_domainmap(unitChip, &ptpDomainmap));
        portmask |= ptpDomainmap.domainmap;

        /* Step 3. Set internal port. */
        ptpCustomEthertype.port      = DV1_RTK_CPU_PORT;
        ptpCustomEthertype.ethertype = etherType;
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_custom_ethertype(unitChip, &ptpCustomEthertype));
        PtpCustomEthertypeEnable.port   = DV1_RTK_CPU_PORT;
        PtpCustomEthertypeEnable.enable = 1u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_ethertype_enable_custom(unitChip, &PtpCustomEthertypeEnable));

        for(port = DV1_RTK_USER_PORT_1ST; port <= DV1_RTK_USER_PORT_12TH; port++)
        {
            /* Exclude no join port */
            if(0u == (((uint16)1u << port) & portmask))
            {
                continue;
            }
            /* Step 4. Set Custom EtherType of PTP to 0x9600 insted of 0x88f7. Must use Big endian */
            ptpCustomEthertype.port      = port;
            ptpCustomEthertype.ethertype = etherType;
            apiresult                    = rtk_ptp_set_custom_ethertype(unitChip, &ptpCustomEthertype);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint32)1u << 0);
            }

            /* Step 5. Enable checking custom etherType. */
            PtpCustomEthertypeEnable.port   = port;
            PtpCustomEthertypeEnable.enable = 1u;
            apiresult                       = rtk_ptp_set_ethertype_enable_custom(unitChip, &PtpCustomEthertypeEnable);
            if(RT_ERR_OK != apiresult)
            {
                callAPIerrorFlag |= ((uint32)1u << 1);
            }
        }
        if(0u != callAPIerrorFlag)
        {
            ret = -RT_ERR_FAILED;
            break;
        }

        /* Step 6. Enable PTP. */
        ptpGlenable.glenable = 1u;
        CHK_FUN_CONTINUE((ret), rtk_ptp_set_glenable(unitChip, &ptpGlenable));

        /* Step 7 Reinit protocol configuration. */
        CHK_FUN_CONTINUE((ret), rtk_ptp_reinitprotocol(unitChip));

    } while(0u == 1u);

    /* Expected Behaviour:
     * 1. Check the ethertype of Sync/Follow_Up, which should be 0x9600.
     * */
    return ret;
}

/**@}*/ /* EXAMPLE */
/**@}*/ /* PTP */
/**@}*/ /* RTL907XDV1_PTP_EXAMPLE */
/**@}*/ /* RTL907XDV1_PTP_CUSTOMIZE_ETHERTYPE_EXAMPLE */