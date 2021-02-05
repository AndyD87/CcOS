/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcKernelModule
 **/
#pragma once

#include "CcBase.h"

#if defined(_MSC_VER) && !defined(CCKERNEL_MODE)
# ifndef CcKernelModuleSHARED
#   ifdef CcKernelModule_EXPORTS
//    Cmake definition for shared build is set
#     define CcKernelModuleSHARED __declspec(dllexport)
#   elif defined(CC_STATIC)
//    CCOS will be build as static library no im-/export
#     define CcKernelModuleSHARED
#   else
//    if no definition found, we are on importing as dll
#     define CcKernelModuleSHARED __declspec(dllimport)
#   endif
# endif
#else
# define CcKernelModuleSHARED
#endif

//! Forware declaration of Kernel module context. It will be created from targe Platfom
typedef struct _CcKernelModuleContext CcKernelModuleContext;

CCEXTERNC_BEGIN
/**
 * @brief Entry point for KernelModules if they are loaded
 * @param pContext: Context from driver to pass to devices
 */
extern void CcKernelModule_load(CcKernelModuleContext* pContext);

/**
 * @brief Callback for KernelModule on driver unload
 * @param pContext: Context from driver to pass to devices
 */
extern void CcKernelModule_unload(CcKernelModuleContext* pContext);
CCEXTERNC_END
