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
 * @copyright Andreas Dirmeier (C) 2019
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtations for malloc and new
 */

#include "Platform/Generic/Features/CcOS_malloc.h"
#include "CcKernel.h"
#include "CcMemoryManager.h"

#ifndef MEMORY_GRANULARITY
  #define MEMORY_GRANULARITY  sizeof(void*)
#endif

#ifndef __bss_end__
  extern uintptr __bss_end__;
#endif

#ifndef __data_end__
  extern uintptr __data_end__;
#endif

CCEXTERNC void* malloc(size_t uiSize)
{
  __malloc_lock(nullptr);
  void* pBuffer = nullptr;
  if(uiSize > 0 &&
    ( CcMemoryManager::isInitialized() ||
      ( CcMemoryManager::init(reinterpret_cast<uintptr>(&__bss_end__),
                              reinterpret_cast<uintptr>(&__data_end__ ),
                              MEMORY_GRANULARITY))))
  {
    pBuffer = CcMemoryManager::malloc(uiSize);
  }
  __malloc_unlock(nullptr);
  return pBuffer;
}

CCEXTERNC void free(void* pBuffer)
{
  __malloc_lock(nullptr);
  CcMemoryManager::free(pBuffer);
  __malloc_unlock(nullptr);
}

#ifndef CCOS_MEMORY_MANAGMENT_MALLOC_ONLY

void* operator new(size_t uiSize)
{
  return malloc(uiSize);
}

void operator delete(void* pBuffer)
{
  free(pBuffer);
}

void* operator new[](size_t uiSize)
{
  return malloc(uiSize);
}

void operator delete[](void* pBuffer)
{
  free(pBuffer);
}

#endif // CCOS_MEMORY_MANAGMENT_MALLOC_ONLY
