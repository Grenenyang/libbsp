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
#include "rtl907xd_v1_api.h"
#include "rtl907xd_v1_api_mirror.h"

#include "rtkas_types.h"
#include "rtl907xd_v1_reg_list.h"
#include "rtl907xd_v1_regField_list.h"
#include "rtl907xd_v1_reg_struct.h"

#include "rtl907xd_v1_asicdrv_mirror.h"
#include "rtl907xd_v1_asicdrv_config.h"

#ifdef RTK_MODULE_PORT_MIRROR
/**
 * @brief This API sets the ingress and egress mirrored port masks, and mirroring port id for a port-based mirroring function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo                      Mirror number (0 to 1) \n
 *                                          Mirroring function number
 * @param[in] mirroringSysPort              Mirroring system port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
 *                                          Mirroring system port is used to monitor the network traffic.
 * @param[in] mirroredIngressSysPortmask    Mirrored system ingress port mask (0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
 *                                          The ingress mirrored port mask contains the ports which are monitored for ingress traffic.
 * @param[in] mirroredEgressSysPortMask     Mirrored system egress port mask (0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
 *                                          The egress mirrored port mask contains the ports which are monitored for egress traffic.
 * @param[in] morg                          Mirror original (0 to 1) \n
 *                                          Mirror original indicates that the switch mirrors original packets or modified packets. \n
 *                                          0: Mirror modified packet \n
 *                                          1: Mirror original packet
 * @param[out] None \n
 * @retval RT_ERR_OK                        Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_INPUT                    The index was out of range. \n
 * @retval -RT_ERR_PORT_ID                  The port was out of range. \n
 * @retval -RT_ERR_PORT_MASK                A port was set as mirroring port and mirrored port. \n
 * @retval Others                           Refer to dv1_reg_field_write() for the error.
 */
