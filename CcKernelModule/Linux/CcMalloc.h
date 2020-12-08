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
 * @page      CcKernelModule
 * @subpage   Windows
 *
 * @page      CcMalloc
 * @copyright Andreas Dirmeier (C) 2019
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Declarations for malloc and new
 */
#ifndef H_CCOS_CCKERNEL_MODULE_LINUX_MALLOC_H_
#define H_CCOS_CCKERNEL_MODULE_LINUX_MALLOC_H_

#include "CcBase.h"

#ifndef KERN_INFO
  #define KERN_INFO "6"
#endif

CCEXTERNC_BEGIN
extern void* CcMalloc_malloc(size_t uiSize);
extern void CcMalloc_free(void* pBuffer);
extern void CcMalloc_print(const char* pFormat, ...);
CCEXTERNC_END

#endif // H_CCOS_CCKERNEL_MODULE_LINUX_MALLOC_H_
