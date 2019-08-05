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
 * @page      Generic
 * @subpage   malloc
 *
 * @page      malloc
 * @copyright Andreas Dirmeier (C) 2019
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Declarations for malloc and new
 */
#ifndef CCOS_CCKERNEL_PLATFORM_GENERIC_MALLOC_H_
#define CCOS_CCKERNEL_PLATFORM_GENERIC_MALLOC_H_

#include "CcBase.h"

#ifndef CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT_MALLOC_ONLY

CCEXTERNC void* malloc(size_t);
CCEXTERNC void free(void*);
CCEXTERNC void __malloc_lock( struct _reent *_r );
CCEXTERNC void __malloc_unlock( struct _reent *_r );

#ifdef __cplusplus
  #ifndef __GNUC__
    #define _GLIBCXX_USE_NOEXCEPT
  #endif
  void* operator new(size_t size);
  void operator delete(void*)_GLIBCXX_USE_NOEXCEPT;
  void* operator new[](size_t size);
  void operator delete[](void*)_GLIBCXX_USE_NOEXCEPT;
#endif

#endif // CCOS_CCKERNEL_GENERIC_MEMORY_MANAGMENT_MALLOC_ONLY

#endif /* CCOS_CCKERNEL_PLATFORM_GENERIC_MALLOC_H_ */