RtkApiRet dv1_drv_mirror_port_based_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringSysPort, RtkPmap mirroredIngressSysPortmask, RtkPmap mirroredEgressSysPortMask, uint32 morg)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MGPKT_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MGPKT_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MBPKT_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MBPKT_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MBA_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MBA_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MMA_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MMA_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MUA_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MUA_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_CROSSVLAN_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_CROSSVLAN_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_SPMDPMOP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_SPMDPMOP_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_FLOWBASEDONLY_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_FLOWBASEDONLY_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_MORG_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_MORG_LEN, morg));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL2 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL2_MIR0_MPTKPP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL2_MIR0_MPTKPP_LEN, 1u));

        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LEN, mirroringSysPort));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL3 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LEN, mirroredIngressSysPortmask));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL4 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL4_MIR0_DPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL4_MIR0_DPM_LEN, mirroredEgressSysPortMask));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets the configurations including mirrored port masks and mirroring port id for a port-based mirroring function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in]  mirrorNo                     Mirror number (0 to 1) \n
 *                                          Mirroring function number
 * @param[out] pMirroringPort               Mirroring port (0 to DV1_RTK_USER_PMAP_ALL) \n
 *                                          Mirroring port is used to monitor the network traffic.
 * @param[out] pMirroredIngressPortmask     Mirrored ingress port mask (0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
 *                                          The ingress mirrored port mask contains the ports which are monitored for ingress traffic.
 * @param[out] pMirroredEgressPortmask      Mirrored egress port mask (0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
 *                                          The egress mirrored port mask contains the ports which are monitored for egress traffic.
 * @param[out] pMorg                        Mirror original (0 to 1) \n
 *                                          Mirror original indicates that the switch mirrors original packets or modified packets. \n
 *                                          0: Mirror modified packet \n
 *                                          1: Mirror original packet \n
 * @retval RT_ERR_OK                        Mirroring port and mirrored ports were obtained. \n
 * @retval -RT_ERR_INPUT                    The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER             One of structure pointers was null. \n
 * @retval Others                           Refer to dv1_reg_field_read() and dv1_drv_pmap_convert() for the error.
 */
RtkApiRet dv1_drv_mirror_port_based_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkPmap *pMirroredIngressPortmask, RtkPmap *pMirroredEgressPortmask, uint32 *pMorg)
{
    uint32    val = 0u;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LEN, &val));
        /* convert sys port to user port */
        *pMirroringPort = dv1_g_sysPort2userPort[val];

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL3 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LEN, &val));
        /* convert sys port to user port */
        CHK_FUN_CONTINUE(ret, dv1_drv_pmap_convert(val, pMirroredIngressPortmask, DV1_EM_SYS_TO_USER));

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL4 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL4_MIR0_DPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL4_MIR0_DPM_LEN, &val));
        /* convert sys port to user port */
        CHK_FUN_CONTINUE(ret, dv1_drv_pmap_convert(val, pMirroredEgressPortmask, DV1_EM_SYS_TO_USER));

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_MORG_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_MORG_LEN, pMorg));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API sets the ingress mirrored port masks, and mirroring port id for a flow-based mirroring function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo                      Mirror number (0 to 1) \n
 *                                          Mirroring function number
 * @param[in] mirroringSysPort              Mirroring system port(DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
 *                                          Mirroring system port is used to monitor the network traffic.
 * @param[in] mirroredIngressSysPortmask    Mirrored system ingress port mask (0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
 *                                          The ingress mirrored port mask contains the ports which are monitored for ingress traffic.
 * @param[in] mirroredEgressSysPortMask     Mirrored system egress port mask (0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
                                            The egress mirrored port mask contains the ports which are monitored for egress traffic.
 * @param[in] morg                          Mirror original (0 to 1) \n
 *                                          Mirror original indicates that the switch mirrors original packets or modified packets. \n
 *                                          0: Mirror modified packet \n
 *                                          1: Mirror original packet
 * @param[out] None \n
 * @retval RT_ERR_OK                        Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_INPUT                    The index was out of range. \n
 * @retval -RT_ERR_PORT_ID                  The port was out of range. \n
 * @retval -RT_ERR_PORT_MASK                A port was set as mirroring port and mirrored port. \n
 * @retval Others                           Refer to dv1_reg_field_write() for the error.
 */
RtkApiRet dv1_drv_mirror_flow_based_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringSysPort, RtkPmap mirroredIngressSysPortmask, RtkPmap mirroredEgressSysPortMask, uint32 morg)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MGPKT_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MGPKT_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MBPKT_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MBPKT_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MBA_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MBA_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MMA_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MMA_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MUA_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_MUA_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_CROSSVLAN_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_CROSSVLAN_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_SPMDPMOP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL0_MIR0_SPMDPMOP_LEN, 0u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_FLOWBASEDONLY_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_FLOWBASEDONLY_LEN, 1u));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_MORG_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_MORG_LEN, morg));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL2 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL2_MIR0_MPTKPP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL2_MIR0_MPTKPP_LEN, 1u));

        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LEN, mirroringSysPort));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL3 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LEN, mirroredIngressSysPortmask));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL4 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL4_MIR0_DPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL4_MIR0_DPM_LEN, mirroredEgressSysPortMask));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets the configurations including mirrored port masks and mirroring port id for a flow-based mirroring function.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo                      Mirror number (0 to 1) \n
 *                                          Mirroring function number
 * @param[out] pMirroringPort               Mirroring port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
 *                                          Mirroring port is used to monitor the network traffic.
 * @param[out] pMirroredIngressPortmask     Mirrored ingress port mask (0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
 *                                          The ingress mirrored port mask contains the ports which are monitored for ingress traffic.
 * @param[out] pMirroredEgressPortmask      Mirrored egress port mask (0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
 *                                          The egress mirrored port mask contains the ports which are monitored for egress traffic.
 * @param[out] pMorg                        Mirror original (0 to 1) \n
 *                                          Mirror original indicates that the switch mirrors original packets or modified packets. \n
 *                                          0: mirror modified packet \n
 *                                          1: mirror original packet \n
 * @retval RT_ERR_OK                        Mirroring port and mirrored ports were obtained. \n
 * @retval -RT_ERR_INPUT                    The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER             One of structure pointers was null. \n
 * @retval Others                           Refer to dv1_reg_field_read() and dv1_drv_pmap_convert() for the error.
 */
RtkApiRet dv1_drv_mirror_flow_based_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkPmap *pMirroredIngressPortmask, RtkPmap *pMirroredEgressPortmask, uint32 *pMorg)
{
    uint32    val = 0u;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LEN, &val));
        /* convert sys port to user port */
        *pMirroringPort = dv1_g_sysPort2userPort[val];

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL3 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LEN, &val));
        /* convert sys port to user port */
        CHK_FUN_CONTINUE(ret, dv1_drv_pmap_convert(val, pMirroredIngressPortmask, DV1_EM_SYS_TO_USER));

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL4 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL4_MIR0_DPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL4_MIR0_DPM_LEN, &val));
        /* convert sys port to user port */
        CHK_FUN_CONTINUE(ret, dv1_drv_pmap_convert(val, pMirroredEgressPortmask, DV1_EM_SYS_TO_USER));

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_MORG_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_MORG_LEN, pMorg));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API sets RSPAN TX action and tag for mirrored packets in remote mirroring.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo            Mirror number (0 to 1) \n
 *                                Mirroring function number
 * @param[in] mirroringSysPort    Mirroring system port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
 *                                Mirroring system port is used to monitor the network traffic.
 * @param[in] action              Action (N/A) \n
 *                                RSPAN TX action \n
 *                                DV1_EM_RSPAN_TXTAG_RETAIN: retain tag \n
 *                                DV1_EM_RSPAN_TXTAG_ADD: add tag \n
 *                                DV1_EM_RSPAN_TXTAG_CHANGE: change tag \n
 *                                DV1_EM_RSPAN_TXTAG_REMOVE: remove tag
 * @param[in] tag                 Tag (N/A) \n
 *                                DV1_VLAN tag
 * @param[out] None
 * @retval RT_ERR_OK              Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_PORT_ID        The port was out of range. \n
 * @retval -RT_ERR_INPUT          The index was out of range. \n
 * @retval -RT_ERR_OUT_OF_RANGE   The action or one field of RSPAN tag was out of range. \n
 * @retval Others                 Refer to dv1_reg_field_write() for the error.
 */
