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

#ifndef CVA_RTK_API_SMI_H__
#define CVA_RTK_API_SMI_H__

#include "rtkas_api_smi.h"
#include "rtl907xc_va_api.h"

#define CVA_RTK_STD_PHY_ADDR_MAX (31u)
#define CVA_RTK_STD_PHY_REG_MAX  (31u)

/******************************************************************************/
/* Typedef Enums                                                              */
/******************************************************************************/
/******************************************************************************/
/* Typedef Structures                                                         */
/******************************************************************************/
/******************************************************************************/
/* Global Variables Declaration                                               */
/******************************************************************************/
/******************************************************************************/
/* Functions Declaration                                                      */
/******************************************************************************/
EXTERN_API RtkApiRet cva_std_mdx_init(uint32 unit, MdxBaudRate baudr, MdxPreamble_e preamble);
EXTERN_API RtkApiRet cva_std_mdx_register_get(uint32 unit, MdxSlvPhyID phyID, MdxSlvReg regNum, MdxSlvData *mdxData);
EXTERN_API RtkApiRet cva_std_mdx_register_set(uint32 unit, MdxSlvPhyID phyID, MdxSlvReg regNum, MdxSlvData mdxData);

#endif /* CVA_RTK_API_SMI_H__ */