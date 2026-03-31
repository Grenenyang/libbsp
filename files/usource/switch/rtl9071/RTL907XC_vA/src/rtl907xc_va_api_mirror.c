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

#include "rtl907xc_va_api_ext.h"
#include "rtl907xc_va_asicdrv_mirror.h"

/**
 * @defgroup RTK_MODULE_PORT_MIRROR Mirror
 * @{
 */

/**
 * @defgroup MIRROR_ASSIGN Mirroring Assignment
 * @{
 */

/**
 * @addtogroup MIRROR_ASSIGN_ASIC Low Level Driver
 * @{
 */

/**
 * @brief This API sets the ingress and egress mirrored port masks, and mirroring port id for a port-based mirroring function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo                  Mirror number (1 to 2) \n
 *                                      Mirroring function number
 * @param[in] mirroringPort             Mirroring port (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *                                      Mirroring port is used to monitor the network traffic.
 * @param[in] mirroredIngressPortmask   Mirrored ingress port mask (0 to CVA_RTK_USER_PMAP_ALL, CVA_RTK_CPU_PMAP) \n
 *                                      The ingress mirrored port mask contains the ports which are monitored for ingress traffic.
 * @param[in] mirroredEgressPortmask    Mirrored egress port mask (0 to CVA_RTK_USER_PMAP_ALL, CVA_RTK_CPU_PMAP) \n
 *                                      The egress mirrored port mask contains the ports which are monitored for egress traffic.
 * @param[in] morg                      Mirror original (0 to 1) \n
 *                                      Mirror original indicates that the switch mirrors original packets or modified packets. \n
 *                                      0: Mirror modified packet \n
 *                                      1: Mirror original packet
 * @param[out] None \n
 * @retval RT_ERR_OK                    Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_INPUT                The index was out of range. \n
 * @retval -RT_ERR_PORT_ID              The port was out of range. \n
 * @retval -RT_ERR_PORT_MASK            A port was set as mirroring port and mirrored port. \n
 * @retval Others                       Refer to cva_drv_mirror_port_based_set() for the error.
 */