RtkApiRet dv1_drv_mirror_rspan_tx_set(uint32 unit, uint8 mirrorNo, RtkPort mirroringSysPort, RtkRspanTxtagAction_e action, RtkRspanTxtag_t tag)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LEN, (uint32)mirroringSysPort));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_REMOTE_MIRROR0_TX0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX0_MIR0_RSPAN_TX_TAG_ACT_LSP, DV1_REMOTE_MIRROR0_TX0_MIR0_RSPAN_TX_TAG_ACT_LEN, (uint32)action));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_REMOTE_MIRROR0_TX1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_VID_LSP, DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_VID_LEN, (uint32)tag.vid));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_REMOTE_MIRROR0_TX1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_CFI_LSP, DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_CFI_LEN, (uint32)tag.cfi));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_REMOTE_MIRROR0_TX1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_PRI_LSP, DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_PRI_LEN, (uint32)tag.pri));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_REMOTE_MIRROR0_TX1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_TPID_LSP, DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_TPID_LEN, (uint32)tag.tpid));
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets RSPAN TX action and tag for mirrored packets in remote mirroring.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo              Mirror number (0 to 1) \n
 *                                  Mirroring function number
 * @param[out] pMirroringPort       Mirroring port (DV1_RTK_USER_PORT_1ST to DV1_RTK_USER_PORT_END) \n
 *                                  Mirroring port is used to monitor the network traffic.
 * @param[out] pAction              Action (N/A) \n
 *                                  RSPAN TX action \n
 *                                  DV1_EM_RSPAN_TXTAG_RETAIN: retain tag \n
 *                                  DV1_EM_RSPAN_TXTAG_ADD: add tag \n
 *                                  DV1_EM_RSPAN_TXTAG_CHANGE: change tag \n
 *                                  DV1_EM_RSPAN_TXTAG_REMOVE: remove tag
 * @param[out] pTag                 Tag (N/A) \n
 *                                  DV1_VLAN tag
 * @param[out] None \n
 * @retval RT_ERR_OK                Mirroring port and mirrored ports were configured. \n
 * @retval -RT_ERR_INPUT            The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER     One of pointers was null. \n
 * @retval Others                   Refer to dv1_reg_field_read() for the error.
 */