RtkApiRet cva_mirror_port_based_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringPort, RtkPmap mirroredIngressPortmask, RtkPmap mirroredEgressPortmask, uint32 morg)
{
    RtkApiRet ret                        = RT_ERR_OK;
    RtkPort   mirroringSysPort           = 0u;
    RtkPmap   mirroredIngressSysPortmask = 0u;
    RtkPmap   mirroredEgressSysPortMask  = 0u;

    do
    {
        /* Check if the mirroringPort is valid */
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(mirroringPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* convert user port to sys port */
        mirroringSysPort = cva_g_userPort2sysPort[mirroringPort];
        CHK_FUN_CONTINUE(ret, cva_drv_pmap_convert(mirroredIngressPortmask, &mirroredIngressSysPortmask, CVA_EM_USER_TO_SYS));
        CHK_FUN_CONTINUE(ret, cva_drv_pmap_convert(mirroredEgressPortmask, &mirroredEgressSysPortMask, CVA_EM_USER_TO_SYS));

        /* mirroring port != ingress or egress mirrored port */
        if((0u != (mirroredIngressSysPortmask & ((uint32)1u << mirroringSysPort))) || (0u != (mirroredEgressSysPortMask & ((uint32)1u << mirroringSysPort))))
        {
            ret = -RT_ERR_PORT_MASK;
            continue;
        }

        /* Set mirroring function */
        if((1u == mirrorNo) || (2u == mirrorNo))
        {
            /* Set mirroring port based registers */
            ret = cva_drv_mirror_port_based_set(unit, (mirrorNo - 1u), mirroringSysPort, mirroredIngressSysPortmask, mirroredEgressSysPortMask, morg);
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets the configurations including mirrored port masks and mirroring port id for a port-based mirroring function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  mirrorNo                     Mirror number (1 to 2) \n
 *                                          Mirroring function number
 * @param[out] pMirroringPort               Mirroring port (0 to CVA_RTK_USER_PMAP_ALL) \n
 *                                          Mirroring port is used to monitor the network traffic.
 * @param[out] pMirroredIngressPortmask     Mirrored ingress port mask (0 to CVA_RTK_USER_PMAP_ALL, CVA_RTK_CPU_PMAP) \n
 *                                          The ingress mirrored port mask contains the ports which are monitored for ingress traffic.
 * @param[out] pMirroredEgressPortmask      Mirrored egress port mask (0 to CVA_RTK_USER_PMAP_ALL, CVA_RTK_CPU_PMAP) \n
 *                                          The egress mirrored port mask contains the ports which are monitored for egress traffic.
 * @param[out] pMorg                        Mirror original (0 to 1) \n
 *                                          Mirror original indicates that the switch mirrors original packets or modified packets. \n
 *                                          0: Mirror modified packet \n
 *                                          1: Mirror original packet \n
 * @retval RT_ERR_OK                        Mirroring port and mirrored ports were obtained. \n
 * @retval -RT_ERR_INPUT                    The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER             One of structure pointers was null. \n
 * @retval Others                           Refer to cva_drv_mirror_port_based_get() for the error.
 */
RtkApiRet cva_mirror_port_based_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkPmap *pMirroredIngressPortmask, RtkPmap *pMirroredEgressPortmask, uint32 *pMorg)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameters is valid */
        if((NULL == pMirroringPort) || (NULL == pMirroredIngressPortmask) || (NULL == pMirroredEgressPortmask))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get mirroring function */
        if((1u == mirrorNo) || (2u == mirrorNo))
        {
            /* Get mirroring port based registers */
            ret = cva_drv_mirror_port_based_get(unit, (mirrorNo - 1u), pMirroringPort, pMirroredIngressPortmask, pMirroredEgressPortmask, pMorg);
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API sets the ingress mirrored port masks, and mirroring port id for a flow-based mirroring function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo                  Mirror number (1 to 2) \n
 *                                      Mirroring function number
 * @param[in] mirroringPort             Mirroring port(CVA_USER_PORT_1ST to CVA_USER_PORT_END) \n
 *                                      Mirroring port is used to monitor the network traffic.
 * @param[in] mirroredIngressPortmask   Mirrored ingress port mask (0 to CVA_USER_PMAP_ALL, CVA_CPU_PMAP) \n
 *                                      The ingress mirrored port mask contains the ports which are monitored for ingress traffic.
 * @param[in] mirroredEgressPortmask    Mirrored egress port mask (0 to CVA_USER_PMAP_ALL, CVA_CPU_PMAP) \n
                                        The egress mirrored port mask contains the ports which are monitored for egress traffic.
 * @param[in] morg                      Mirror original (0 to 1) \n
 *                                      Mirror original indicates that the switch mirrors original packets or modified packets. \n
 *                                      0: Mirror modified packet \n
 *                                      1: Mirror original packet
 * @param[out] None \n
 * @retval RT_ERR_OK                    Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_INPUT                The index was out of range. \n
 * @retval -RT_ERR_PORT_ID              The port was out of range. \n
 * @retval -RT_ERR_PORT_MASK            A port was set as mirroring port and mirrored port. \n
 * @retval Others                       Refer to cva_drv_mirror_flow_based_set() for the error.
 */
RtkApiRet cva_mirror_flow_based_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringPort, RtkPmap mirroredIngressPortmask, RtkPmap mirroredEgressPortmask, uint32 morg)
{
    RtkApiRet ret = RT_ERR_OK;

    RtkPort mirroringSysPort           = 0u;
    RtkPmap mirroredIngressSysPortmask = 0u;
    RtkPmap mirroredEgressSysPortMask  = 0u;

    do
    {
        /* Check if the mirroringPort is valid */
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(mirroringPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* convert user port to sys port */
        mirroringSysPort = cva_g_userPort2sysPort[mirroringPort];
        CHK_FUN_CONTINUE(ret, cva_drv_pmap_convert(mirroredIngressPortmask, &mirroredIngressSysPortmask, CVA_EM_USER_TO_SYS));
        CHK_FUN_CONTINUE(ret, cva_drv_pmap_convert(mirroredEgressPortmask, &mirroredEgressSysPortMask, CVA_EM_USER_TO_SYS));

        /* mirroring port != ingress or egress mirrored port */
        if((0u != (mirroredIngressSysPortmask & ((uint32)1u << mirroringSysPort))) || (0u != (mirroredEgressSysPortMask & ((uint32)1u << mirroringSysPort))))
        {
            ret = -RT_ERR_PORT_MASK;
            continue;
        }

        /* Set mirroring function */
        if((1u == mirrorNo) || (2u == mirrorNo))
        {
            /* Set mirroring flow based registers */
            ret = cva_drv_mirror_flow_based_set(unit, (mirrorNo - 1u), mirroringSysPort, mirroredIngressSysPortmask, mirroredEgressSysPortMask, morg);
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets the configurations including mirrored port masks and mirroring port id for a flow-based mirroring function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo                      Mirror number (1 to 2) \n
 *                                          Mirroring function number
 * @param[out] pMirroringPort               Mirroring port (CVA_USER_PORT_1ST to CVA_USER_PORT_END) \n
 *                                          Mirroring port is used to monitor the network traffic.
 * @param[out] pMirroredIngressPortmask     Mirrored ingress port mask (0 to CVA_USER_PMAP_ALL, CVA_CPU_PMAP) \n
 *                                          The ingress mirrored port mask contains the ports which are monitored for ingress traffic.
 * @param[out] pMirroredEgressPortmask      Mirrored egress port mask (0 to CVA_USER_PMAP_ALL, CVA_CPU_PMAP) \n
 *                                          The egress mirrored port mask contains the ports which are monitored for egress traffic.
 * @param[out] pMorg                        Mirror original (0 to 1) \n
 *                                          Mirror original indicates that the switch mirrors original packets or modified packets. \n
 *                                          0: mirror modified packet \n
 *                                          1: mirror original packet \n
 * @retval RT_ERR_OK                        Mirroring port and mirrored ports were obtained. \n
 * @retval -RT_ERR_INPUT                    The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER             One of structure pointers was null. \n
 * @retval Others                           Refer to cva_drv_mirror_flow_based_get() for the error.
 */
RtkApiRet cva_mirror_flow_based_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkPmap *pMirroredIngressPortmask, RtkPmap *pMirroredEgressPortmask, uint32 *pMorg)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameters is valid */
        if((NULL == pMirroringPort) || (NULL == pMirroredIngressPortmask) || (NULL == pMirroredEgressPortmask))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get mirroring function */
        if((1u == mirrorNo) || (2u == mirrorNo))
        {
            /* Get mirroring flow based registers */
            ret = cva_drv_mirror_flow_based_get(unit, (mirrorNo - 1u), pMirroringPort, pMirroredIngressPortmask, pMirroredEgressPortmask, pMorg);
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    return ret;
}
/**@}*/ /* MIRROR_ASSIGN_ASIC */
/**@}*/ /* MIRROR_ASSIGN */

/**
 * @defgroup MIRROR_FILTER Mirroring Filter
 * @{
 */

/**
 * @addtogroup MIRROR_FILTER_ASIC Low Level Driver
 * @brief MIRROR FILTER Low Level Driver
 * @{
 */

/**
 * @brief This API sets the mirror filter used to prevent from forwarding normal packets to the mirroring port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo       Mirror number (1 to 2) \n
 *                           Mirroring function number
 * @param[in] enable         Enable (0 to 1) \n
 *                           Filter status \n
 *                           DISABLE: The mirroring filter doesn't prevent sending forwarded traffic to the mirroring port. \n
 *                           ENABLE: The mirroring filter prevents sending forwarded traffic to the mirroring port.
 * @param[out] None
 * @retval RT_ERR_OK         Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_INPUT     The index was out of range. \n
 * @retval -RT_ERR_ENABLE    The enable was out of range.
 */
RtkApiRet cva_mirror_filter_set(uint32 unit, uint8 mirrorNo, RtkEnable enable)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if enable is valid */
        if(CVA_RTK_ENABLE_END <= enable)
        {
            ret = -RT_ERR_ENABLE;
            continue;
        }

        /* Set mirroring function */
        if(1u == mirrorNo)
        {
            /* Set mirroring filter register */
            CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MIRROR0_PORT_EGRESS_FILTER_CONTROL, CVA_MIRROR0_PORT_EGRESS_FILTER_CONTROL_MIR0_MIREGFILTER_LSP, CVA_MIRROR0_PORT_EGRESS_FILTER_CONTROL_MIR0_MIREGFILTER_LEN, (uint32)enable));
        }
        else if(2u == mirrorNo)
        {
            /* Set mirroring filter register */
            CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_MIRROR1_PORT_EGRESS_FILTER_CONTROL, CVA_MIRROR1_PORT_EGRESS_FILTER_CONTROL_MIR1_MIREGFILTER_LSP, CVA_MIRROR1_PORT_EGRESS_FILTER_CONTROL_MIR1_MIREGFILTER_LEN, (uint32)enable));
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets the mirror filter used to prevent from forwarding normal packets to the mirroring port.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo            Mirror number (1 to 2) \n
 *                                Mirroring function number
 * @param[out] pEnable            Enable (0 to 1) \n
 *                                Filter status \n
 *                                DISABLE: The mirroring filter doesn't prevent sending forwarded traffic to the mirroring port. \n
 *                                ENABLE: The mirroring filter prevents sending forwarded traffic to the mirroring port.
 * @retval RT_ERR_OK              Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_INPUT          The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER   The pointer was null.
 */
RtkApiRet cva_mirror_filter_get(uint32 unit, uint8 mirrorNo, RtkEnable *pEnable)
{
    uint32    val = 0u;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid */
        if(NULL == pEnable)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get mirroring function */
        if(1u == mirrorNo)
        {
            /* Get mirroring filter register */
            CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MIRROR0_PORT_EGRESS_FILTER_CONTROL, CVA_MIRROR0_PORT_EGRESS_FILTER_CONTROL_MIR0_MIREGFILTER_LSP, CVA_MIRROR0_PORT_EGRESS_FILTER_CONTROL_MIR0_MIREGFILTER_LEN, &val));
            *pEnable = (0x1u == val) ? CVA_ENABLED : CVA_DISABLED;
        }
        else if(2u == mirrorNo)
        {
            /* Get mirroring filter register */
            CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_MIRROR1_PORT_EGRESS_FILTER_CONTROL, CVA_MIRROR1_PORT_EGRESS_FILTER_CONTROL_MIR1_MIREGFILTER_LSP, CVA_MIRROR1_PORT_EGRESS_FILTER_CONTROL_MIR1_MIREGFILTER_LEN, &val));
            *pEnable = (0x1u == val) ? CVA_ENABLED : CVA_DISABLED;
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    return ret;
}
/**@}*/ /* MIRROR_FILTER_ASIC */
/**@}*/ /* MIRROR_FILTER */

/**
 * @defgroup MIRROR_SAMPLE Mirroring Sample Rate
 * @{
 */

/**
 * @addtogroup MIRROR_SAMPLE_ASIC Low Level Driver
 * @brief MIRROR SAMPLE Low Level Driver
 * @{
 */

/**
 * @brief This API sets sample rate of mirroring function. \n
 *        There is always only one mirrored packet is transmitted from the mirroring port in the sample size.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo            Mirror number (1 to 2) \n
 *                                Mirroring function number
 * @param[in] samples             Samples (0 to 65535) \n
 *                                Sample size indicates control of the quantity of mirrored packets
 * @param[out] None
 * @retval RT_ERR_OK              Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_INPUT          The index was out of range. \n
 * @retval -RT_ERR_OUT_OF_RANGE   The sample size was out of range.
 */
RtkApiRet cva_mirror_sample_rate_set(uint32 unit, uint8 mirrorNo, uint32 samples)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if samples is valid */
        if(MIRROR_SAMPLE_MAX < samples)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Set mirroring function */
        if(1u == mirrorNo)
        {
            /* Set mirroring sample rate register */
            CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_TRAFFIC_MIRROR0_SAMPLE_CONTROL, CVA_TRAFFIC_MIRROR0_SAMPLE_CONTROL_MIR0_SAMPLE_RATE_LSP, CVA_TRAFFIC_MIRROR0_SAMPLE_CONTROL_MIR0_SAMPLE_RATE_LEN, samples));
        }
        else if(2u == mirrorNo)
        {
            /* Set mirroring sample rate register */
            CHK_FUN_CONTINUE(ret, cva_reg_field_write(unit, CVA_TRAFFIC_MIRROR1_SAMPLE_CONTROL, CVA_TRAFFIC_MIRROR1_SAMPLE_CONTROL_MIR1_SAMPLE_RATE_LSP, CVA_TRAFFIC_MIRROR1_SAMPLE_CONTROL_MIR1_SAMPLE_RATE_LEN, samples));
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets sample rate of mirroring function. \n
 *        There is always only one mirrored packet is transmitted from the mirroring port in the sample size.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo            Mirror number (1 to 2) \n
 *                                Mirroring function number
 * @param[out] pSamples           Sample size(0 to 65535) \n
 *                                Sample size indicates control of the quantity of mirrored packets
 * @retval RT_ERR_OK              Sample size was obtained. \n
 * @retval -RT_ERR_INPUT          The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER   The pointer was null.
 */
RtkApiRet cva_mirror_sample_rate_get(uint32 unit, uint8 mirrorNo, uint32 *pSamples)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameter is valid */
        if(NULL == pSamples)
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get mirroring function */
        if(1u == mirrorNo)
        {
            /* Get mirroring sample rate register */
            CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_TRAFFIC_MIRROR0_SAMPLE_CONTROL, CVA_TRAFFIC_MIRROR0_SAMPLE_CONTROL_MIR0_SAMPLE_RATE_LSP, CVA_TRAFFIC_MIRROR0_SAMPLE_CONTROL_MIR0_SAMPLE_RATE_LEN, pSamples));
        }
        else if(2u == mirrorNo)
        {
            /* Get mirroring sample rate register */
            CHK_FUN_CONTINUE(ret, cva_reg_field_read(unit, CVA_TRAFFIC_MIRROR1_SAMPLE_CONTROL, CVA_TRAFFIC_MIRROR1_SAMPLE_CONTROL_MIR1_SAMPLE_RATE_LSP, CVA_TRAFFIC_MIRROR1_SAMPLE_CONTROL_MIR1_SAMPLE_RATE_LEN, pSamples));
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    return ret;
}
/**@}*/ /* MIRROR_SAMPLE_ASIC */
/**@}*/ /* MIRROR_SAMPLE */