RtkApiRet dv1_drv_mirror_rspan_tx_get(uint32 unit, uint8 mirrorNo, RtkPort *pMirroringPort, RtkRspanTxtagAction_e *pAction, RtkRspanTxtag_t *pTag)
{
    uint32    val = 0u;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LSP, DV1_TRAFFIC_MIRROR0_CONTROL1_MIR0_DP_LEN, &val));
        /* convert sys port to user port */
        *pMirroringPort = dv1_g_sysPort2userPort[val];

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_REMOTE_MIRROR0_TX1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_VID_LSP, DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_VID_LEN, &val));
        pTag->vid = (uint16)(val & 0xFFFFu);

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_REMOTE_MIRROR0_TX1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_CFI_LSP, DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_CFI_LEN, &val));
        pTag->cfi = (uint8)(val & 0xFFu);

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_REMOTE_MIRROR0_TX1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_PRI_LSP, DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_PRI_LEN, &val));
        pTag->pri = (uint8)(val & 0xFFu);

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_REMOTE_MIRROR0_TX1 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_TPID_LSP, DV1_REMOTE_MIRROR0_TX1_MIR0_RSPAN_TX_TAG_TPID_LEN, &val));
        pTag->tpid = (uint16)(val & 0xFFFFu);

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_REMOTE_MIRROR0_TX0 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_TX0_MIR0_RSPAN_TX_TAG_ACT_LSP, DV1_REMOTE_MIRROR0_TX0_MIR0_RSPAN_TX_TAG_ACT_LEN, &val));

        if(0u == val)
        {
            *pAction = EM_RSPAN_TXTAG_RETAIN;
        }
        else if(1u == val)
        {
            *pAction = EM_RSPAN_TXTAG_ADD;
        }
        else if(2u == val)
        {
            *pAction = EM_RSPAN_TXTAG_CHANGE;
        }
        else if(3u == val)
        {
            *pAction = EM_RSPAN_TXTAG_REMOVE;
        }
        else
        {
            *pAction = EM_RSPAN_TXTAG_END;
        }
    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API sets RSPAN RX configurations including enable and vid for remote mirroring.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo              Mirror number (0 to 1) \n
 *                                  Mirroring function number
 * @param[in] mirroredSysProtmask   Mirrored system port mask(0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
 *                                  The mirrored system port mask contains the ports which are monitored for mirrored traffic.
 * @param[in] enable                Enable (0 to 1) \n
 *                                  RSPAN check \n
 *                                  DISABLE: no action \n
 *                                  ENABLE: packets with matched VID are regarded as mirrored traffic directly
 * @param[in] vid                   DV1_VLAN ID (0 to 4095) \n
 *                                  DV1_VLAN identifier
 * @param[out] None
 * @retval RT_ERR_OK                RSPAN check and VID were configured. \n
 * @retval -RT_ERR_INPUT            The index was out of range. \n
 * @retval -RT_ERR_ENABLE           The enable was out of range. \n
 * @retval -RT_ERR_OUT_OF_RANGE     The action or one field of RSPAN tag was out of range. \n
 * @retval Others                   Refer to dv1_reg_field_write() for the error.
 */
RtkApiRet dv1_drv_mirror_rspan_rx_set(uint32 unit, uint8 mirrorNo, RtkPmap mirroredSysProtmask, RtkEnable enable, uint16 vid)
{
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_TRAFFIC_MIRROR0_CONTROL3 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LEN, mirroredSysProtmask));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_REMOTE_MIRROR0_RX + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_RX_MIR0_RSPAN_RX_TAG_CHK_LSP, DV1_REMOTE_MIRROR0_RX_MIR0_RSPAN_RX_TAG_CHK_LEN, (uint32)enable));
        CHK_FUN_CONTINUE(ret, dv1_reg_field_write(unit, (DV1_REMOTE_MIRROR0_RX + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_RX_MIR0_RSPAN_RX_TAG_VID_LSP, DV1_REMOTE_MIRROR0_RX_MIR0_RSPAN_RX_TAG_VID_LEN, (uint32)vid));

    } while(0u == 1u);

    return ret;
}