/**
 * @defgroup MIRROR_RSPAN Mirror RSPAN
 * @{
 */

/**
 * @addtogroup MIRROR_RSPAN_ASIC Low Level Driver
 * @brief MIRROR RSPAN Low Level Driver
 * @{
 */

/**
 * @brief This API sets RSPAN TX action and tag for mirrored packets in remote mirroring.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo            Mirror number (1 to 2) \n
 *                                Mirroring function number
 * @param[in] mirroringPort       Mirroring port (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *                                Mirroring port is used to monitor the network traffic.
 * @param[in] action              Action (N/A) \n
 *                                RSPAN TX action \n
 *                                CVA_EM_RSPAN_TXTAG_RETAIN: retain tag \n
 *                                CVA_EM_RSPAN_TXTAG_ADD: add tag \n
 *                                CVA_EM_RSPAN_TXTAG_CHANGE: change tag \n
 *                                CVA_EM_RSPAN_TXTAG_REMOVE: remove tag
 * @param[in] tag                 Tag (N/A) \n
 *                                CVA_VLAN tag
 * @param[out] None
 * @retval RT_ERR_OK              Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_PORT_ID        The port was out of range. \n
 * @retval -RT_ERR_INPUT          The index was out of range. \n
 * @retval -RT_ERR_OUT_OF_RANGE   The action or one field of RSPAN tag was out of range. \n
 * @retval Others                 Refer to cva_drv_mirror_rspan_tx_set() for the error.
 */