/**
 * @brief This API gets RSPAN RX configurations including enable and vid for remote mirroring.
 * @param[in] unit Stacking unit (1 to 7) \n
 *            Stacking unit ID is used to indicate which switch is to be accessed.
 * @param[in] mirrorNo            Mirror number (0 to 1) \n
 *                                Mirroring function number
 * @param[out] pMirroredPortmask  Mirrored port mask(0 to DV1_RTK_USER_PMAP_ALL, DV1_RTK_CPU_PMAP) \n
 *                                The mirrored port mask contains the ports which are monitored for mirrored traffic.
 * @param[out] pEnable            Enable(0 to 1) \n
 *                                RSPAN check \n
 *                                DISABLE: no action \n
 *                                ENABLE: packets with matched VID are regarded as mirrored traffic directly
 * @param[out] pVid               DV1_VLAN ID (0 to 4095) \n
 *                                DV1_VLAN identifier \n
 * @retval RT_ERR_OK              RSPAN check and VID were configured. \n
 * @retval -RT_ERR_INPUT          The index was out of range. \n
 * @retval -RT_ERR_NULL_POINTER   One of pointers was null. \n
 * @retval Others                 Refer to dv1_reg_field_read() and dv1_drv_pmap_convert() for the error.
 */
RtkApiRet dv1_drv_mirror_rspan_rx_get(uint32 unit, uint8 mirrorNo, RtkPmap *pMirroredPortmask, RtkEnable *pEnable, uint16 *pVid)
{
    uint32    val = 0u;
    RtkApiRet ret = RT_ERR_OK;

    do
    {
        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_TRAFFIC_MIRROR0_CONTROL3 + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LSP, DV1_TRAFFIC_MIRROR0_CONTROL3_MIR0_SPM_LEN, &val));
        /* convert sys port to user port */
        CHK_FUN_CONTINUE(ret, dv1_drv_pmap_convert(val, pMirroredPortmask, DV1_EM_SYS_TO_USER));

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_REMOTE_MIRROR0_RX + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_RX_MIR0_RSPAN_RX_TAG_CHK_LSP, DV1_REMOTE_MIRROR0_RX_MIR0_RSPAN_RX_TAG_CHK_LEN, &val));
        *pEnable = (RtkEnable)val;

        CHK_FUN_CONTINUE(ret, dv1_reg_field_read(unit, (DV1_REMOTE_MIRROR0_RX + ((uint32)mirrorNo * DV1_MIRROR_FUNCTION_OFFSET)), DV1_REMOTE_MIRROR0_RX_MIR0_RSPAN_RX_TAG_VID_LSP, DV1_REMOTE_MIRROR0_RX_MIR0_RSPAN_RX_TAG_VID_LEN, &val));
        *pVid = (uint16)(val & 0xFFFFu);

    } while(0u == 1u);

    return ret;
}
#endif