RtkApiRet cva_rspan_tx_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringPort, RtkRspanTxtagAction_e action, RtkRspanTxtag_t tag)
{
    RtkApiRet ret              = RT_ERR_OK;
    RtkPort   mirroringSysPort = 0u;

    do
    {
        /* Check if the mirroringPort is valid */
        if(0u == CVA_IS_VALID_PORT_ID_WO_CPUPORT(mirroringPort))
        {
            ret = -RT_ERR_PORT_ID;
            continue;
        }

        /* Check if action is valid */
        if(EM_RSPAN_TXTAG_END <= action)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* Check if the RSPAN action is Add or Change then the CVA_VLAN tag is valid */
        if(((EM_RSPAN_TXTAG_ADD == action) || (EM_RSPAN_TXTAG_CHANGE == action)) && ((MIRROR_CFI_MAX < tag.cfi) || (MIRROR_PRI_MAX < tag.pri) || (MIRROR_VID_MAX < tag.vid)))
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* convert user port to sys port */
        mirroringSysPort = cva_g_userPort2sysPort[mirroringPort];

        /* Set mirroring function */
        if((1u == mirrorNo) || (2u == mirrorNo))
        {
            /* Set RSPAN TX registers */
            ret = cva_drv_mirror_rspan_tx_set(unit, (mirrorNo - 1u), mirroringSysPort, action, tag);
        }
        else
        {
            ret = -RT_ERR_INPUT;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets RSPAN TX action and tag for mirrored packets in remote mirroring.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo              Mirror number (1 to 2) \n
 *                                  Mirroring function number
 * @param[out] pMirroringPort       Mirroring port (CVA_RTK_USER_PORT_1ST to CVA_RTK_USER_PORT_END) \n
 *                                  Mirroring port is used to monitor the network traffic.
 * @param[out] pAction              Action (N/A) \n
 *                                  RSPAN TX action \n
 *                                  CVA_EM_RSPAN_TXTAG_RETAIN: retain tag \n
 *                                  CVA_EM_RSPAN_TXTAG_ADD: add tag \n
 *                                  CVA_EM_RSPAN_TXTAG_CHANGE: change tag \n
 *                                  CVA_EM_RSPAN_TXTAG_REMOVE: remove tag
 * @param[out] pTag                 Tag (N/A) \n
 *                                  CVA_VLAN tag
 * @param[out] None \n
 * @retval RT_ERR_OK                Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_INPUT            The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER     One of pointers was null. \n
 * @retval Others                   Refer to cva_drv_mirror_rspan_tx_get() for the error.
 */
RtkApiRet cva_rspan_tx_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkRspanTxtagAction_e *pAction, RtkRspanTxtag_t *pTag)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameters is valid */
        if((NULL == pMirroringPort) || (NULL == pAction) || (NULL == pTag))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get mirroring function */
        if((1u == mirrorNo) || (2u == mirrorNo))
        {
            /* Get RSPAN TX registers */
            ret = cva_drv_mirror_rspan_tx_get(unit, (mirrorNo - 1u), pMirroringPort, pAction, pTag);
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API sets RSPAN RX configurations including enable and vid for remote mirroring.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo            Mirror number (1 to 2) \n
 *                                Mirroring function number
 * @param[in] mirroredPortmask    Mirrored port mask(0 to CVA_RTK_USER_PMAP_ALL, CVA_RTK_CPU_PMAP) \n
 *                                The mirrored port mask contains the ports which are monitored for mirrored traffic.
 * @param[in] enable              Enable (0 to 1) \n
 *                                RSPAN check \n
 *                                DISABLE: no action \n
 *                                ENABLE: packets with matched VID are regarded as mirrored traffic directly
 * @param[in] vid                 CVA_VLAN ID (0 to 4095) \n
 *                                CVA_VLAN identifier
 * @param[out] None
 * @retval RT_ERR_OK              RSPAN check and VID were configured. \n
 * @retval -RT_ERR_INPUT          The index was out of range. \n
 * @retval -RT_ERR_ENABLE         The enable was out of range. \n
 * @retval -RT_ERR_OUT_OF_RANGE   The action or one field of RSPAN tag was out of range. \n
 * @retval Others                 Refer to cva_drv_mirror_rspan_rx_set() for the error.
 */
RtkApiRet cva_rspan_rx_set(uint32 unit, uint8 mirrorNo, RtkPmap mirroredPortmask, RtkEnable enable, uint16 vid)
{
    RtkApiRet ret                 = RT_ERR_OK;
    uint32    mirroredSysProtmask = 0u;

    do
    {
        /* Check if the enable is valid */
        if(CVA_RTK_ENABLE_END <= enable)
        {
            ret = -RT_ERR_ENABLE;
            continue;
        }

        /* Check if the vid is valid */
        if(MIRROR_VID_MAX < vid)
        {
            ret = -RT_ERR_OUT_OF_RANGE;
            continue;
        }

        /* convert user port to sys port */
        CHK_FUN_CONTINUE(ret, cva_drv_pmap_convert(mirroredPortmask, &mirroredSysProtmask, CVA_EM_USER_TO_SYS));

        /* Set mirroring function */
        if((1u == mirrorNo) || (2u == mirrorNo))
        {
            /* Set RSPAN RX registers */
            ret = cva_drv_mirror_rspan_rx_set(unit, (mirrorNo - 1u), mirroredSysProtmask, enable, vid);
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets RSPAN RX configurations including enable and vid for remote mirroring.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo            Mirror number (1 to 2) \n
 *                                Mirroring function number
 * @param[out] pMirroredPortmask  Mirrored port mask(0 to CVA_RTK_USER_PMAP_ALL, CVA_RTK_CPU_PMAP) \n
 *                                The mirrored port mask contains the ports which are monitored for mirrored traffic.
 * @param[out] pEnable            Enable(0 to 1) \n
 *                                RSPAN check \n
 *                                DISABLE: no action \n
 *                                ENABLE: packets with matched VID are regarded as mirrored traffic directly
 * @param[out] pVid               CVA_VLAN ID (0 to 4095) \n
 *                                CVA_VLAN identifier \n
 * @retval RT_ERR_OK              RSPAN check and VID were configured. \n
 * @retval -RT_ERR_INPUT          The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER   One of the pointers was null. \n
 * @retval Others                 Refer to cva_drv_mirror_rspan_rx_get() for the error.
 */
RtkApiRet cva_rspan_rx_get(uint32 unit, uint8 mirrorNo, RtkPmap *pMirroredPortmask, RtkEnable *pEnable, uint16 *pVid)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        /* Check if the parameters is valid */
        if((NULL == pMirroredPortmask) || (NULL == pEnable) || (NULL == pVid))
        {
            ret = -RT_ERR_NULL_POINTER;
            continue;
        }

        /* Get mirroring function */
        if((1u == mirrorNo) || (2u == mirrorNo))
        {
            /* Get RSPAN RX registers */
            ret = cva_drv_mirror_rspan_rx_get(unit, (mirrorNo - 1u), pMirroredPortmask, pEnable, pVid);
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }

    } while(0u == 1u);

    return ret;
}
/**@}*/ /* MIRROR_RSPAN_ASIC */
/**@}*/ /* MIRROR_RSPAN */

/**
 * @defgroup MIRROR_RESET Mirror Reset
 * @{
 */

/**
 * @addtogroup MIRROR_RESET_ASIC Low Level Driver
 * @brief MIRROR RESET Low Level Driver
 * @{
 */

/**
 * @brief This API resets mirror function 1 or 2.
 * @param[in] unit        Stacking unit (1 to 7) \n
 *                        Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo    Mirror number (1 to 2) \n
 * @param[out] None
 * @retval RT_ERR_OK      Mirror function 1 and 2 will reset. \n
 * @retval RT_ERR_INPUT   The index was out of range. \n
 * @retval Others         Refer to cva_reg_field_write() for the error.
 */
RtkApiRet cva_mirror_reset(uint32 unit, uint8 mirrorNo)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        if(1u == mirrorNo)
        {
            /* reset mirroring on all kinds of traffic for mirror function 1*/
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_TRAFFIC_MIRROR0_CONTROL0, 0x0));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_TRAFFIC_MIRROR0_CONTROL1, 0x0));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_REMOTE_MIRROR0_RX, 0x0));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_REMOTE_MIRROR0_TX0, 0x0));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_REMOTE_MIRROR0_TX1, 0x0));
        }
        else if(2u == mirrorNo)
        {
            /* reset mirroring on all kinds of traffic for mirror function 2*/
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_TRAFFIC_MIRROR1_CONTROL0, 0x0));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_TRAFFIC_MIRROR1_CONTROL1, 0x0));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_REMOTE_MIRROR1_RX, 0x0));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_REMOTE_MIRROR1_TX0, 0x0));
            CHK_FUN_CONTINUE(ret, cva_reg_write(unit, CVA_REMOTE_MIRROR1_TX1, 0x0));
        }
        else
        {
            ret = -RT_ERR_INPUT;
            continue;
        }
    } while(0u == 1u);

    return ret;
}
/**@}*/ /* MIRROR_RESET_ASIC */
/**@}*/ /* MIRROR_RESET */
/**@}*/ /* RTK_MODULE_PORT_MIRROR */
